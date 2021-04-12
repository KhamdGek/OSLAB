#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "task.h"
#include "list.h"
#include "schedulers.h"
#include "cpu.h"
//Initallized NULL 
struct node * head = NULL;
struct node * current = NULL;
struct node * new = NULL;
struct node * final = NULL;
int num = 0; //Initallized the number of task to Null

//Function to Add SJF the Task/Priority/Burst
void add(char * name, int priority, int burst) {
  if (head == NULL) { //if Queue is empty
    head = malloc(sizeof(struct node)); //Initallize the queue with header
    head -> task = malloc(sizeof(struct task));
    head -> task -> name = name; //Set the task name 
    head -> task -> burst = burst; //Set the Burst time
    head -> task -> priority = priority; //set the Priority for the Task
    head -> next = NULL; //Set the node of header pointing to NULL
    current = head; // Set the current pointer @starting value[0]
  }  //Queue contains only 1 value with header pointing to NULL and current starting index
  else { //If Queue is already present
    new = malloc(sizeof(struct node)); //intialize a new index 
    new -> task = malloc(sizeof(struct task)); 
    new -> task -> burst = burst; //set the burst time of the new index 
    new -> task -> name = name; //Set the task name 
    new -> task -> priority = priority; //Set the priority of the task new
    if (!(current -> next)) //Check if the current is pointing to Null i.e only one value in queue
    { 
      if (((new -> task -> burst) > (current -> task -> burst)) || ((new -> task -> burst) ==(current -> task -> burst)))//If New task being added burst is greater than current or equal
      { //Current ->new -> Null
        current -> next = new; // set the new task to second index ie New -> index[1] & Current -> index[0]
        new -> next = NULL;  // Set the new added task pointer to NULL
      }// if the second node burst is smaller than the current burst  
	  else { //New -> Current-> null 
	  new->next = current; // set new to header ie index[0] and current to index[1]	
	  head = new; //Relocate the header to index[0]
          current = new;
      }	   
    }

    else { //Check if the Queue contains =>2 values in queue
    	  while(1) { //Run through the entire Queue burst 
    	    if ((new -> task -> burst < current -> next -> task -> burst)) //Compare if new task burst in smaller than current next value which is last entered previous new  
    	    { 
		  if 	(new->task->burst > current->task->burst) { //Swap the Current -> next with new task Current->new-> new old
			  new->next = current->next;
	  		  current->next = new ;
	  		  current = head;
	  		  break;}
	   	  else if(new->task->burst < current->task->burst) //If new value burst is smaller than current 
	   	  { //Swap the current with new task New-> current->new 
		  head = new;
      		  new->next = current;
	          current = head;
	          break;}	  
             }
	  
	    else if (new -> task -> burst == current -> next -> task -> burst) { //If incoming task == prev new task 
	   	current = current -> next ; //Current -> prev new -> new 
	  	if (current->next == NULL) {    
       		new->next = NULL;}
	  	else if (new-> task-> burst == current->next->task->burst) {
		   	current = current -> next ;   
			new->next = current->next;}
		else {
		new->next = current->next;}   
	   	current->next = new;
	  	current = head;		    
		break;}  
	  // if the new burst is greater than the current burst 
	    else if ((new -> task -> burst) > (current -> next -> task -> burst)) {
	  	current = current -> next;
	  	if ( current->next == NULL) { 
	  		current->next = new;  
	  		new->next = NULL;
	  		current = head;
	  		break;}
	  	}
	  }  
    }
  }
}

// invoke the scheduler Extra task 
void schedule() {
 float turnaroundtime = 0;
 float ResponseTime = 0;
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
printf("The average turnaround time is : %f time units \n" , (turnaroundtime)/num);    
printf("The average ResponseTime is : %f time units \n" , (ResponseTime)/num);
printf("The average WaitTime is : %f time units\n " , (WaitTime)/num);
}
