/*** Khawaja Hammad 2017-EE-135 ******/
/***4.27 Task ***/
#include <pthread.h> 
#include <stdio.h> 
int number = 0; 
long fib[500]; 
/****** Set the function ******/
void *fib_Seq(void *param) {  
    if (number <= 0)  //Checking if the entered number is less than 0 
        pthread_exit(0); // Terminate the thread 
    fib[0] = 0; //Else initalize the 1st index to 0
    if (number > 1) {  
        fib[1] = 1; // initalize the 2nd index to 1 
        for (int i = 2; i < number; i++) // Run the fib seq for loop 
            fib[i] = fib[i-1] + fib[i-2]; } 
        pthread_exit(0); /* once completed terminate the thread*/ } 
int main(int argc, char *argv[]) { 
    pthread_t tid; // Thread ID 
    pthread_attr_t attr; 
    pthread_attr_init(&attr); 
    printf("Print this many Fibonacci numbers: "); 
    scanf("%d", &number); 
    if (number > 500) { 
        printf("Printing as many as possible: 500\n"); 
        number = 500; } 
    /*Calling the child thread */ 
    pthread_create(&tid, &attr, fib_Seq, argv[1]); //Create a Thread excuting the fib_Seq function by passing thread ID and attributes
    pthread_join(tid, NULL); //wait for thread tid to finish  
    /* Printing the output of the thread */
    if (number > 0) 
        printf("%ld", fib[0]); 
    for (int i = 1; i < number; i++) 
        printf(", %ld", fib[i]); 
    printf("\n"); 
    return 0; 

}
