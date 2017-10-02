README.md

Included: server.c,  client.c

Compilation: gcc -o server server.c,  gcc -o client client.c

Execution:
On CSE01 give the command
./server portnumber

Likewise on CSE02 - CSE06
./client portnumber

Enter "Bye" to disconnect client from server.
Enter "Shutdown" to shutdown the server.

Details & Explanation:
The program is a client-server program that is able to serve multiple
clients simutaneously by using the select() call on one socket. It was a good choice for the problem given that the server has to interact with multiple clients at a given time. The server also manages multiple connections on the same port. The server creates a socket and binds the socket. Intstead of using the listen() call, the server uses the select() call to listen for traffic on the socket. Select() is given a timeout value of zero so that it does not block. If nothing has been received on the socket, the program continues. If something has been received, then the server uses the recvfrom() system call to read the incoming data directly. It does not create a second socket to manage the communication. This allows the server to receive data from any number of clients on just one socket. Similiarly on the client side the client uses the sendto() system call to send data to the server. The client creates a socket but does not use connect() call to communicate with the server. sendto() contains the necessary the IP and port for which the data is to be sent. 

The program uses the example program from Systems Programming with C and Unix as a guide.

The server will run on the port indicated and once the client connects the client can send messages to the server. Vowels are counted on the server from the client's message and the vowels counted are then sent back to the client.
