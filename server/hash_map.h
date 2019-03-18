#ifndef HASH_MAP_H_INCLUDED
#define HASH_MAP_H_INCLUDED

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef void* T;
typedef void(*callback)(T);
typedef unsigned int u_int;
struct Node {
    T data;
    struct  Node * next_node;
    char *  key_;
    u_int   hash_function;
};

typedef struct {
    struct  Node ** buckets;
    size_t  BUCKETS;
    size_t  _elements;
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
