#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED

#ifndef HASH_MAP_H_INCLUDED
#include "hash_map.h"
#endif // HASH_MAP_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define SIZE(head, tail) ((-1) * (head - tail)) + 1
#define INFINITE_LOOP 1
#define Q_TOKEN '?'
#define EQ_TOKEN '='
#define STR_END '\0'
#define HTTP_END "\r\n\r\n"

typedef enum {
    HTML,
    JSON,
    XML,
    HTMLX
} ContentType;

typedef enum {
    GET,
    PUT,
    DELETE,
    POST
} RequestType;

typedef struct {

} HTTPResponse;

typedef struct {
    char * req_type;
    char * absolute_path;
    Map  * query_params;
    Map  * headers;
} RequestHeader;

typedef struct {
    RequestHeader * header;
} HTTPRequest;

typedef void *(*mapping_function)(HTTPRequest *, HTTPResponse *);

typedef struct {
    mapping_function _func;
    RequestType     rq_type;
} Mapping;

RequestHeader * parse_header(const char *);
int free_header(RequestHeader *);

#endif // HTTP_H_INCLUDED
