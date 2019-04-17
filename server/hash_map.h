#ifndef HASH_MAP_H_INCLUDED
#define HASH_MAP_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef G_DEFINES_H_INCLUDED
#include "g_defines.h"
#endif // G_DEFINES_H_INCLUDED

typedef void* T;
typedef void(*callback)(T);

/*
struct Node {
    enum {
        NORMAL,
        MAP
    } _type;
    T data;
    struct node * next_node;
    union {
        struct {
            char *  key;
            u_int   hash_function;
        }_map;
    };
}
*/
struct Node {
    u_int   hash_function;
    T data;
    char *  key_;
    struct  Node * next_node;
};

typedef struct {
    size_t  BUCKETS;
    size_t  _elements;
    struct  Node ** buckets;
} Map;
/** Create a map datastructure **/
Map * create_map();
T map_remove_key(Map * map_, char * key__);
int map_put(Map * map_, char * key__, T data);
int map_contains_key(Map * map_, char * key__);
int map_override_key(Map * map_, char * key__, T new_data);
int map_merge_data(Map * map_, char * key__, callback c);
T map_get_value(Map *, const char *);
void free_map(Map * map_);
#endif // HASH_MAP_H_INCLUDED
