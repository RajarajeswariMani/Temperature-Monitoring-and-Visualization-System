CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude -O2
SRC := $(wildcard src/*.cpp)
LIB_SRC := $(filter-out src/main.cpp, $(SRC))
BIN := temp_monitor_cpp_demo
TEST_BIN := temp_monitor_cpp_tests

.PHONY: all clean run test
all: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(BIN)

run: $(BIN)
	./$(BIN)

$(TEST_BIN): $(LIB_SRC) tests/test_suite.cpp
	$(CXX) $(CXXFLAGS) -Itests $(LIB_SRC) tests/test_suite.cpp -o $(TEST_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -f $(BIN) $(TEST_BIN)
	rm -rf build
