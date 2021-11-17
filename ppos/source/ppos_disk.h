// PingPongOS - PingPong Operating System
// Prof. Carlos A. Maziero, DINF UFPR
// Versão 1.2 -- Julho de 2017

// interface do gerente de disco rígido (block device driver)

#ifndef __DISK_MGR__
#define __DISK_MGR__

#include "ppos.h"

// estruturas de dados e rotinas de inicializacao e acesso
// a um dispositivo de entrada/saida orientado a blocos,
// tipicamente um disco rigido.

#define DISK_MODE_FCFS 1
#define DISK_MODE_SSTF 2
#define DISK_MODE_CSCAN 3
#define DISK_MODE DISK_MODE_SSTF

// estrutura que representa um pedido de acesso ao disco
typedef struct request_t
{
  struct request_t* next; // proximo request
  struct request_t* prev; // request anterior
  task_t* task;           // task que criou esse request
  void*   buffer;         // buffer
  int     cmd;            // comando a ser executado
  int     block;          // bloco
  int     result;         // resultado da execucao do comando
} request_t;

// estrutura que representa um disco no sistema operacional
typedef struct disk_t
{
  task_t            manager_task;      // task gerenciadora do disco
  mutex_t           requests_lock;     // mutex para a fila de requests
  struct request_t* first_request;     // primeiro request na fila 
  struct request_t* last_request;      // ultimo request na fila
  struct request_t* executing_request; // request sendo executado
  int               current_block;     // ultimo bloco acessado pelo disco
  int               size;              // tamanho total do disco em blocos
  int               blocks_covered;    // numero de blocos percorridos
} disk_t ;

// inicializacao do gerente de disco
// retorna -1 em erro ou 0 em sucesso
// numBlocks: tamanho do disco, em blocos
// blockSize: tamanho de cada bloco do disco, em bytes
int disk_mgr_init (int *numBlocks, int *blockSize) ;

// leitura de um bloco, do disco para o buffer
int disk_block_read (int block, void *buffer) ;

// escrita de um bloco, do buffer para o disco
int disk_block_write (int block, void *buffer) ;

#endif