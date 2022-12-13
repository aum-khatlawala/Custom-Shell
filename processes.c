#include "commands.h"
#include "headers.h"
#include "utils.h"
#include "processes.h"
#include "prompt.h"

typedef struct processes bgprocess;

struct processes {
    pid_t pid;
    int running;
    char procname[1000];
    bgprocess *next;
};
bgprocess *head = NULL;
bgprocess *current = NULL;

int counter = 1;

void new_bg_proc(pid_t pid, int running, char procname[1000]) {
    bgprocess *newproc = (bgprocess *) malloc(sizeof(bgprocess));
    newproc -> pid = pid;
    newproc -> running = running;
    strcpy(newproc -> procname, procname);
    newproc -> next = head;
    head = newproc;
    printf("[%d] %d\n", counter, pid);
    counter++;
}

void process(int fg_or_bg) {
    // clock_t begin = clock();
    int status;
    pid_t pid = fork();
    time_t begin;
    time(&begin);
    char * args2[1000];
    for (int j = 0; j < i; j++) {
        args2[j] = malloc(1000);
        strcpy(args2[j], args[j]);
    }
    args2[i] == NULL;
    if (pid >= 0) {
        if (pid == 0) {
            if (execvp(args2[0], args2) < 0) {
                printf("Invalid command\n");
            }
            exit(0);
        }
        else {
            if (fg_or_bg == 0) {
                // printf("Hi");
                strcpy(fg_name, args2[0]);
                fg_pid = pid;
                // printf("%d\n", pid);
                current_pid = pid;
                waitpid(pid, &status, WUNTRACED);
            }
            if (fg_or_bg == 1) {
                allprocs[proc_arr_index].no = proc_arr_index;
                strcpy(allprocs[proc_arr_index].procname, args2[0]);
                for (int j = 1; j < i; j++) {
                    strcat(allprocs[proc_arr_index].procname, " ");
                    strcat(allprocs[proc_arr_index].procname, args2[j]);
                }
                allprocs[proc_arr_index].pid = pid;
                allprocs[proc_arr_index].state = 1;
                proc_arr_index++;
                new_bg_proc(pid, 1, args2[0]);
                bg_proc_count++;
            }
        }
    }
    else {
        printf("Couldn't create child process - Forking error\n");
    }
    time_t end;
    time(&end);
    // // printf("%d %d\n", (int)(end), (int)(begin));
    timetaken = (double)(end - begin);
    
    // printf("%ld\n", CLOCKS_PER_SEC);
    // printf("%f\n", timetaken);
}

char *delete (pid_t pid) {
    bgprocess *prev;
    bgprocess *current = head;
    prev = current;
    int c = 0;
    if (head == NULL)
    { 
        return NULL;
    }
    while (current->pid != pid)
    {
        if (current->next == NULL)
        {
            return NULL;
        }
        else
        {
            prev = current;
            current = current->next;
        }
        c++;
    }
    if (current->running == 1) {
        if (current != head) {
            prev->next = current->next;
        }
        else {
            head = current->next;
        }
        for (int j = 0; j < proc_arr_index; j++) {
            if (allprocs[j].pid == pid) {
                if (allprocs[j].state == 1) {
                    allprocs[j].state = 3;
                }
                else {
                    return NULL;
                }
            }
        }
        // if (proc_arr_index == 0) {
        //     allprocs[0].no = '\0';
        //     strcpy(allprocs[0].procname, '\0');
        //     allprocs[0].pid = '\0';
        //     allprocs[0].state = 3;
        // }
        // else {
        //     for (int j = 0; j < proc_arr_index; j++) {
        //         if (allprocs[j].pid == pid) {
        //             for (int k = 0; k < proc_arr_index; k++) {
        //                 allprocs[k] = allprocs[k+1];
        //             }
        //             proc_arr_index--;
        //         }
        //     }
        // }
        return current->procname;
    }
    else {
        return NULL;
    }
}

void proc_terminated () {
    int flag;

    // pid_t waitpid (child_pid, &status, options);

    // Options Parameter:
    // 1. If 0 means no option parent has to wait for terminates child.
    // 2. If WNOHANG means parent does not wait if child does not terminate just check and return waitpid().(not block parent process)
    // 3. If child_pid is -1 then means any arbitrarily child, here waitpid() work same as wait() work.

    // Return value of waitpid():
    // 1. pid of child, if child has exited
    // 2. 0, if using WNOHANG and child hasn’t exited.

    pid_t child_pid = waitpid(-1, &flag, WNOHANG | WUNTRACED);
    int final_pid;
    char * proc_name = malloc(1000);
    if (child_pid > 0) {
        proc_name = delete(child_pid);
        int j = 0;
        int flag = 0;
        for (; j < proc_arr_index; j++) {
            if (allprocs[j].pid == child_pid) {
                if (allprocs[j].state == 1) {
                    allprocs[j].state = 3;
                    flag = 1;
                }
            }
        }
        if (!proc_name) {
            // printf("\n");
        }
        else {
            if (WIFEXITED(flag)) {
                fprintf(stderr, "\n%s with pid %d exited normally\n", proc_name, child_pid);
            }
            else {
                fprintf(stderr, "\n%s with pid %d exited abruptly\n", proc_name, child_pid);
            }
            prompt(timetaken);
        }
    }
    fflush(stderr);
    fflush(stdout);
}
