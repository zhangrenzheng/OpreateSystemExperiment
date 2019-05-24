#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define N 10
#define _DEBUG_MODE                                     //Comment this line to cancel debugging

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
sema_t t_mutex[N];
int number = 1;
int temp = 0;

void *t(void *arg)
{
    sema_wait(&mutex_sema);
#ifdef _DEBUG_MODE
    printf("# DEBUG_MODE: temp = %d\n", temp);
#endif
    sema_wait(&t_mutex[temp % N]);

    printf("Thread %d Receive: %d\n", (temp + 1) % N, number);
    ++ number;
    printf("    Thread %d Send: %d\n", (temp + 1) % N, number);

    t_mutex[temp % N].value = 0;
    
    sema_signal(&t_mutex[(++ temp) % N]);
    sema_signal(&mutex_sema);

    return NULL;
}


int main()
{
    int i;
    pthread_t t_tid[N];
    sema_init(&mutex_sema, 1);
    sema_init(&t_mutex[0], 1);
    for(i = 1; i < N; ++ i)
        sema_init(&t_mutex[i], 0);
    
    for(i = 0; i < (2 * N); ++ i)
        pthread_create(&t_tid[i % N], NULL, t, (void *)&temp);

    for(i = 0; i < N; ++ i)
        pthread_join(t_tid[i], NULL);

    return 0;
}

