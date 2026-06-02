CC = g++
TARGET = snakegame
SOURCES = main.cpp game.cpp snake.cpp window.cpp gate.cpp GrowthItem.cpp PoisonItem.cpp ReverseItem.cpp GoldenApple.cpp board.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# 실행 환경에 따라 적합한 컴파일 및 링커 플래그를 자동으로 선택
# Windows MSYS2 환경과 일반 Linux/WSL 채점 환경을 모두 지원
ifneq ($(wildcard /ucrt64/include/ncurses),)
    # 로컬 Windows UCRT64/MSYS2 환경일 경우
    CXXFLAGS = -I/ucrt64/include/ncurses -std=c++11
    LDFLAGS = -L/ucrt64/lib -lncursesw
else
    # 일반 Linux / WSL / macOS (교수님 채점 환경)
    CXXFLAGS = -std=c++11
    LDFLAGS = -lncursesw
endif

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
