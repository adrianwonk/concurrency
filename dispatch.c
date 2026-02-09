#include "dispatch.h"
#include "main.h"
#include "randomNumber.h"
#include <semaphore.h>
// Create resources owned per thread
pthread_t thread_pool[THREADN];
int thread_count[THREADN];
int thread_seed[THREADN];
int thread_id[THREADN];

sem_t init_sem;

void init_threads(int *threads_initialised){
    if (*threads_initialised == 1) return;
    sem_init(&init_sem, 0, 0);
    
    for (int i = 0; i < THREADN; i++){
        thread_id[i] = i;
        thread_count[i] = 0;
        thread_seed[i] = 66*i;
        pthread_create(thread_pool+i, NULL, consumer, thread_id+i);
    }
    
    for (int i = 0; i < THREADN; i++)
        sem_wait(&init_sem);

    asyncPrintf("finished initialising threads!\n");
    *threads_initialised = 1;
}

void *consumer(void *ptr){
    const int id = *(int *)ptr;

    asyncPrintf("initialised thread %d, now sleeping\n", id);
    sem_post(&init_sem);
    while (thread_state == 0){
        sleep(1);
    }
    asyncPrintf("thread %d finished sleeping, now generating numbers \n");

    // Each thread can generate a number independently. Each thread only has to generate AMT / THREADN numbers
    for (int i = 0; i < AMT / THREADN && thread_state == 1; i++){
        int x = generateNum(id, thread_seed + id);
        thread_count[id]++;
    }
    asyncPrintf("TERMINATING thread %d. Generated %d times\n", id, thread_count[id]);
}
void start_threads(){
    thread_state = 1;
}
void join_threads(){
    for (int i = 0; i < THREADN; i++)
        pthread_join(thread_pool[i], NULL);
}
