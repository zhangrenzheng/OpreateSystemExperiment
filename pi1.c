#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define N 100                           // 莱布尼级数项数

double workerSum = 0,
       masterSum = 0,
       PI = 0;

void *worker(void *arg)
{
    int i;
    for(i = (N / 2) + 1; i <= N; i ++)
    {
        if((i % 2) == 0)
            workerSum -= 1 / (2 * (double)i  - 1);
        else
            workerSum += 1 / (2 * (double)i  - 1);
    }
}

void master()
{
   int i;
   for(i = 1; i <= (N / 2); i ++)
   {
        if((i % 2) == 0)
            masterSum -= 1 / (2 * (double)i  - 1);
        else
            masterSum += 1 / (2 * (double)i  - 1);

   }
}

int main()
{
    pthread_t worker_tid;
    pthread_create(&worker_tid, NULL, worker, NULL);
    master();
    pthread_join(worker_tid, NULL);
    printf("workerSum = %f, mastersum = %f.\n", workerSum, masterSum);
    PI = (workerSum + masterSum) * 4;
    printf("N = %d, PI = %f\n", N, PI);
    return 0;
}
