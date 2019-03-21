#include "thread_pool.h"

static void * handle_task(void *);

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////

thread_pool * make_thread_pool(unsigned int n_threads) {
    if(n_threads <= 0)
        return NULL;
    thread_pool * th_pool;
    if((th_pool = malloc(sizeof(thread_pool)))) {
        BlockingQueue * blocking_queue = make_queue();
        if(blocking_queue != NULL){
            th_pool->num_threads = n_threads;
            u_int i;
            for(i = 0; i < n_threads; ++i) {
               pthread_t _thread;
               pthread_create(&_thread, NULL, &handle_task, (void*)th_pool);
            }
            return th_pool;
        }
        free(blocking_queue);
    }
    free(th_pool);
    return NULL;
}

int add_task(thread_pool * pool, _thread_function function_, void * param) {
    thread_task * task;
    if((task = malloc(sizeof(thread_task)))) {
        task->function_ = function_;
        task->params = param;
        return queue_put(&pool->blocking_queue, (void*) task);
    }
    return 0;
}

static void * handle_task(void * param) {
    thread_pool * pool = (thread_pool *) param;
    BlockingQueue * queue = pool->blocking_queue;
    while(INFINITE_LOOP) {

        thread_task * task = (thread_task *) queue_pop(queue);
        _thread_function function_ = task->function_;
        #ifndef TH_POOL_DEBUG
            printf("[DEBUG]: param: %ui", &task->params);
        #endif // TH_POOL_DEBUG
        function_(task->params);
        free(task);
    }
    return NULL;
}
