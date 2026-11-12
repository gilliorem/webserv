## REMI

Following Nov 6 session:

Actions that we need to do:
separate in different parts:

## Initialize

The client|server connection needs to be established through a config file

Directives are divided into simple directives and block directives. 
A simple directive consists of the name and parameters separated by spaces and ends with a semicolon (;).
A block directive has the same structure as a simple directive, but instead of the semicolon it ends with a set of additional instructions surrounded by braces ({ and }).
If a block directive can have other directives inside braces, it is called a context (examples: events, http, server, and location).

Directives placed in the configuration file outside of any contexts are considered to be in the main context. The events and http directives reside in the main context, server in http, and location in server.

The rest of a line after the # sign is considered a comment.


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

the architecture of a parser that checks

[x] get starting with nginx 60

[x] Refactor the Config class - 20 + 20 + 20
[x] Move the unrelated to config member - 10
[x] refactor the Config constructor - 10

Expected class - 20
[] create the Expected class 

[] check if the config file exist - 5
[] check if we can open it  - 5
[] check if we can read it - 5

reading it:
[] save everything in a file buffer - 10

check the file buffer:
[] check the length of the file (have a max char limit) - 10
[] check the presence of the spec keywords (listen, root, size) - 10

[] fill the Req attribute with the parsed data of conf file - 10

---


open file - read file
`FILE* filePtr = fopen("webserv.conf", "r")` | file is open
we now put the file content in a buffer
`size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );`

size is the element size we will read
if we read char per char, size will be 1.
count is the total number of elements we will read. If we read 
1024 char then its 1024 (size * sizeof element)
stream is the pointer that fopen returns;

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
