CC = g++
CFLAGS = -std=c++11 -Wall -pthread
TARGET = scheduling_algorithm_demo

all: $(TARGET)

$(TARGET): scheduling_algorithm_demo.cpp
	$(CC) $(CFLAGS)  -o $(TARGET) scheduling_algorithm_demo.cpp

clean:
	rm $(TARGET)