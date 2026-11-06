## REMI

Following Nov 6 session:

Actions that we need to do:
separate in different parts:

## Initialize

The client|server connection needs to be established through a config file

give it a config file that has


server
{
    http
    {
        listen 8080
        root /Users/regilliot/code/forty_two/webserv
        max_size 1028
        max_file
    }
}

Config file parsing:

open file - read file
`std::ifstream infile(webserv.conf)` | file is open
we now put the file content in a buffer
size_t fread ( void * ptr, size_t size, size_t count, FILE * stream ); 

- parse the file content
- split per line
- split per space (key <space> value)
- save those values in our config class

capture the port number

give it to the connection established function

`address.sin_port = htons(port);`


construct a request at once
Request req(configFile)
-- parse the config file

and then construct a response with the request
Response res(req)
