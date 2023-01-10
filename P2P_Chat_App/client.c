/*

command line argument :
    filename server_ipaddress port_No

argv[0] filename
argv[1] server_ipaddress
argv[2] port_No

*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>


void error(const char *msg){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[]){
    if(argc < 2){
        fprintf(stderr, "Port No is not provided. Program terminated\n");
        exit(1);
    }

    int sockfd, portNo, n;
    char buffer[255];

    struct sockaddr_in serv_addr;//sockaddr_in will give us server and client address
    struct hostent *server;

    if(argc < 3){
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(1);
    }

    portNo = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0){
        error("Error opening Socket.");
    }

    server = gethostbyname(argv[1]);
    if(server == NULL)
        fprintf(stderr, "Error, No such host");

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *) server -> h_addr_list, (char *) &serv_addr.sin_addr.s_addr, server -> h_length);
    serv_addr.sin_port = htons(portNo);
    if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
        error("Connection Failed");
    
    while(1){
        bzero(buffer, 255);
        fgets(buffer, 255, stdin);
        n = write(sockfd, buffer, strlen(buffer));
        if(n < 0)
            error("Error on writing");
        
        bzero(buffer, 255);
        n = read(sockfd, buffer, 255);
        if(n < 0)
            error("Error on reading");
        printf("Server: %s", buffer);

        int i = strcmp("Bye", buffer);
        if(i == 0)
            break;
    }
    close(sockfd);
    return 0;
}