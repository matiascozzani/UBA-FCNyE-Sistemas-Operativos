#include<unistd.h>
#include<sys/utsname.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
int main(int argc, char* argv[], char* envp[])
{
    struct utsname asd;
    uname(&asd);
    char path[4096];
    readlink("/proc/self/exe", path, sizeof(path));
    int pid = fork();
    if(pid == 0){
        printf("Soy Julieta\n");
        sleep(1);
        int pid_1 = fork();
        if(pid_1 == 0){
            printf("Soy Jennifer\n");
            sleep(1);
            exit(0);
        };
        exit(0);
    }
    else{
        printf("Soy Juan\n");
        sleep(1);
        int status;
        waitpid(pid, &status, 0);
        int pid_2 = fork();
        if(pid_2 == 0){
            printf("Soy Jorge\n");
            sleep(1);
        }
    }
    return 0;
}
