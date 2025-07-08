#include "header.h"

#include "activities.h"

void sigchld_handler(int sig){
    int status;
    pid_t pid;
    while((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if(WIFEXITED(status)) printf("Process %d exited normally\n", pid);
        else printf("Process %d exited abnormally\n", pid);
    }
}

void sigtstp_handler(int sig){
    if(fg_pid == -1) return;
    
    if(kill(fg_pid,19) < 0) perror("kill");
    else{
        add_process(fg_pid,"fg");
        fg_pid = -1;
    }
}

void sigint_handler(int sig){
    if(fg_pid == -1) return;

    if(kill(fg_pid,9) < 0) perror("kill");
    else fg_pid = -1;

    return;
}