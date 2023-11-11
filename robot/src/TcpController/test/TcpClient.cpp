#include <thread>
#include <iostream>
#include <arpa/inet.h> // inet_addr()
#include <netinet/tcp.h>
#include <netdb.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#define TCP_PORT 8081
#define BUFFER_SIZE 1024

int main()
{
    std::cout << "Starting Tcp Client" << std::endl;

    // Setup client
    bool okToContinue = true;
    int sockfd;
    struct sockaddr_in servaddr;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        okToContinue = false;
    }
    else
    {
        printf("Socket successfully created..\n");
    }

    if(okToContinue)
    {
        /*
        int yes = 1;
        int result = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char*) &yes, sizeof(int));

        if(result < 0)
        {
            printf("Failed to set socket opts!\n");
            okToContinue = false;
        }
        */
    }

    if(okToContinue)
    {
        bzero(&servaddr, sizeof(servaddr));

        // assign IP, PORT
        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = inet_addr("192.168.1.30");
        servaddr.sin_port = htons(TCP_PORT);

        // connect the client socket to server socket
        if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
        {
            printf("connection with the server failed...\n");
            okToContinue = false;
        }
        else
        {
            printf("connected to the server..\n");
        }

        // Wait for Ctrl-C
        if(okToContinue)
        {
            char buff[BUFFER_SIZE];
            while (true)
            {
                bzero(buff, sizeof(buff));
                std::cout << "Enter the string : ";

                std::cin >> buff;

                if(strcmp(buff, "exit") == 0)
                {
                    break;
                }

                printf("Sending: %s\n", buff);

                write(sockfd, buff, sizeof(buff));
                bzero(buff, sizeof(buff));

                usleep(10000);
            }

            std::cout << "Ctrl-C pressed. Shutting down..." << std::endl;

            // close the socket
            close(sockfd);
        }
    }

    return 0;
}