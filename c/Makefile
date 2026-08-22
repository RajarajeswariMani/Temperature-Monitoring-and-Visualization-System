CC := gcc
CFLAGS := -std=c11 -Wall -Wextra -Wpedantic -Iinclude -O2
SRC := $(wildcard src/*.c)
LIB_SRC := $(filter-out src/main.c, $(SRC))
BIN := temp_monitor_demo
TEST_BIN := temp_monitor_tests

.PHONY: all clean run test
all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

run: $(BIN)
	./$(BIN)

$(TEST_BIN): $(LIB_SRC) tests/test_suite.c
	$(CC) $(CFLAGS) $(LIB_SRC) tests/test_suite.c -o $(TEST_BIN)

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -f $(BIN) $(TEST_BIN)
