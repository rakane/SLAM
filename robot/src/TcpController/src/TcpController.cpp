#include <iostream>
#include <netdb.h> 
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h>

#include "../../MotorController/MotorControllerInterface.h"
#include "TcpController.h"

#define TCP_PORT 8081
#define BUFFER_SIZE 1024

TcpController::TcpController(SLAM::MotorControllerInterface* motorController)
    : mutex_(), 
      state_(TcpControllerState::IDLE),
      motorController_(motorController)
{
    std::cout << "TcpController::TcpController()" << std::endl;
}

TcpController::~TcpController()
{
    std::cout << "TcpController::~TcpController()" << std::endl;
}

void TcpController::run()
{
    std::cout << "TcpController::run()" << std::endl;

    int sockfd, connfd; 
    struct sockaddr_in servaddr, cli; 
   
    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("Controller socket creation failed...\n"); 
        return;
    } 
    else
    {
        printf("Controller socket successfully created..\n"); 
    }

    /*
    int yes = 1;
    int result = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char*) &yes, sizeof(int));

    if(result < 0)
    {
        printf("Failed to set controller socket opts!\n");
        return;
    }
    */

    bzero(&servaddr, sizeof(servaddr)); 
   
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(TCP_PORT); 
   
    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) 
    { 
        printf("Controller socket bind failed...\n"); 
        return;
    } 
    else
    {
        printf("Controller socket successfully binded..\n"); 
    }
   
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 0)) != 0) 
    { 
        printf("Controller listen failed...\n"); 
        return;
    } 
    else
    {
        printf("Controller server listening..\n"); 
    }

    socklen_t len = sizeof(cli); 
   
    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (struct sockaddr*)&cli, &len); 
    if (connfd < 0) 
    { 
        printf("Controller server accept failed...\n"); 
        return;
    } 
    else
    {
        printf("Controller server accepted the client with address: %s and port: %d\n", inet_ntoa(cli.sin_addr), ntohs(cli.sin_port));
    }

    TcpControllerState localState = TcpControllerState::RUNNING;

    char buffer[BUFFER_SIZE];
    while(localState != TcpControllerState::TERMINATED)
    {
        // clear the buffer
        bzero(buffer, BUFFER_SIZE); 

        std::cout << "Reading data from socket..." << std::endl;
   
        // read the message from client and copy it in buffer 
        int bytesRead = read(connfd, buffer, sizeof(buffer)); 

        if(bytesRead > 0)
        {
            // Ensure that buffer is null terminated
            buffer[BUFFER_SIZE - 1] = '\0';
    
            // print buffer which contains the client contents 
            printf("Command received: %s \n", buffer); 

            char command = buffer[0];

            // Commands:
            // F - Forward
            // B - Backward
            // L - Left
            // R - Right
            // S - Stop
            // E - Exit
            if(command == 'E')
            {
                std::cout << "Exit command received. Shutting down..." << std::endl;
                {
                    std::lock_guard<std::mutex> lock(mutex_);
                    state_ = TcpControllerState::TERMINATED;
                }
                break;
            }

            if(command == 'F' || command == 'B' || command == 'L' || command == 'R' || command == 'S')
            {
                SLAM::Direction direction = commandToMotorDirection(command);

                motorController_->processDirectionCommand(direction);
            } 
            else 
            {
                printf("Unknown command: %c \n", command);
            }
            
        }
        else
        {
            std::cout << "No data received. " << std::endl;
        }

        // Update state
        {
            std::lock_guard<std::mutex> lock(mutex_);
            localState = state_;
        }
    }

    std::cout << "TcpController::run() - Closing socket" << std::endl;
   
    // After chatting close the socket 
    close(sockfd); 
}

void TcpController::terminate()
{
    std::cout << "TcpController::terminate()" << std::endl;

    std::lock_guard<std::mutex> lock(mutex_);
    state_ = TcpControllerState::TERMINATED;
}

SLAM::Direction TcpController::commandToMotorDirection(const char command)
{
    SLAM::Direction direction = SLAM::Direction::STOP;

    switch(command)
    {
        case 'F':
            direction = SLAM::Direction::FORWARD;
            break;
        case 'B':
            direction = SLAM::Direction::BACKWARD;
            break;
        case 'L':
            direction = SLAM::Direction::LEFT;
            break;
        case 'R':
            direction = SLAM::Direction::RIGHT;
            break;
        case 'S':
            direction = SLAM::Direction::STOP;
            break;
        default:
            direction = SLAM::Direction::STOP;
            break;
    }

    return direction;
}
