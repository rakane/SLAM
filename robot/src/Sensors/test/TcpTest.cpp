#include <thread>
#include <iostream>
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

#include "../src/TcpController.h"
#include "TestMotorController.h"

#define TCP_PORT 8081
#define BUFFER_SIZE 1024

// Function to flag ctrl-c
bool ctrl_c_pressed;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

int main()
{
    std::cout << "Starting TcpTest" << std::endl;

    // Setup TCP server
    SLAM::MotorControllerInterface* motorController = new TestMotorController();
    TcpController* tcpController = new TcpController(motorController);

    std::thread tcpServerThread(&TcpController::run, tcpController);

    // Trap Ctrl-C
    signal(SIGINT, ctrlc);

    if(false)
    {
        usleep(3000000);

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
            bzero(&servaddr, sizeof(servaddr));
        
            // assign IP, PORT
            servaddr.sin_family = AF_INET;
            servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
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
                while (!ctrl_c_pressed)
                {
                    bzero(buff, sizeof(buff));
                    std::cout << "Enter the string : ";
                    
                    std::cin >> buff;

                    if(strcmp(buff, "exit") == 0)
                    {
                        break;
                    }

                    write(sockfd, buff, sizeof(buff));
                    bzero(buff, sizeof(buff));

                    usleep(10000); 
                }

                std::cout << "Ctrl-C pressed. Shutting down..." << std::endl;

                // close the socket
                close(sockfd);
            }
        }
    }
    else
    {
        std::cout << "Waiting for Ctrl-C..." << std::endl;

        while (!ctrl_c_pressed)
        {
            usleep(10000); 
        }
    }

    // Shutdown
    tcpController->terminate();
    tcpServerThread.join();

    delete tcpController;
    delete motorController;

    return 0;
}
