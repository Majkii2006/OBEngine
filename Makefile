CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -Iinclude

SRC = src/order.cpp src/id_gen.cpp src/orderbook.cpp main.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = main

build: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)

run:
	./main
