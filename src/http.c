#include "include/http.h"

char *HTTPStatusMsg[] = {
    [OK] = "OK",
    [CREATED] = "Created",
    [NO_CONTENT] = "No Content",
    [BAD_REQUEST] = "Bad Request",
    [UNAUTHORIZED] = "Unauthorized",
    [FORBIDDEN] = "Forbidden",
    [NOT_FOUND] = "Not Found",
    [METHOD_NOT_ALLOWED] = "Method Not Allowed",
    [CONFLICT] = "Conflict",
    [GONE] = "Gone",
    [URI_TOO_LONG] = "URI Too Lont",
    [UNSUPPORTED_MEDIA_TYPE] = "Unsupported Media Tyep",
    [TOO_MANY_REQUESTS] = "Too Many Request",
    [INTERNAL_SERVER_ERROR] = "Internal Server Error",
    [NOT_IMPLEMENTED] = "Not Immplemented",
    [SERVICE_UNAVAILABLE] = "Service Unavailable",
    [HTTP_VERSION_NOT_SUPPORTED] = "HTTP Version Not Supported",
};
