#include "blocking_queue.h"

__SINLINE struct QNode * make_node(void *);

////////////////////////////////////////////////
////////////////////////////////////////////////


BlockingQueue *
make_queue() {
    BlockingQueue * queue;
    if((queue = malloc(sizeof(BlockingQueue)))){

        if((pthread_mutex_init(__ADDRESS(queue, lock), NULL)) == 0) {

            if((pthread_cond_init(__ADDRESS(queue, condition), NULL)) == 0) {
                queue->first = NULL;
                queue->_state = 0;
                queue->_th_signal = 0;
                queue->nodes = 0;
                return queue;
            }

            pthread_mutex_destroy(__ADDRESS(queue, lock));
        }

        free(queue);
    }

    return NULL;
}

int
queue_put(BlockingQueue * __restrict queue, void * data) {
    pthread_mutex_lock(__ADDRESS(queue, lock));
    struct QNode * node = queue->first;

    if(queue->first == NULL) {

        queue->first = make_node(data);

    } else {

        while(node->next != NULL)
            node = node->next;

        node->next = make_node(data);

        if(node->next == NULL) {
            pthread_mutex_unlock(__ADDRESS(queue, lock));
            return 1;
        }
    }
    printf("Added value\n");
    ON_FLAG(queue->_state);
    queue->nodes += 1;
    pthread_mutex_unlock(__ADDRESS(queue, lock));
    pthread_cond_signal(__ADDRESS(queue, condition));

    return 0;
}

void *
queue_pop(BlockingQueue * __restrict queue) {

    pthread_mutex_lock(__ADDRESS(queue, lock));
    while(AND_FLAG(queue->_state) == 0) {
        printf("\nWaiting..\n");
        pthread_cond_wait(__ADDRESS(queue, condition), __ADDRESS(queue, lock));
        if(AND_FLAG(queue->_th_signal) == 1)
            goto off;

    }

    struct QNode * first = queue->first;
    if(first == NULL)
        return NULL;
    void * data_ = first->data;

    queue->first = queue->first->next;

    free(first);

    queue->nodes -= 1;

    if(queue->nodes == 0)
        OFF_FLAG(queue->_state);

    pthread_mutex_unlock(__ADDRESS(queue, lock));
    return data_;

    off:
        pthread_mutex_unlock(__ADDRESS(queue, lock));
        return NULL;
}

void
free_queue(BlockingQueue * queue) {
    struct QNode * head = queue->first;
    while(head != NULL) {
        struct QNode * temp = head;
        free(head->data);
        free(head);
        head = head->next;
    }
    pthread_mutex_destroy(__ADDRESS(queue, lock));
    pthread_cond_destroy(__ADDRESS(queue, condition));
    free(queue);
}



////////////////////////////////////////////////
////////////////////////////////////////////////


__SINLINE struct QNode *
make_node(void * data_) {
    struct QNode * new_;
    if((new_ = malloc(sizeof(struct QNode)))) {
        new_->data = data_;
        new_->next = NULL;
        return new_;
    }
    return NULL;
}
