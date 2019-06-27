#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *worker(void *fd)
{
    int workerSum = 0;
    int i = 51;
    for(; i <= 100; ++i)
        workerSum += i;
    FILE *writefile = fdopen(*(int *)fd, "w");
    fprintf(writefile, "%d", workerSum);
    fclose(writefile);
}

int master()
{
    int masterSum = 0;
    int i = 1;
    for(; i <= 50; ++i)
        masterSum += i;
    return masterSum;
}

int main()
{
    int fd[2];
    pipe(fd);
    int masterSum = 0;
    int workerSum = 0;
    int sum = 0;
    pthread_t worker_tid;
    pthread_create(&worker_tid, NULL, worker, &fd[1]);
    masterSum = master();
    FILE *readfile = fdopen(fd[0], "r");
    fscanf(readfile, "%d", &workerSum);
    pthread_join(worker_tid, NULL);

    printf("workerSum = %d, masterSum = %d.\n", workerSum, masterSum);
    printf("sum = %d\n", workerSum + masterSum);
    return 0;
}
