CC = g++

CFLAGS = -Wall -g -std=c++20

INCLUDES = -I../../BBIO -I../../MotorController -I../src -I.

LFLAGS = 

# define any libraries and frameworks to link into executable:
LIBS = -lbluetooth

SRCS = TestDriver.cpp ../../BBIO/PWM.cpp ./TestMotorController.cpp ../src/BluetoothController.cpp

OBJS = $(SRCS:.c=.o)

MAIN = BluetoothTest

# make
all: $(MAIN)

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm ./BluetoothTest
