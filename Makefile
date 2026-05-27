CC = g++
TARGET = snakegame
SOURCES = main.cpp game.cpp snake.cpp window.cpp gate.cpp GrowthItem.cpp PoisonItem.cpp ReverseItem.cpp map.cpp
OBJECTS = $(SOURCES:.cpp=.o)

CXXFLAGS = -I/ucrt64/include/ncurses -std=c++11
LDFLAGS = -L/ucrt64/lib -lncursesw

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)

