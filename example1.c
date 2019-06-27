#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    int pid;    
    int fd[2];
    int sum = 0;
    char buf[32];

    pipe(fd); 
    pid = fork();
    if (pid == 0) { 
        // child
        dup2(fd[1], 1); 
        close(fd[0]);
        close(fd[1]);
		int s = 10;
        fprintf(stdout, "%d", s);
        exit(0);
    }
    // parent
    dup2(fd[0], 0); 
    close(fd[0]);
    close(fd[1]);

    fscanf(stdin, "%d", &sum);
    printf("sum = %d\n", sum);
   
    return 0;
}
