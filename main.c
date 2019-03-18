#include <stdio.h>
#include <stdlib.h>
#include "HTTP.h"


void * sum(int a, int b) {
    return a + b;
}

typedef void(*test)(void *);

void t(test test, void * arg) {

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

    Map * map_ = create_map();

    return 0;
}
