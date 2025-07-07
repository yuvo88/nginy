from enum import Enum
import sys
import socket
from dataclasses import dataclass
import ctypes

URI_LENGTH = 256
HTTP_REQUEST_SIZE = 6024
KEY_SIZE = 32
HEADER_VALUE_SIZE = 256
HEADER_ARRAY_LENGTH = 20


class HttpHeader(ctypes.Structure):
    _fields_ = [
        ("header_key", ctypes.c_char * KEY_SIZE),
        ("header_value", ctypes.c_char * HEADER_VALUE_SIZE),
    ]


class HttpHeaders(ctypes.Structure):
    _fields_ = [
        ("length", ctypes.c_uint8),
        ("headers", HttpHeader * HEADER_ARRAY_LENGTH),
    ]


class HttpRequestLine(ctypes.Structure):
    _fields_ = [("method", ctypes.c_int), ("uri", ctypes.c_char * URI_LENGTH)]


class HttpRequest(ctypes.Structure):
    _fields_ = [("headers", HttpHeaders), ("request_line", HttpRequestLine)]


def main():
    if len(sys.argv) != 2:
        sys.exit(1)

    unix_socket_address = sys.argv[1]
    server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
    server.bind(unix_socket_address)
    server.listen(1)
    conn, _ = server.accept()
    data = conn.recv(HTTP_REQUEST_SIZE)
    data_struct = HttpRequest.from_buffer_copy(data)
    print(data_struct.headers.headers[0].header_key)


if __name__ == "__main__":
    main()
