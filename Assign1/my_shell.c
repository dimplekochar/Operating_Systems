#include  <stdio.h>
#include  <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
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
	char *listcc[]{
	"sleep"};
	pid_t pid=fork();
	int status;
	if (pid<0){
		//forking failed
		return;
	}
	else if (pid==0){
		signal(SIGINT, SIG_DFL);
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
		//sleep(1); //this sleep is solely put because for some reason the ls, echo, cat output is printed after child terminates and '$' is printed and shell is waiting for next. 
		while(waitpid(pid, &status, WNOHANG)==0);
		return;
	}
}

pid_t forkexecbg(char** token){
	pid_t pid=fork();
	char *listcc[]{
	"sleep"};

	if (pid<0){
		//forking failed
		return 0;
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
		//sleep(1); //this sleep is solely put because for some reason the ls, echo, cat output is printed after child terminates and '$' is printed and shell is waiting for next.  
		return pid;
	}
}

pid_t forkexecnw(char** token){
	pid_t pid=fork();
	char *listcc[]{
	"sleep"};
	if (pid<0){
		//forking failed
		return 0;
	}
	else if (pid==0){
		signal(SIGINT, SIG_DFL);
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
		//sleep(1); //this sleep is solely put because for some reason the ls, echo, cat output is printed after child terminates and '$' is printed and shell is waiting for next.  
		return pid;
	}
}
	

int main(int argc, char* argv[]) {
	int status;
	int pidbuf=0;
	pid_t pids[100];
	char  line[MAX_INPUT_SIZE];            
	char  **tokens;              
	int i, j;

	char *listc[]{
	"ls",
	"exit",
	"echo",
	"sleep"
};

char *amper[]{
	"&",
	"&&",
	"&&&"
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
		signal(SIGINT, SIG_IGN);
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
		bool amperb[3]={0, 0, 0};
		for (i=0; tokens[i]!=NULL; i++){
			for (j=0; j<3; j++){
				if (strcmp(tokens[i], amper[j]) == 0){
					amperb[j]=1;
					break;
				}
			}	
			if (amperb[0]==1 || amperb[1]==1 || amperb[2]==1)
				break;
		}
		
		if (amperb[0]==1){
			int k=0;
			int m=0;
			while(tokens[k]!=NULL){
				char **temp = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
				int l=0;
				while(strcmp(tokens[k], amper[0]) != 0){
					temp[l]=tokens[k];
					l++; 
					k++;
					if (tokens[k]==NULL) break;
				}
				if (tokens[k]!=NULL) k++;
				
				
					pids[pidbuf]=forkexecbg(temp);
					pidbuf++;
		}
					 
				
			}
		
		else if (amperb[1]==1){
			int k=0;
			while(tokens[k]!=NULL){
				char **temp = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
				int l=0;
				while(strcmp(tokens[k], amper[1]) != 0){
					temp[l]=tokens[k];
					l++; 
					k++;
					if (tokens[k]==NULL) break;
				}
				if (tokens[k]!=NULL) k++;
				forkexec(temp);
			}
		}
		else if (amperb[2]==1){
			pid_t pidss[100];
			int k=0;
			int m=0;
			while(tokens[k]!=NULL){
				char **temp = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
				int l=0;
				while(strcmp(tokens[k], amper[2]) != 0){
					temp[l]=tokens[k];
					l++; 
					k++;
					if (tokens[k]==NULL) break;
				}
				if (tokens[k]!=NULL) k++;
				
				
					pidss[m]=forkexecnw(temp);
					m++; 
				
			}
			for (i=0; i<m; i++){
				while(waitpid(pidss[i], &status, WNOHANG)==0);
			}
		}

				

		else if(strcmp(tokens[0], listc[1]) == 0){
        		
            			printf("Shell: Goodbye. \n");
				for (i=0; i<pidbuf; i++){
					if(pids[i]!=0){
					if(waitpid(pids[i], &status, WNOHANG)==0){
						kill(pids[i], 9);
    					}}
		}
        			break;
		}
		else if(strcmp(tokens[0], listc[3]) == 0){
					
					int val = atoi(tokens[1]);
					sleep(val);
				}


		else{ 
			forkexec(tokens);
			
		}
		//if(strcmp(tokens[0], listc[3]) == 0){
		//	int val = atoi(tokens[1]);
		//	sleep(val);
		//}
		//else{ 
		//	forkexec(tokens);
			
		//}
		// Freeing the allocated memory	
		for(i=0;tokens[i]!=NULL;i++){
			free(tokens[i]);
		}
		free(tokens);
		
		for (i=0; i<pidbuf; i++){
		if(pids[i]!=0){
		if(waitpid(pids[i], &status, WNOHANG)==0);
    		else		
				{printf("Shell: Background process finished\n");
					pids[i]=0;}}
		}

	}
	return 0;
}
