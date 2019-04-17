#include <stdio.h>
#include <stdlib.h>
#include "thread_pool.h"
#include "server.h"


void * _map(HTTPRequest * req, HTTPResponse * res) {
    return (void*) "index.html";
}

int main(void) {
    Mapping _mapping;
    _mapping._func = &_map;
    add_mapping("/index", &_mapping);
    start_server(NULL, NULL);
    return 0;
}


