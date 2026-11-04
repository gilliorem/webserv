*This project has been created as part of the 42 curriculum by regillio, yucchen.*

# Webserv notes

## Client (Google Chrome)


## Server (web serv: what we are doing)

### Establish the Connection
- SOCKET: create an endpoint for communication. works like `open` : returns an 'fd' that would be our element to communicate with outside.
    `int socket(int domain, int type, int protocol)`
    - domain: LOCAL (on the same machine)(for a client it would be different)
    - type: `SOCK_STREAM`
    - protocol: ipv4

- PORT: hardcoded | 1000 -> 320000 (the specific door number for the communication)
- BIND: 

### Read the Client REQUEST
- LISTEN : listening what the client is sending 
    `while (true)`

- ACCEPT (one connection per client) : read/process the client data stream 
    - read

-- parse & check the Request
- header
-- field 1
-- field 2
-- field n

- body

### Write the RESPONSE back to the client

- `write()` the Response back

- Base on the initial request of the client: we need to build a response accordingly
- If the request gets parsed and all the check pass:
    - write the response according to the initial request.

# Webserv

## Description
Webserv is an HTTP server implemented in C++98.

The goal of this project is to understand how web servers work by implementing socket communication, HTTP request parsing, response generation, and non-blocking I/O.

The server aims to handle multiple client connections and support configurable HTTP behavior.

## Features
- HTTP/1.1 request and response handling
- GET, POST, and DELETE methods
- Non-blocking I/O using an I/O multiplexing mechanism
- Configuration file parsing
- Static file serving
- File uploads
- Custom error pages and HTTP redirections
- CGI execution

## Instructions
### Compilation
To compile the project, simply run:
```bash
make
```

### Execution
```bash
./webserv [configuration file]
```
The configuration file will define server settings such as listening ports, document roots, and HTTP behavior.

## Resources
References
- MDN HTTP (https://developer.mozilla.org/en-US/docs/Web/HTTP): HTTP requests, responses, and status codes.

### How AI was used
