#ifndef DISPATCH_H
#define DISPATCH_H

#include "main.h"
void init_threads(int *);
void *consumer(void *);
void join_threads();

#endif
