#include "header.h"

#include "hop.h"
#include "reveal.h"
#include "shell.h"
#include "seek.h"
#include "tokenize.h"
#include "log.h"
#include "proclore.h"
#include "activities.h"
#include "global.h"
#include "neonate.h"
#include "syscall.h"
#include "ping.h"
#include "bg.h"
#include "fg.h"
#include "iMan.h"

void run(char *command, char* home_path, char* previous, bool background){
    if(!strncmp(command,"hop",3)) hop(command,home_path,previous);
    else if(!strncmp(command,"reveal",6)) reveal(command, home_path, previous);
    else if(!strncmp(command,"log",3)) handle_log(command,home_path,previous);
    else if(!strncmp(command,"seek",4)) call_seek(command,home_path,previous);
    else if(!strcmp(command,"activities")) print_processes();
    else if(!strncmp(command,"proclore",8)) call_proclore(command);
    else if(!strncmp(command,"ping",4)) call_ping(command);
    else if(!strncmp(command,"fg",2)) call_fg(command);
    else if(!strncmp(command,"bg",2)) call_bg(command);
    else if(!strncmp(command,"neonate",7)) neonate(command);
    else if(!strncmp(command,"iMan",4)) iMan(command);
    else call_execute(command,background);
}

void execute(char* command, char* home_path, char* previous, bool background){

    int saved_stdout = dup(1);
    int saved_stdin = dup(0);
    
    if(strchr(command, '<')){

        char *dupcommand = strdup(command);
        char *startfile = strchr(dupcommand, '<') + 2;
        char *file = strtok(startfile, " ");
        file = trim(file);

        int fd = open(file, O_RDONLY);
        if(fd < 0){
            perror(RED "Not able to open file" RESET);
            return;
        }
        dup2(fd, STDIN_FILENO);
        close(fd);

        free(dupcommand);
    }

    if(strstr(command, ">>")){

        char *dupcommand = strdup(command);
        char *startfile = strstr(dupcommand, ">>") + 3;
        char *file = strtok(startfile, " ");
        file = trim(file);

        int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if(fd < 0){
            perror(RED "Not able to open file" RESET);
            return;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);

        free(dupcommand);
    }

    else if(strchr(command, '>')){

        char *dupcommand = strdup(command);
        char *startfile = strchr(dupcommand, '>') + 2;
        char *file = strtok(startfile, " ");
        file = trim(file);

        int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(fd < 0){
            perror(RED "Not able to open file" RESET);
            return;
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);

        free(dupcommand);
    }

    int cur = 0;
    while(cur < strlen(command)){
        if(command[cur] == '>' || command[cur] == '<') break;
        cur++;
    }

    char* curcommand = (char*)malloc(sizeof(char)*MAX_LEN);
    curcommand = strncpy(curcommand,command,cur);
    
    curcommand[cur] = '\0';
    curcommand = trim(curcommand);

    if(!strcmp(curcommand,"exit") || !strcmp(curcommand,"quit")){
        free(curcommand);
        exit(0);
    }

    run(curcommand,home_path,previous,background);
    free(curcommand);

    dup2(saved_stdout,1);
    dup2(saved_stdin,0);
    close(saved_stdout);
    close(saved_stdin);
}

void handlePiping(char* command, char* home_path, char* previous, bool background){

    int count = 0;
    for(int i = 0; i < strlen(command); i++) if(command[i] == '|') count++;

    int p[count*2];
    for(int i = 0; i < count; i++){
        if(pipe(p+i*2) < 0){
            perror("pipe failed");
            return;
        }
    }

    int ccount = 0;
    char *comms[100];

    char *pcommand = strtok(command,"|");
    while(pcommand != NULL){
        comms[ccount] = (char*)malloc(sizeof(char)*MAX_LEN);
        strcpy(comms[ccount],pcommand);
        comms[ccount] = trim(comms[ccount]);
        ccount++;
        pcommand = strtok(NULL,"|");
    }

    int cur = 0;

    while(cur < count+1){

        int saved_stdin = dup(0);
        int saved_stdout = dup(1);

        if(cur != 0){
            if(dup2(p[(cur-1)*2],0) < 0){
                perror("dup2 failed");
                return;
            }
            close(p[(cur-1)*2]);
        }

        if(cur != count){
            if(dup2(p[cur*2+1],1) < 0){
                perror("dup2 failed");
                return;
            }
            close(p[cur*2+1]);
        }

        execute(comms[cur],home_path,previous,background);

        dup2(saved_stdout,1);
        dup2(saved_stdin,0);
        close(saved_stdout);
        close(saved_stdin);

        cur++;
    }
}