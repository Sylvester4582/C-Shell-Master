#include "header.h"

#include "shell.h"

char* get_path(char* home_path, char* path, char* previous){

    if(!strcmp(path,"..")) return strdup(path);
    else if(!strcmp(path,"~")) return strdup(home_path);
    else if(!strcmp(path,"-")) return strdup(previous);

    if(strcmp(path,".")){

        char* dir = (char*)malloc(sizeof(char)*MAX_LEN);

        if(!strncmp(path,"~",1)){
            strcpy(dir,home_path);
            strcat(dir,path+1);
        }
        else if(!strncmp(path,"/",1)){
            strcpy(dir,path);
        }
        else{
            directory(dir);
            strcat(dir,"/");
            strcat(dir,path);
        }

        return dir;
    }

    char* cur = (char*)malloc(sizeof(char)*MAX_LEN);
    directory(cur);
    return cur;
}

int change_directory(char* home_path, char* path, char* previous, bool print){

    char* cur_path = get_path(home_path,path,previous);
    char* prevdir = (char*)malloc(sizeof(char)*MAX_LEN);
    directory(prevdir);

    if(chdir(cur_path) != 0){
        if(print) perror(RED "Error changing directory" RESET);
        free(cur_path);
        free(prevdir);
        return -1;
    }

    char* dir = (char*)malloc(sizeof(char)*MAX_LEN);
    directory(dir);
    if(print) printf("%s\n",dir);

    if(print) strcpy(previous,prevdir);

    free(prevdir);
    free(cur_path);
    free(dir);

    return 0;
}

void hop(char* command , char* home_path , char* previous){
    char* targetfolder = strtok(command+3, " ");

    if(targetfolder == NULL) change_directory(home_path,"~",previous,true);

    while(targetfolder != NULL){
        change_directory(home_path, targetfolder,previous,true);
        targetfolder = strtok(NULL, " ");
    }
}