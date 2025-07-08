#include "header.h"

#include "hop.h"
#include "shell.h"

int cmp(const void *str1, const void *str2){
    char *const *pp1 = str1;
    char *const *pp2 = str2;
    return strcmp(*pp1, *pp2);
}

void print_details(char* filename, bool l, bool file, char* home_path, char* previous){
    
    struct stat *buf;
    buf = malloc(sizeof(struct stat));

    char* ppath = get_path(home_path,filename,previous);
    
    if(stat(ppath, buf) == -1){
        free(buf);
        free(ppath);
        perror(RED "stat" RESET);
        return;
    }

    free(ppath);
    mode_t perms = buf->st_mode;

    if(l){

        printf( (S_ISDIR(perms)) ? "d" : "-");
        printf( (perms & S_IRUSR) ? "r" : "-");
        printf( (perms & S_IWUSR) ? "w" : "-");
        printf( (perms & S_IXUSR) ? "x" : "-");
        printf( (perms & S_IRGRP) ? "r" : "-");
        printf( (perms & S_IWGRP) ? "w" : "-");
        printf( (perms & S_IXGRP) ? "x" : "-");
        printf( (perms & S_IROTH) ? "r" : "-");
        printf( (perms & S_IWOTH) ? "w" : "-");
        printf( (perms & S_IXOTH) ? "x" : "-");
        printf("  ");

        printf("%d  ", (int)buf->st_nlink);

        struct passwd  *pwd;
        pwd = getpwuid(buf->st_uid);
        printf("%s  ", pwd->pw_name);

        struct group *grp;
        grp = getgrgid(buf->st_gid);
        printf("%s  ", grp->gr_name);

        printf("%jd  ",(intmax_t)buf->st_size);

        struct tm *tm;
        char datestring[256];

        tm = localtime(&buf->st_mtime);
        strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);
        printf("%s  ", datestring);
    }

    if(!file) printf(BLU "%s\n" RESET, filename);
    else if((perms & S_IXUSR) || (perms & S_IXGRP) || (perms & S_IXOTH)) printf(GRN "%s\n" RESET, filename);
    else printf("%s\n", filename);
    
    free(buf);
}

void reveall(char* home_path, char* file, char* previous, bool l, bool a){

    char* pre = (char*)malloc(sizeof(char)*MAX_LEN);
    directory(pre);

    if(change_directory(home_path,file,previous,false) == -1){
        print_details(file,l,true,home_path,previous);
        free(pre);
        return;
    }

    DIR *d;
    struct dirent *dir;
    d = opendir(".");

    char* s[1000];
    int cur = 0;

    int totalblocks = 0;

    if(d){
        while((dir = readdir(d)) != NULL){

            struct stat *buf;
            buf = malloc(sizeof(struct stat));
            
            if(stat(dir->d_name, buf) == -1){
                perror(RED "stat" RESET);
                continue;
            }
            totalblocks += buf->st_blocks;
            free(buf);

            if(a){
                s[cur] = (char*)malloc(sizeof(char)*MAX_LEN);

                if(dir->d_type == DT_DIR) strcpy(s[cur],"D");
                else strcpy(s[cur],"F");
                strcpy(s[cur]+1,dir->d_name);
                cur++;
            }
            else if(strncmp(dir->d_name,".",1)){
                s[cur] = (char*)malloc(sizeof(char)*MAX_LEN);

                if(dir->d_type == DT_DIR) strcpy(s[cur],"D");
                else strcpy(s[cur],"F");
                strcpy(s[cur]+1,dir->d_name);
                cur++;
            }
        }
        closedir(d);

        if(l) printf("total %d\n",totalblocks);

        qsort(s,cur,sizeof(*s),cmp);

        for(int i = 0; i < cur; i++){
            if(s[i][0] == 'D') print_details(s[i]+1,l,false,home_path,previous);
            else print_details(s[i]+1,l,true,home_path,previous);
            free(s[i]);
        }
        change_directory(home_path,pre,previous,false);
        free(pre);
    }
    else{
        perror(RED "Error accessing directory" RESET);
        change_directory(home_path,pre,previous,false);
        free(pre);
    }
}

void reveal(char* command, char* home_path, char* previous){
    bool l = false, a = false;
    int start = -1;

    for(int i = 6; i < strlen(command); i++){
        if(command[i] == '-'){
            i++;
            while(i < strlen(command) && command[i] != ' '){
                if(command[i] == 'l') l = true;
                else if(command[i] == 'a') a = true;
                i++;
            }
        }
        else if(command[i] != ' '){
            start = i;
            break;
        }   
    }

    if(start == -1) reveall(home_path,".",previous,l,a);
    else reveall(home_path,command+start,previous,l,a);
}