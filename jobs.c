#include "commands.h"
#include "headers.h"
#include "utils.h"
#include "discover.h"
#include "processes.h"
#include "p_info.h"
#include "history.h"
#include "jobs.h"

int name_based_sorting(const void *a, const void *b) {
    procs *proc1 = (procs *)a;
    procs *proc2 = (procs *)b;
    return strcmp(proc1->procname, proc2->procname);
}

void jobs() {
    // printf ("rg\n");
    int flag = 0;
    if (i != 1) {
        if (strcmp(args[1], "-r") == 0) {
            flag = 1;
        }
        if (strcmp(args[1], "-s") == 0) {
            flag = 2;
        }
    }
    if (proc_arr_index > 0) {
        qsort(allprocs, proc_arr_index, sizeof(procs), name_based_sorting);
        for (int j = 0; j < proc_arr_index; j++) {
            if (flag == 0) {
                // [proc_index] Running / Stopped proc_name [pid]
                if (allprocs[j].state == 1) {
                    printf("[%d] Running %s [%d]\n", allprocs[j].no + 1, allprocs[j].procname, allprocs[j].pid);
                }
                if (allprocs[j].state == 0) {
                    printf("[%d] Stopped %s [%d]\n", allprocs[j].no + 1, allprocs[j].procname, allprocs[j].pid);
                }
            }
            if (flag == 1) {
                if (allprocs[j].state == 1) {
                    printf("[%d] Running %s [%d]\n", allprocs[j].no + 1, allprocs[j].procname, allprocs[j].pid);
                }
            }
            if (flag ==  2) {
                if (allprocs[j].state == 0) {
                    printf("[%d] Stopped %s [%d]\n", allprocs[j].no + 1, allprocs[j].procname, allprocs[j].pid);
                }
            }
        }
    }
}

void sig() {
    int job_num = atoi(args[1]);
    int signal_num = atoi(args[2]);
    if (i != 3) {
        printf("Number of arguments insufficient\n");
    }
    if (job_num > proc_arr_index || job_num <= 0) {
        printf("No job with job number %d\n", job_num);
    }
    else {
        if (allprocs[job_num - 1].state == 1) {
            if (signal_num == 17 || signal_num == 19 || signal_num == 23) {
                kill(allprocs[job_num - 1].pid, SIGTSTP);
                allprocs[job_num - 1].state = 0;
            }
            else {
                kill(allprocs[job_num - 1].pid, signal_num);
            }
        }
    }
}

void fg() {
    int job_num = atoi(args[1]);
    if (i != 2) {
        printf("Number of arguments insufficient\n");
    }
    if (job_num > proc_arr_index || job_num <= 0) {
        printf("No job with job number %d\n", job_num);
    }
    else {
        // printf("%d\n", allprocs[job_num - 1].pid);
        signal(SIGTTOU, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        tcsetpgrp(STDIN_FILENO, allprocs[job_num - 1].pid);
        kill(allprocs[job_num - 1].pid, 18);
        // if (allprocs[job_num - 1].state == 0) {
        //     pid_t pid = fork();
        //     char buffer[1000];
        //     strcpy(buffer, allprocs[job_num - 1].procname);
        //     char *comm = strtok(buffer, " ");
        //     char args2[1000][1000];
        //     int j = 0;
        //     while (comm != NULL) {
        //         strcpy(args2[j], comm);
        //         comm = strtok(NULL, " ");
        //         j++;
        //     }
        //     args2[j] == NULL;
        //     // for (int k = 0; k < j; k++) {
        //     //     printf("%s\n", args2[k]);
        //     // }
        //     if (pid >= 0) {
        //         if (pid == 0) {
        //             if (execvp(args2[0], args2) < 0) {
        //                 printf("Invalid command\n");
        //             }
        //             exit(0);
        //         }
        //         else {
        //             strcpy(fg_name, args2[0]);
        //             fg_pid = pid;
        //             printf("%d\n", pid);
        //             int status;
        //             waitpid(pid, &status, WUNTRACED);
        //         }
        //     }
        //     allprocs[job_num - 1].state = 2;
        // }
        // else {
            int status;
            waitpid(allprocs[job_num - 1].pid, &status, WUNTRACED);
            tcsetpgrp(STDIN_FILENO, main_pid);
            allprocs[job_num - 1].state = 2;
        // }
    }
}

void bg() {
    int job_num = atoi(args[1]);
    if (i != 2) {
        printf("Number of arguments insufficient\n");
    }
    if (job_num > proc_arr_index || job_num <= 0) {
        printf("No job with job number %d\n", job_num);
    }
    else {
        if (allprocs[job_num - 1].state == 1) {
            return;
        }
        else {
            setpgid(allprocs[job_num - 1].pid, 0);
            kill(allprocs[job_num - 1].pid, SIGCONT);
            allprocs[job_num - 1].state = 1;
            return;
        }
    }
}