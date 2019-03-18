#include "blocking_queue.h"


static inline void make_node(void *, QNOde *);

////////////////////////////////////////////////
////////////////////////////////////////////////

int put(Queue * queue, void * data) {
    pthread_mutex_lock(&queue->lock);
    struct QNode * first = queue->first;
    struct QNode * new_;
    if(first != NULL) {
        while(first != NULL) {
            first = first->next;
        }
        make_node(data, &new_);
    } else {
        make_node(data, &new_);
    }
    if(new_ != NULL) {
        first = new_;
    } else {
        return 1;
        pthread_mutex_unlock(&queue->lock);
    }
    pthread_mutex_unlock(&queue->lock);
    return 0;
}

void * pop(Queue * queue) {
    pthread_mutex_lock(&queue->lock);
    while(queue->)
    struct QNode * first = queue->first;
    void * data_;
    if(first->next != NULL) {
        struct QNode * next = first->next;
        data_ = first->next->data;
        queue->first = next;
    } else {
        data_ = first->data;
    }
    first = NULL;
    first->next = NULL;
    free(first);
    pthread_mutex_lock(&queue->lock);
    return data_;

}

void free_queue(Queue * queue) {
    struct QNode * head;
    while(head != NULL) {
        struct QNode * temp = head;
        free(temp->data);
        free(temp);
        head = head->next;
    }
    free(queue);
}

////////////////////////////////////////////////
////////////////////////////////////////////////

static inline void make_node(void * data_, QNode * new_) {
    if((*new_ = malloc(sizeof(struct Node)))) {
        *new_->data;
    } else {
        free(node;)
        *new_ = NULL;
    }
}
