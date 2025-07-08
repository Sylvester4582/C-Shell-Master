#include "header.h"

void printPID(int pid){

    char file_name[100];
    snprintf(file_name,sizeof(file_name),"/proc/%d/stat",pid);
    FILE *fp = fopen(file_name,"r");
    if(fp == NULL){
        perror(RED "Failed" RESET);
        return;
    }

    int pgrp;
    char state;
    unsigned long vsize;
    char exe_path[1024];

    fscanf(fp, "%*d %*s %c %*d %*d %d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %*d %*u %lu", &state, &pgrp, &vsize);

    snprintf(file_name, sizeof(file_name), "/proc/%d/exe", pid);

    ssize_t len = readlink(file_name, exe_path, sizeof(exe_path) - 1);
    if (len != -1) exe_path[len] = '\0';
    else strcpy(exe_path, "Executable path not found");

    char ground = '+';
    if(getpgrp() != pid) ground = ' '; 

    printf("pid : %d\n",pid);
    printf("Process Status : %c%c\n", state, ground);
    printf("Process Group : %d\n", pgrp);
    printf("Virtual Memory : %lf KB\n", (double)((double)vsize/(double)1024));
    printf("Executable Path : %s\n", exe_path);

    fclose(fp);
}

void call_proclore(char* command){
    int pid;
    if(strlen(command) == 8) pid = getpid();
    else pid = atoi(command+9);

    printPID(pid);
}

int isRunning(int pid){
    char file_name[100];
    snprintf(file_name,sizeof(file_name),"/proc/%d/stat",pid);
    FILE *fp = fopen(file_name,"r");
    if(fp == NULL) return -1;
    
    char state;
    fscanf(fp, "%*d %*s %c %*d %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %*d %*u %*u", &state);

    fclose(fp);

    if(state == 'T') return 0;
    return 1;
}