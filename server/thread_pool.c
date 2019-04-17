#include "thread_pool.h"

void * handle_task(void *);

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

thread_pool *
make_thread_pool(u_int n_threads) {
    if(n_threads <= 0)
        return NULL;
    thread_pool * th_pool;
    if((th_pool = malloc(sizeof(thread_pool)))) {
        BlockingQueue * blocking_queue = make_queue();
        if(blocking_queue != NULL){
            th_pool->num_threads = n_threads;
            u_int i;
            pthread_t _thread;
            th_pool->blocking_queue = blocking_queue;
            for(i = 0; i < n_threads; ++i) {
               pthread_create(&_thread, NULL, (_thread_function*)&handle_task, (void*)&th_pool);
            }
            sleep(3);
            return th_pool;
        }
    }
    free(th_pool);
    return NULL;
}

int
add_task(thread_pool * pool, _thread_function function_, void * param) {
    thread_task * task;
    if((task = malloc(sizeof(thread_task)))) {
        task->function_ = function_;
        task->params = param;
        return queue_put(pool->blocking_queue, (void*) task);
    }
    return 1;
}
int
stop_executor(thread_pool * __restrict pool) {
    BlockingQueue * _q = pool->blocking_queue;
    ON_FLAG(_q->_th_signal);
    pthread_cond_broadcast(__ADDRESS(_q, condition));
    free_executor(pool);
    return 0;
}

void
free_executor(thread_pool * __restrict pool) {
    BlockingQueue * _q = pool->blocking_queue;
    free_queue(_q);
    free(pool);
}

void *
handle_task(void * param) {

    thread_pool * pool = (thread_pool *) param;
    BlockingQueue * queue = pool->blocking_queue;

    while(INFINITE_LOOP) {

        thread_task * task = (thread_task *) queue_pop(queue);

        if(AND_FLAG(queue->_th_signal) == 1)
            return NULL;
        _thread_function function_ = task->function_;

        function_(task->params);

        free(task);
    }

    return NULL;
}
