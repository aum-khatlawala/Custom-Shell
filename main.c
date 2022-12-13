#include "headers.h"
#include "utils.h"
#include "commands.h"
#include "prompt.h"
#include "processes.h"
#include "history.h"
#include "redirs.h"
// #include "ctrls.h"

void ctrlZ() {
    // printf("Hi");
    if (current_pid != main_pid) {
        // printf("hel");
        // if (fg_pid == -1) {
        //     fflush(stdout);
        // }
            new_bg_proc(fg_pid, 0, args[0]);
            allprocs[proc_arr_index].no = proc_arr_index;
            strcpy(allprocs[proc_arr_index].procname, args[0]);
            allprocs[proc_arr_index].pid = fg_pid;
            allprocs[proc_arr_index].state = 0;
            proc_arr_index++;
            // printf("%d\n", proc_arr_index);
            kill(fg_pid, SIGTSTP);
            bg_proc_count++;
            fg_pid = -1;
            // printf("\n");
            return;
    }
    else {
        // printf("g");
        printf("\n");
        // prompt(timetaken);
        return;
    }
    return;
}

void ctrlC() {
    pid_t current_pid = getpid();
    if (current_pid != main_pid) {
        if (fg_pid == -1) {
            printf("\n");
            prompt(timetaken);
            fflush(stdout);
        }
        else {
            kill(fg_pid,SIGINT);
            tcsetpgrp(STDIN_FILENO,main_pid);
            return;
        }
    }
    else {
        // printf("\n");
        // prompt(timetaken);
        return;
    }
}












void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}














int main()
{
    bg_proc_count = 0;
    char cwd[2000];
    char s[10] = " \r\t\n";
    char *token, *token2;
    getcwd(cwd, sizeof(cwd));
    strcpy(homedirectory, cwd);
    loadhistory();
    main_pid = getpid();
    signal(SIGCHLD, proc_terminated);
    signal(SIGINT, ctrlC);
    // printf("Hi\n");
    signal(SIGTSTP, ctrlZ);
    // strcpy(previousdirectory, homedirectory);
    // signal(SIGCHLD, proc_terminated);
    // initialize();
    char prev_command[1000] = "";
    timetaken = 0.0;
    proc_arr_index = 0;
    char *inp = malloc(sizeof(char) * 1000);
    char c;
    while (1)
    {
        tab_flag = 0;
        fg_pid = -1;
        i = 0;
        pipe_num = 0;
        // prompt(timetaken);
        // timetaken = 0.0;
        // fgets(input, 1000, stdin);


        setbuf(stdout, NULL);
        enableRawMode();
        prompt(timetaken);
        timetaken = 0.0;
        memset(inp, '\0', 1000);
        int pt = 0;

        while (read(STDIN_FILENO, &c, 1) == 1) {
            if (iscntrl(c)) {if (c == 10) break;
                else if (c == 27) {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
                        printf("\rarrow key: %s", buf);
                    }
                } else if (c == 127) { // backspace
                    if (pt > 0) {
                        if (inp[pt-1] == 9) {
                            for (int i = 0; i < 7; i++) {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        printf("\b \b");
                    }
                } else if (c == 9) { // TAB character
                    inp[pt++] = c;
                    for (int i = 0; i < 8; i++) { // TABS should be 8 spaces
                        printf(" ");
                    }
                    tab_flag = 1;
                    partial_string = malloc(sizeof(char) * 1000);
                    char *finder = strrchr(inp, ' ');
                    // printf("%s\n", retval);
                    if (finder == NULL) {
                    // printf("%s\n", str1);
                        strcpy(partial_string, inp);
                    }
                    else {
                        finder++;
                        strcpy(partial_string, finder);
                    }
                    // printf("\n%s\n", partial_string);
                    printf("\n");
                    i = 1;
                    strcpy(args[0], "ls");
                    // strcpy(args[1], ">");
                    // strcpy(args[2], "temp.txt");
                    checkcommand();
                    // prompt(timetaken);
                    // printf("%s\n", inp);
                    break;
                } else if (c == 4) {
                    exit(0);
                } else {
                    printf("%d\n", c);
                }
            } else {
                inp[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();

        printf("\n");

        if(tab_flag == 1) {
            continue;
        }
        if(strlen(inp) == 0) {
            printf("\n");
            break;
        }
        strcat(inp, "\n");
        char copy[1000] = "";
        strcpy(copy, inp);
        // savehistory();
        token = strtok(inp, s);
        if (strcmp(inp, "exit") == 0)
        {
            savehistory();
            break;
        }
        // int k = 0;
        // token2 = strtok(input, ";\n");
        // while (token != NULL) {
        //     strcpy(semi_colon_sep_args[k], token2);
        //     token2 = strtok(NULL, ";\n");
        //     k++;
        // }
        int j = 0;
        while (token != NULL)
        {
            strcpy(args[i], token);
            if(strcmp(token,";") == 0 || strcmp(token,"&") == 0)
            {
                if (strcmp(token,"&") == 0) {
                    i++;
                    checkcommand();
                    // commands();
                    // thus, setting i to -1
                    i=-1;
                }
                else {
                    checkcommand();
                    // commands();
                    // thus, setting i to -1
                    i=-1;
                    // strcpy(binput, semi_colon_sep_args[j]);
                    // j++;
                }
            }
            token = strtok(NULL, s);
            i++;
        }
        checkcommand();
        // commands();
        if (strcmp(prev_command, copy) != 0) {
            writetohistory(copy);
        }
        strcpy(prev_command, copy);
    }
    return 0;
}
