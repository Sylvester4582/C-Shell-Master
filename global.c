#include "header.h"

int dur = -1;
char* firstwordcommand;

char *ltrim(char *s){
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s){
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s){
    return rtrim(ltrim(s)); 
}

void remove_consecutive_spaces(char *str) {
    int i = 0, j = 0;
    int space_flag = 0;

    while(str[i] != '\0'){
        if(isspace(str[i])){
            if(!space_flag){
                str[j++] = ' ';
                space_flag = 1;
            }
        }else{
            str[j++] = str[i];
            space_flag = 0;
        }
        i++;
    }
    str[j] = '\0';
}

void set_global_flags(int t, char* firstword){
    dur = t;
    if(dur != -1) firstwordcommand = strdup(firstword);
}

int get_global_dur(){
    return dur;
}

char* get_global_firstword(){
    return firstwordcommand;
}