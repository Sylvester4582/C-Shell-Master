#include "header.h"

void call_ping(char* command){
    if(strlen(command) == 4) return;

    char* temp = strtok(command+4," ");
    if(temp == NULL) return;

    pid_t pid = atoi(temp);

    temp = strtok(NULL," ");
    if(temp == NULL) return;

    int signal_number = atoi(temp)%32;

    if(kill(pid, signal_number) < 0) perror("kill\n");
}