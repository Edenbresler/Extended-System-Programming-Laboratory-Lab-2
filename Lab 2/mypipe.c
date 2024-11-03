#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main(int argc, char **argv) {
    int my_pipe[2];
    char buffer[2028]; 
    pipe(my_pipe);
    pid_t pid = fork();
    
    if (pid == 0) {
        close(my_pipe[0]);
        const char *my_message = "hello";
        write(my_pipe[1], my_message,strlen(my_message) + 1);
        close(my_pipe[1]);
    }
    else{
        close(my_pipe[1]);
        read(my_pipe[0], buffer, sizeof(buffer));
        printf("Parent received: %s\n", buffer);
        close(my_pipe[0]);
    }
    return 0;
}