#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"
//Priority based Scheduler Algo
//Pointer for queue is iniated
struct node * head = NULL;
struct node * current = NULL;
struct node * new = NULL;
struct node * final = NULL;
//Add function to add values to queue based on priority 
void add(char * name, int priority, int burst) {
  if (head == NULL) { //If the queue is empty initate the queue
    head = malloc(sizeof(struct node)); 
    head -> task = malloc(sizeof(struct task));
    head -> task -> name = name; //Task name to be assigned
    head -> task -> burst = burst; //Task burst time assigned 
    head -> task -> priority = priority; //Task priority assigned 
    head -> next = NULL; //set the pointer->next to null 
    current = head;// set the current pointer to head
  } 
  //If queue is already iniated 
  else {
    new = malloc(sizeof(struct node));//Iniate the task as new 
    new -> task = malloc(sizeof(struct task));
    new -> task -> burst = burst; //Assign the burst time
    new -> task -> name = name; //Assign the name of new task 
    new -> task -> priority = priority; //Assign priority to the task
    if (!(current -> next)) //If the current task is pointing to null is contains only 1 value for now
    { //IF THE CURRENT TASK HAS HIGH PRIO DO NOTHING BUT SET THE POINTER OF CURRENT->NEXT TO NEW TASK
      if (((new -> task -> priority) < (current -> task -> priority)) || ((new -> task -> priority) ==(current -> task -> priority))) {
        current -> next = new;  
        new -> next = NULL;
      } 
      else {
      //if THE NEW TASK HAS HIGH PRIO THAN CURRENT TASK SWAP
	 new->next = current; 	
	 head = new;
	 current = new;	  
      }	   
    }
    else {//IF MORE THAN TWO INDEX VALUES IN QUEUE DO RUN THE FOLLOWING CODE
    	while(1) {
    		if ((new -> task -> priority > current -> next -> task -> priority) ) {
			if (new->task->priority < current->task->priority) 
			{//IF THE CURRENT TASK HAS HIGH PRIO DO NOTHING BUT SET THE POINTER OF CURRENT->NEXT TO NEW TASK
	  			new->next = current->next;
	  			current->next = new ;
	  			current = head; 
	  			break;
	  		}
	  		else if (new->task->priority > current->task->priority) 
	  		{//if THE NEW TASK HAS HIGH PRIO THAN CURRENT TASK SWAP
				head = new;
      				new->next = current;
	  			current = head;
	  			break;
	  		}
	  		// if the new priority == the current priority swap
	  		else if(new->task->priority == current->task->priority) {
				new->next = current->next;
        			current->next = new;	
        			break;		
		  	}  
	  	}	  
		else if (new -> task -> priority == current -> next -> task -> priority) {
	   		current = current -> next ;
	   		if (current->next == NULL) {
				new->next = NULL;}
	   	else if (new-> task-> priority == current->next->task->priority) {
			current = current -> next ;   
			new->next = current->next;}   
	 	else {
			new->next = current->next;}   
	   	current->next = new;
	 	current = head;		    
		break;  
	  }  
	  // if the new priority is less than the current priority
	  else if ((new -> task -> priority) <(current -> next -> task -> priority)) {
	  current = current -> next;
	  if ( current->next == NULL) {
	//printf("testing"); 
	  current->next = new;  
	  new->next = NULL;
	  current = head;
	  break;
	  }   
      }
     }  
    }
  }
}

// invoke the scheduler
void schedule() {
int num = 0;
float ResponseTime = 0;
float turnaroundtime = 0;
float WaitTime = 0;
int burst = 0;

  struct node * ref = head;
  while (ref != NULL) {
    num = num + 1;
    run(ref -> task, ref -> task -> burst);
    burst = burst + ref->task->burst;
	turnaroundtime = turnaroundtime + burst ; 
	if (ref->next !=NULL) {
	ResponseTime = ResponseTime + burst;
	}
    ref = ref -> next;
  }

  

WaitTime = turnaroundtime - burst;

printf("The average turnaround time is : %f time units \n" , (float)turnaroundtime/num);    
printf("The average ResponseTime is : %f time units \n" , (float)ResponseTime/num);
printf("The average WaitTime is : %f time units\n " , (float)WaitTime/num);



}
