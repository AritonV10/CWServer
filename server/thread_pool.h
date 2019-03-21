#ifndef THREAD_POOL_H_INCLUDED
#define THREAD_POOL_H_INCLUDED

#ifndef HTTP_H_INCLUDED
    #include "HTTP.h"
#endif // HTTP_H_INCLUDED

#ifndef BLOCKING_QUEUE_H_INCLUDED
    #include "blocking_queue.h"
#endif // BLOCKING_QUEUE_H_INCLUDED

#include <pthread.h>


typedef short n_threads;
typedef void*(*_thread_function)(void*);

typedef struct {
    n_threads num_threads;
    int flag;
    pthread_t * thread_array;
    BlockingQueue * blocking_queue;
} thread_pool;

typedef struct {
    _thread_function function_;
    void * params;
} thread_task;


thread_pool * make_thread_pool(unsigned int);
int add_task(thread_pool *, _thread_function, void *);


#endif // THREAD_POOL_H_INCLUDED
