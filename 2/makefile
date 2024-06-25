CC = g++
CFLAGS = -std=c++11 -Wall
TARGET = main
SRCS = main.cpp kosa.cpp deque_Kosa.cpp list_Kosa.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

main.o: main.cpp kosa.hpp deque_Kosa.hpp list_Kosa.hpp
	$(CC) $(CFLAGS) -c main.cpp

kosa.o: kosa.cpp kosa.hpp
	$(CC) $(CFLAGS) -c kosa.cpp

deque_Kosa.o: deque_Kosa.cpp deque_Kosa.hpp
	$(CC) $(CFLAGS) -c deque_Kosa.cpp

list_Kosa.o: list_Kosa.cpp list_Kosa.hpp
	$(CC) $(CFLAGS) -c list_Kosa.cpp

clean:
	$(RM) $(TARGET) $(OBJS)

run: all
	./$(TARGET)

.PHONY: all clean run
