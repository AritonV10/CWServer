#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#ifndef THREAD_POOL_H_INCLUDED
#include "thread_pool.h"
#endif // THREAD_POOL_H_INCLUDED

#ifndef HASH_MAP_H_INCLUDED
#include "hash_map.h"
#endif // HASH_MAP_H_INCLUDED

#ifndef FILE_H_INCLUDED
#include "file.h"
#endif // FILE_H_INCLUDED

#ifndef BLOCKING_QUEUE_H_INCLUDED
#include "blocking_queue.h"
#endif // BLOCKING_QUEUE_H_INCLUDED

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>


#define SOCK_OPEN_ERR "There has been an error creating the socket"
#define DEFAULT_PORT 8080
#define DEFAULT_NO_THREADS 20
#define BUFLEN (1 << 11) // 4096

#define P_ERROR(e, message) {\
            if(e == 0){\
                perror(message);\
                exit(1);\
            }\
        }


// todo: try with files

typedef struct {
    u_int value : 1;
} byte;



void start_server(const char * IP, const char * port);


#endif // SERVER_H_INCLUDED
