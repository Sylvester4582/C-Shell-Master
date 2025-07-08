#include "header.h"

#include "hop.h"
#include "shell.h"

void find(char* filename,char *cur_path,bool f, bool di, char *s[], int* count, char* home_path, char* previous){

    DIR *d;
    struct dirent *dir;
    d = opendir(".");

    if(!d) return;

    while((dir = readdir(d)) != NULL){

        if(!strcmp(dir->d_name,"..") || !strcmp(dir->d_name,".")) continue;

        struct stat *buf;
        buf = malloc(sizeof(struct stat));
        if(stat(dir->d_name, buf) == -1){
            free(buf);
            continue;
        }
        free(buf);

        char *new_path = (char*)malloc(sizeof(char)*MAX_LEN);
        strcpy(new_path,cur_path);
        strcat(new_path,"/");
        strcat(new_path,dir->d_name);

        if(!strncmp(dir->d_name,filename,strlen(filename))){

            if(dir->d_type == DT_DIR && di){
                s[*count] = (char*)malloc(sizeof(char)*MAX_LEN);
                strcpy(s[*count],"D");
                strcpy(s[*count]+1,new_path);
                (*count)++;   
            }

            else if(dir->d_type != DT_DIR && f){
                s[*count] = (char*)malloc(sizeof(char)*MAX_LEN);
                strcpy(s[*count],"F");
                strcpy(s[*count]+1,new_path);
                (*count)++;
            }
        }

        if(dir->d_type == DT_DIR){
            char *cur = (char*)malloc(sizeof(char)*MAX_LEN);
            directory(cur);

            if(change_directory(home_path,dir->d_name,previous,false) == -1){
                free(cur);
                continue;
            }
            find(filename,new_path,f,di,s,count,home_path,previous);

            change_directory(home_path,cur,previous,false);
            free(cur);
        }

        free(new_path);
    }

    closedir(d);
}

void seek(char *filename, char *path, char *home_path, char *previous, bool d, bool f, bool e){

    char *cur_path = (char*)malloc(sizeof(char)*MAX_LEN);
    directory(cur_path);

    if(change_directory(home_path,path,previous,false) == -1){
        printf(RED "Error changing directory\n" RESET);
        free(cur_path);
        return;
    }

    char *s[MAX_LEN];
    int total = 0;

    find(filename,".",f,d,s,&total,home_path,previous);

    if(total == 0) printf(RED "No Matches Found !\n" RESET);
    else if(total >= 1){
        for(int i = 0; i < total; i++){
            if(s[i][0] == 'D'){
                if(total == 1) printf(BLU "%s/\n" RESET,s[i]+1);
                else printf(BLU "%s\n" RESET,s[i]+1);
            }
            else printf(GRN "%s\n" RESET,s[i]+1 );
        }
    }
    if(total == 1 && e){
        char file_dir_path[MAX_LEN];
        directory(file_dir_path);
        strcat(file_dir_path,s[0]+2);

        if(s[0][0] == 'F'){
            FILE *file_ptr;
            char str[MAX_LEN];

            file_ptr = fopen(file_dir_path,"r");
            if(file_ptr == NULL) printf(RED "Missing permissions for task!\n" RESET);
            else{
                while(fgets(str, MAX_LEN, file_ptr) != NULL){
                    printf("%s", str);
                    fflush(stdout);
                }
                fclose(file_ptr);
            }
        }
        else if(change_directory(home_path,file_dir_path,previous,false) == -1) printf(RED "Missing permissions for task!\n" RESET);           
    } 

    for(int i = 0; i < total; i++) free(s[i]);
    change_directory(home_path,cur_path,previous,false);
    free(cur_path);
}

void call_seek(char* command, char* home_path, char* previous){
    if(strlen(command) == 4) return;

    bool d = false, f = false, e = false;
    int start = -1;

    for(int i = 4; i < strlen(command); i++){
        if(command[i] == '-'){
            i++;
            while(i < strlen(command) && command[i] != ' '){
                if(command[i] == 'f') f = true;
                else if(command[i] == 'd') d = true;
                else if(command[i] == 'e') e = true;
                i++;
            }
        }
        else if(command[i] != ' '){
            start = i;
            break;
        }
    }

    if(start == -1) return;

    int end = start;
    while(end < strlen(command) && command[end] != ' ') end++;

    char filename[MAX_LEN],path[MAX_LEN];
    strncpy(filename,command+start,end-start);

    if(end == strlen(command)){
        char cur_path[MAX_LEN];
        directory(cur_path);
        strcpy(path,cur_path);
    }
    else strcpy(path,command+end+1);

    if(f && d){
        printf(RED "Invalid flags!\n" RESET);
        return;
    }

    if(!f && !d){
        f = true;
        d = true;
    }

    seek(filename,path,home_path,previous,d,f,e);
}