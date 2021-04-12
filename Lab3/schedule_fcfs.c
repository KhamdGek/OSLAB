
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

//implement ll
struct node *head = NULL; 
struct node *last = NULL;
struct node *new = NULL;
int num = 0;


void add(char *name, int priority, int burst) {
if (head == NULL) {
    // memory allocation for nodes
 head = malloc(sizeof(struct node)) ;
 last = malloc(sizeof(struct node)) ;

// set the name of the task
head->task = malloc(sizeof(struct task));
//attributes [Processes id] , [priority] , [burst] 
head->task->name = name;
head->task->burst = burst;
head->task->priority = priority;
// set the next node to be null
head->next = NULL;

last = head;
}    

else {
    
new =  malloc(sizeof(struct node));    
last->next = new ;
new->task = malloc(sizeof(struct task));
new->task->name = name;
new->task->burst = burst;
new->task->priority = priority;
new->next = NULL;    
last = new;
    
}
}

// invoke the scheduler
void schedule() {
int final = 0;
float turnaroundtime = 0;
float WaitTime = 0;
float ResponseTime = 0;

// create a dummy node for iteration
struct node * ref = head;     
while (ref !=NULL   ) {
//count number of processes in txt file
num = num +1;
run(ref->task,ref->task->burst);  



//calulated numerical values
final =  final + ref->task->burst; // 5 15 30
turnaroundtime = turnaroundtime + final ; //5 (5+10+5)20  50      (5 + 5+10 + 5+10+15)
if (ref->next !=NULL) {
ResponseTime = ResponseTime + final;
}
ref = ref->next;    

}
WaitTime = turnaroundtime - final;

printf("The average turnaround time is : %f time units \n" , turnaroundtime/num);    
printf("The average ResponseTime is : %f time units \n" , ResponseTime/num);
printf("The average WaitTime is : %f time units\n " , WaitTime/num);

}






