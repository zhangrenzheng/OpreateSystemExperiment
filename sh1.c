#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int mysys(char *command)
{
    pid_t pid;
    int i = 0;
    int counter = 0;
    int error;
    int returnValue = 0;
    int index = 0;
    char cmd[5][20];
    char param[5][50];

    while(command[i] != '\n' && command[i] != '\0')
    {
        i ++;
    }
    if(command[i] == '\n')
        command[i] = '\0';
    i = 0;

    while(command[i] == ' ')
        i ++;
    
    while(command[i] != ' ' && command[i] != '\0')
    {
        cmd[index][counter] = command[i];
        counter ++;
        i ++;
    }
    cmd[index][counter] = '\0';
    counter = 0;

    while(command[i] == ' ')
        i ++;

    while(command[i] != '\0')
    {
        param[index][counter] = command[i];
        counter ++;
        i ++;
    }
    param[index][counter] = '\0';

    //printf("cmd = %s, param = %s\n", cmd[index], param[index]);

    
    if(!strcmp(cmd[index], "cd"))
    {
        chdir(param[index]);
        return returnValue;
    }
    

    if(!strcmp(cmd[index], "exit"))
    {
        returnValue = 1;
        return returnValue;
    }

    if(!strcmp(cmd[index], "pwd"))
    {
        pid = fork();
        if (pid == 0)
        {
            error = execlp(cmd[index], cmd[index], NULL);
            if (error < 0)
                perror("execl");
        }
        wait(NULL);
        return returnValue;
    }

    pid = fork();
    if (pid == 0)
    {
        error = execlp(cmd[index], cmd[index], param[index], NULL);
        if (error < 0)
            perror("execl");
    }
    wait(NULL);
    
    return returnValue;
}

int main()
{
    char command[200];
    int returnValue;

    while(1)
    {
        printf("> ");
        fgets(command, 200, stdin);
        returnValue = mysys(command);
        
        if(returnValue == 1)
            break;
    }
}
