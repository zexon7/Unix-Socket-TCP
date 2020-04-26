# TCP socket file transfer

### Idea of program
    1.First, create a socket in both server.c and client.c 
    2.Bind the server's socket address at 127.0.0.1 on port 8080
    3.Client can input IP address and Port number to connect to the server
    4.Client can upload file by input upload command
    5.Client can input goodbye to close socket

### How to use
    1.Open terminal and input make
    2.Start server by open a new terminal and input ./server
    3.Then, input ./connect [IP address] [Port number] in a new terminal
    4.Input upload and the program will ask for filename
    5.Input goodbye to close socket
