CC = g++

CFLAGS = -Wall -g -std=c++20

INCLUDES = -I./include/rplidar -I./include/rplidar/hal -I./src/Sensors/src -I./src/Mapper/src -I./src/BBIO -I./src/MotorController -I./src/BluetoothController/src

LFLAGS = ./lib/rplidar/librplidar_sdk.a

# define any libraries and frameworks to link into executable:
LIBS = -pthread -lbluetooth

SRCS = SLAM.cpp ./src/Mapper/src/Map.cpp ./src/Mapper/src/Mapper.cpp ./src/Sensors/src/Lidar.cpp ./src/BBIO/PWM.cpp ./src/MotorController/Motor.cpp ./src/MotorController/MotorController.cpp ./src/BluetoothController/src/BluetoothController.cpp

OBJS = $(SRCS:.c=.o)

MAIN = SLAM

# make
all: $(MAIN)
	@cp SLAM ./bin

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm ./SLAM
	rm ./bin/SLAM
