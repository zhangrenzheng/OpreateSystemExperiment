#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define TOTAL 100
#define N 4                             //使用N个线程

struct param{
    int start;
    int end;
};

struct result{
    double sum;
};

void *worker(void *arg)
{
    struct param *param;
    struct result *result;
    double sum = 0;
    int i;
    param = (struct param *)arg;

    for(i = param->start; i < param->end; i ++)
    {
        if(i % 2 == 0)
            sum -= 1 / (2 * (double)i - 1);
        else
            sum += 1 / (2 * (double)i - 1);
    }
    result = malloc(sizeof(struct result));
    result->sum = sum;
    return result;
}

int main()
{
    int i = 0;
    double PI = 0;
    double sum = 0;

    pthread_t workers[N];
    struct param params[N];
    
    for(i = 0; i < N; i ++)
    {
        struct param *param;
        param = &params[i];
        param->start = i * (TOTAL / N) + 1; 
        param->end = (i + 1) * (TOTAL / N) + 1;
        pthread_create(&workers[i], NULL, worker, param);
    }

    for(i = 0; i < N; i ++)
    {
        struct result *result;
        pthread_join(workers[i], (void **)&result);
        sum += result->sum;
        free(result);
    }

    PI = sum * 4;
    printf("TOTAl = %d, PI = %f\n", TOTAL, PI);
    return 0;
}
