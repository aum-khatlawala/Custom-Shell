#include "commands.h"
#include "headers.h"
#include "utils.h"
#include "discover.h"
#include "processes.h"
#include "p_info.h"
#include "history.h"
#include "redirs.h"
// #include "tab_op.h"

int redir_flag() {
    for (int j = 0; j < i; j++) {
        for (int k = 0; k < strlen(args[j]); k ++) {
            if (args[j][k] == '<' || args[j][k] == '>') {
                return 1;
            }
        }
    }
    return 0;
}

int pipe_flag() {
    for (int j = 0; j < i; j++) {
        for (int k = 0; k < strlen(args[j]); k ++) {
            if (args[j][k] == '|') {
                return 1;
            }
        }
    }
    return 0;
}

void checkcommand() {
    int normal_stdin = dup(STDOUT_FILENO);
    int normal_stdout = dup(STDIN_FILENO);
    int redir = redir_flag();
    int pipe = pipe_flag();
    // printf("%d\n%d\n\n", redir, pipe);
    if (redir == 1) {
        redir_command();
    }
    else if (pipe == 1) {
        pipe_command();
    }
    else {
        commands();
    }
    dup2(normal_stdin, STDIN_FILENO);
    dup2(normal_stdout, STDOUT_FILENO);
}
// echo 
void commands() {
    // if (tab_flag == 1) {
    //     tab_op();
    // }
    // printf("Hi");
    char temp[1000][1000];
    int i_dash = i;
    for(int j = 0; j < i; j++) {
        // printf("hi\n");
        strcpy(temp[j], args[j]);
        // printf("%s\n", temp[j]);
    }
    // printf("\n");
    for(int j = 0; j < i; j++) {
        if (strcmp(temp[j], ">") == 0 || strcmp(temp[j], "<") == 0 || strcmp(temp[j], ">>") == 0) {
            i = j;
            break;
        }
        strcpy(args[j], temp[j]);
        // printf("%s ", args[j]);
    }
    int pipe_flag = 0;
    for (int j = 0; j < i; j++) {
        if(strcmp(args[j], "|") == 0) {
            pipe_flag = 1;
            break;
        }
    }
    if (pipe_flag == 1) {
        // printf("hi\n");
        char command[200] = "";
        for (int j = 0; j < i; j++) {
            strcat(command, args[j]);
            strcat(command, " ");
        }
        char *piper = strtok(command, "|");
        char pipearr[100][100];
        int j = 0;
        while (piper != NULL) {
            strcpy(pipearr[j], piper);
            piper = strtok(NULL, "|");
            j++;
        }
        char current_pipe[100];
        strcpy(current_pipe, pipearr[pipe_num]);
        // printf("%s\n", current_pipe);
        // return;
        char *tokenisepipe = strtok(current_pipe, " ");
        j = 0;
        // printf("args:\n");
        while (tokenisepipe != NULL) {
            strcpy(args[j], tokenisepipe);
            // printf("%s\n", args[j]);
            tokenisepipe = strtok(NULL, " ");
            j++;
        }
        // printf("\n");
        i = j;
        // return;
        // int normal_stdin = dup(STDOUT_FILENO);
        // int normal_stdout = dup(STDIN_FILENO);
        // int redir = redir_flag();
        // printf("%d\n%d\n\n", redir, pipe);
        // if (redir == 1) {
        //     redir_command();
        // }
        // dup2(normal_stdin, STDIN_FILENO);
        // dup2(normal_stdout, STDOUT_FILENO);
    }
    if (args[i-1][0] == '&') {
            i--;
            process(1);
    } else if (strcmp(args[0], "echo") == 0) {
        // printf("echo\n");
        for (int j = 1; j < i; j++) {
            printf("%s ", args[j]);
        }
        printf("\n");
    } else if (strcmp(args[0], "cd") == 0) {
        char temp[1000];
        getcwd(temp, sizeof(temp));
        if (i == 1) {
            chdir(homedirectory);
        } else if (strcmp(args[1], ".") == 0) {
            // printf("%s\n", temp);
            chdir(temp);
        } else if (strcmp(args[1], "~") == 0) {
            chdir(homedirectory);
        } else if (strlen(args[1]) > 2 && strchr(args[1], '~') != NULL) {
            chdir(homedirectory);
            chdir(args[1] + 2);
        } else if (strcmp(args[1], "-") == 0) {
            printf("%s\n", previousdirectory);
            chdir(previousdirectory);
        } else {
            // handles the cases where dir_name is given and the ".." case
            int y_or_n = chdir(args[1]);
            if (y_or_n == -1) {
                printf("Directory doesn't exist\n");
            }
        }
        strcpy(previousdirectory, temp);
    } else if (strcmp(args[0], "pwd") == 0) {
        char pwd[2000];
        getcwd(pwd, sizeof(pwd));
        printf("%s\n", pwd);
    } else if (strcmp(args[0], "ls") == 0) {
        char pwd[2000];
        getcwd(pwd, sizeof(pwd));
        int iterator = 1;
        int lf = 0;
        int af = 0;
        int num_files_folders_tbl = 0;
        char files_folders_tbl[1000][1000];
        struct stat stats;
        while (iterator < i) {
            if (strcmp(args[iterator], "-l") == 0) {
                lf = 1;
            } else if (strcmp(args[iterator], "-a") == 0) {
                af = 1;
            } else if (strcmp(args[iterator], "-la") == 0) {
                lf = 1;
                af = 1;
            } else if (strcmp(args[iterator], "-al") == 0) {
                af = 1;
                lf = 1;
            } else if (strcmp(args[iterator], "~") == 0) {
                strcpy(files_folders_tbl[num_files_folders_tbl], homedirectory);
                num_files_folders_tbl++;
            } else {
                strcpy(files_folders_tbl[num_files_folders_tbl], args[iterator]);
                num_files_folders_tbl++;
            }
            iterator++;
        }
        if (num_files_folders_tbl == 0) {
            strcpy(files_folders_tbl[num_files_folders_tbl], pwd);
            num_files_folders_tbl++;
        }
        for (int f_num = 0; f_num < num_files_folders_tbl; f_num++) {
            // printf("yay");
            if (files_folders_tbl[f_num][0] == '~') {
                char temp[1000];
                memmove(files_folders_tbl[f_num], files_folders_tbl[f_num] + 1, strlen(files_folders_tbl[f_num]));
                strcpy(temp, files_folders_tbl[f_num]);
                strcpy(files_folders_tbl[f_num], homedirectory);
                // strcat(files_folders_tbl[f_num], "/");
                strcat(files_folders_tbl[f_num], temp);

            }
            int filef = 0;
            DIR * dir_tbl;
            // printf("%s\n", files_folders_tbl[f_num]);
            dir_tbl = opendir(files_folders_tbl[f_num]);
            if (dir_tbl == NULL) {
                if (access(files_folders_tbl[f_num], F_OK) != 0) {
                    perror("");
                    continue;
                }
            }
            struct dirent * list;
            struct stat fs;
            int r, s;
            r = stat(files_folders_tbl[f_num], & fs);
            struct dirent **namelist;
            if (!S_ISDIR(fs.st_mode)) {
                filef = 1;
            }
            if (filef == 0) {
                dir_tbl = opendir(files_folders_tbl[f_num]);
                s = scandir(files_folders_tbl[f_num], &namelist, 0, alphasort);
                if (s < 0) {
                    perror("Scandir");
                }
                if (lf == 0) {
                    for (int iter = 0; iter < s; iter++) {
                        r = stat(namelist[iter]->d_name, & fs);
                        if (S_ISREG(fs.st_mode)) {
                            if (af == 0) {
                                if (namelist[iter] -> d_name[0] != '.') {
                                    if (tab_flag == 0) {
                                        if ((fs.st_mode & S_IXUSR) && (fs.st_mode & S_IXGRP) && (fs.st_mode & S_IXOTH))
                                        {
                                        printf("\033[0;31m");
                                        printf("%s\n", namelist[iter] -> d_name);
                                        printf("\033[0m");
                                        }
                                        else {
                                        printf("\033[0;32m");
                                        printf("%s\n", namelist[iter] -> d_name);
                                        printf("\033[0m");
                                        }
                                    }
                                    else {
                                        // printf("%s ", partial_string);
                                        // printf("%d ", strlen(partial_string));
                                        // printf("%s ", namelist[iter] -> d_name);
                                        int tab_out = strncmp(partial_string, namelist[iter] -> d_name, strlen(partial_string)-1);
                                        // printf("%d\n", tab_out);
                                        if (tab_out == 0)
                                        {
                                            // num_matched++;
                                            // strcpy(str_matched, namelist[iter] -> d_name);
                                            if ((fs.st_mode & S_IXUSR) && (fs.st_mode & S_IXGRP) && (fs.st_mode & S_IXOTH))
                                            {
                                            printf("\033[0;31m");
                                            printf("%s\n", namelist[iter] -> d_name);
                                            printf("\033[0m");
                                            }
                                            else {
                                            printf("\033[0;32m");
                                            printf("%s\n", namelist[iter] -> d_name);
                                            printf("\033[0m");
                                            }
                                        }
                                    }
                                }
                            } else {
                                if ((fs.st_mode & S_IXUSR) && (fs.st_mode & S_IXGRP) && (fs.st_mode & S_IXOTH))
                                    {
                                    printf("\033[0;31m");
                                    printf("%s\n", namelist[iter] -> d_name);
                                    printf("\033[0m");
                                    }
                                    else {
                                    printf("\033[0;32m");
                                    printf("%s\n", namelist[iter] -> d_name);
                                    printf("\033[0m");
                                }
                            }
                        }
                        // else if (r == 0 && fs.st_mode & S_IXUSR) {
                        //     printf("\033[0;31m");
                        //     printf("%s\n", list -> d_name);
                        //     printf("\033[0m");
                        // }
                        else if (S_ISDIR(fs.st_mode)) {
                            if (af == 0) {
                                if (tab_flag == 0) {
                                    if (namelist[iter] -> d_name[0] != '.') {
                                        printf("\033[0;34m");
                                        printf("%s\n", namelist[iter] -> d_name);
                                        printf("\033[0m");
                                    }
                                } else {
                                    int tab_out = strncmp(partial_string, namelist[iter] -> d_name, strlen(partial_string) - 1);
                                    if (tab_out == 0)
                                    {
                                        // num_matched++;
                                        // strcpy(str_matched, namelist[iter] -> d_name);
                                        if (namelist[iter] -> d_name[0] != '.') {
                                        printf("\033[0;34m");
                                        printf("%s\n", namelist[iter] -> d_name);
                                        printf("\033[0m");
                                        }
                                    }
                                }
                            } else {
                                    printf("\033[0;34m");
                                    printf("%s\n", namelist[iter] -> d_name);
                                    printf("\033[0m");
                                
                            }
                        }
                    }
                }
                if (lf == 1) {
                    // in the format:
                    //
                    // total 8
                    // drwxrwxr-x 2 aum aum 4096 أغس 31 02:05 test2
                    // drwxrwxr-x 2 aum aum 4096 أغس 31 14:39 test3
                    int total = 0;
                    char filepath[1000];
                    for (int iter = 0; iter < s; iter++) {
                        if (af == 0) {
                            if (namelist[iter] -> d_name[0] != '.') {
                                strcpy(filepath, files_folders_tbl[f_num]);
                                strcat(filepath, "/");
                                strcat(filepath, namelist[iter]-> d_name);
                                lstat(filepath, & stats);
                                total += stats.st_blocks;
                            }
                        } else {
                            strcpy(filepath, files_folders_tbl[f_num]);
                            strcat(filepath, "/");
                            strcat(filepath, namelist[iter] -> d_name);
                            lstat(filepath, & stats);
                            total += stats.st_blocks;
                        }
                    }
                    printf("total %d\n", total / 2);
                    dir_tbl = opendir(files_folders_tbl[f_num]);
                    for (int iter = 0; iter < s; iter++) {
                        if (af == 0) {
                            if (namelist[iter] -> d_name[0] != '.') {
                                char details[1000] = "";
                                strcpy(filepath, files_folders_tbl[f_num]);
                                strcat(filepath, "/");
                                strcat(filepath, namelist[iter] -> d_name);
                                lstat(filepath, & stats);
                                struct passwd * password = getpwuid(stats.st_uid);
                                struct group * group = getgrgid(stats.st_gid);
                                if (S_ISDIR(stats.st_mode)) {
                                    strcat(details, "d");
                                } else {
                                    strcat(details, "-");
                                }
                                if (stats.st_mode & S_IRUSR) {
                                    strcat(details, "r");
                                } else {
                                    strcat(details, "-");
                                }
                                if (stats.st_mode & S_IWUSR) {
                                    strcat(details, "w");
                                } else {
                                    strcat(details, "-");
                                }
                                if (stats.st_mode & S_IXUSR) {
                                    strcat(details, "x");
                                } else {
                                    strcat(details, "-");
                                }
                                if (stats.st_mode & S_IRGRP) {
                                    strcat(details, "r");
                                } else {
                                    strcat(details, "-");
                                }
                                if (stats.st_mode & S_IWGRP) {
                                    strcat(details, "w");
                                } else {
                                    strcat(details, "-");
                                }
                                if (stats.st_mode & S_IXGRP) {
                                    strcat(details, "x");
                                } else {
                                    strcat(details, "-");
                                }
                                if (stats.st_mode & S_IROTH) {
                                    strcat(details, "r");
                                } else {
                                    strcat(details, "-");
                                }
                                if (stats.st_mode & S_IWOTH) {
                                    strcat(details, "w");
                                } else {
                                    strcat(details, "-");
                                }
                                if (stats.st_mode & S_IXOTH) {
                                    strcat(details, "x");
                                } else {
                                    strcat(details, "-");
                                }
                                strcat(details, "\t");
                                char temp[100];
                                unsigned short int det = stats.st_nlink;
                                sprintf(temp, "%hu", det);
                                strcat(details, temp);
                                strcat(details, "\t");
                                char name[33];
                                char hostname[500];
                                gethostname(hostname, 500);
                                getlogin_r(name, 32);
                                strcat(details, name);
                                strcat(details, "\t");
                                strcat(details, hostname);
                                strcat(details, "\t");
                                long long det2 = stats.st_size;
                                sprintf(temp, "%lld", det2);
                                strcat(details, temp);
                                strcat(details, "\t");

                                time_t present = time(NULL);
                                struct tm ltime, ptime;
                                char time_tbl[100];
                                localtime_r( & stats.st_mtime, & ltime);
                                localtime_r( & present, & ptime);
                                if (ltime.tm_year != ptime.tm_year) {
                                    strftime(time_tbl, sizeof(time_tbl), "%b %e %Y", localtime( & stats.st_mtime));
                                } else {
                                    strftime(time_tbl, sizeof(time_tbl), "%b %e %H:%M", localtime( & stats.st_mtime));
                                }
                                strcat(details, time_tbl);
                                printf("%s ", details);
                                r = stat(namelist[iter] -> d_name, & fs);
                                if (S_ISREG(fs.st_mode)) {
                                    if ((fs.st_mode & S_IXUSR) && (fs.st_mode & S_IXGRP) && (fs.st_mode & S_IXOTH))
                                    {
                                    printf("\033[0;31m");
                                    printf("%s\n", namelist[iter] -> d_name);
                                    printf("\033[0m");
                                    }
                                    else{
                                    printf("\033[0;32m");
                                    printf("%s\n", namelist[iter] -> d_name);
                                    printf("\033[0m");
                                    }
                                } else if (S_ISDIR(fs.st_mode)) {
                                    if (namelist[iter] -> d_name[0] != '.') {
                                        printf("\033[0;34m");
                                        printf("%s\n", namelist[iter] -> d_name);
                                        printf("\033[0m");
                                }
                                }
                            }
                        } else {
                            char details[1000] = "";
                            strcpy(filepath, files_folders_tbl[f_num]);
                            strcat(filepath, "/");
                            strcat(filepath, namelist[iter] -> d_name);
                            lstat(filepath, & stats);
                            struct passwd * password = getpwuid(stats.st_uid);
                            struct group * group = getgrgid(stats.st_gid);
                            if (S_ISDIR(stats.st_mode)) {
                                strcat(details, "d");
                            } else {
                                strcat(details, "-");
                            }
                            if (stats.st_mode & S_IRUSR) {
                                strcat(details, "r");
                            } else {
                                strcat(details, "-");
                            }
                            if (stats.st_mode & S_IWUSR) {
                                strcat(details, "w");
                            } else {
                                strcat(details, "-");
                            }
                            if (stats.st_mode & S_IXUSR) {
                                strcat(details, "x");
                            } else {
                                strcat(details, "-");
                            }
                            if (stats.st_mode & S_IRGRP) {
                                strcat(details, "r");
                            } else {
                                strcat(details, "-");
                            }
                            if (stats.st_mode & S_IWGRP) {
                                strcat(details, "w");
                            } else {
                                strcat(details, "-");
                            }
                            if (stats.st_mode & S_IXGRP) {
                                strcat(details, "x");
                            } else {
                                strcat(details, "-");
                            }
                            if (stats.st_mode & S_IROTH) {
                                strcat(details, "r");
                            } else {
                                strcat(details, "-");
                            }
                            if (stats.st_mode & S_IWOTH) {
                                strcat(details, "w");
                            } else {
                                strcat(details, "-");
                            }
                            if (stats.st_mode & S_IXOTH) {
                                strcat(details, "x");
                            } else {
                                strcat(details, "-");
                            }
                            strcat(details, "\t");
                            char temp[100];
                            unsigned short int det = stats.st_nlink;
                            sprintf(temp, "%hu", det);
                            strcat(details, temp);
                            strcat(details, "\t");
                            char name[33];
                            char hostname[500];
                            gethostname(hostname, 500);
                            getlogin_r(name, 32);
                            strcat(details, name);
                            strcat(details, "\t");
                            strcat(details, hostname);
                            strcat(details, "\t");
                            long long det2 = stats.st_size;
                            sprintf(temp, "%lld", det2);
                            strcat(details, temp);
                            strcat(details, "\t");

                            time_t present = time(NULL);
                            struct tm ltime, ptime;
                            char time_tbl[100];
                            localtime_r( & stats.st_mtime, & ltime);
                            localtime_r( & present, & ptime);
                            if (ltime.tm_year != ptime.tm_year) {
                                strftime(time_tbl, sizeof(time_tbl), "%b %e %Y", localtime( & stats.st_mtime));
                            } else {
                                strftime(time_tbl, sizeof(time_tbl), "%b %e %H:%M", localtime( & stats.st_mtime));
                            }
                            strcat(details, time_tbl);
                            printf("%s ", details);
                            r = stat(namelist[iter] -> d_name, & fs);
                            if (S_ISREG(fs.st_mode)) {
                                if ((fs.st_mode & S_IXUSR) && (fs.st_mode & S_IXGRP) && (fs.st_mode & S_IXOTH))
                                {
                                    printf("\033[0;31m");
                                    printf("%s\n", namelist[iter] -> d_name);
                                    printf("\033[0m");
                                }
                                else{
                                printf("\033[0;32m");
                                printf("%s\n", namelist[iter] -> d_name);
                                printf("\033[0m");
                                }
                            } else if (S_ISDIR(fs.st_mode)) {
                                
                                printf("\033[0;34m");
                                printf("%s\n", namelist[iter] -> d_name);
                                printf("\033[0m");
                            }

                        }
                    }
                }
            } else {
                if (lf == 0) {
                    printf("%s\n", files_folders_tbl[f_num]);
                } else {
                    char filepath[1000];
                    // printf("hello");
                    strcpy(filepath, pwd);
                    strcat(filepath, "/");
                    strcat(filepath, files_folders_tbl[f_num]);
                    // strcpy(filepath, files_folders_tbl[f_num]);
                    lstat(filepath, & stats);
                    char details[1000] = "";

                    struct passwd * password = getpwuid(stats.st_uid);
                    struct group * group = getgrgid(stats.st_gid);
                    if (S_ISDIR(stats.st_mode)) {
                        strcat(details, "d");
                    } else {
                        strcat(details, "-");
                    }
                    if (stats.st_mode & S_IRUSR) {
                        strcat(details, "r");
                    } else {
                        strcat(details, "-");
                    }
                    if (stats.st_mode & S_IWUSR) {
                        strcat(details, "w");
                    } else {
                        strcat(details, "-");
                    }
                    if (stats.st_mode & S_IXUSR) {
                        strcat(details, "x");
                    } else {
                        strcat(details, "-");
                    }
                    if (stats.st_mode & S_IRGRP) {
                        strcat(details, "r");
                    } else {
                        strcat(details, "-");
                    }
                    if (stats.st_mode & S_IWGRP) {
                        strcat(details, "w");
                    } else {
                        strcat(details, "-");
                    }
                    if (stats.st_mode & S_IXGRP) {
                        strcat(details, "x");
                    } else {
                        strcat(details, "-");
                    }
                    if (stats.st_mode & S_IROTH) {
                        strcat(details, "r");
                    } else {
                        strcat(details, "-");
                    }
                    if (stats.st_mode & S_IWOTH) {
                        strcat(details, "w");
                    } else {
                        strcat(details, "-");
                    }
                    if (stats.st_mode & S_IXOTH) {
                        strcat(details, "x");
                    } else {
                        strcat(details, "-");
                    }
                    strcat(details, "\t");
                    // printf("%s\n", details);
                    char temp[100];
                    unsigned short int det = stats.st_nlink;
                    sprintf(temp, "%hu", det);
                    strcat(details, temp);
                    strcat(details, "\t");
                    char name[33];
                    char hostname[500];
                    gethostname(hostname, 500);
                    getlogin_r(name, 32);
                    strcat(details, name);
                    strcat(details, "\t");
                    strcat(details, hostname);
                    strcat(details, "\t");
                    long long det2 = stats.st_size;
                    sprintf(temp, "%lld", det2);
                    strcat(details, temp);
                    strcat(details, "\t");

                    time_t present = time(NULL);
                    struct tm ltime, ptime;
                    char time_tbl[100];
                    localtime_r( & stats.st_mtime, & ltime);
                    localtime_r( & present, & ptime);
                    if (ltime.tm_year != ptime.tm_year) {
                        strftime(time_tbl, sizeof(time_tbl), "%b %e %Y", localtime( & stats.st_mtime));
                    } else {
                        strftime(time_tbl, sizeof(time_tbl), "%b %e %H:%M", localtime( & stats.st_mtime));
                    }
                    strcat(details, time_tbl);
                    printf("%s ", details);
                    printf("\033[0;32m");
                    printf("%s\n", files_folders_tbl[f_num]);
                    printf("\033[0m");
                }
            }
        }
    } else if (strcmp(args[0], "discover") == 0) {
        discover();
    } else if(strcmp(args[0], "pinfo") == 0) {
        p_info();
    } else if (strcmp(args[0], "history") == 0) {
        // writetohistory();
        printhistory();
    } else if (strcmp(args[0], "exit") == 0) {
        // savehistory();
    } else if (strcmp(args[0], "jobs") == 0) {
        jobs();
    } else if (strcmp(args[0], "sig") == 0) {
        sig();
    } else if (strcmp(args[0], "fg") == 0) {
        fg();
    } else if (strcmp(args[0], "bg") == 0) {
        bg();
    } else if (i == 0) {
    } else {
        if (args[i-1][0] == '&') {
            i--;
            process(1);
        } else {
            process(0);
            // printf("\n");
        }
    }
    for(int j = 0; j < i; j++) {
        strcpy(args[j], temp[j]);
    }
    i = i_dash;
}