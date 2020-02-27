#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv){
  shell_loop();   // will continue running shell on a loop
  return EXIT_SUCCESS;
}

void shell_loop(){
  char *line;
  char **parse_args;
  int run_args;

  do{
    printf("$ >> ");
    line = shell_read_line();
    parse_args = shell_parse_line();
    run_args = shell_runCM();
    
    free(line);
    free(run_args);
      
  }while(status);
  
}

#define SHELL_RL_SIZE 1024;
char *shell_read_line(void){
  int lnsize = SHELL_RL_SIZE;
  int start_pos = 0;
  char *buffer = malloc(sizeof(char) * lnsize);

  if(!buffer){
    fprintf(stderr, "shell: allocation error");
    exit(EXIT_FAILURE);
  }

  int c;
  while(1){
    c = getchar();

    if(c == EOF || c == '\n'){
      buffer[start_pos] = '\0';
      return buffer;
    }
    else{
      buffer[start_pos] = c;
    }
    
    start_pos++;

    if(start_pos >= lnsize){
      lnsize += SHELL_RL_SIZE;
      buffer = realloc(buffer, lnsize);
      
      if(!buffer){
	fprintf(stderr, "shell: allocation error");
	exit(EXIT_FAILURE);
      }
    }
    
  }
}


#define SHELL_PRS_SIZE 64;
#define SHELL_PRS_DELIM " \t\r\n\a";
char **shell_parse_line(char *line){
  int bufsize = SHELL_PRS_SIZE;
  int pos = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *sgl_token;
  
  if(!tokens = ){
    fprintf(stderr, "shell: allocation error");
    exit(EXIT_FAILURE);
  }

  sgl_token = strtok(line, SHELL_PRS_DELIM);

  while(sgl_token != NULL){
    tokens[pos] = sgl_token;
    pos++;
    
    if(pos >= bufsize){
      bufsize += SHELL_PRS_SIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if(!tokens){
	fprintf(stderr, "shell: allocation error");
	exit(EXIT_FAILURE);
      }
    }
    sgl_token = strtok(NULL, SHELL_PRS_DELIM);
  }
  tokens[pos] = NULL;
  return tokens;
}


