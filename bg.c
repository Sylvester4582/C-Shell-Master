#include "header.h"

void call_bg(char* command){
    if(strlen(command) == 3) return;
    int pid = atoi(command+3);
    if(kill(pid,SIGCONT) < 0) perror("kill\n");
}