
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
struct node * current = NULL;


void add(char *name, int priority, int burst) {
if (head == NULL) {
    head = malloc(sizeof(struct node));

    // set the name of the task 
    head -> task = malloc(sizeof(struct task));
    head -> task -> name = name;
    head -> task -> burst = burst;
    head -> task -> priority = priority;
    // set the next node to be null
    head -> next = NULL;

    current = head;
  
  } else {

    new = malloc(sizeof(struct node));

    new -> task = malloc(sizeof(struct task));
    new -> task -> burst = burst;
    new -> task -> name = name;
    new -> task -> priority = priority;
    // if current->next is NULL
    if (!(current -> next)) {
      if (((new -> task -> priority) < (current -> task -> priority)) || ((new -> task -> priority) ==(current -> task -> priority))) {
        current -> next = new; // head points to second node 
        new -> next = NULL;
		
      }
	// if the second node burst is smaller than the current burst  
	  else {
		 
	  // set new to point to head which is in the second position
	  new->next = current; 	
	  // head now holds the address of new which is in the first position
	  head = new;
	  // reset current to new
      current = new;	  
	  // we still have the second node connected to null  
      }	   
    }
 
	// T3 and on execute from here I think 
		else {
    	  
	  while(1) {
    
      if ((new -> task -> priority > current -> next -> task -> priority) ) {
	  
	  if (new->task->priority < current->task->priority) {
	  new->next = current->next;
	  current->next = new ;
	  current = head; 
	  break;
	  }
	  else if (new->task->priority > current->task->priority) {

	  head = new;
      new->next = current;
	  current = head;
	  break;
	  }
	  // if the new priority == the current priority
	  else if(new->task->priority == current->task->priority) {
		new->next = current->next;
        current->next = new;	
        break;		
		  
	  }  
	  
	  }	   
	  else if (new -> task -> priority == current -> next -> task -> priority) {
	   current = current -> next ;
	   
	   if (current->next == NULL) {    
       new->next = NULL;
	   
	   }  
	   else if (new-> task-> priority == current->next->task->priority) {
		   
		current = current -> next ;   
		new->next = current->next;   
		   
	   }   
	   
	   else {
		   
	   new->next = current->next;	   
		   
	   }   
	   
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

current = head;
struct node * ref = head;	
struct node *beginning = malloc(sizeof(struct node));
int newburst = 0;
while (ref !=NULL) {

// ref->next = 3 (this is the second 3rd) 
if (ref->next ==NULL) {
run(ref->task, ref->task->burst);
break;	
}
else {



//	ref->task->priority = first 3
// ref->next->task->priority = second 3
	

if ((ref->task->priority != ref->next->task->priority)) {	

run(ref->task,ref->task->burst);
ref = ref->next; // ref = first 3
}


else {

	
if (ref->next !=NULL) {
	
// first 3 = second 3 priority
	
if ((ref->task->priority == ref->next->task->priority)) {
	
// set current to point to ref ( location of first 3)	
current = ref ;  

// current->next is 3
while (current->next != NULL) {
	
current = current->next; 	 // current is third 3 

// the 3rd 3 priority != the 3rd 3 priority)
if (current->task->priority != current->next->task->priority) {	

beginning = current->next; // set the beginning 


current->next = NULL;
current = ref; // move current back to ref so we can do round robin
break;
}
else {
//current = current->next;	// current = 2nd 3 priority
}
}

while (ref !=NULL   ) {
if (ref->task->burst >= 10) {
newburst = ref->task->burst-10; // set newburst to 0
ref->task->burst = 10;
run(ref->task,10);	
}
else if (ref-> task-> burst  < 10) {
newburst = 0;
run(ref->task,ref->task->burst);	

}	
 


while(1) {	
if (current->next !=NULL) { 
current = current->next;	// current points to newT3
if (current->next == NULL) { // 
if (newburst != 0) {
struct node * newnode = malloc(sizeof(struct node));
newnode->task = malloc(sizeof(struct task));	
newnode->task->name = ref->task->name ;
newnode->task->burst = newburst;
newnode->task->priority = ref->task->priority;
ref = ref->next ;
current->next = newnode;	
newnode->next = NULL;
current = ref;

break;	
}

else if (newburst == 0) {
ref = ref->next;	 // ref points to new T3
current = ref; // current points to new T3 
break;
}	

	
}
}
else { // if this is the last element
	
ref = ref->next; // ref == NULL after this
current = beginning;



break;	
}	
}	
}

ref = beginning;

}	

}
}
}

} 


}



