# Webserv notes

## Client (Google Chrome)


## Server (web serv: what we are doing)


### Establish the Connection
- SOCKET: create an endpoint for communication. works like `open` : returns an 'fd' that would be our element to communicate with outside.
    `int socket(int domain, int type, int protocol)`
    - domain: LOCAL (on the same machine)(for a client it would be different)
    - type: SOCK_STREAM
    - protocol: ip4

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


