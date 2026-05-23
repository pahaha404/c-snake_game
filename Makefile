CC=g++
TARGET=snakegame
OBJECTS=main.cpp

CXXFLAGS=-I/ucrt64/include/ncurses -std=c++11
LDFLAGS=-L/ucrt64/lib -lncursesw

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)