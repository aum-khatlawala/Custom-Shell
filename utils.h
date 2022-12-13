#ifndef __UTILS_H
#define __UTILS_H

// defining global variables through common utils.h file

int i;                      // value of argc of the shell
char input[1000];
char homedirectory[1000];
char previousdirectory[2000];
char args[1000][1000];
char semi_colon_sep_args[1000][1000];
char binput[1000];
char fg_name[1000];
double timetaken;
int num_history;
int redirection_flag;
pid_t main_pid;
pid_t fg_pid;
pid_t current_pid;
int pipe_num;

typedef struct all_procs procs;
struct all_procs {
    pid_t pid;
    char procname[100];
    int no;
    int state;
};

int proc_arr_index;
procs allprocs[100];
int allprocscount;
int bg_proc_count;
int tab_flag;
char *partial_string;
// int num_matched;
// char *str_matched;

#endif