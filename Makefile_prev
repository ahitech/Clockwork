CXX = g++
CXXFLAGS = -Wall -std=c++17 -Ilib -Isrc
LDFLAGS = -Llib -lhdate -lbe -lintl

SRC = src/core/HebrewDate.cpp
TEST = tests/test_date.cpp

all: test

test: test_date
	./test_date

test_date: $(SRC) $(TEST)
	$(CXX) $(CXXFLAGS) $(TEST) $(SRC) -o test_date $(LDFLAGS)

clean:
	rm -f test_date
