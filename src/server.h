#ifndef __server_h
#define __server_h
#include <stdint.h>
#define HEADER_KEY_SIZE 32
#define HEADER_VALUE_SIZE 256
#define HEADER_ARRAY_LENGTH 20
#define URI_LENGTH 256
#define URI_PARAM_KEY_LENGTH 32
#define URI_PARAM_VALUE_LENGTH 32
#define URI_PARAMS_LENGTH 16
typedef enum Method { GET, UNKNOWN } Method;
typedef struct UriParam {
    char headerKey[URI_PARAM_KEY_LENGTH];
    char headerValue[URI_PARAM_VALUE_LENGTH];
} UriParam;
typedef struct UriParams {
    UriParam uriParams[URI_PARAMS_LENGTH];
    uint8_t length;
} UriParams;
typedef struct HttpHeader {
    char headerKey[HEADER_KEY_SIZE];
    char headerValue[HEADER_VALUE_SIZE];
} HttpHeader;
typedef struct HttpHeaders {
    uint8_t length;
    HttpHeader headers[HEADER_ARRAY_LENGTH];
} HttpHeaders;

typedef struct HttpUri {
    char path[URI_LENGTH];
    UriParams uriParams;
} HttpUri;

typedef struct HttpRequestLine {
    Method method;
    HttpUri uri;
} HttpRequestLine;

typedef struct HttpRequest {
    HttpHeaders headers;
    HttpRequestLine requestLine;
} HttpRequest;

void handleConnection (int serverFileDescriptor, void* handlerFunction);
int createServer (const char* bindIp, int bindPort);
void closeFileDescriptor (int fileDescriptor);
#endif
