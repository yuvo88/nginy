#ifndef __server_h
#define __server_h
#include <stdint.h>
#define HEADER_KEY_SIZE 32
#define HEADER_VALUE_SIZE 256
#define HEADER_ARRAY_LENGTH 20
#define URI_LENGTH 256
typedef enum Method { GET, UNKNOWN } Method;
typedef struct HttpHeader {
    char headerKey[HEADER_KEY_SIZE];
    char headerValue[HEADER_VALUE_SIZE];
} HttpHeader;
typedef struct HttpHeaders {
    uint8_t length;
    HttpHeader headers[HEADER_ARRAY_LENGTH];
} HttpHeaders;

typedef struct HttpRequestLine {
    Method method;
    char uri[URI_LENGTH];
} HttpRequestLine;
typedef struct HttpRequest {
    HttpHeaders headers;
    HttpRequestLine requestLine;
} HttpRequest;
void handleConnection (int serverFileDescriptor);
int createServer (const char* bindIp, int bindPort);
void closeFileDescriptor (int fileDescriptor);
#endif
