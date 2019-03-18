#include "thread_pool.h"


static void * run_thread(Queue *, thread_pool *);

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

thread_pool * make_thread_pool(
    Queue * tasks_queue,
    unsigned int n_threads,
    _thread_function _function
    void * param) {
    pthread_crea
    if(n_threads <= 0)
        return NULL;
    thread_pool * th_pool;
    if((th_pool = malloc(sizeof(thread_pool)))) {
        th_pool->num_threads = n_threads;
        int i;
        for(i = 0; i < n_threads; ++i) {
            pthread_create(&th_pool->thread_array[i], _function, &param);
        }
    }
}

