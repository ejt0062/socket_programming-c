/*    
    Emanuel Taylor
	CSCE 3530 Program #1
	ejt0062@unt.edu
	9/28/17
    
    Compilation: gcc -o client client.c
    Execution  : ./client 5000
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>      //System type definitions
#include <sys/socket.h>     //Network system functions
#include <netinet/in.h>     //Protocol and struct defintions
#include <netdb.h>
#include <error.h>
#include <arpa/inet.h>

#define BUF_SIZE    1024
//#define SERVER_IP   "129.120.151.94"
//#define SERVER_PORT 5000

int main(int argc, char *argv[])
{
    int sockfd, port_no;
    int send_len, bytes_sent;
    int incoming_len, recv_msg_size;
    char text[80], buf[BUF_SIZE];
    struct sockaddr_in addr_send;
    struct sockaddr_in remote_addr;
    struct hostent *server_ip;
    
    //Check to see if user put in a port number
    if(argc < 2)
    {
        printf("\nPort number is missing...\n");
        exit(0);
    }
    
    //Get port number from the user
    port_no = atoi(argv[1]);
    //Create socket for sending data
    sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd < 0)
        error(EXIT_FAILURE, 0, "socket() failed\n");

    server_ip = gethostbyname("129.120.151.94");//IP address of UNT's cse01 machine
    if(server_ip == NULL)
    {
        printf("\nNo such host...\n");
        exit(0);
    }
    
    //Fill the address structure for sending data
    memset(&addr_send, 0, sizeof(addr_send));   //Clear structure
    addr_send.sin_family = AF_INET;             //Address family
    //addr_send.sin_addr.s_addr = inet_addr(server_ip);
    memcpy(&addr_send.sin_addr, server_ip->h_addr, server_ip->h_length);
    addr_send.sin_port = htons((unsigned short)port_no);
    
    while(1)
    {
        printf("Enter client's message: ");
        //scanf("%s", text);
        fgets(text, BUF_SIZE, stdin);
        //Remove trailing newline, if there.
        if((strlen(text) > 0) && (text[strlen (text) - 1] == '\n'))
            text[strlen(text) - 1] = '\0';
        
        //Send client's message
        strcpy(buf, text);
        send_len = strlen(text);
        bytes_sent = sendto(sockfd, buf, send_len, 0, (struct sockaddr *) &addr_send, sizeof(addr_send));
        
        if(strcmp(text, "Bye") == 0)
            break;
        
        //Receive message from server
        incoming_len = sizeof(remote_addr); //Who sent to us?
        recv_msg_size = recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *) &remote_addr, &incoming_len);
        if(recv_msg_size > 0)               //What was sent?
        {
            buf[recv_msg_size] = '\0';
            printf("From %s server received: %s\n", inet_ntoa(remote_addr.sin_addr), buf);
        }
            
        if(strcmp(text, "Bye") == 0)
            break;
    }
    
    close(sockfd);
}