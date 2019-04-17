#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED

#ifndef HASH_MAP_H_INCLUDED
#include "hash_map.h"
#endif // HASH_MAP_H_INCLUDED
#ifndef G_DEFINES_H_INCLUDED
#include "g_defines.h"
#endif // G_DEFINES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define SIZE(head, tail) ((-1) * (head - tail)) + 1
#define INFINITE_LOOP 1
#define Q_TOKEN '?'
#define EQ_TOKEN '='
#define STR_END '\0'
#define HTTP_END "\r\n\r\n\0"



typedef enum {
  OK,
  NOT_FOUND,
  BAD_REQUEST,
  FORBIDDEN,
  UNAUTHORIZED
} ResponseStatus;

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
} HTTPRequest;

typedef void *(*mapping_function)(HTTPRequest *, HTTPResponse *);
Map * APP_MAPPINGS;

typedef struct {
    RequestType     rq_type;
    mapping_function _func;
} Mapping;

HTTPRequest * parse_header(const char *);
int free_header(HTTPRequest *);
void add_mapping(char * __restrict, Mapping *);
#endif // HTTP_H_INCLUDED
