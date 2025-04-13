CXX = g++
CXXFLAGS = -Wall -std=c++17 -Ilib -Isrc
LDFLAGS = -Llib -lhdate -lbe -lstdc++ -lintl

SRCS = \
    src/core/HebrewDate.cpp \
    src/modules/TodayModule.cpp \
    src/main.cpp

OBJS = $(SRCS:.cpp=.o)
TARGET = clockwork_gui

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET) $(OBJS)

test: test_date
	./test_date

test_date: src/core/HebrewDate.cpp tests/test_date.cpp
	$(CXX) $(CXXFLAGS) tests/test_date.cpp src/core/HebrewDate.cpp -o test_date $(LDFLAGS)
