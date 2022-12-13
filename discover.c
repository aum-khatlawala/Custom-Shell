#include "commands.h"
#include "headers.h"
#include "utils.h"
#include "discover.h"

char init_target_dir[2000];

void discover() {
        int df = 0;
        int ff = 0;
        int iterator = 1;
        char target_dir[2000];
        char pwd[2000];
        getcwd(pwd, sizeof(pwd));
        char temp[1000];
        getcwd(temp, sizeof(temp));
        strcpy(target_dir, pwd);
        char file_name[1000] = "";
        int filef = 0;
        // int folderf = 0;
        while (iterator < i) {
            if (strcmp(args[iterator], "-d") == 0) {
                df = 1;
            } else if (strcmp(args[iterator], "-f") == 0) {
                ff = 1;
            } else if (strcmp(args[iterator], "-df") == 0) {
                df = 1;
                ff = 1;
            } else if (strcmp(args[iterator], "-fd") == 0) {
                ff = 1;
                df = 1;
            } else if (strcmp(args[iterator], ".") == 0) {
                chdir(temp);
                getcwd(target_dir, sizeof(target_dir));
                // folderf = 1;
            } else if (strcmp(args[iterator], "~") == 0) {
                chdir(homedirectory);
                getcwd(target_dir, sizeof(target_dir));
                // folderf = 1;
            } else if (strlen(args[iterator]) > 2 && strchr(args[iterator], '~') != NULL) {
                chdir(homedirectory);
                chdir(args[iterator] + 2);
                getcwd(target_dir, sizeof(target_dir));
                // folderf = 1;
            } else if (strchr(args[iterator], '/') != NULL) {
                // handles the cases where dir_name is given case
                int y_or_n = chdir(args[iterator]);
                if (y_or_n == -1) {
                    printf("Directory doesn't exist\n");
                    return;
                } else {
                    getcwd(target_dir, sizeof(target_dir));
                    // folderf = 1;
                }
            } else if (strcmp(args[iterator], "..") == 0) {
                chdir("..");
                getcwd(target_dir, sizeof(target_dir));
                // folderf = 1;
            } else if (strchr(args[iterator], '"') != NULL) {
                filef = 1;
                strcpy(file_name, args[iterator] + 1);
                file_name[strlen(file_name) - 1] = '\0';
            }
            iterator++;
        }
        int f_d_none = 0; // 0 for none, 1 for file and 2 for directory
        if (filef == 1) {
            if (ff == 1) {
                ff = 0;
                f_d_none = 1;
            }
            if (df == 1) {
                df = 0;
                f_d_none = 2;
            }
            if (ff == 1 && df == 1) {
                ff = 0;
                df = 0;
                f_d_none = 0;
            }
        }

        int fdf = 0;
        if (ff == 0 && df == 0) {
            fdf = 1;
        }
        if (ff == 1 && df == 1) {
            fdf = 1;
        }
        if (filef == 1) {
            fdf = 0;
        }
        if (fdf == 1) {
            printf("%s\n", target_dir);
            strcpy(init_target_dir, target_dir);
            discoverallfilesindirectory(target_dir);
        }
        else if (ff == 1) {
            strcpy(init_target_dir, target_dir);
            discoverallfilesindirectory(target_dir);
        }
        else if (df == 1) {
            printf("%s\n", target_dir);
            strcpy(init_target_dir, target_dir);
            discoveralldirectoriesindirectory(target_dir);
        }
        else if (filef == 1) {
            strcpy(init_target_dir, target_dir);
            discoverfileindirectory(target_dir, file_name, f_d_none);
        }

        chdir(pwd);

}

