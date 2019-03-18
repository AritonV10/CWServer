#ifndef BLOCKING_QUEUE_H_INCLUDED
#define BLOCKING_QUEUE_H_INCLUDED

#include <pthread.h>

struct QNode {
    void * data;
    struct QNode * next;
};

typedef struct {
    struct QNode * first;
    pthread_mutex_t lock;
} Queue;
//
void * queue_pop();
int queue_put(void *);


#endif // BLOCKING_QUEUE_H_INCLUDED
