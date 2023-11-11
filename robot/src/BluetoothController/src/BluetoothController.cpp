
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

#include "../MotorController/MotorController.h"
#include "BluetoothController.h"


BluetoothController::BluetoothController(SLAM::MotorController* motorController)
    : stateMutex_(), state_(BluetoothState::IDLE), 
      bluetoothSocket_(-1), bluetoothClient_(-1), motorController_(motorController)
{
    std::cout << "BluetoothController::BluetoothController()" << std::endl;
}

BluetoothController::~BluetoothController()
{
    std::cout << "BluetoothController::~BluetoothController()" << std::endl;
}

void BluetoothController::run()
{
    std::cout << "BluetoothController::run()" << std::endl;

    BluetoothState localState = BluetoothState::IDLE;
    
    // Bluetooth setup
    struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
    char buf[1024] = { 0 };
    int s, client, bytes_read;
    socklen_t opt = sizeof(rem_addr);

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // bind socket to port 1 of the first available 
    // local bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    //loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) 1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));

    // put socket into listening mode
    listen(s, 1);
	
    // accept one connection
    client = accept(s, (struct sockaddr *)&rem_addr, &opt);

    ba2str( &rem_addr.rc_bdaddr, buf );
    fprintf(stderr, "accepted connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));

    localState = BluetoothState::RUNNING;

    while(localState != BluetoothState::TERMINATED)
    {
        bytes_read = read(client, buf, sizeof(buf));

        if(bytes_read > 0)
        {
            // Ensure buffer is null terminated
            buf[sizeof(buf) - 1] = '\0';

            printf("Received bluetooth data: %s\n", buf);
            
        }        

        // sleep for 1 second
        usleep(1000000);

        // Update state
        {
            std::lock_guard<std::mutex> lock(stateMutex_);
            localState = state_;
        }
    }

    // Close connection
	printf("Bluetooth server closing...\n");
    close(client);
    close(s);
}

void BluetoothController::terminate()
{
    std::cout << "BluetoothController::terminate()" << std::endl;

    std::lock_guard<std::mutex> lock(stateMutex_);

    state_ = BluetoothState::TERMINATED;
}
