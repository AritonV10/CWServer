#ifndef BLOCKING_QUEUE_H_INCLUDED
#define BLOCKING_QUEUE_H_INCLUDED

#include <pthread.h>
#include <stdlib.h>

#define __SINLINE static inline

typedef enum { STATE_EMPTY, STATE_NOT_EMPTY } state;;

struct QNode {
    void * data;
    struct QNode * next;
};

typedef struct {
    struct QNode * first;
    pthread_mutex_t lock;
    pthread_cond_t condition;
    state state_;
    int nodes;
} BlockingQueue;

BlockingQueue * msake_queue();
void * queue_pop(BlockingQueue *);
int queue_put(BlockingQueue *, void *);
void free_queue(BlockingQueue *);


#endif // BLOCKING_QUEUE_H_INCLUDED
