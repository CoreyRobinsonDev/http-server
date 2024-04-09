#ifndef HTTP_H
#define HTTP_H

#include <netinet/in.h>
#include <stdint.h>
#include <time.h>

#define BUFF_SIZE 2048
#define VERSION "HTTP/1.1"

typedef enum: uint8_t {
    GET,
    POST,
    PUT,
    PATCH,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE,
    HEAD
} HTTPMethod;

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
} HTTPStatusCode;
#undef X


typedef struct {
    char header[BUFF_SIZE+1];
    char body[BUFF_SIZE+1];
} Request;

typedef struct {
    char header[BUFF_SIZE+1];
    char body[BUFF_SIZE+1];
} Response;

#endif // !HTTP_H