#include "commands.h"
#include "headers.h"
#include "utils.h"
#include "ctrls.h"
#include "processes.h"

void ctrlZ() {
    pid_t current_pid = getpid();
    if (current_pid == main_pid) {
        if (fg_pid == -1) {
            fflush(stdout);
        }
        else {
            allprocs[proc_arr_index].no = proc_arr_index;
            strcpy(allprocs[proc_arr_index].procname, fg_name);
            allprocs[proc_arr_index].pid = fg_pid;
            allprocs[proc_arr_index].state = 0;
            proc_arr_index++;
            kill(fg_pid, SIGTSTP);
            new_bg_proc(fg_pid, 0, fg_name);
            bg_proc_count++;
            fg_pid = -1;
            printf("\n");
        }
    }
    else {
        return;
    }
    return;
}