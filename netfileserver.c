#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
//hi
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8888
#define LISTEN_SIZE 10

void messageToken(char * buffer){
    printf("MESSAGE RN : %s\n", buffer);
    
    
}

int main(int argc, char ** argv) {
    
    int sock;
    struct sockaddr_in server;
    int mysock;
    char buff[1024];
    int returnval;
    
    //Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printf("ERROR - UNABLE TO CREATE SOCKET");
        exit(1);
    }
    
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    //Call bind
    if (bind(sock, (struct sockaddr *) &server, sizeof(server))) {
        printf("ERROR - UNABLE TO BIND");
        exit(1);
    }
    //Listen
    listen(sock, LISTEN_SIZE);
    
    //Accept
    do {
        mysock = accept(sock, (struct sockaddr*) 0, 0);
        if (mysock == -1) {
            printf("ERROR - UNABLE TO ACCEPT");
            exit(1);
        } else {
            memset(buff, 0, sizeof(buff));
            if ((returnval = recv(mysock, buff, sizeof(buff), 0)) < 0) {
                printf("ERROR - UNABLE TO ACCEPT");
                exit(1);
            } else if (returnval == 0) {
                printf("Ending connection");
            } else {
                printf("got the msg, returnval = %d\n", returnval);
            }
            printf("MSG : %s\n", buff);
            
        }
        messageToken(buff);
        
        close(mysock);
    } while (1);
    
    return 0;
}


