#ifndef THREAD_POOL_H_INCLUDED
#define THREAD_POOL_H_INCLUDED

#ifndef HTTP_H_INCLUDED
    #include "HTTP.h"
#endif // HTTP_H_INCLUDED

#ifndef BLOCKING_QUEUE_H_INCLUDED
    #include "blocking_queue.h"
#endif // BLOCKING_QUEUE_H_INCLUDED

#ifndef G_DEFINES_H_INCLUDED
    #include "g_defines.h"
#endif // G_DEFINES_H_INCLUDED
#include <pthread.h>


typedef short n_threads;
typedef void*(*_thread_function)(void*);

typedef struct {
    u_int   flag : 1;
    n_threads num_threads;
    pthread_t * thread_array;
    BlockingQueue * blocking_queue;
} thread_pool;

typedef struct {
    void * params;
    _thread_function function_;
} thread_task;


thread_pool * make_thread_pool(unsigned int);
int add_task(thread_pool * __restrict, _thread_function, void *);
int stop_executor(thread_pool * __restrict);
void free_executor(thread_pool *);

#endif // THREAD_POOL_H_INCLUDED
