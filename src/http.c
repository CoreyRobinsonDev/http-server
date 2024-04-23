#include "include/http.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
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
    Request req = {0};
    char line[BUFF_SIZE]; 
    bool req_line = true;

    for (int i = 0; req_buf[i] != '\0'; i++) {
        memset(line, '\0', sizeof(char) * BUFF_SIZE);
        for (int ii = 0; req_buf[i] != '\n'; i++) {
            line[ii] = req_buf[i];
            ii++;
        }

        // parse body
        if (strncmp(line, "\r", sizeof(char)) == 0) {
            i++;
            for (int ii = 0; req_buf[i] != '\0'; i++, ii++) {
                req.payload[ii] = req_buf[i];
            }
        }
 
        // first line
        if (req_line) {
            req_line = false;
            char word[URI_MAX_BUFF];
            int iter = 0;

            for (int ii = 0; line[ii] != '\0'; ii++) {
                memset(word, '\0', sizeof(word));
                iter++;
                for (int iii = 0; line[ii] != ' ' && line[ii] != '\0'; iii++) {
                    word[iii] = line[ii];
                    ii++;
                }
                switch(iter) {
                    case 1: 
                        set_method(&req,  word);
                        break;
                    case 2:
                        strncpy(req.path, word, sizeof(req.path));
                        break;
                    case 3:
                        strncpy(req.version, word, sizeof(req.version));
                        break;
                    default: break;
                }
            }
        // parse header
        } else {
            char key[128];
            char value[256];
            int ii = 0;
            memset(key, '\0', sizeof(key));
            memset(value, '\0', sizeof(value));

            for (; line[ii] != ':' && line[ii] != '\0'; ii++) {
                key[ii] = line[ii]; 
            }
            ii += 2;

            for (int iii = 0; line[ii] != '\n' && line[ii] != '\0'; ii++) {
                value[iii] = line[ii];
                iii++;
            }

            // key-value check
            if (strncmp(key, "Content-Type", sizeof("Content-Type")) == 0) {
                set_content_type(&req, value);
            } else if (strncmp(key, "Content-Length", sizeof("Content-Length")) == 0) {
                req.content_len = (int)strtol(value, (char**)NULL, 10);
            } else if (strncmp(key, "Cookie", sizeof("Cookie")) == 0) {
                int val_len = strnlen(value, 256);
                value[val_len] = ';';
                int count = 0;
                char cookie[MAX_COOKIE_BUFF];
                memset(cookie, '\0', sizeof(cookie));

                for (int ii = 0,iii = 0; value[ii] != '\0'; ii++,iii++) {
                    if (value[ii] == ';') {
                        ii += 2;
                        iii = 0;
                        strncpy(req.cookies[count], cookie, sizeof(char) * MAX_COOKIE_BUFF);
                        memset(cookie, '\0', sizeof(cookie));
                        count++;
                        if (count == MAX_COOKIES) break;
                    }
                    cookie[iii] = value[ii];
                }
            } else if (strncmp(key, "Host", sizeof("Host")) == 0) {
                strncpy(req.host, value, sizeof(req.host));
            }
        }
    }

    print_req(req);

    return req;
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
    printf("\t payload = \n"); 
    printf("%s\n", req.payload);
    printf("\t method = %s\n", method);
    printf("\t content_type = %s\n", content_type);
    printf("\t content_len = %d\n", req.content_len);
    printf("\t path = %s\n", req.path);
    printf("\t version = %s\n", req.version);
    printf("\t cookies = \n");
    for (int i = 0; i < MAX_COOKIES; i++) {
        if (req.cookies[i] == NULL || strnlen(req.cookies[i], MAX_COOKIE_BUFF) == 0) break;
        printf("\t\t %s\n", req.cookies[i]);
    }
    printf("\t host = %s\n", req.host);
    printf("}\n");

}
