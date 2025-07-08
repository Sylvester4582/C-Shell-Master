#ifndef ACTIVITY
#define ACTIVITY

#define MAX_PROCESSES 100

typedef struct{
    pid_t pid;
    char command[256];
    char state[10];
    bool killed;
}Process;

void add_process(pid_t pid, const char* command);
void print_processes();

#endif