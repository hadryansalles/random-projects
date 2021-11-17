#include "ppos.h"
#include "disk.h"
#include "ppos_disk.h"
#include "ppos-core-globals.h"

#include <signal.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

disk_t disk;

// retorna o primeiro request na fila
request_t* mode_fcfs() {
    return disk.first_request;
}

// retorna o request ao bloco mais próximo do ultimo bloco acessado 
request_t* mode_sstf() {
    int current_block = disk.current_block;
    request_t* r = disk.first_request;
    request_t* best = r;

    // percorre a fila de requests
    while(r != NULL) {
        // se o request quer acessar um bloco mais próximo do ultimo acessado
        if(abs(r->block - current_block) < abs(best->block - current_block)) {
            // seleciona ele
            best = r;
        }
        r = r->next;
    }

    return best;
}

// retorna o primeiro request com bloco a direita do ultimo bloco acessado
request_t* mode_cscan() {
    int current_block = disk.current_block;
    request_t* r = disk.first_request;
    request_t* best = r;
    int min_dist = disk.size;

    // percorre a lista de requests
    while(r != NULL) {
        // verifica a distancia entre o bloco desse request e a posição atual do disco
        int dist = r->block - current_block;
        // se a distancia for negativa, adiciona uma volta inteira a ela (tamanho do disco)
        if(dist < 0) {
            dist += disk.size;
        }
        // se a distancia for menor
        if(dist < min_dist) {
            // seleciona esse request
            min_dist = dist;
            best = r;
        }
        r = r->next;
    }
    return best;
}

// Função responsável por retornar a próxima requisição agendada 
// conforme o método de escalonamento de requisições definido: FCFS, SSTF ou CSCAN.
request_t* request_scheduler() {
    request_t* scheduled = NULL;

    // seleciona o request que sera executado 
    // de acordo com o modo de escalonamento
#if DISK_MODE == DISK_MODE_FCFS
    scheduled = mode_fcfs();
#elif DISK_MODE == DISK_MODE_SSTF
    scheduled = mode_sstf();
#elif DISK_MODE == DISK_MODE_CSCAN
    scheduled = mode_cscan();
#else
#error INVALID DISK MODE, PLEASE USE: DISK_MODE_FCFS, DISK_MODE_SSTF OR DISK_MODE_CSCAN
#endif

    // se o pedido escalonado possui um anterior
    if(scheduled != NULL && scheduled->prev != NULL) {
        // liga o pedido anterior ao proximo
        scheduled->prev->next = scheduled->next;
    }
    // se o pedido escalonado possui um proximo
    if(scheduled != NULL && scheduled->next != NULL) {
        // liga o proximo pedido ao anterior
        scheduled->next->prev = scheduled->prev;
    }
    // se o pedido escalonado for o primeiro pedido do disco
    if(scheduled != NULL && scheduled == disk.first_request) {
        // liga o primeiro pedido ao proximo
        disk.first_request = scheduled->next;
    }
    // se o pedido escalonado for o ultimo pedido do disco
    if(scheduled != NULL && scheduled == disk.last_request) {
        // liga o ultimo pedido ao anterior
        disk.last_request = scheduled->prev;
    }
    return scheduled;
}

// corpo da tarefa gerenciadora do disco
void disk_manager_task(void* args) {
    int shouldExit = 0;
    while(!shouldExit) {
        // obtem acesso a fila de requests
        mutex_lock(&disk.requests_lock);

        if(disk.executing_request != NULL) {
            // incrementa o numero de blocos percorridos
            disk.blocks_covered += abs(disk.current_block - disk.executing_request->block);
            // atualiza a posicao atual do disco
            disk.current_block = disk.executing_request->block;
            // acorda a task que realizou o request que foi executado anteriormente
            task_resume(disk.executing_request->task);
            // atualiza o request atualmente sendo executado para nulo
            disk.executing_request = NULL;
        }

        // se houver algum request na fila
        if(disk.first_request != NULL) {
            // seleciona um request para ser executado
            request_t* r = request_scheduler();
            disk.executing_request = r;
            // executa a operacao do request
            r->result = disk_cmd(r->cmd, r->block, r->buffer);
            // se o resultado for diferente de 0 houve algum erro
            if(r->result != 0) {
                // nesse caso reseta o request que esta sendo executado
                disk.executing_request = NULL;
                // acorda a task que realizou esse request
                task_resume(r->task);
            }
        }

        // libera acesso a fila de requests
        mutex_unlock(&disk.requests_lock);

        task_sleep(3);

        mutex_lock(&disk.requests_lock);
        if(disk.executing_request == NULL && disk.first_request == NULL) {
            shouldExit = 1; 
        }
        mutex_unlock(&disk.requests_lock);
    }
    printf("Gerenciador de disco finalizado.\n");
    printf("Blocos percorridos: %d\n", disk.blocks_covered);
    // finaliza a tarefa
    task_exit(0);
}

// Função responsável por criar a tarefa gerenciadora do disco. 
// É chamada somente quando o SIGUSR1 é gerado pelo disco.
void signal_handler() {
    // acorda a task do gerenciador de disco
    task_resume(&disk.manager_task);
}

int disk_mgr_init (int *numBlocks, int *blockSize) {

    // inicaliza o disco fisico
    int result   = disk_cmd(DISK_CMD_INIT, 0, 0);
    (*numBlocks) = disk_cmd(DISK_CMD_DISKSIZE, 0, 0);
    (*blockSize) = disk_cmd(DISK_CMD_BLOCKSIZE, 0, 0);

    // se houve algum erro durante inicializacao retorna codigo de erro
    if(result || *numBlocks < 0 || *blockSize < 0) {
        return -1;
    }

    // configura uma funcao para tratar o signal SIGUSR1
    struct sigaction action;
    action.sa_handler = signal_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if(sigaction(SIGUSR1, &action, 0) < 0) {
        perror("Erro em sigaction");
        exit(1);
    }

    // inicializa as variaveis do disco virtual
    disk.size = *numBlocks;
    disk.blocks_covered = 0;
    disk.executing_request  = NULL;
    disk.first_request = NULL;
    disk.last_request = NULL;

    // cria o mutex de acesso a fila de pedidos
    mutex_create(&disk.requests_lock);

    // cria a tarefa gerenciadora do disco
    task_create(&disk.manager_task, disk_manager_task, NULL);
    disk.manager_task.system_task = 1;

    return 0;
}

// adiciona o request a fila de requests bloqueando a tarefa que o chamou
int wait_request(int cmd, int block, void* buffer) {

    // cria um novo request
    request_t* r = malloc(sizeof(request_t));
    r->cmd = cmd;
    r->block = block;
    r->buffer = buffer;
    r->next = NULL;
    r->prev = NULL;

    // obtem acesso a fila de requests
    mutex_lock(&disk.requests_lock);

    if(disk.first_request == NULL) {
        disk.first_request = r;
        disk.last_request = r;
    } else {
        disk.last_request->next = r;
        r->prev = disk.last_request; 
        disk.last_request = r;
    }

    if(disk.executing_request == NULL) {
        task_resume(&disk.manager_task);
    }

    mutex_unlock(&disk.requests_lock);

    task_suspend(taskExec, &r->task);
    task_yield();

    int result = r->result;
    free(r);
    return result;
}

int disk_block_read (int block, void *buffer) {
    return wait_request(DISK_CMD_READ, block, buffer);
}

int disk_block_write (int block, void *buffer) {
    return wait_request(DISK_CMD_WRITE, block, buffer);
}