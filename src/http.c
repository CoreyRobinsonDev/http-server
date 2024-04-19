#include "include/http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void set_method(Request *req, char *method) {
    switch(method[0]) {
        case 'G': req->method = GET; break;
        case 'O': req->method = OPTIONS; break;
        case 'H': req->method = HEAD; break;
        case 'T': req->method = TRACE; break;
        case 'D': req->method = DELETE; break;
        case 'C': req->method = CONNECT; break;
        case 'P':
            switch (method[1]) {
                case 'O': req->method = POST; break;
                case 'U': req->method = PUT; break;
                case 'A': req->method = PATCH; break;
            }
            break;
        default: req->method = INVALID_METHOD;
    }
}

void set_content_type(Request *req, char *content_type) {
    char *json = "application/json";
    char *html = "text/html";
    char *text = "text/plain";

    req->content_type = INVALID_TYPE;
    if (strncmp(content_type, json, sizeof(char) * strlen(json)) == 0) {
        req->content_type = JSON;
    } else if (strncmp(content_type, html, sizeof(char) * strlen(html)) == 0) {
        req->content_type = HTML;
    } else if (strncmp(content_type, text, sizeof(char) * strlen(text)) == 0) {
        req->content_type = TEXT;
    }
}

Request parse_req(char *req_buf) {
}

// Prints given [Request];
void print_req(Request req) {
    char method[10];
    char content_type[40];

    switch (req.method) {
        case GET: strncpy(method, "GET", 10); break;
        case POST: strncpy(method, "POST", 10); break;
        case PUT: strncpy(method, "PUT", 10); break;
        case PATCH: strncpy(method, "PATCH", 10); break;
        case DELETE: strncpy(method, "DELETE", 10); break;
        case OPTIONS: strncpy(method, "OPTIONS", 10); break;
        case HEAD: strncpy(method, "HEAD", 10); break;
        case CONNECT: strncpy(method, "CONNECT", 10); break;
        case TRACE: strncpy(method, "TRACE", 10); break;
        case INVALID_METHOD: strncpy(method, "INVALID", 10); break;
    }

    switch (req.content_type) {
        case JSON: strncpy(content_type, "application/json", 40); break;
        case HTML: strncpy(content_type, "text/html", 40); break;
        case TEXT: strncpy(content_type, "text/plain", 40); break;
        case INVALID_TYPE: strncpy(content_type, "INVALID", 40); break;
    }

    printf("Request {\n");
    printf("\t header =\n"); 
    printf("%s\n", req.header);
    printf("\t payload = %s\n", req.payload);
    printf("\t method = %s\n", method);
    printf("\t content_type = %s\n", content_type);
    printf("\t path = %s\n", req.path);
    printf("\t query_parms = %s\n", req.query_parms);
    printf("\t version = %s\n", req.version);
    printf("}\n");

}
