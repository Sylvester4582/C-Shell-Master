#include "header.h"

void directory(char * path){
    getcwd(path, MAX_LEN);
}

char* get_directory(char* home){

    char* cur = (char*)malloc(sizeof(char)*MAX_LEN);
    directory(cur);

    if(strlen(cur) < strlen(home)) return cur;
    if(!strcmp(cur,home)){
        free(cur);
        char* tilda = strdup("~");
        return tilda;
    }
    if(!strncmp(home,cur,strlen(home))){
        char* path = (char*)malloc(sizeof(char)*MAX_LEN);
        path[0] = '~';
        strcpy(path+1,cur+strlen(home));
        free(cur);
        return path;
    }
    return cur;
}

char* get_system_name(){

    struct utsname buffer;
    uname(&buffer);
    char* systemName = (char*)malloc(sizeof(char)*MAX_LEN); 
    strcpy(systemName,buffer.nodename);

    return systemName;
}

char* get_username(){

    uid_t uid = getuid();
    struct passwd *user;
    user = getpwuid(uid);

    char* userName = (char*)malloc(sizeof(char)*MAX_LEN); 
    strcpy(userName,user->pw_name);

    return userName;
}