/*----------------------------------------------------------------------------------------------------------------------
    pthread_rwlock_xxx fonksiyonları ile okuma ve yazma kilitlemeleri yapılabilir
----------------------------------------------------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define QUEUE_SIZE      10

void exit_sys(const char *msg);
void exit_sys_thread(const char *msg, int errno);
void *thread_proc_producer(void *param);
void *thread_proc_consumer(void *param);
void *thread_proc_searcher(void *param);

sem_t g_sem_producer, g_sem_consumer;
pthread_rwlock_t g_rwlock;
int g_queue[QUEUE_SIZE];
int g_head, g_tail;
int g_count;

int main(int argc, char **argv)
{
    pthread_t tid_producer1, tid_producer2, tid_consumer, tid_searcher1, tid_searcher2;
    int errnum;
    
    if ((errnum = pthread_rwlock_init(&g_rwlock, NULL)) < 0)
        exit_sys_thread("pthread_rwlock_init", errnum);    
    
    if ((errnum = sem_init(&g_sem_producer, 0, QUEUE_SIZE)) < 0)
        exit_sys_thread("sem_init", errnum);
    
    if ((errnum = sem_init(&g_sem_consumer, 0, 0)) < 0)
        exit_sys_thread("sem_init", errnum);
    
    if ((errnum = pthread_create(&tid_producer1, NULL, thread_proc_producer, NULL)) < 0)
        exit_sys_thread("pthread_create", errnum);
    
    if ((errnum = pthread_create(&tid_producer2, NULL, thread_proc_producer, NULL)) < 0)
        exit_sys_thread("pthread_create", errnum);

    if ((errnum = pthread_create(&tid_consumer, NULL, thread_proc_consumer, NULL)) < 0)
        exit_sys_thread("pthread_create", errnum);        
    
    
    if ((errnum = pthread_create(&tid_searcher1, NULL, thread_proc_searcher, NULL)) < 0)
        exit_sys_thread("pthread_create", errnum);        

    if ((errnum = pthread_create(&tid_searcher2, NULL, thread_proc_searcher, NULL)) < 0)
        exit_sys_thread("pthread_create", errnum);      


    if (pthread_join(tid_producer1, NULL) < 0)
        exit_sys("pthread_join");
    
    if (pthread_join(tid_producer2, NULL) < 0)
        exit_sys("pthread_join");
    
    if (pthread_join(tid_consumer, NULL) < 0)
        exit_sys("pthread_join");    
    
    if ((errnum = sem_destroy(&g_sem_producer)) < 0)
        exit_sys_thread("sem_destroy", errnum);
    
    if ((errnum = sem_destroy(&g_sem_consumer)) < 0)
        exit_sys_thread("sem_destroy", errnum);
    
    if ((errnum = pthread_rwlock_destroy(&g_rwlock)) < 0)
        exit_sys_thread("pthread_rwlock_destroy", errnum);
    
    return 0;
}

void *thread_proc_producer(void *arg)
{    
    for (;;) {
        
        usleep(rand() % 300000);
        
        sem_wait(&g_sem_producer);
        
        /* wrlock yapılacak*/
        pthread_rwlock_wrlock(&g_rwlock);
        g_queue[g_tail++] = g_count++;
        g_tail %= QUEUE_SIZE;        
        pthread_rwlock_unlock(&g_rwlock);        
        
        sem_post(&g_sem_consumer);       
        
        if (g_count >= 99)
            break;        
        
    }
    fprintf(stderr, "producer ends..\n");
    
    return NULL;
}

void *thread_proc_consumer(void *arg)
{
    int val;
    
    for (;;) {
        sem_wait(&g_sem_consumer);
    
        pthread_rwlock_wrlock(&g_rwlock);
        val = g_queue[g_head++];
        g_head %= QUEUE_SIZE;
        pthread_rwlock_unlock(&g_rwlock);
        sem_post(&g_sem_producer);

        printf("%d ", val);
        fflush(stdout);
        usleep(rand() % 30000);
        if (val == 99)
            break;
    }    
    fprintf(stderr, "consumer ends..\n");
    
    return NULL;
}

void *thread_proc_searcher(void *arg)
{
    int i;
    int sum = 0;
    
    for (;;) {
        sleep(3);        
        pthread_rwlock_rdlock(&g_rwlock);       
        for (i = 0; i < QUEUE_SIZE; ++i) {
            fprintf(stderr, "val=%d\n", g_queue[i]);
            sum += g_queue[i];       
        }
        
        fprintf(stderr, "Total:%d\n", sum);                    
        pthread_rwlock_unlock(&g_rwlock);
    }
    
    return NULL;
}

void exit_sys(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void exit_sys_thread(const char *msg, int errno)
{
    fprintf(stderr, "%s:%s\n", msg, strerror(errno));
    exit(EXIT_FAILURE);
}
