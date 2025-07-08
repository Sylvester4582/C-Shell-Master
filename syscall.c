#include "header.h"

#include "activities.h"
#include "global.h"

void execc(char* command){
    char *myargs[100];
    int cur = 0;

    char *firstword = strtok(command," ");
    while(firstword != NULL){
        myargs[cur] = firstword;
        cur++;
        firstword = strtok(NULL," ");
    }
    myargs[cur] = NULL;

    if(execvp(myargs[0],myargs) == -1) perror("execvp error");
}

void call_execute(char* command, bool background){

    if(background){
        int rc = fork();
        if(rc < 0) return;
        else if(rc == 0){
            if(!strncmp(command,"vi",2)){
                add_process(getpid(),"vi");
                kill(getpid(),19);
            }
            else if(!strncmp(command,"htop",4)){
                add_process(getpid(),"htop");
                kill(getpid(),19);
            }
            else if(!strncmp(command,"emacs",5)){
                add_process(getpid(),"emacs");
                kill(getpid(),19);
            }
            else if(!strncmp(command,"gedit",5)){
                add_process(getpid(),"gedit");
                kill(getpid(),19);
            }
            else if(!strncmp(command,"neo",3)){
                add_process(getpid(),"neo");
                kill(getpid(),19);
            }
            else if(!strncmp(command,"nano",4)){
                add_process(getpid(),"nano");
                kill(getpid(),19);
            }
            else{
                execc(command);
                exit(EXIT_SUCCESS);
            }
        }
        else{
            char *firstword = strtok(command," ");
            add_process(rc, firstword);
            printf("Background process PID: %d\n", rc);
        }
    }
    else{
        int rc = fork();

        time_t time1,time2;
        time(&time1);

        if(rc < 0) return;
        else if(rc == 0){
            execc(command);
            exit(EXIT_SUCCESS);
        }
        else{
            fg_pid = rc;

            int status;
            waitpid(rc,&status,WUNTRACED);
            
            time(&time2);

            char *firstword = strtok(command," ");
            int tt = (int)difftime(time2,time1);

            if(tt >= 2) set_global_flags(tt,firstword);
        }
    }
}