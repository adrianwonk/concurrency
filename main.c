#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>

#define THREADN 4
#define AMT 1000000
#define UPPER_BOUND 11

void signalHandler(int sig);
// Resources per random number
int freq[UPPER_BOUND];
pthread_mutex_t locks[UPPER_BOUND];

// Resources for main thread
int threads_initialised = 0;
int stop = 0;

// Resource for IO
pthread_mutex_t IO_lock = PTHREAD_MUTEX_INITIALIZER;

// Create resources owned per thread
pthread_t thread_pool[THREADN];
int thread_count[THREADN];
int thread_seed[THREADN];
int thread_id[THREADN];


void generateNum(int i){
    int x = ((double) rand_r(thread_seed + i) / RAND_MAX * UPPER_BOUND);
    if (x==UPPER_BOUND)
        x = 10;

    // RACE CONDITION for modifying freq[i]. Serialise access to each index. (Mutex lock)
    pthread_mutex_lock(locks+x);
    freq[x] += 1;
    pthread_mutex_unlock(locks+x);
}

void *consumer(void *ptr){
    signal(SIGINT, signalHandler);
    int id = *(int *)ptr;

    pthread_mutex_lock(&IO_lock);
    printf("initialised thread %d\n", id);
    pthread_mutex_unlock(&IO_lock);

    // Each thread can generate a number independently. Each thread only has to generate AMT / THREADN numbers
    for (int i = 0; i < AMT / THREADN && stop != 1; i++){
        generateNum(id);
        thread_count[id]++;
    }

    pthread_mutex_lock(&IO_lock);
    printf("Thread %d terminating. Generated %d times\n", id, thread_count[id]);
    pthread_mutex_unlock(&IO_lock);
}

void init_threads(){
    if (threads_initialised == 1) return;
    
    for (int i = 0; i < THREADN; i++){
        thread_id[i] = i;
        thread_count[i] = 0;
        thread_seed[i] = 66*i;
        pthread_create(thread_pool+i, NULL, consumer, thread_id+i);
        pthread_detach(thread_pool[i]);
    }

    threads_initialised = 1;
    pthread_mutex_lock(&IO_lock);
    printf("finished initialising threads!\n");
    pthread_mutex_unlock(&IO_lock);
}

void signalHandler(int sig) {
    stop = 1;
}

int main(){
    for (int i = 0; i < UPPER_BOUND; i++){
        pthread_mutex_init(locks+i,NULL);
        freq[i] = 0;
    }


    signal(SIGINT, signalHandler);
    init_threads();

    pthread_mutex_lock(&IO_lock);
    printf("main thread waiting\n");
    pthread_mutex_unlock(&IO_lock);

    for (int i = 0; i < THREADN; i++)
        pthread_join(thread_pool[i], NULL);

    printf("main thread finished waiting\n");

    for (int i = 0; i < UPPER_BOUND; i++){
        printf("%d: occured %d times\n", i, freq[i]);
    }

    int sum = 0;
    for (int i = 0; i < UPPER_BOUND; i++){
        sum += freq[i];
    }
    printf("%d numbers generated\n",sum);

    return 0;
}
