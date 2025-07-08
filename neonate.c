#include "header.h"

int orig_flags;
struct termios orig_termios;

void disableRawMode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    fcntl(STDIN_FILENO, F_SETFL, orig_flags);
}

void enableRawMode(){
    tcgetattr(STDIN_FILENO, &orig_termios);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void setNonBlockingInput(){
    orig_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, orig_flags | O_NONBLOCK);
}

void neonate(char* command){

    if(strlen(command) <= 11) return;
    
    int frequency = atoi(command+11);
    time_t lastrun = 0;

    char c;
    enableRawMode();
    setNonBlockingInput();

    while(true){

        if(read(STDIN_FILENO, &c, 1) == 1){
            if(c == 'x'){
                disableRawMode();
                break;
            }
        }

        time_t cur_time = time(0);
        if((int)difftime(cur_time,lastrun) >= frequency){

            FILE *fptr = fopen("/proc/loadavg","r");
            if(fptr == NULL) return;

            int pid;
            fscanf(fptr,"%*s %*s %*s %*s %d",&pid);

            printf("%d\n",pid);
            fclose(fptr);
            lastrun = cur_time;
        }

        usleep(10000);
    }
}