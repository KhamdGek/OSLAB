#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#define MAX_THREADS 512

void *pThread_fn( void * );

int elements, a = 0, b = 1, c = 1, count = 3;
pthread_t tid[ MAX_THREADS ];

int main( int argc, char *argv[] )
{
    int            ret_val = 0, tmp;
    pthread_attr_t attr;
	
    elements = atoi( argv[1] );
	if(elements <= 0)
	{
		printf("Invalid input");
		return(-1);
	}
	
	printf("0");
	if(elements != 1)
	{
		printf(", 1");
		if(elements > 2)
		{
			pthread_attr_init ( &attr );
			pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );
			
			pthread_create( &tid[0], &attr, pThread_fn, (void *)tmp);
		
			pthread_join(tid[0],NULL);
		}
	}
	
	printf("\n");
    return( ret_val );
}

void *pThread_fn( void *tmp1 )
{
	int  		   t, tmp;
	pthread_attr_t attr;
	
	c = a + b;
	printf(", %d", c);
	count++;
	
	if(count <= elements)
	{
		a = b;
		b = c;
		t = count - 3;
		pthread_attr_init ( &attr );
		pthread_attr_setscope( &attr, PTHREAD_SCOPE_SYSTEM );
		
		pthread_create( &tid[t], &attr, pThread_fn, (void *)tmp);
		
		pthread_join(tid[t], NULL);
	}
	
    pthread_exit(0);
}