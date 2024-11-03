#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <linux/limits.h>
#include <sys/types.h>
#include "LineParser.h"
#include <fcntl.h>

#include <sys/wait.h>

int debug_mode =0;

void execute(cmdLine *pCmdLine) {

    
    if(strcmp(pCmdLine->arguments[0] ,"cd")==0){
        if (pCmdLine->argCount > 1) {
            if (chdir(pCmdLine->arguments[1])==-1)
            {
                perror("cd operation error");
            }
        }
        
    }
    else if(strcmp(pCmdLine->arguments[0] ,"wakeup")==0){
        if (pCmdLine->argCount > 1) {
            pid_t pid = atoi(pCmdLine->arguments[1]);
            if (kill(pid, SIGCONT) == 0) {
                printf("wakeup pid: %d\n",pid);
            }
            else{
                printf("wakeup error");
            }
        }
    
    }
    else if(strcmp(pCmdLine->arguments[0] ,"nuke")==0){
        if (pCmdLine->argCount > 1) {
            pid_t pid = atoi(pCmdLine->arguments[1]);
            if (kill(pid, SIGTERM) == 0) { 
                printf("nuke pid: %d\n",pid);
            }
            else{
                printf("nuke error");
            }
        }
    }
    else{
        pid_t new_process = fork();

        if (new_process == 0) {
            
            if(pCmdLine->inputRedirect != NULL){
               
                int input_fd = open(pCmdLine->inputRedirect,O_RDONLY,0644);
                if(input_fd ==-1){
                    perror("error input file");
                    exit(EXIT_SUCCESS);
                }
                dup2(input_fd, STDIN_FILENO);
                close(input_fd);
            }
            if(pCmdLine->outputRedirect != NULL){
             
                int output_fd =open(pCmdLine->outputRedirect,O_RDWR | O_CREAT |O_TRUNC ,0644); 
                if(output_fd ==-1){
                    perror("error output file");
                    exit(EXIT_SUCCESS);
                }
                dup2(output_fd, 1);
                close(output_fd);
            }
            if (execvp(pCmdLine->arguments[0], pCmdLine->arguments) == -1) {
                
                perror("Execv_error");
                freeCmdLines(pCmdLine);
                _exit(EXIT_FAILURE);
            }
        }
        else if (new_process > 0) {
            if (debug_mode==-1){
                fprintf(stderr , "the PID is : %d\n" , new_process);
                fprintf(stderr , "the executing command is : %s\n" , pCmdLine->arguments[0]);
            }
            if(pCmdLine->blocking !=0){
             
                waitpid(new_process,NULL,0);
            }
        }
    }
}

int main(int argc, char **argv) {
    
    char* current_directory  = (char*) malloc(PATH_MAX);
    char* user_input = (char*) malloc(2048);
    cmdLine* command_line = (cmdLine*)malloc(sizeof(cmdLine));
    while (1) {
        for (int i = 1; i < argc; i++){
            if((argv[i][0]=='-') && (argv[i][1]=='d')){
                debug_mode =-1;
            }
        }

        if (getcwd(current_directory, PATH_MAX) != NULL) {
            printf("%s\n ", current_directory);
        }

        if (fgets(user_input, 2048, stdin) == NULL) {
            perror("User inputfgets error"); 
            exit(0);
        }

        if (strcmp(user_input, "quit\n") == 0) {

            if(command_line != NULL){
                freeCmdLines(command_line);
            }

            break;
        }


        const char* const_user_input = user_input;
        cmdLine *command_line = parseCmdLines(const_user_input);

        
        execute(command_line);


        if(command_line != NULL){
           
            freeCmdLines(command_line);
        }
    }
    free(current_directory);
    free(user_input);
    return 0;
}
