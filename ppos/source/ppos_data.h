// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.1 -- Julho de 2016

// Estruturas de dados internas do sistema operacional

#ifndef __PPOS_DATA__
#define __PPOS_DATA__

#include <stdio.h>
#include <ucontext.h>		// biblioteca POSIX de trocas de contexto
#include "queue.h"		// biblioteca de filas genéricas

// Estrutura que define um Task Control Block (TCB)
typedef struct task_t
{
   struct task_t *prev, *next ;		// ponteiros para usar em filas
   int id ;				// identificador da tarefa
   ucontext_t context ;			// contexto armazenado da tarefa
   unsigned char state;  // indica o estado de uma tarefa (ver defines no final do arquivo ppos.h): 
                          // n - nova, r - pronta, x - executando, s - suspensa, e - terminada
   struct task_t* queue;
   struct task_t* joinQueue;
   int exitCode;
   unsigned int awakeTime; // used to store the time when it should be waked up

   // ... (outros campos deve ser adicionados APOS esse comentario)
   int static_prio;                  // prioridade estatica, escala negativa de -20 a 20
   int dynamic_prio;                 // prioridade dinamica, utiliza envelhecimento de 1 
   int system_task;                  // indica se a tarefa é de sistema ou não, evitando preempcao
   unsigned int quantum;             // quantidade de ticks que a tarefa poderá usar antes de ser preemptada
   unsigned int execution_ticks;     // tick em que a tarefa comecou a ser executada
   unsigned int processor_ticks;     // numero de ticks em que a tarefa esteve em um processador
   unsigned int last_processor_tick; // tick da ultima vez que a terfa obteve um processador
   unsigned int number_activations;  // numero de ativacoes da tarefa
   
} task_t ;

// estrutura que define um semáforo
typedef struct {
    struct task_t *queue;
    int value;

    unsigned char active;
} semaphore_t ;

// estrutura que define um mutex
typedef struct {
    struct task_t *queue;
    unsigned char value;

    unsigned char active;
} mutex_t ;

// estrutura que define uma barreira
typedef struct {
    struct task_t *queue;
    int maxTasks;
    int countTasks;
    unsigned char active;
    mutex_t mutex;
} barrier_t ;

// estrutura que define uma fila de mensagens
typedef struct {
    void* content;
    int messageSize;
    int maxMessages;
    int countMessages;
    
    semaphore_t sBuffer;
    semaphore_t sItem;
    semaphore_t sVaga;
    
    unsigned char active;
} mqueue_t ;

#endif

