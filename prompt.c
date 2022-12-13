#include "commands.h"
#include "headers.h"
#include "utils.h"
#include "prompt.h"
#include "processes.h"

void prompt(double timetakenforproc)
{
    char name[33];
    char hostname[500];
    char currentdirectory[500];
    gethostname(hostname, 500);
    getlogin_r(name, 32);
    getcwd(currentdirectory, sizeof(currentdirectory));
    printf("\033[0;31m");
    if (timetakenforproc < 1.0) {
    if (strcmp(currentdirectory, homedirectory) == 0)
    {
        
        printf("<%s@%s:~> ", name, hostname);
    }
    else if (strstr(currentdirectory, homedirectory))
    {
        printf("<%s@%s:~/%s> ", name, hostname, currentdirectory + strlen(homedirectory) + 1);
    }
    else
    {
        printf("<%s@%s:-%s> ", name, hostname, currentdirectory);
    }
    }


    else {
    if (strcmp(currentdirectory, homedirectory) == 0)
    {
        printf("<%s@%s:~took %d s> ", name, hostname, (int)(timetaken));
    }
    else if (strstr(currentdirectory, homedirectory))
    {
        printf("<%s@%s:~/%s took %d s> ", name, hostname, currentdirectory + strlen(homedirectory) + 1, (int)(timetaken));
    }
    else
    {
        printf("<%s@%s:-%s took %d s> ", name, hostname, currentdirectory, (int)(timetaken));
    }
    }
    printf("\033[0m");
}