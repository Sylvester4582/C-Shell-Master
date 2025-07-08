#include "header.h"

#include "shell.h"
#include "tokenize.h"
#include "global.h"
#include "signal.h"

pid_t fg_pid = -1;

int main(int argc, char *argv[]){

    signal(SIGCHLD, sigchld_handler);
    signal(SIGTSTP, sigtstp_handler);
    signal(SIGINT, sigint_handler);

    char home_path[MAX_LEN],previous[MAX_LEN];
    directory(home_path);
    directory(previous);

    while(true){

        char* username = get_username();
        char* system_name = get_system_name();
        char* dir = get_directory(home_path);

        printf(BLU "<%s@%s:%s" RESET,username,system_name,dir);

        int dur = get_global_dur();

        if(dur == -1) printf(BLU "> " RESET);
        else{
            char* firstword = get_global_firstword();
            printf(BLU " %s : %ds> " RESET,firstword,dur);
            free(firstword);
            set_global_flags(-1,NULL);
        }

        free(username);
        free(system_name);
        free(dir);

        char command[MAX_LEN];
        fgets(command,sizeof(command),stdin);
        command[strcspn(command,"\n")] = '\0';

        if(feof(stdin)){
            kill(0,SIGKILL);
            printf("\n");
            exit(0);
        }

        tokenize(command,home_path,previous);
    }
    
    return 0;
}