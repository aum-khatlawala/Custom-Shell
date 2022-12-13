#include "commands.h"
#include "headers.h"
#include "utils.h"
#include "discover.h"
#include "processes.h"
#include "p_info.h"
#include "history.h"
#include "redirs.h"

void redir_command() {
    char command[200] = "";
    for (int j = 0; j < i; j++) {
        strcat(command, args[j]);
        strcat(command, " ");
    }
    // printf("%s\n", command);
    int inputredir = 0;
    int outputredir = 0;
    int append = 0;
    char inputfilename[200];
    char outputfilename[200];
    char buffer[200];
    strcpy(buffer, command);
    char *com = strtok(buffer, " ");
    while(com != NULL) {
        // printf("%s\n", com);
        if (strcmp(com, "<") == 0) {
            inputredir = 1;
            com = strtok(NULL, " ");
            if (com == NULL) {
                printf("No input file inputted\n");
                return;
            }
            else {
                strcpy(inputfilename, com);
            }
        }
        if (strcmp(com, ">") == 0) {
            outputredir = 1;
            com = strtok(NULL, " ");
            if (com == NULL) {
                printf("No output file inputted\n");
                return;
            }
            else {
                strcpy(outputfilename, com);
            }
        }
        if  (strcmp(com, ">>") == 0) {
            outputredir = 1;
            append = 1;
            com = strtok(NULL, " ");
            if (com == NULL) {
                printf("No output file inputted\n");
                return;
            }
            else {
                strcpy(outputfilename, com);
            }
        }
        com = strtok(NULL, " ");
    }



    if (inputredir == 1 && outputredir == 1) {
        int inputfile = open(inputfilename, O_RDONLY);
        if (inputfile >= 0) {
            dup2(inputfile, STDIN_FILENO);
            int outputfile;
            if (append == 1) {
                outputfile = open(outputfilename, O_WRONLY | O_APPEND | O_CREAT, 0644);
            }
            else {
                outputfile = open(outputfilename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
            }
            if (outputfile >= 0) {
                dup2(outputfile, STDOUT_FILENO);
                close(inputfile);
                close(outputfile);
            }
            else {
                printf("Could not open output file\n");
                return;
            }
        }
        else {
            printf("Could not open input file\n");
            return;
        }
    }
    else if (inputredir == 1) {
        int inputfile = open(inputfilename, O_RDONLY);
        if (inputfile >= 0) {
            dup2(inputfile, STDIN_FILENO);
            close(inputfile);
        }
        else {
            printf("Could not open input file\n");
            return;
        }
    }
    else if (outputredir == 1) {
        int outputfile;
        if (append == 1) {
            outputfile = open(outputfilename, O_WRONLY | O_APPEND | O_CREAT, 0644);
        }
        else {
            outputfile = open(outputfilename, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        }
        if (outputfile >= 0) {
            dup2(outputfile, STDOUT_FILENO);
            close(outputfile);
        }
        else {
            printf("Could not open output file\n");
            return;
        }
    }
    commands();
    return;
}
void pipe_command() {
    // printf ("grfedw\n");
    char command[200] = "";
    for (int j = 0; j < i; j++) {
        strcat(command, args[j]);
        strcat(command, " ");
    }
    char buffer[100];
    int num_pipes = 0;
    int normal_stdin = dup(STDIN_FILENO);
    int normal_stdout = dup(STDOUT_FILENO);
    strcpy(buffer, command);
    // printf("%s\n", buffer);
    for (int j = 0; j < strlen(command); j++) {
        if (command[j] == '|') {
            num_pipes++;
        }
    }
    int pipe_array[num_pipes][2];
    for (int j = 0; j < num_pipes; j++) {
        if (pipe(pipe_array[j]) < 0) {
            printf("Error in creating pipe\n");
            return;
        }
        // pipe_num = j;
        // commands();
    }
    // printf("ytgrcexw\n");
    char *arg = strtok(buffer, "|");
    // printf("%s\n", arg);
    pipe_num = 0;
    dup2(pipe_array[0][1], STDOUT_FILENO);
    close(pipe_array[0][1]);
    commands();
    arg = strtok(NULL, "|");
    int j = 1;
    for (; j < num_pipes; j++) {
        dup2(pipe_array[j - 1][0], STDIN_FILENO);
        dup2(pipe_array[j][1], STDOUT_FILENO);
        close(pipe_array[j - 1][0]);
        close(pipe_array[j][1]);
        pipe_num = j;
        commands();
        arg = strtok(NULL, "|");
    }
    dup2(pipe_array[num_pipes - 1][0], STDIN_FILENO);
    close(pipe_array[num_pipes - 1][0]);
    dup2(normal_stdout, STDOUT_FILENO);
    pipe_num = j;
    commands();
    dup2(normal_stdin, STDIN_FILENO);
    return;


    // char *arg = strtok(buffer, "|");
    // dup2(pipe_array[0][1], STDOUT_FILENO);
    // close(pipe_array[0][1]);
    // // close(pipe_array[0][1]);
    // pipe_num = 0;
    // commands();
    // // printf("%s\n", arg);
    // arg = strtok(NULL, "|");
    // int j = 1;
    // printf("arg_val: %s", arg);
    // // printf("hello\n");
    // for (; j < num_pipes; j++) {
    //     dup2(pipe_array[j - 1][0], STDIN_FILENO);
    //     dup2(pipe_array[j][1], STDOUT_FILENO);
    //     close(pipe_array[j - 1][0]);
    //     close(pipe_array[j][1]);
    //     pipe_num = j;
    //     commands();
    //     arg = strtok(NULL, "|");
    // }
    // dup2(pipe_array[num_pipes - 1][0], STDIN_FILENO);
    // close(pipe_array[num_pipes - 1][0]);
    // dup2(normal_stdout, STDOUT_FILENO);
    // pipe_num = j;
    // commands();
    // dup2(normal_stdin, STDIN_FILENO);
    // return;





    // char *saveptr;
    // char *arg = strtok_r(buffer, "|", &saveptr);
    // printf("args: %s\n", arg);
    // printf("Write end: %d\n", pipe_array[0][1]);
    // dup2(pipe_array[0][1], STDOUT_FILENO);
    // int j = 0;
    // pipe_num = j;
    // commands();
    // j++;
    // arg = strtok_r(NULL, "|", &saveptr);
    // printf("args: %s\n", arg);
    // for (; j < num_pipes; j++) {
    //     dup2(pipe_array[j - 1][0],  STDIN_FILENO);
    //     printf("Read end: %d\n", pipe_array[j - 1][0]);

    //     // printf("hello\n");
    //     dup2(pipe_array[j][1], STDOUT_FILENO);
    //     printf("Write end: %d\n", pipe_array[j][1]);
    //     pipe_num = j;
    //     commands();
    //     arg = strtok_r(NULL, "|", &saveptr);
    // }
    // dup2(pipe_array[num_pipes - 1][0], STDIN_FILENO);
    // dup2(normal_stdout, STDOUT_FILENO);
    // pipe_num = j;
    // commands();
    // dup2(normal_stdin, STDIN_FILENO);
    // return;
}