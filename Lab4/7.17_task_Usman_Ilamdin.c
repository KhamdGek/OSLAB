/* Usman IlamDin --------2017-EE-119*/
/*Task_7.17 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define NUMBER_OF_POINTS        1000000
#define NUMBER_OF_THREADS   1
#define NUMBER_OF_SLAVES    100
 
void *runner(void *param);
/* Points in the circle */
int circle_count = 0;
 
/* This will protect circle_count */
pthread_mutex_t mutex;
 
/* Generates a double precision random number */
double random_double()
{
    return random() / ((double)RAND_MAX + 1);
}
 
int main (int argc, const char * argv[]) {
    int points_per_thread = NUMBER_OF_POINTS/ NUMBER_OF_THREADS;
    int i;
    clock_t begin = clock();
    double estimated_pi;
     
    pthread_t runners[NUMBER_OF_THREADS];
 
    pthread_mutex_init(&mutex,NULL);
 
    /* seed the random number generator */
    srandom((unsigned)time(NULL));
 
    for (i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_create(&runners[i], 0, runner, &points_per_thread);
     
    for (i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_join(runners[i],NULL);
     
    /* estimating Pi */
    estimated_pi = 4.0 * circle_count / NUMBER_OF_POINTS;
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("NUMBER OF POINTS = %d\n",NUMBER_OF_POINTS);
    printf("NUMBER OF SLAVES = %d\n",NUMBER_OF_SLAVES);
    printf("Pi = %f\n",estimated_pi);
    printf("time = %f\n",time_spent*4/NUMBER_OF_SLAVES);
     
    return 0;
}
 
void *runner(void *param)
{
    int POINTS;
    POINTS = *((int *)param);
    int i;
    int hit_count = 0;
    double x,y;
     
    for (i = 0; i < POINTS; i++) {
         
        /*generate random numbers between -1.0 and +1.0 (exclusive)*/
        /* to obtain a random (x, y) point*/
        x = random_double() * 2.0 - 1.0;
        y = random_double() * 2.0 - 1.0;
        /* is (x, y) point within the circle ? */
        if ( sqrt(x*x + y*y) < 1.0 )
            ++hit_count; /* yes, (x, y) point is in circle */      
    }
 
    /* no race condition on circle count */
    pthread_mutex_lock(&mutex);
    circle_count += hit_count;
    pthread_mutex_unlock(&mutex);
     
    pthread_exit(0);
}
