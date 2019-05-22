#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define ARRAYSIZE 20
#define _DEBUG_MODE

int array[ARRAYSIZE];

void initArray(int array[], int n)
{
    int i = 0;
    for(i = 0; i < n; i ++)
    {
        array[i] = 1 + rand() % 100;
    }
}

void printArray(int array[], int n)
{
    int i = 0;
    for(i = 0; i < n; i ++)
    {   
        printf("%3d", array[i]);
    }
    printf("\n");
}

void master()
{
    int i = 0, j = 0;
    int temp;
    for(i = 0; i < (ARRAYSIZE / 2); i ++)
    {
        for(j = i; j < (ARRAYSIZE / 2); j ++)
        {
            if(array[i] > array[j])
            {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}

void *worker()
{
    int i = 0, j = 0;
    int temp;
    for(i = (ARRAYSIZE / 2); i < ARRAYSIZE; i ++)
    {
        for(j = i; j < ARRAYSIZE; j ++)
        {
            if(array[i] > array[j])
            {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}

void merge()
{
    int tempArray[ARRAYSIZE];
    int i = 0, j = (ARRAYSIZE / 2), k = 0;
    while(i != (ARRAYSIZE / 2)  && j != ARRAYSIZE)
    {
        if(array[i] < array[j])
            tempArray[k ++] = array[i ++];
        else
            tempArray[k ++] = array[j ++];
    }
    while(i != (ARRAYSIZE / 2))
        tempArray[k ++] = array[i ++];
    while(j != ARRAYSIZE)
        tempArray[k ++] = array[j ++];
    for(i = 0; i < ARRAYSIZE; i ++)
        array[i] = tempArray[i];
}

int main()
{   
    initArray(array, ARRAYSIZE);
    printf("Before sort:\n");
    printArray(array, ARRAYSIZE);


    pthread_t worker_tid;
    pthread_create(&worker_tid, NULL, worker, NULL);
    master();
    pthread_join(worker_tid, NULL);

    printf("After sorted:\n");
    printArray(array, ARRAYSIZE);

    merge();
    printf("After merge sorted:\n");
    printArray(array, ARRAYSIZE);
    return 0;
}
