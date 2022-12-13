#include "commands.h"
#include "headers.h"
#include "utils.h"
#include "p_info.h"

void p_info() {
    pid_t pid;
    int f;
    char buf[1000], proc_info[1000][1000], pidstat[1000], pidexe[1000], dir[1000];
    char * token;
    int c, x;
    if (i != 1) {
        c = atoi(args[1]);
        pid = c;
    }
    else {
        pid = getpid();
    }
    sprintf(pidstat, "/proc/%d/stat", pid);
    f = open(pidstat, O_RDONLY);
    if (f >= 0) {
        FILE * fp;
        int r = read(f, buf, 10000);
        token = strtok(buf, " ");
        int k = 0;
        while (token != NULL) {
            strcpy(proc_info[k], token);
            // printf("%s/", proc_info[k]);
            token = strtok(NULL, " ");
            k++;
        }
        printf("pid: %d\n", pid);
        printf("Process Status: {%s", proc_info[2]);
        if (strcmp(proc_info[0], proc_info[7]) == 0) {
            printf("+}\n");
        }
        else {
            printf("}\n");
        }
        printf("Memory: %s {Virtual Memory}\n", proc_info[22]);
    }
    else {
        perror("Process doesn't exist");
        return;
    }

    sprintf(pidexe, "/proc/%d/exe", pid);
    x = readlink(pidexe, buf, sizeof(buf));
    printf("executable Path: ");
    if(x > 0) {
        buf[x] = 0;
        strcpy(dir, buf);
        if(strstr(dir, homedirectory)) {
            printf("~%s\n", dir + strlen(homedirectory));
        }
        else {
            printf("%s\n", dir);
        }
    }
    else {
        perror("");
        return;
    }
    close(f);
}