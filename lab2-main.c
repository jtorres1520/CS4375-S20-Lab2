#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>


int shell_cd(char **args);
int shell_help(char **args);
int shell_exit(char **args);

char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &shell_cd,
  &shell_help,
  &shell_exit
};

int shell_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

int shell_cd(char **args){
  if (args[1] == NULL) {
    fprintf(stderr, "shell: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("shell");
    }
  }
  return 1;
}

int shell_help(char **args){
  int i;
  printf("Lab 2 Assignment: Jacob Torres and Jacob Barberena's Shell\n");
  printf("The following are built in:\n");

  for (i = 0; i < shell_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }
  return 1;
}

int shell_exit(char **args){
  return 0;
}

int shell_runCM(char **args){
  if(args[0] == NULL){   // empty command
    return 1;
  }

  for(int i = 0; i < shell_num_builtins(); i++){
    if(strcmp(args[0], builtin_str[i]) == 0){
      return (*builtin_func[i])(args);
    }
  }
  return shell_launch(args);
}

#define SHELL_RL_SIZE 1024
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


#define SHELL_PRS_SIZE 64
#define SHELL_PRS_DELIM " \t\r\n\a"
char **shell_parse_line(char *line){
  int bufsize = SHELL_PRS_SIZE;
  int pos = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *sgl_token;
  
  if(!tokens){
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


int shell_launch(char **args){
  pid_t pid;
  pid_t wpid;
  int status;

  pid = fork();
  if(pid == 0){   // child process
    if(execvp(args[0], args) == -1){
      perror("shell");
    }
    exit(EXIT_FAILURE);
  }
  else if(pid < 0){
    perror("shell");
  }
  else{   // parent process
    do{
      wpid = waitpid(pid, &status, WUNTRACED);
    }while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

void shell_loop(){
  char *line;
  char **parse_args;
  int run_args;

  do{
    printf("$ >> ");
    
    line = shell_read_line();
    parse_args = shell_parse_line(line);
    run_args = shell_runCM(parse_args);
    
    free(line);
    free(parse_args);
      
  }while (run_args);
  
}

int main(int argc, char **argv){
  shell_loop();   // will continue running shell on a loop
  return EXIT_SUCCESS;
}
