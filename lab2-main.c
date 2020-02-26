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




