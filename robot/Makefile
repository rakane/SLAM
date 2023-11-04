CC = g++

CFLAGS = -Wall -g -std=c++20

INCLUDES = -I./include -I./include/hal -I./src/Sensors/Lidar.h -I./src/Mapper/Map.h -I./src/Mapper/Mapper.h

LFLAGS = ./lib/librplidar_sdk.a

# define any libraries and frameworks to link into executable:
LIBS = -pthread 

SRCS = SLAM.cpp ./src/Mapper/Map.cpp ./src/Mapper/Mapper.cpp ./src/Sensors/Lidar.cpp

OBJS = $(SRCS:.c=.o)

MAIN = SLAM

# make
all: $(MAIN)
	@cp SLAM ./bin

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	rm ./SLAM
	rm ../bin/SLAM
