#ifndef BLOCKING_QUEUE_H_INCLUDED
#define BLOCKING_QUEUE_H_INCLUDED

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#define __ADDRESS(queue, _elem) &(queue->_elem)

#ifndef G_DEFINES_H_INCLUDED
    #include "g_defines.h"
#endif // G_DEFINES_H_INCLUDED



struct QNode {
    void * data;
    struct QNode * next;
};

typedef struct {
    u_int _state     : 1;
    u_int _th_signal : 1;
    u_int nodes;
    struct QNode * first;
    pthread_mutex_t lock;
    pthread_cond_t condition;
} BlockingQueue;

BlockingQueue * make_queue();
void * queue_pop(BlockingQueue * __restrict);
int queue_put(BlockingQueue * __restrict, void *);
void free_queue(BlockingQueue *);

#endif // BLOCKING_QUEUE_H_INCLUDED
