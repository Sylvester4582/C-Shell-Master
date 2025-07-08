#include "header.h"
#include "activities.h"
#include "proclore.h"

Process processes[MAX_PROCESSES];
int process_count = 0;

void add_process(pid_t pid, const char* command) {
    processes[process_count].pid = pid;
    strcpy(processes[process_count++].command, command);
    processes[process_count].killed = false;
}

void update_process_state(int i){
    int status;
    
    if(waitpid(processes[i].pid, &status, WNOHANG) != 0){
        processes[i].killed = true;
        return;
    }

    int running = isRunning(processes[i].pid);

    if(running== 1) strcpy(processes[i].state,"Running");
    else if(running == 0) strcpy(processes[i].state, "Stopped");
}

int compare_processes(const void *a, const void *b) {
    Process *procA = (Process *)a;
    Process *procB = (Process *)b;

    if(procA->killed) return 1;
    if(procB->killed) return 0;

    return (procA->pid > procB->pid);
}

void print_processes(){
    
    qsort(processes, process_count, sizeof(Process), compare_processes);

    for(int i = 0; i < process_count; i++){
        if(processes[i].killed) continue;
        update_process_state(i);
        if(!processes[i].killed){
            if(!i) printf("%d : %s - %s\n", processes[i].pid, processes[i].command, processes[i].state);
            else if(processes[i].pid != processes[i-1].pid) printf("%d : %s - %s\n", processes[i].pid, processes[i].command, processes[i].state);
        }
    }
}