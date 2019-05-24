#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

void mysys(char *command)
{
    pid_t pid;

    if(command == NULL)
    {
        printf("Error: Wrong Shell string!\n");
        exit(1);
    }

    char cmd[100];
    char param[100];
    int counter = 0,
        i = 0;

    while(command[i] == ' ')
    {
        i ++;
    }

    while(command[i] != ' ')
    {
        cmd[counter] = command[i];
        counter ++;
        i ++;
    }
    cmd[counter] = '\0';

    while(command[i] == ' ')
    {
        i ++;
    }

    counter = 0;
    while(command[i] != '\0')
    {
        param[counter] = command[i];
        counter ++;
        i ++;
    }
    param[counter] = '\0';

    pid = fork();

    if(pid == 0)
        execlp(cmd, cmd, param, NULL);

    wait(NULL);
}

int main()
{
    printf("--------------------------------------------------\n");
    mysys("echo HELLO WORLD");
    printf("--------------------------------------------------\n");
    mysys("ls /");
    printf("--------------------------------------------------\n");
    return 0;
}
