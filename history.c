#include "commands.h"
#include "headers.h"
#include "utils.h"
#include "discover.h"
#include "history.h"

char hist_array[20][1000];

void savehistory() {
    FILE * fptr;
    char * filename = (char *)malloc(2000 * sizeof(char));
    char lastline[1000];
    strcpy(filename, homedirectory);
    strcat(filename, "/history.txt");
    fptr = fopen(filename, "w+");
    if (fptr == NULL) {
        perror("");
        return;
    }
    // printing num_history in file because we need to preserve the number of commands stored across multiple sessions
    fprintf(fptr, "%d\n", num_history);
    // printf("%d\n", num_history);
    for (int iter = 0; iter < num_history; iter++) {
        fprintf(fptr, "%s", hist_array[iter]);
    }
    fclose(fptr);
}

void writetohistory(char * tbw) {
    if (num_history < 20) {
        strcpy(hist_array[num_history], tbw);
        num_history++;
    }
    // if 20 commands are already there, first move every command up one place and then insert the lastest command at the end
    else {
        for (int iter = 0; iter < 20; iter++) {
            strcpy(hist_array[iter], hist_array[iter+1]);
        }
        strcpy(hist_array[19], tbw);
    }
    // for (int iter = 0; iter < num_history; iter++) {
    //     printf("%s\n", hist_array[iter]);
    // }
}

void loadhistory() {
    FILE * fptr;
    char * filename = (char *)malloc(2000 * sizeof(char));
    char lastline[1000];
    strcpy(filename, homedirectory);
    strcat(filename, "/history.txt");
    fptr = fopen(filename, "r+");
    if (fptr == NULL) {
        perror("");
        return;
    }
    char * buffer = (char *)malloc(1000 * sizeof(char));
    size_t buffer_size = 1000;
    getline(&buffer, &buffer_size, fptr);
    num_history = atoi(buffer);
    // printf("%d\n", num_history);
    for (int iter = 0; iter < num_history; iter++) {
        getline(&buffer, &buffer_size, fptr);
        strcpy(hist_array[iter], buffer);
    }
    fclose(fptr);
}

void printhistory() {
    
    if (num_history <= 10) {
        for (int iter = 0; iter < num_history; iter++) {
            printf("%s", hist_array[iter]);
        }
    }
    else {
        int start_point = num_history - 10;
        for (int iter = start_point; iter < num_history; iter++) {
            printf("%s", hist_array[iter]);
        }
    }
    if (strcmp(hist_array[num_history - 1], "history\n") != 0) {
        printf("history\n");
    }
    // writetohistory("history\n");
}