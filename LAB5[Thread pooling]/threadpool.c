/**
 * Implementation of thread pool.
 * 2016003736 이민수 
 * 소프트웨어학부 컴퓨터전공 3학년
 */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include "threadpool.h"

#define QUEUE_SIZE 10
#define NUMBER_OF_THREADS 3

#define TRUE 1

// this represents work that has to be 
// completed by a thread in the pool

sem_t semqueue;

typedef struct 
{
    void (*function)(void *p);
    void *data;
}task;

task queue[QUEUE_SIZE];
int queuenum = 0;
int deqcurpos = 0;
int enqcurpos = 0;

// the work queue
task worktodo;

task enqwork;

// the worker bee
pthread_t bee1;
pthread_t bee2;
pthread_t bee3;

int choosethread = 0;

// insert a task into the queue
// returns 0 if successful or 1 otherwise, 
int enqueue(task t) 
{
    sem_wait(&semqueue);
    if(queuenum > 10){
	sem_post(&semqueue);
	return 1;
    }
    queue[enqcurpos] = t;
    queuenum ++;
    enqcurpos = (enqcurpos + 1)%10;
    sem_post(&semqueue);
    return 0;
}

// remove a task from the queue
task dequeue() 
{
    worktodo = queue[deqcurpos];
    deqcurpos = (deqcurpos + 1)%10;
    return worktodo;
}

// the worker thread in the thread pool
void *worker(void *param)
{
    task newwork;
    sem_wait(&semqueue);
    if(queuenum != 0){
	queuenum--;
	newwork = dequeue();
	(*newwork.function)(newwork.data);
	//queuenum--;
    }
    sem_post(&semqueue);
    pthread_exit(0);
}

/**
 * Submits work to the pool.
 */
int pool_submit(void (*somefunction)(void *p), void *p)
{
    enqwork.function = somefunction;
    enqwork.data = p;
    int eq = enqueue(enqwork);

    if(choosethread  == 0){
    	pthread_create(&bee1,NULL,worker,NULL);
    }else if(choosethread == 1){
	pthread_create(&bee2,NULL,worker,NULL);
    }else if(choosethread == 2){
	pthread_create(&bee3,NULL,worker,NULL);
    }
    choosethread = (choosethread + 1)%3;

    return eq;
}

// initialize the thread pool
void pool_init(void)
{
    sem_init(&semqueue,0,1);
}

// shutdown the thread pool
void pool_shutdown(void)
{
    pthread_join(bee1,NULL);
    pthread_join(bee2,NULL);
    pthread_join(bee3,NULL);
}
