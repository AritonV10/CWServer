#include "HTTP.h"
#define HTTP_DEBUG


static char * alloc_string(char **, char *, size_t *);
static Map * parse_query_params(char **, char *, size_t *);
static void append_to_string(char *, int *, char *, char *);
static void parse_absolute_path(const char *, RequestHeader * , char *, char *);

////////////////////////////////////

RequestHeader * parse_header(
    const char * request_msg) {

    char * head     = request_msg;
    char * tail     = request_msg;
    int query_flag  = 1;
    size_t ch_len   = 0;
    RequestHeader * req_info;
    // get the request type
    if((req_info = malloc(sizeof(RequestHeader)))) {

        // get the request type
        while(*tail != ' '){ ++tail; }
        req_info->req_type = alloc_string(&head, tail, &ch_len);
        // move the pointer to the non empty space
        ++tail;
        ++head;
        // invalid header
        if(*head != '/')
            return NULL;
        // get the request path as well as the
        // query parameters if it contains any
        while(*tail != ' ') {
            if(*tail == Q_TOKEN) {
                query_flag = 0;
                break;
            }
            ++tail;
        }
        char * path =
                alloc_string(&head, tail, &ch_len);
        if(path != NULL) {
            req_info->absolute_path = path;
        } else {
            return NULL;
        }

        if(query_flag == 0) {
            Map * map =
                    parse_query_params(&head, tail, &ch_len);
            if(map != NULL) {
                req_info->query_params = map;
            } else {
                return NULL;
            }
        }

        while(*tail != '\n') {
            ++tail;
        }

        head = tail;
        req_info->headers = create_map();
        if(req_info->headers != NULL) {
            while(*tail != '\r') {
                if(*tail == ':') {
                    ++head;
                    char * header_name = alloc_string(&head, tail, &ch_len);
                    for(;;) {
                        if(*tail == '\n') {
                            head += 2;
                            // the trailing whitespace between ':'
                            char * header_value = alloc_string(&head, tail, &ch_len);
                            map_put(req_info->headers, header_name, (T) header_value);
                            break;
                        }
                        ++tail;
                    }
                    continue;
                }
                ++tail;
            }
        } else {
            return NULL;
            free_header(req_info);
        }

        return req_info;
    }
    free_header(req_info);
    return NULL;
}

int free_header(RequestHeader * header) {

    if(!header)
        return -1;

    free(header->absolute_path);
    free(header->req_type);
    free_map(header->headers);
    free_map(header->query_params);
    free(header);
}

///////////////////////////////////////////////
///////////////////////////////////////////////

static char * alloc_string(
    char ** head,
    char * tail,
    size_t * ch_len) {

    size_t len   = 0;
    size_t size_ = SIZE(*head, tail);
    char           * new_string;
    if((new_string = malloc(sizeof(char) * size_))) {

        for(; *head != tail; ++*head){
            new_string[len++] = **head;
        }

        new_string[len] = STR_END;
        *ch_len = len;

        return new_string;
    }

    return NULL;
}

static Map * parse_query_params(
    char ** head,
    char * nav_pointer,
    size_t * ch_len) {

    Map     * query_params  = create_map();
    int     i;
    size_t  s;
    (void)  ch_len;
    (void)  i;
    if(query_params != NULL) {

        if(*nav_pointer == Q_TOKEN) {

            *head = nav_pointer;

            for(; *nav_pointer != ' ';) {

                if(*(nav_pointer) == EQ_TOKEN) {

                    ++*head;
                    char * query = alloc_string(&*head, nav_pointer, &s);
                    *head = nav_pointer;

                    for(;;++nav_pointer) {

                        if(*nav_pointer == '&' || *nav_pointer == ' ') {
                            ++*head;
                            char * query_value = alloc_string(&*head, nav_pointer, &s);

                            #ifdef HTTP_DEBUG
                                printf("[Query-Params] %s = %s\n", query, query_value);
                            #endif // HTTP_DEBUG

                            map_put(query_params, query, (T) query_value);
                            *head = nav_pointer;
                            break;
                        }
                    }
                    continue;
                }
                ++nav_pointer;
            }
        }
        return query_params;
    }
    return NULL;
}

static inline void append_to_string(
    char * source,
    int * i,
    char * head,
    char * tail) {

    for(; head != tail; ++head, ++*i) {
        source[*i] = *head;

    }
    source[*i] = STR_END;
}
