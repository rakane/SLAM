CC = g++

CFLAGS = -Wall -g -std=c++20

INCLUDES = -I../../MotorController/src -I.

LFLAGS = 

# define any libraries and frameworks to link into executable:
LIBS = -pthread

SRCS = TcpTest.cpp ../src/TcpController.cpp

OBJS = $(SRCS:.c=.o)

MAIN = TcpTest

# make
all: $(MAIN)

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm ./TcpTest
