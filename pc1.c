#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 4

int buffer1[CAPACITY] = {0};
int buffer2[CAPACITY] = {0};
int in = 0;
int out = 0;
int com = 0;

int buffer_is_empty()
{
    return com == out;
}

int buffer_is_full()
{
    return (in + 1) % CAPACITY == com;
}

int isProduced()
{
    return (buffer1[com] >= 97 && buffer1[com] <= 122);
}

int get_item()
{
    int item;

    item = buffer2[out];
    buffer1[out] = 0;
    out = (out + 1) % CAPACITY;
    return item;
}

void put_item(int item)
{
    buffer1[in] = item;
    in = (in + 1) % CAPACITY;
}

pthread_mutex_t mutex;
pthread_cond_t wait_empty_buffer;
pthread_cond_t wait_full_buffer;
pthread_cond_t compute_cond;

#define ITEM_COUNT (CAPACITY * 2)

void *consume(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        pthread_mutex_lock(&mutex);
        while (buffer_is_empty())
            pthread_cond_wait(&wait_full_buffer, &mutex);

        item = get_item(); 
        printf("        consume item: %c\n", item); 

        pthread_cond_signal(&wait_empty_buffer);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *produce(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        pthread_mutex_lock(&mutex);
        while (buffer_is_full()) 
            pthread_cond_wait(&wait_empty_buffer, &mutex);

        item = 'a' + i;
        put_item(item);
        printf("produce item: %c\n", item); 

        pthread_cond_signal(&compute_cond);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void *compute(void *arg)
{
    int i;
    for(i = 0; i < ITEM_COUNT; i ++)
    {
        pthread_mutex_lock(&mutex);
        while(!isProduced())
            pthread_cond_wait(&compute_cond, &mutex);

        buffer2[com] = buffer1[com] - 32;
        printf("    compute item: %c\n", buffer2[com]);
        com = (com + 1) % CAPACITY;

        pthread_cond_signal(&wait_full_buffer);
        pthread_mutex_unlock(&mutex);

    }
    return NULL;
}

int main()
{ 
    pthread_t consumer_tid;
    pthread_t computer_tid;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&wait_empty_buffer, NULL);
    pthread_cond_init(&wait_full_buffer, NULL);
    pthread_cond_init(&compute_cond, NULL);

    pthread_create(&computer_tid, NULL, compute, NULL);
    pthread_create(&consumer_tid, NULL, consume, NULL);
    produce(NULL);
    pthread_join(computer_tid, NULL);
    pthread_join(consumer_tid, NULL);

    pthread_mutex_destroy(&mutex);

    return 0;
}
