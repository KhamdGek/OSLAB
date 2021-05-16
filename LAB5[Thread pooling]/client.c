/*
 * Example client program that uses thread pool.
 */
#include <stdio.h>
#include <unistd.h>
#include "threadpool.h"

#define NCHAR 256
#define NTASK 32
#define NPOOH 62


struct data
{
    int a;
    int b;
};

void add(void *param)
{
    struct data *temp;

    temp = (struct data*)param;
    printf("\n \n I add two values %d and %d result = %d\n",
           temp->a, temp->b, temp->a + temp->b);
}

void multiple(void *param)
{	
    struct data *temp;

    temp = (struct data*)param;
    printf("\n \n I multiplied two values %d and %d result = %d\n",
           temp->a, temp->b, temp->a * temp->b);

}
void number(void *param)
{
    int i, num;
    
    num = *(int *)param;
    for (i = 0; i < NCHAR; ++i)
        printf("%d", num);
    printf("<%d>", num);
}

void donothing(void *param)
{
    /* do nothing */;
}

int main(void)
{
    int i, num[NTASK];
    struct data work;
    struct data work1;
    // initialize the thread pool
    pool_init();

    // 1st test
    work.a = 5;
    work.b = 10;
    pool_submit(add, &work);
    
    // 2nd test
    for (i = 0; i < NTASK; ++i) {
        num[i] = i;
        if (pool_submit(number, num+i))
            fprintf(stderr, "%d: Queue is full.\n", i);
    }
    
    // wait for free slots in the task queue
    sleep(1);

    // 3rd test
    pool_submit(donothing, NULL);
    pool_submit(donothing, NULL);
    work1.a = 20;
    work1.b = 15;
    pool_submit(multiple, &work1);

    pool_shutdown();
    return 0;
}
