#include "server.h"

////////////////////////////////////////////////
// req -> assign thread -> parse header -> check if It's a get or post -> look at the accept ->
// /static/styles/style.css
// /static

////////////////////////////////////////////////

static char * views_path;
static char * public_views;
static char * resources;

void * handle_request(void *);
static __attribute__((always_inline)) void str_cat(char * __restrict, char * __restrict, char * __restrict);
__SINLINEV _response(int *, ResponseStatus);

///////////////////////////////////////////////
///////////////////////////////////////////////

void start_server(const char * IP, const char * port) {
    int server_socket;
    int cli_sock;
    struct sockaddr_in  server_addr;
    struct sockaddr_in  client_addr;
    socklen_t cli_len = sizeof(client_addr);
    P_ERROR((server_socket = socket(PF_INET, SOCK_STREAM, 0)), "Error");
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

    listen(server_socket, 10);
    thread_pool * t_pool = make_thread_pool(10);

    while(1) {
        // waiting for connectiong
        cli_sock = accept(server_socket, (struct sock_addr*)&client_addr, &cli_len);

        add_task(t_pool, handle_request, (void*) &cli_sock);

    }
    close(cli_sock);
    close(server_socket);
}




void * handle_request(void * param) {
    int       cli_sock    = *((int*) param);

    u_int     recv_len        = 0;
    u_int     read            = 0;
    byte      is_header       = { .value = 1};
    byte      is_eoh;
    byte      has_body        = { .value = 1};
    char      buf[1024];
    char      header[3000];
    char    * header_body;
    HTTPRequest     * req;
    HTTPResponse    * res     = NULL;

    while(has_body.value) {
        recv_len = recv(cli_sock, (void*)&buf, 1024, NULL);

        if(recv_len == 0)
            break;
        if(AND_FLAG(is_header.value) == 1) {
             while(recv_len != 0) {
                header[read] = buf[read++];
                if(read >= 4) {

                    char * _sstr = &header[read - 3];
                    char * eoh   = HTTP_END;
                    is_eoh.value = 0;

                    while(*eoh != '\0') {
                        if(*_sstr != *eoh) {
                            is_eoh.value |= 0b1;
                            break;
                        }
                        ++_sstr;
                        ++eoh;
                    }

                    if(is_eoh.value == 0) {
                        header[read] = '\0';
                        req = parse_header(&header[0]);
                        is_header.value &= 0b0;
                        if(map_get_value(req->headers, "Content-Type") == NULL)
                            has_body.value &= 0;
                        break;
                    }
                }
                --recv_len;
             }
        }
    }

    char * _path = (req->absolute_path) + 1;

    while(*_path != '\\' || *_path != '\0') {
        ++_path;
    }
    // /context/.../static
    // /context/static/style/java.js
    if(*_path == '\\') {

        if(strcmp(_path, resources)) {

            while(*_path != '.') {
                ++_path;
            }

            ++_path;

            //switch()
        } else if(strcmp(_path, views_path)) {
            _response(cli_sock, BAD_REQUEST);
            return;
        }
    }
    Mapping * _mapping =
    (Mapping*) map_get_value(
    APP_MAPPINGS, req->absolute_path);

    if(_mapping == NULL) {
        _response(&cli_sock, NOT_FOUND);
        return;
    }

    char * _view = (char*) _mapping->_func(req, res);

    if(_view == NULL) {
        return; // internal error
    }

    size_t view_len;
    _view = no_chuncks_f(_view, &view_len);

    char * _ok = "HTTP/1.1 200 OK\n";
    size_t _ok_len = strlen(_ok);
    char * response_msg = malloc(sizeof(char) * view_len + _ok_len);
    char * _c = response_msg;

    str_cat(_ok, _view, _c);

    send(cli_sock, (void*) response_msg, view_len + _ok_len, NULL);
    close(cli_sock);
    free(response_msg);
    free_header(req);

    // check if the methods are the same
}

////////////////////////////////////////////////////
////////////////////////////////////////////////////


static __attribute__((always_inline)) void
str_cat(char * __restrict _arg1, char * __restrict _arg2, char * __restrict _arg3) {

    for(;*_arg1 != '\0';)
        *_arg3++ = *_arg1++;
    for(;*_arg2 != '\0';)
        *_arg3++ = *_arg2++;
}

__SINLINEV
_response(int * _cli_sock, ResponseStatus _resp) {
    char * resp_str;
    switch(_resp) {
        case NOT_FOUND:
            resp_str = "HTTP/1.1 404 Not Found\r\n\r\n";
            break;
        case BAD_REQUEST:
            resp_str = "HTTP/1.1 400 Bad Request\r\n\r\n";
            break;
        case FORBIDDEN:
            resp_str = "HTTP/1.1 403 Forbidden\r\n\r\n";
            break;
        case UNAUTHORIZED:
            resp_str = "HTTP/1.1 401 Unauthorized\r\n\r\n";
            break;
    }
    send(*_cli_sock, (void*)resp_str, strlen(resp_str), NULL);
    close(*_cli_sock);
}



