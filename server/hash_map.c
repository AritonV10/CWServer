#include "hash_map.h"

#define DEBUG

///////////////////////////////////////////

static inline void str_len(const char*, size_t *);
static inline void compute_hash(const char *, u_int *);
static struct Node * _make_node(char*, u_int, struct Node*, T);
static void rehash(Map*);

///////////////////////////////////////////

static u_int BUCKETS = 16;
static double LOAD_FACTOR = 0.75;

///////////////////////////////////////////

Map * create_map() {
    Map * map_;

    if((map_ = malloc(sizeof(Map)))) {
        if((map_->buckets = calloc(BUCKETS, sizeof(struct Node)))) {
            map_->BUCKETS = BUCKETS;
            map_->_elements = 0;
            return map_;
        }
    };
    free_map(map_);
    return NULL;
}

int map_put(Map * map_, char * key, T data) {
    // no map passed
    if(!map_)
        return -1;
    u_int hash_;
    compute_hash(key, &hash_);

    // no key passed
    if(hash_ == 0)
        return -1;
    u_int __kpos = hash_ % map_->BUCKETS;

    #ifdef DEBUG
        printf("[Map-Put] Key: %s | Position: %i | Data: %s\n", key, __kpos, data);
    #endif // DEBUG
    if(((double)map_->BUCKETS * LOAD_FACTOR) == map_->_elements) {
        rehash(map_);
    }

    map_->_elements += 1;
    struct Node * node = _make_node(key, hash_, NULL, data);
    if(map_->buckets[__kpos] == 0) {
        map_->buckets[__kpos] = node;
    } else {
        struct Node * current_node = map_->buckets[__kpos];
        while(1) {
            if(strcmp(key, current_node->key_) == 0) {
                return 1;
            } else if(!current_node->next_node) {
                current_node->next_node = node;
                return 0;
            }
            current_node = current_node->next_node;
        }
    }
    return 1;
}

T map_remove_key(Map * map, char * key) {
    // use a callback
    u_int hash;
    compute_hash(key, &hash);

    if(hash == 0)
        return NULL;

    short __kpos = hash % map->BUCKETS;
    struct Node * node = map->buckets[__kpos];
    T returned_data;

    while(node != 0) {
        if(strcmp(key, node->key_) == 0){
            returned_data = node->data;
            map->buckets[__kpos] = 0;
            #ifdef DEBUG
                printf("[Head] Removing key %s\n", node->key_);
            #endif // DEBUG
            free(node);
            return returned_data;
        } else if(strcmp(key, node->next_node->key_) == 0) {
            struct Node * curr = node->next_node;
            returned_data = curr->data;
            node->next_node = curr->next_node;
             #ifdef DEBUG
                printf("[Chain] Removing key %s", curr->key_);
            #endif // DEBUG
            free(curr);
            return returned_data;
        }
        node = node->next_node;
    }
    return NULL;
}

T map_get_value(Map * map, const char * key) {
    u_int hash_;
    compute_hash(key, &hash_);

    if(hash_ == 0)
        return NULL;

    u_int __kpos = hash_ % map->BUCKETS;
    struct Node * node = map->buckets[__kpos];
    #ifdef DEBUG
        printf("[Map-Search] Key: %s\n", key);
    #endif
    while(node != 0) {
        #ifdef DEBUG
            printf("Checking %s\n", node->key_);
        #endif // DEBUG
        if(strcmp(node->key_, key) == 0) {
            #ifdef DEBUG
                printf("Returning value for %s\n", node->key_);
            #endif // DEBUG
            return node->data;
        }
        node = node->next_node;
    }
    return NULL;
}

void free_map(Map * map) {
    u_int i;

    if(!map)
        return;

    for(i = 0; i < map->BUCKETS; ++i) {
        if(map->buckets[i] != 0) {
            struct Node * node = map->buckets[i];
            while(node) {
                struct Node * temp = node->next_node;
                #ifdef DEBUG
                    printf("\nFreeing key: %s\n", node->key_);
                #endif // DEBUG
                free(node->key_);
                free(node->data);
                free(node);
                node = temp;
            }
        }
    }
    free(map->buckets);
    free(map);
}

int map_override_key(Map * map_, char * key_to_overrride, T new_data) {
    if(!map_)
        return -1;
    u_int hash_;
    compute_hash(key_to_overrride, &hash_);

    if(hash_ == 0)
        return -1;

    short __kpos = hash_ % map_->BUCKETS;
    struct Node * node = map_->buckets[__kpos];
    if(strcmp(key_to_overrride, node->key_) == 0) {
        node->data = new_data;
        return 0;
    } else {
            while(node != 0) {
                if(strcmp(key_to_overrride, node->key_) == 0) {
                    node->data = new_data;
                    return 0;
                }
                node = node->next_node;
            }
    }
    return -1;
}

int map_merge_data(Map * map, char * key_to_merge, callback function) {
    if(!map)
        return -1;
    u_int hash_;
    compute_hash(key_to_merge, &hash_);

    if(hash_ == 0)
        return -1;

    short __kpos = hash_ % map->BUCKETS;
    struct Node * node = map->buckets[__kpos];
    if(strcmp(key_to_merge, node->key_) == 0) {
        function((T)&(node->data));
        return 0;
    } else {
            while(node != 0) {
                if(strcmp(key_to_merge, node->key_) == 0) {
                    function((T)&(node->data));
                    return 0;
                }
                node = node->next_node;
            }
    }
    return -1;
}

///////////////////////////////////////////////////
///////////////////////////////////////////////////

// compute the length of the key for hashing
static inline void str_len(const char * string, size_t * __klen) {
    for(*__klen = 0; string[*__klen] != '\0'; *++__klen){}
}

// todo: inline it and pass the hash to variable
static inline void compute_hash(const char * key, u_int * hash_) {
   u_int hash = 0xAAAAAAAA;
   u_int i;
   u_int __klen;
   str_len(key, &__klen);
   for (i = 0; i < __klen; ++key, ++i) {
      hash ^= ((i & 1) == 0) ? (  (hash <<  7) ^ (*key) * (hash >> 3)) :
                               (~((hash << 11) + ((*key) ^ (hash >> 5))));
   }
   if(__klen == 0)
      *hash_ = 0;
   else
      *hash_ = hash;
}

static struct Node * _make_node(char * key, u_int hash, struct Node * next, T data) {
    struct Node * node;
    if((node = malloc(sizeof(struct Node)))) {
        node->data = data;
        node->hash_function = hash;
        node->key_ = key;
        node->next_node = next;
        return node;
    }
    return NULL;
}

// to do
static inline void rehash(Map * map) {

}
