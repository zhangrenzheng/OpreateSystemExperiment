#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

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

typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sema_t;

void sema_init(sema_t *sema, int value)
{
    sema->value = value;
    pthread_mutex_init(&sema->mutex, NULL);
    pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    while (sema->value <= 0)
        pthread_cond_wait(&sema->cond, &sema->mutex);
    sema->value --;
    pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    ++ sema->value;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}

sema_t mutex_sema;
sema_t empty_buffer_sema;
sema_t full_buffer_sema;
sema_t compute_cond;

#define ITEM_COUNT (CAPACITY * 2)

void *consume(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        sema_wait(&full_buffer_sema);
        sema_wait(&mutex_sema);

        item = get_item();
        printf("        consume item: %c\n", item); 

        sema_signal(&mutex_sema);
        sema_signal(&empty_buffer_sema);
    }

    return NULL;
}

void *produce()
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        sema_wait(&empty_buffer_sema);
        sema_wait(&mutex_sema);

        item = i + 'a';
        put_item(item);
        printf("produce item: %c\n", item); 

        sema_signal(&mutex_sema);
        //sema_signal(&full_buffer_sema);
        sema_signal(&compute_cond);

        
    }

    return NULL;
}

void *compute(void *arg)
{
    int i;
    for(i = 0; i < ITEM_COUNT; i ++)
    {
        sema_wait(&compute_cond);
        sema_wait(&mutex_sema);

        buffer2[com] = buffer1[com] - 32;
        printf("    compute item: %c\n", buffer2[com]);
        com = (com + 1) % CAPACITY;

        sema_signal(&mutex_sema);
        sema_signal(&full_buffer_sema);

    }
    return NULL;
}


int main()
{ 
    pthread_t consumer_tid;
    pthread_t computer_tid;

    sema_init(&mutex_sema, 1);
    sema_init(&empty_buffer_sema, CAPACITY - 1);
    sema_init(&compute_cond, 0);
    sema_init(&full_buffer_sema, 0);

    pthread_create(&consumer_tid, NULL, consume, NULL);
    pthread_create(&computer_tid, NULL, compute, NULL);
    produce();

    pthread_join(consumer_tid, NULL);
    pthread_join(computer_tid, NULL);
    return 0;
}
