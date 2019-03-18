#include "server.h"

////////////////////////////////////////////////
////////////////////////////////////////////////

static char * views_path;
static char * public_views;
static char * resources;

///////////////////////////////////////////////
///////////////////////////////////////////////

void start_server(Map * mappings, const char * IP, const char * port) {
    if(!mappings)
        return -1;
    int server_socket;
    int cli_len;
    int cli_sock;
    struct sockaddr_in  server_addr;
    struct sockaddr_in  req_addr;

    if((server_socket = socket(PF_INET, SOCK_STREAM, 0)) == 0) {
        perror(SOCK_OPEN_ERR);
        exit(1);
    }
    server_addr.sin_family         = AF_INET;
    server_addr.sin_port           = htons(DEFAULT_PORT);
    server_addr.sin_addr.s_addr   = INADDR_ANY;
    if(
    bind(
    server_socket, (struct sockaddr *)&server_addr,
    sizeof(server_addr)) < 0) {
        perror("Bind has failed");
        exit(1);
    }
    char * buffer[BUFLEN];
    cli_len = sizeof(req_addr);
    listen(server_socket, 10);
    thread_pool * t_pool = malloc(sizeof(thread_pool));


    // to do: parse req;
    Queue queue;
    while(1) {
        // waiting for connectiong
        cli_sock = accept(server_socket, (struct sock_addr*)&req_addr, &cli_len);

        // put the new connection in queue to be handled by the thread pool
        queue_put((void*) cli_sock);
    }
    close(cli_sock);
    close(server_socket);
}

void * handle_request(void * param) {
    Queue * cli_queue = (Queue *) param;
    while(1) {
        int     cli_sock    = (int) queue_pop(cli_queue);
        char    BUFFER[BUFLEN];
        int     read        = 0;
        int     position    = 0;
        int     is_header   = 1;
        char    * header;
        char    * header_body;
        HTTPRequest     * req;
        HTTPResponse    * res;
        recv(cli_sock, &BUFFER, &read, NULL);
        while(position != read) {
            break;
        }
        mapping_function s = map_get_value(APP_MAPPINGS, req->header->absolute_path);
        char * html_path = mapping_function(req, res);

    }

}


////////////////////////////////////////////////////
////////////////////////////////////////////////////

/*
static Header * parse_header(
    const char * header,
    const char * end
    ) {

}
*/
