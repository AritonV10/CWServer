#include "blocking_queue.h"


__SINLINE struct QNode * make_node(void *);

////////////////////////////////////////////////
////////////////////////////////////////////////


BlockingQueue * make_queue() {
    BlockingQueue * queue;
    if((queue = malloc(sizeof(BlockingQueue)))){
        if((pthread_mutex_init(&queue->lock, NULL)) == 0) {
            if((pthread_cond_init(&queue->condition, NULL)) == 0) {
                queue->first = NULL;
                queue->state_ = STATE_EMPTY;
                queue->nodes = 0;
                return queue;
            }
        }
    }
    // to do: exit the program and update the logs
    pthread_mutex_destroy(&queue->lock);
    pthread_cond_destroy(&queue->condition);
    free(queue);
    return NULL;
}

int queue_put(BlockingQueue * queue, void * data) {

    pthread_mutex_lock(&queue->lock);
    struct QNode * node = queue->first;

    while(node != NULL)
        node = node->next;

    node = make_node(data);

    if(node == NULL) {
        pthread_mutex_unlock(&queue->lock);
        // warn that there is not enough memory
        return 1;
    }

    queue->state_ = STATE_NOT_EMPTY;
    queue->nodes += 1;

    pthread_mutex_unlock(&queue->lock);
    pthread_cond_signal(&queue->condition);
    return 0;
}

void * queue_pop(BlockingQueue * queue) {

    pthread_mutex_lock(&queue->lock);
    while(queue->state_ == STATE_EMPTY) {
        printf("\nWaiting..\n");
        pthread_cond_wait(&queue->condition, &queue->lock);
    }
    struct QNode * first = queue->first;
    void * data_ = first->data;

    queue->first = queue->first->next;

    first->next = NULL;
    free(first);
    queue->nodes -= 1;
    if(queue->nodes == 0)
        queue->state_ = STATE_EMPTY;
    pthread_mutex_unlock(&queue->lock);
    return data_;
}

void free_queue(BlockingQueue * queue) {
    struct QNode * head = queue->first;
    while(head != NULL) {
        struct QNode * temp = head;
        free(head->data);
        free(head);
        head = head->next;
    }
    pthread_mutex_destroy(&queue->lock);
    pthread_cond_destroy(&queue->condition);
    free(queue);
}



////////////////////////////////////////////////
////////////////////////////////////////////////


__SINLINE struct QNode * make_node(void * data_) {
    struct QNode * new_;
    if((new_ = malloc(sizeof(struct QNode)))) {
        new_->data = data_;
        new_->next = NULL;
        return new_;
    }
    return NULL;
}
