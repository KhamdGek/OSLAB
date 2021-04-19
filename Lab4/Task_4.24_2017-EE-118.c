/* Arslan_Shafique_2017-EE-118*/
/*Task_4.24 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#define NUMBER_OF_POINTS        1000000
#define NUMBER_OF_THREADS   2
 
void *runner(void *param);
/* Points in the circle */
int circle_count = 0;
 
/* Generates a double precision random number */
double random_double()
{
    return random() / ((double)RAND_MAX + 1);
}
 
int main (int argc, const char * argv[]) {
    int points_per_thread = NUMBER_OF_POINTS/ NUMBER_OF_THREADS;
    int i;
     
    double Pi;
     
    pthread_t workers[NUMBER_OF_THREADS];
 
    /* seed the random number generator */
    srandom((unsigned)time(NULL));
    clock_t begin = clock();
    for (i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_create(&workers[i], 0, runner, &points_per_thread);
     
    for (i = 0; i < NUMBER_OF_THREADS; i++)
        pthread_join(workers[i],NULL);
     
    /* estimating Pi */
    Pi = 4.0 * circle_count / NUMBER_OF_POINTS;
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("NUMBER OF POINTS = %d\n",NUMBER_OF_POINTS);
    printf("Pi = %f\n",Pi);
    printf("time = %f\n",time_spent);
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
     
    circle_count += hit_count;
     
    pthread_exit(0);
}