void discoverallfilesindirectory(char * dir_path) {
    // printf("%s\n", dir_path);
    DIR *dir;
    struct dirent *list;
    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("");
        return;
    }
    while ((list = readdir(dir)) != NULL) {
        if (list -> d_type == DT_DIR) {
            if (strcmp(list -> d_name, ".") != 0 && strcmp(list -> d_name, "..") != 0) {
                char subdir_path[1000] = "";
                strcpy(subdir_path, dir_path);
                strcat(subdir_path, "/");
                strcat(subdir_path, list -> d_name);
                discoverallfilesindirectory(subdir_path);
            }
        }
        else {
            char target[1000] = "";
            strcpy(target, dir_path);
            strcat(target, "/");
            strcat(target, list -> d_name);
            printf(".%s\n", target + strlen(init_target_dir));
        }
    }
}

void discoveralldirectoriesindirectory(char * dir_path) {
    DIR *dir;
    struct dirent *list;
    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("");
        return;
    }
    while ((list = readdir(dir)) != NULL) {
        if (list -> d_type == DT_DIR) {
            if (strcmp(list -> d_name, ".") != 0 && strcmp(list -> d_name, "..") != 0) {
                char subdir_path[1000] = "";
                strcpy(subdir_path, dir_path);
                strcat(subdir_path, "/");
                strcat(subdir_path, list -> d_name);
                printf(".%s\n", subdir_path + strlen(init_target_dir));
                discoveralldirectoriesindirectory(subdir_path);
            }
        }
    }
}

void discoverfileindirectory(char * dir_path, char * filename, int f_d_or_none) {
    DIR *dir;
    struct dirent *list;
    dir = opendir(dir_path);
    if (dir == NULL) {
        perror("");
        return;
    }
    while ((list = readdir(dir)) != NULL) {
        if (f_d_or_none == 0) {
        if (list -> d_type == DT_DIR) {
            if (strcmp(filename, list -> d_name) == 0) {
                char target[1000] = "";
                strcpy(target, dir_path);
                strcat(target, "/");
                strcat(target, list -> d_name);
                printf(".%s\n", target + strlen(init_target_dir));
            }
            else {
            if (strcmp(list -> d_name, ".") != 0 && strcmp(list -> d_name, "..") != 0) {
                char subdir_path[1000] = "";
                strcpy(subdir_path, dir_path);
                strcat(subdir_path, "/");
                strcat(subdir_path, list -> d_name);
                discoverfileindirectory(subdir_path, filename, f_d_or_none);
            }
            }
        }
        else {
            if (strcmp(filename, list -> d_name) == 0) {
                char target[1000] = "";
                strcpy(target, dir_path);
                strcat(target, "/");
                strcat(target, list -> d_name);
                printf(".%s\n", target + strlen(init_target_dir));
            }
        }
        }

        if (f_d_or_none == 1) {
        if (list -> d_type == DT_DIR) {
            if (strcmp(list -> d_name, ".") != 0 && strcmp(list -> d_name, "..") != 0) {
                char subdir_path[1000] = "";
                strcpy(subdir_path, dir_path);
                strcat(subdir_path, "/");
                strcat(subdir_path, list -> d_name);
                discoverfileindirectory(subdir_path, filename, f_d_or_none);
            }
        }
        else {
            if (strcmp(filename, list -> d_name) == 0) {
                char target[1000] = "";
                strcpy(target, dir_path);
                strcat(target, "/");
                strcat(target, list -> d_name);
                printf(".%s\n", target + strlen(init_target_dir));
            }
        }
        }

        if (f_d_or_none == 2) {
        if (list -> d_type == DT_DIR) {
            if (strcmp(filename, list -> d_name) == 0) {
                char target[1000] = "";
                strcpy(target, dir_path);
                strcat(target, "/");
                strcat(target, list -> d_name);
                printf(".%s\n", target + strlen(init_target_dir));
            }
            else {
            if (strcmp(list -> d_name, ".") != 0 && strcmp(list -> d_name, "..") != 0) {
                char subdir_path[1000] = "";
                strcpy(subdir_path, dir_path);
                strcat(subdir_path, "/");
                strcat(subdir_path, list -> d_name);
                discoverfileindirectory(subdir_path, filename, f_d_or_none);
            }
            }
        }
        else {}
        }
    }
}