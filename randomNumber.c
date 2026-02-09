#include "randomNumber.h"
#include "dispatch.h"
#include "main.h"
// Resources per random number
int freq[UPPER_BOUND];
pthread_mutex_t locks[UPPER_BOUND];
atomic_bool initialised = false;

int generateNum(int i, int *seed){
    int x = ((double) rand_r(seed + i) / RAND_MAX * UPPER_BOUND);
    if (x==UPPER_BOUND)
        x = 10;

    // RACE CONDITION for modifying freq[i]. Serialise access to each index. (Mutex lock)
    pthread_mutex_lock(locks+x);
    freq[x] += 1;
    pthread_mutex_unlock(locks+x);
    return x;
}

void init_generator(){
    if (initialised) return;
    for (int i = 0; i < UPPER_BOUND; i++){
        pthread_mutex_init(locks+i,NULL);
        freq[i] = 0;
    }
    initialised = true;
}

void printResults(){
    for (int i = 0; i < UPPER_BOUND; i++){
        asyncPrintf("%d: occured %d times\n", i, freq[i]);
    }

    int sum = 0;
    for (int i = 0; i < UPPER_BOUND; i++){
        sum += freq[i];
    }
    asyncPrintf("%d numbers generated\n",sum);
}
