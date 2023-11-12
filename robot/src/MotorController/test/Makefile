CC = g++

CFLAGS = -Wall -g -std=c++20

INCLUDES = -I../../BBIO/ -I../src -I.

LFLAGS = 

# define any libraries and frameworks to link into executable:
LIBS =

SRCS = MotorControllerTest.cpp ../src/MotorController.cpp ../src/Motor.cpp ../../BBIO/GPIO.cpp

OBJS = $(SRCS:.c=.o)

MAIN = MotorControllerTest

# make
all: $(MAIN)

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm ./MotorControllerTest
