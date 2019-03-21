#include <stdio.h>
#include <stdlib.h>
#include "blocking_queue.h"

void * task1(void * p) {
    BlockingQueue * q = (BlockingQueue*) p;
    while(1) {
        int x = (int)queue_pop(q);
        printf("%d\n", x);
    }
    return NULL;
}

void * task(void * p) {
    printf("Test");
    int x = 5;
    int * c = (int*) p;
    *c += x;
}

int main(void) {
   // char * test = "GET /path/path2?param=9 HTTP/1.1\nHost: localhost:8888\nConnection: keep-alive\nCache-Control: max-age=0\nUpgrade-Insecure-Requests: 1\nUser-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64)\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\nAccept-Encoding: gzip, deflate\n\r\n\r\n\0";
    //size_t s;
    /*
    Map * map = create_map();
    map_put(map, "sum", (void*)&sum);
    func function_ = map_get_value(map, "sum");
    printf("%i", function_(2, 3));
    */
    //RequestHeader * h = parse_header(test);
    //char * HTTP_TEST = "GET \test?query=1&query=2 HTTP\1.1\nHost: "
    //start_server("", "");


/*
    for(int i = 0; i < 16; ++i) {
        if(h->headers->buckets[i] != 0)
            printf("%s\n", h->headers->buckets[i]->key_);
    }
    printf("%s", map_get_value(h->headers, "Connection"));
    */

    BlockingQueue * queue = make_queue();

    queue_put(queue, 1);
    queue_put(queue, 2);
    struct QNode * node = queue->first;

    while(node != NULL) {
        printf("%d", node->data);
        node = node->next;
    }

    free_queue(queue);


    /*
    int x = 2;
    thread_pool * pool = make_thread_pool(1);
    add_task(pool, &task, &x);
    sleep(2);
    printf("%d", x);
*/
    return 0;
}
