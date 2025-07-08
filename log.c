#include "header.h"

#include "hop.h"
#include "shell.h"
#include "tokenize.h"

FILE* get_file_pointer(char* home_path, char* previous, char* mode){

    char* pre = (char*)malloc(sizeof(char)*MAX_LEN);
    directory(pre);
    change_directory(home_path,"~",previous,false);

    FILE* filePointer;

    filePointer = fopen("log.txt", mode);
    if(filePointer == NULL){
        perror(RED "Failed" RESET);
        change_directory(home_path,pre,previous,false);
        free(pre);
        return NULL;
    }

    change_directory(home_path,pre,previous,false);
    free(pre);

    return filePointer;
}

int get_commands(char** commands, char* home_path, char* previous){

    char buffer[MAX_LEN];
    FILE * filePointer = get_file_pointer(home_path,previous,"r");

    if(filePointer == NULL) return -1;

    int cur = 0;

    while(fgets(buffer, MAX_LEN, filePointer)){
        buffer[strcspn(buffer, "\n")] = 0;
        commands[cur] = strdup(buffer);
        cur++;
    }

    fclose(filePointer);

    return cur;
}

void saveCommand(char* command , char* home_path, char* previous){

    char *commands[20];
    int cur = get_commands(commands,home_path,previous);

    if(cur == -1) return;

    if((cur && !strcmp(command,commands[cur-1])) || strstr(command,"log")){
        for(int i = 0; i < cur; i++) if(commands[i] != NULL) free(commands[i]);
        return;
    }

    if(cur == 15){
        FILE * filePointer = get_file_pointer(home_path,previous,"w");
        if(filePointer == NULL){
            perror(RED "Failed: " RESET);
            return;
        }
        for(int i = 1; i < 15; i++) fprintf(filePointer,"%s\n",commands[i]);
        fprintf(filePointer,"%s\n",command);
        fclose(filePointer);
    }
    else{
        FILE * filePointer = get_file_pointer(home_path,previous,"a");
        if(filePointer == NULL){
            perror(RED "Failed: " RESET);
            return;
        }
        fprintf(filePointer,"%s\n",command);
        fclose(filePointer);
    } 

    for(int i = 0; i < cur; i++) if(commands[i] != NULL) free(commands[i]);
}

void handle_log(char* command, char* home_path, char* previous){
    if(!strncmp(command,"log execute",11)){
        char *commands[20];
        int cur = get_commands(commands,home_path,previous);

        int execute_command = atoi(command+12);
        if(execute_command <= cur) tokenize(commands[cur-execute_command],home_path,previous);
    }
    else if(!strncmp(command,"log purge",9)){
        FILE * filePointer = get_file_pointer(home_path,previous,"w");
        if(filePointer == NULL){
            perror(RED "Failed: " RESET);
            return;
        }
        fclose(filePointer);
    }
    else{
        char *commands[20];
        int cur = get_commands(commands,home_path,previous);
        for(int i = 0; i < cur; i++) printf("%s\n",commands[i]);
    }
}