#include<stdio.h>
#include<string.h>
#include <fcntl.h>
#include<stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define INVALID_CONNECTION_MODE 300
#define INVALID_FLAGS 221
#define PORT 8888
#define IPADDRESS "127.0.0.1"
int hostfound = 0;


int netopen(const char *pathname, int flags){
    
    //SERVER ERRORS:
        //HAVING ACCESS TO FILE
        //FILE IS A FOLDER AND NOT A FILE
    
    
    
    
    /*checks to see if they are valid flags*/
    if(flags != O_RDWR && flags != O_RDONLY && flags != O_WRONLY){
        printf("ERROR : INVALID FLAG for File : %s\n", pathname);
        h_errno = INVALID_FLAGS;
        return -1;
    }
    
    /*checks to see if host was found in netserverinit*/
    if(hostfound == 0){
        printf("ERROR : HOST NOT FOUND\n");
        h_errno = HOST_NOT_FOUND;
        return -1;
    }
    
    struct sockaddr_in addr;
    int fd;
    int bytes;/*number of bytes returned by write function*/
    int returnbytes;/*number of bytes returned by read function*/
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("SOCKET GOOD\n");
    if(fd == -1)
    {
        printf("Error opening socket\n");
        return -1;
    }
    
    bzero(&addr, sizeof(addr));
    
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IPADDRESS);
    addr.sin_family = AF_INET;
    
    
    if(connect(fd, (struct sockaddr *)&addr,sizeof(struct sockaddr_in) ) == -1)
    {
        printf("Error Connecting To Socket\n");
        return -1;
    }
    printf("CONNECT GOOD\n");
    char message[512];
    char * functiontype = malloc(5);
    functiontype = "open";
    sprintf(message, "%s %s %d", functiontype, pathname, flags);
    printf("%s\n", message);
    bytes = write(fd, message,strlen(message));
    
    if(bytes == -1){
        printf("Error Writing To Socket");
        return -1;
    }
    
    char receiver [50];
    bzero(receiver, strlen(receiver));
    returnbytes = read(fd, receiver, strlen(receiver));
    
    if(returnbytes == -1){
        printf("ERROR Reading From Socket\n");
        return -1;
    }
    
    return returnbytes;
}

ssize_t netread(int fildes, void *buf, size_t nbyte){
    if(hostfound == 0){
        printf("ERROR : HOST NOT FOUND\n");
        h_errno = HOST_NOT_FOUND;
        return -1;
    }
    struct sockaddr_in addr;
    int fd;
    int bytes;
    int returnbytes;
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(fd == -1)
    {
        printf("Error opening socket\n");
        return -1;
    }
    
    bzero(&addr, sizeof(addr));
    
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = 0;
    addr.sin_addr.s_addr = inet_addr(IPADDRESS);
    addr.sin_family = AF_INET;
    
    
    if(connect(fd, (struct sockaddr *)&addr,sizeof(struct sockaddr_in) ) == -1)
    {
        printf("Error Connecting To Socket\n");
        return -1;
    }
    char message[512];
    char * functiontype = malloc(5);
    functiontype = "read";
    sprintf(message, "%s %d %zu", functiontype, fildes, nbyte);
    
    bytes = write(fd, message,strlen(message));
    
    if(bytes == -1){
        printf("Error Writing To Socket");
        return -1;
    }
    
    char receiver [50];
    bzero(receiver, strlen(receiver));
    returnbytes = read(fd, receiver, strlen(receiver));
    
    if(returnbytes == -1){
        printf("ERROR Reading From Socket\n");
        return -1;
    }
    
    return returnbytes;
    
}

ssize_t netwrite(int fildes, void *buf, size_t nbyte){
    if(hostfound == 0){
        printf("ERROR : HOST NOT FOUND\n");
        h_errno = HOST_NOT_FOUND;
        return -1;
    }
    struct sockaddr_in addr;
    int fd;
    int bytes;
    int returnbytes;
    
    fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if(fd == -1)
    {
        printf("Error opening socket\n");
        return -1;
    }
    
    bzero(&addr, sizeof(addr));
    
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = 0;
    addr.sin_addr.s_addr = inet_addr(IPADDRESS);
    addr.sin_family = AF_INET;
    
    
    if(connect(fd, (struct sockaddr *)&addr,sizeof(struct sockaddr_in) ) == -1)
    {
        printf("Error Connecting To Socket\n");
        return -1;
    }
    char message[512];
    char * functiontype = malloc(5);
    functiontype = "write";
    sprintf(message, "%s %d %zu", functiontype, fildes, nbyte);
    
    bytes = write(fd, message,strlen(message));
    
    if(bytes == -1){
        printf("Error Writing To Socket");
        return -1;
    }
    
    char receiver [50];
    bzero(receiver, strlen(receiver));
    returnbytes = read(fd, receiver, strlen(receiver));
    
    if(returnbytes == -1){
        printf("ERROR Reading From Socket\n");
        return -1;
    }
    
    return returnbytes;
    
}

int netclose(int fd){
    if (close(fd) == -1) {
        h_errno = 9;
        return -1;
    }
}

int netserverinit(char * hostname, int connectionMode){
    
    /*checks to see if the connectionMode is invalid*/
    if(connectionMode < 1 || connectionMode >3){
        printf("ERROR : INCORRECT CONNECTION MODE \n");
        h_errno = INVALID_CONNECTION_MODE;
        return -1;
    }
    
    struct addrinfo addressinformation;
    struct addrinfo *result;
    int addrinfovalue;
    
    bzero(&addressinformation, sizeof(addressinformation));
    addressinformation.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    addressinformation.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    addressinformation.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    addressinformation.ai_protocol = 0;          /* Any protocol */
    addressinformation.ai_canonname = NULL;
    addressinformation.ai_addr = NULL;
    addressinformation.ai_next = NULL;
    
    addrinfovalue = getaddrinfo(hostname, NULL, &addressinformation, &result);
    
    if(addrinfovalue != 0){
        printf("ERROR : HOST NOT FOUND\n");
        h_errno = HOST_NOT_FOUND;
        return -1;
    }
    //IPADDRESS = hostname;
    hostfound = 1;
    return 0;
}



int main(void)
{
    //int i = 0;
    //int err;
    //netopen("DOCUMENTS", O_RDWR);
    //netopen("WHOA", 5);
    netserverinit("127.0.0.1", 2);
    netopen("/Users/AkhilY/Desktop/test.c", O_RDWR);
    //netserverinit("NO", 4);
    return 0;
}
