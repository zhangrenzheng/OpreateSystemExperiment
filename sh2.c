#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>

int judge(char array[], char ch)
{
    int i = 0;
    while(array[i] != '\0')
    {
        if(array[i] == ch)
            break;
        ++ i;
    }
    if(array[i] == ch)
        return 1;
    else
        return 0;
}

void mysys(char *command)
{
    pid_t pid;

    if(command == NULL)
    {
        printf("Error: Wrong Shell string!\n");
        exit(1);
    }

    pid = fork();

    if(pid == 0)
        execl("/bin/sh", "sh", "-c", command, NULL);

    wait(NULL);
}

int main()
{
    char command[100];
    char cmd[100];
    char param[100];
    char subcmd[100];
    char file[100];
    int counter = 0,
        i = 0;

    while(1)
    {
        printf("> ");
        counter = 0,
        i = 0;

        gets(command);

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

        if(!strcmp(cmd, "exit"))
            break;
        else if(!strcmp(cmd , "cd"))
            chdir(param);
            else
            {
                //printf("command: %s\n", command);
                //printf("%d\n", judge(command, '>'));
                if(judge(command, '>'))
                {
                    i = 0;
                    counter = 0;
                    while(command[i] != '>')
                    {
                        subcmd[counter] = command[i];
                        ++ counter;
                        ++ i;
                    }
                    subcmd[counter] = '\0';

                    counter = 0;
                    while(command[i] == ' ' || command[i] == '>')
                    {
                        ++ i;
                    }
                    while(command[i] != '\0')
                    {
                        file[counter] = command[i];
                        ++ i;
                        ++ counter;
                    }
                    file[counter] = '\0';
                    //printf("subcmd: %s, file: %s\n", subcmd, file);
                    int fd = open(file, O_CREAT|O_RDWR, 0644);
                    int std = dup(1);
                    dup2(fd, 1);
                    mysys(subcmd);
                    dup2(std, 1);

                }
                else
                    mysys(command);
            }
    }
    return 0;
}
