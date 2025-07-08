#include "header.h"

#include "command.h"
#include "shell.h"
#include "hop.h"
#include "log.h"
#include "global.h"

void tokenize(char* command, char* home_path, char* previous){

    if(!strlen(command)) return;

    saveCommand(command,home_path,previous);

    const char outer_delimiters[] = ";";
    const char inner_delimiters[] = "&";

    char* token;
    char* outer_saveptr = NULL;
    char* inner_saveptr = NULL;

    token = strtok_r(command, outer_delimiters, &outer_saveptr);

    while(token != NULL){

        int count = 0;
        for(int i = 0; i < strlen(token); i++) if(token[i] == '&') count++;

        char* inner_token = strtok_r(token, inner_delimiters, &inner_saveptr);

        int cur_cnt = 0;

        while(inner_token != NULL){
            inner_token = trim(inner_token);
            remove_consecutive_spaces(inner_token);
            
            if(strlen(inner_token)) handlePiping(inner_token,home_path,previous,(++cur_cnt <= count));
            
            inner_token = strtok_r(NULL, inner_delimiters,&inner_saveptr);
        }

        token = strtok_r(NULL, outer_delimiters,&outer_saveptr);
    }
}