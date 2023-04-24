#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>
int main(int argc, char* argv[]){
    int pipes[2];
    if(pipe(pipes)==-1) perror("PIPE");

    pid_t pid = fork();
    if(pid==-1) perror("ERROR");
    if(pid==0){
        close(pipes[0]);
        dup2(pipes[1], STDOUT_FILENO);
        execlp("ls", "ls", "-al", NULL);
        exit(0);
    }

    pid_t pid2 = fork();
    if(pid2==-1) perror("ERROR");
    if(pid2==0){
        close(pipes[1]);
        dup2(pipes[0], STDIN_FILENO);
        execlp("wc", "wc", "-l", NULL);
        exit(0);
    }
    exit(0);

}