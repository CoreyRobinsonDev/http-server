#ifndef HTTP_H
#define HTTP_H

#include <netinet/in.h>
#include <stdint.h>
#include <time.h>

#define BUFF_SIZE 2048
#define URI_MAX_BUFF 2048
#define MAX_COOKIE_BUFF 128
#define MAX_COOKIES 8
#define VERSION "HTTP/1.1"

typedef enum: uint8_t {
    INVALID_METHOD,
    GET,
    POST,
    PUT,
    PATCH,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    HEAD,
} HTTPMethod;

typedef enum: uint8_t {
    INVALID_TYPE,
    JSON,       // application/json
    HTML,       // text/html
    TEXT,       // text/plain
} ContentType;

#define STATUS_CODES\
    X(OK,200)\
    X(CREATED,201)\
    X(NO_CONTENT,204)\
    X(BAD_REQUEST,400)\
    X(UNAUTHORIZED,401)\
    X(FORBIDDEN,403)\
    X(NOT_FOUND,404)\
    X(METHOD_NOT_ALLOWED,405)\
    X(CONFLICT,409)\
    X(GONE,410)\
    X(URI_TOO_LONG,414)\
    X(UNSUPPORTED_MEDIA_TYPE,415)\
    X(TOO_MANY_REQUESTS,429)\
    X(INTERNAL_SERVER_ERROR,500)\
    X(NOT_IMPLEMENTED,501)\
    X(SERVICE_UNAVAILABLE,503)\
    X(HTTP_VERSION_NOT_SUPPORTED,505)\

#define X(name,code) name = code,
typedef enum {
    STATUS_CODES
} StatusCode;
#undef X


typedef struct {
    char payload[BUFF_SIZE+1];
    ContentType content_type;
    HTTPMethod method;
    char path[URI_MAX_BUFF+1];
    char version[16];
    int content_len;
    char cookies[MAX_COOKIES][MAX_COOKIE_BUFF];
    char host[64];
} Request;

typedef struct response_t {
    char version[16];
    StatusCode status_code;
    char* status_msg;
    char* date;
    char server;
    int content_len;
    ContentType content_type;
    char payload[BUFF_SIZE+1];
    void (*set_status)(struct response_t* self, StatusCode);
    void (*set_payload)(struct response_t* self, char*);
} Response;

Request parse_req(char *req_buf);
Response generate_response(Request req);
char* parse_res(Response res);
void print_req(Request req);
void print_res(Response res);
Response init_res();
void set_status(Response* self, StatusCode status_code);
void set_payload(Response* self, char* filename);


#endif // !HTTP_H
