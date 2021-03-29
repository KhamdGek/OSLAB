#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
*
*/

char **tokenize(char *line)
{
  char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
  char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  int i, tokenIndex = 0, tokenNo = 0;

  for(i =0; i < strlen(line); i++){

    char readChar = line[i];

    if (readChar == ' ' || readChar == '\n' || readChar == '\t'){
      token[tokenIndex] = '\0';
      if (tokenIndex != 0){
	tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
	strcpy(tokens[tokenNo++], token);
	tokenIndex = 0; 
      }
    } else {
      token[tokenIndex++] = readChar;
    }
  }
 
  free(token);
  tokens[tokenNo] = NULL ;
  return tokens;
}

void execute(char **argv)
{
	pid_t pid;
	int status;
	
	printf("\n");
	pid = fork();
	if ( pid<0 ){
		printf("*** ERROR: forking child process failed\n");
          	exit(1);
     	}
     	else if (pid == 0) {          /* for the child process:         */
          if (execvp(*argv, argv) < 0) {     /* execute the command  */
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
        }
        else {                    /* for the parent:      */
          while (wait(&status) != pid)       /* wait for completion  */
               ;       
     	}
}

int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;
	int flag=0;
	char input_buffer[MAX_INPUT_SIZE + 1]={0}; 
	FILE* fp;
	fflush(stdout);
        fflush(stdin);
	if(argc == 2) {
		fp = fopen(argv[1],"r");
		if(fp < 0) {
			printf("File doesn't exists.");
			return -1;
		}
	}

	while(1) {
					
		/* BEGIN: TAKING INPUT */
		bzero(line, sizeof(line));
		if(argc == 2) { // batch mode
			if(fgets(line, sizeof(line), fp) == NULL) { // file reading finished
				break;	
			}
			line[strlen(line) - 1] = '\0';
		} else { // interactive mode
			printf("$ ");
			scanf("%[^\n]", line);
			flag=1;
			getchar();
		}	
		printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */
		// ADDED CODE TO CHECK THE HISTORY! //
		if(strncmp(line, "!!", 2) == 0) {
    			printf("Detected !! viewing History if any: \n");
        		if(!input_buffer[0]) {  // no history yet
            			printf("No history available yet!\n");
            			flag=0;
        		}
        		else{
				printf("%s\n", input_buffer);
				flag=1;
				execute(tokens);
			}    
    		}
		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
		if( strncmp(line, "!!",2) == 0){ 
			continue;
		}
		else{
			strcpy(input_buffer, line);
		}		
   		
       //do whatever you want with the commands, here we just print them
		if(flag==1){
			for(i=0;tokens[i]!=NULL;i++){
				printf("found token %s (remove this debug output later)\n", tokens[i]);	
			}
			flag=0;
		}
		
		// Freeing the allocated memory	
		//for(i=0;tokens[i]!=NULL;i++){
		//	free(tokens[i]);
		//}
		//printf("Check2");
		//free(tokens);
	}
	fflush(stdout);
        fflush(stdin);
        fflush(stderr);
	return 0;
}

