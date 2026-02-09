#include "main.h"
#include "dispatch.h"
#include "randomNumber.h"
#include <stdarg.h>

// Resources for main thread
int threads_initialised = 0;
atomic_int thread_state = 0;
void signalHandler(int sig);

int main(){
    signal(SIGINT, signalHandler);
    asyncPrintf("starting program\n");
    
    asyncPrintf("initialising threads\n");
    init_threads(&threads_initialised);

    asyncPrintf("starting threads\n");
    thread_state++;

    join_threads();
    asyncPrintf("all threads joined\n");

    printResults();


    return 0;
}

void signalHandler(int sig) {
    thread_state = 2;
}

pthread_mutex_t IO_lock = PTHREAD_MUTEX_INITIALIZER;
void asyncPrintf(const char *s, ...){
    va_list args;
    va_start (args, s);

    pthread_mutex_lock(&IO_lock);
    vprintf(s, args);
    printf("thread state: %d\n\n", thread_state);
    pthread_mutex_unlock(&IO_lock);
}
