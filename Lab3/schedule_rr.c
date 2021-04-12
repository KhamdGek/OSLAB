
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"

struct node *head = NULL;
struct node *last = NULL;
struct node *new = NULL;
struct node *test = NULL;
int num = 0;

void add(char *name, int priority, int burst) {
num = num + 1;
if (head == NULL) {
 head = malloc(sizeof(struct node)) ;
 last = malloc(sizeof(struct node)) ;

 // set the name of the task 
head->task = malloc(sizeof(struct task));
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

struct node * current = head;
struct node * ref = head;
struct node * monitor = NULL;
monitor = malloc(sizeof ( struct node) );
monitor = head;
int newburst = 0;
int totburst = 0;
float turnaroundtime = 0;
float ResponseTime = 0;
float WaitTime = 0;
int check = 1;

while (ref !=NULL   ) {



if (ref->task->burst >= 10) {	


newburst = ref->task->burst-10; // set newburst to 10



if (current!=head) {
while(monitor!=current) {
if (strcmp(current->task->name ,monitor->task->name) == 0) {	
check = 0;
break;	
}	
monitor = monitor->next;
}
monitor = head;
}	


if (ref->task->burst >10 && ref!=head) {
if (ref->next !=NULL) {
	if (check == 1){
ResponseTime = ResponseTime + totburst;
	}
}
}

if (ref->task->burst ==10 ) {

if (check ==1){
ResponseTime = ResponseTime + totburst;
}
	
}	

totburst = totburst + 10; // totburst is 60
if (ref->task->burst ==10) {
	
turnaroundtime = totburst +  turnaroundtime; // turnaroundtime = 60
}	



ref->task->burst = 10;

run(ref->task,10);	
}
else if (ref-> task-> burst  < 10 ) {
newburst = 0;
if (check ==1) {
ResponseTime = ResponseTime + totburst;
}
totburst = totburst + ref->task->burst; 

turnaroundtime = turnaroundtime + totburst ; 

run(ref->task,ref->task->burst);	

}	
 


while(1) {	
if (current->next !=NULL) { 
current = current->next; // current = T4;
if (current->next == NULL) { 
if (newburst != 0) {
struct node * newnode = malloc(sizeof(struct node));
newnode->task = malloc(sizeof(struct task));	
newnode->task->name = ref->task->name ;
newnode->task->burst = newburst;
newnode->task->priority = ref->task->priority;
ref = ref->next ; // ref points to P2
current->next = newnode;	// P4 points to New Node P1
newnode->next = NULL; // P4 points to null
current = ref;     // current points to P2

break;	
}

else if (newburst == 0) {
ref = ref->next;	 // ref points to T3
current = ref; // current points to T3
break;
}	

	
}
}
else { // if this is the last element

if (newburst !=0) {
struct node * new = malloc(sizeof(struct node));
new->task = malloc(sizeof(struct task));	
new->task->name = ref->task->name ;
new->task->priority = ref->task->priority;
new->task->burst = newburst;
ref->next = new;
ref = ref->next;
new->next = NULL;
current = new;
}
else {
ref = ref->next;
}
break;	
}	
}

	
}
WaitTime = turnaroundtime - totburst;
printf("The average turnaround time is : %f time units \n" , turnaroundtime/num);  	
printf("The average response time is : %f time units \n" , ResponseTime/num);
printf("The average Wait time is : %f time units \n" , WaitTime/num);
}



