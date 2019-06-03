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
	

	

void forkexec(char** token){
	pid_t pid=fork();
	int status;
	char *listcc[]{
	"sleep"};
	if (pid<0){
		//forking failed
		return;
	}
	else if (pid==0){
		if(strcmp(token[0], listcc[0]) == 0){
					int val = atoi(token[1]);
					sleep(val);
				}
		
		else if (execvp(token[0], token)<0){
			printf("Shell: Incorrect command \n");
		}
			//since execvp() returns a negative value if the execution fails (e.g., the request file does not exist). 		
		exit(0);
	}
	else{
		//sleep(1); //this sleep is solely put because for some reason the ls, echo, cat output is printed after child terminates and '$' is printed and shell is waiting for next 
		//wait();  
		while(waitpid(pid, &status, WNOHANG)==0);
		return;
	}
}
	

int main(int argc, char* argv[]) {
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i;

	char *listc[]{
	"cd",
	"exit",
	"cat",
	"echo",
	"sleep",
	"ls"
};


		
	FILE* fp;
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
			getchar();
		}
		//printf("Command entered: %s\n", line);
		/* END: TAKING INPUT */

		line[strlen(line)] = '\n'; //terminate with new line
		tokens = tokenize(line);
   
       //do whatever you want with the commands, here we just print them

		//for(i=0;tokens[i]!=NULL;i++){
		//	printf("found token %s\n", tokens[i]);
		//	}
		int i;
		if(strcmp(tokens[0], listc[0]) == 0){
        		if (chdir(tokens[1]) == 0); //success
         		else {
            			printf("Shell: Incorrect command \n");
        		} 
		}
		else if(strcmp(tokens[0], listc[1]) == 0){
        		
            			printf("Shell: Goodbye. \n");
        			break;
		}


		else{ 
			forkexec(tokens);
			
		}
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);

	}
	return 0;
}
