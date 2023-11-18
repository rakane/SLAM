CC = g++

CFLAGS = -Wall -g -std=c++20

INCLUDES = -I../../../include/rplidar -I../../../include/rplidar/hal  -I../../Mapper/src -I../src -I.

LFLAGS = ../../../lib/rplidar/librplidar_sdk.a 

# define any libraries and frameworks to link into executable:
LIBS = -pthread

SRCS = LidarCalibrationTest.cpp ../src/Lidar.cpp

OBJS = $(SRCS:.c=.o)

MAIN = LidarCalibrationTest

# make
all: $(MAIN)

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm ./LidarCalibrationTest
