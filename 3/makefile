CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = main
SRCS = main.cpp kosa.cpp
HEADERS = kosa.hpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)


$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)


%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $<


clean:
	rm -f $(TARGET) $(OBJS)


run: all
	./$(TARGET)


.PHONY: all clean run
