#ifndef THREAD_POOL_H_INCLUDED
#define THREAD_POOL_H_INCLUDED

#ifndef HTTP_H_INCLUDED
    #include "HTTP.h"
#endif // HTTP_H_INCLUDED

#ifndef BLOCKING_QUEUE_H_INCLUDED
    #include "blocking_queue.h"
#endif // BLOCKING_QUEUE_H_INCLUDED

#define BUFLEN 1024

typedef short n_threads;
typedef void*(*_thread_function)(void*);

typedef struct {
    n_threads num_threads;
    int flag;
    pthread_t * thread_array;
    Queue * blocking_queue;
} thread_pool;

typedef struct {
    _thread_function function_;
    void * params;
} thread_task;


thread_pool * make_thread_pool(Queue *, unsigned int, _thread_function, void *);
void add_task(Queue *, mapping_function);


#endif // THREAD_POOL_H_INCLUDED
