/* 
    Emanuel Taylor
	CSCE 3530 Program #1
	ejt0062@unt.edu
	9/28/17

    Compilation: gcc -o server server.c
    Execution  : ./server 5000
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>      //System type defintions
#include <sys/socket.h>     //Network system functions
#include <netinet/in.h>     //Protocol & struct defintions
#include <error.h>
#include <arpa/inet.h>

#define BUF_SIZE    1024
//#define LISTEN_PORT 5000

int main(int argc, char *argv[])
{
    int sockfd, port_no, i;
    int vcount, vowels;
    int recv_msg_size, send_len, bytes_sent;
    int incoming_len;
    int select_ret;
    char buf[BUF_SIZE], buf2[BUF_SIZE], text[BUF_SIZE];
    struct sockaddr_in my_addr;
    struct timeval timeout={0,0};
    struct sockaddr_in remote_addr;
    fd_set readfds;
    
    //Check to see if user gave port number
    if(argc < 2)
	{
        printf("\nPort number is missing...\n");
		exit(0);
	}
    
    //Create socket for receiving
    sockfd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(sockfd < 0)
        error(EXIT_FAILURE, 0, "socket() failed\n");
    
    //Make local address structure
    memset(&my_addr, 0, sizeof(my_addr));                   //Clear structure
    port_no = atoi(argv[1]);                                //Get port number from cmd line
    my_addr.sin_family = AF_INET;                           //Address family
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);            //Current IP
    my_addr.sin_port = htons((unsigned short)port_no);      //Assigned port number from user
    
    //Bind socket to the local address
    i = bind(sockfd, (struct sockaddr *) &my_addr, sizeof(my_addr));
    if(i < 0)
    {
        printf("bind() failed\n");
        exit(0);
    }
    
    //Listen ...
    printf("Server started and listening to the port %d\n", port_no);
    while(1)
    {
        do
        {
            FD_ZERO(&readfds);                      //Zero out socket set
            FD_SET(sockfd, &readfds);               //Add socket to listen to
            select_ret = select(sockfd+1, &readfds, NULL, NULL, &timeout);
            if(select_ret > 0)                      //Anything arrive on any socket?
            {
                incoming_len = sizeof(remote_addr); //Who sent to us?
                recv_msg_size = recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr *) &remote_addr, &incoming_len);
                if(recv_msg_size > 0)               //What was sent?
                {
                    buf[recv_msg_size] = '\0';
                    printf("From %s client received: %s\n", inet_ntoa(remote_addr.sin_addr), buf);
                }
                
                //Algorithm for counting vowels
                vowels = 0;                     //Zero result vowel counter
                for(vcount = 0; vcount < recv_msg_size; vcount++)
                {
                    //printf("%c ", buf[vcount]); //test print
                    
                    switch(buf[vcount])
                    {
                        case 'a':
                        case 'e':
                        case 'i':
                        case 'o':
                        case 'u':
                        case 'A':
                        case 'E':
                        case 'I':
                        case 'O':
                        case 'U':
                            vowels++;
                            break;
                    }
                }
                //printf("\n");
                
                //Sent message to client
                //char text[] = "The number of vowels present is ";
                //Send formatted output to a string
                sprintf(text, "The number of vowels present is %d", vowels);
                //Print it server side
                printf("The number of vowels present is %d\n", vowels);
                //Copy the text to store in text array to sendto()
                strcpy(buf2, text);
                send_len = strlen(text);
                bytes_sent = sendto(sockfd, buf2, send_len, 0, (struct sockaddr *) &remote_addr, sizeof(remote_addr));
                
                /*if(strcmp(buf, "Bye") == 0)
                    break; */
            }
        }while(select_ret > 0);
        
        //Shutdown the server
        if(strcmp(buf, "Shutdown") == 0)
            break;
    }
    
    close(sockfd);
}