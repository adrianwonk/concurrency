#ifndef MAIN_H
#define MAIN_H
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <signal.h>
#include <stdatomic.h>
#include <unistd.h>

#define THREADN 4
#define AMT 1000000000
#define UPPER_BOUND 11

extern atomic_int thread_state;

void asyncPrintf(const char *s, ...);

#endif

