#include "header.h"

#include "global.h"

void call_fg(char* command){
    if(strlen(command) == 3) return;
    
    int pid = atoi(command+3);
    int shellpid = getpgid(0);

    kill(pid,SIGCONT);
    tcsetpgrp(0,getpgid(pid));

    time_t time1,time2;
    time(&time1);

    fg_pid = pid;

    int status;
    if(waitpid(pid, &status, WUNTRACED) < 0){
        printf(RED "error in pid\n" RESET);
        return;
    }

    time(&time2);
    char *firstword = strtok(command," ");
    int tt = (int)difftime(time2,time1);

    if(tt >= 2) set_global_flags(tt,firstword);

    tcsetpgrp(0,shellpid);
}