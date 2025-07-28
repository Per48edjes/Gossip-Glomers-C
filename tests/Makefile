CFLAGS += $(shell pkg-config --cflags json-c)
CFLAGS += -Wall -Wextra -Wpedantic
LDFLAGS += $(shell pkg-config --libs json-c)
SRC_DIR := ../src
BUILD_DIR:= ../build
LIB_DIR := ../lib
TEST_LIB := ../tests/lib
UNITY_SRC_DIR := ../../unity/src

# Test executables 
TESTS := tcp-test vec_deque-tests dictionary-tests
TEST_EXECS := $(patsubst %, $(BUILD_DIR)/%.out, $(TESTS))

# Test source files 
VEC_DEQUE_TEST_SRC := $(TEST_LIB)/vec_deque-tests.c
DICTIONARY_TEST_SRC := $(TEST_LIB)/dictionary-tests.c
TCP_TEST_SRC := $(TEST_LIB)/tcp-test.c

# Test object files
VEC_DEQUE_TEST_OBJS := $(BUILD_DIR)/vec_deque-tests.o $(BUILD_DIR)/vec_deque.o $(BUILD_DIR)/unity.o
DICTIONARY_TEST_OBJS := $(BUILD_DIR)/dictionary-tests.o $(BUILD_DIR)/collections.o $(BUILD_DIR)/unity.o
TCP_TEST_OBJS := $(BUILD_DIR)/tcp-test.o $(BUILD_DIR)/util.o $(BUILD_DIR)/tcp.o $(BUILD_DIR)/collections.o $(BUILD_DIR)/stopwatch.o


all: $(TEST_EXECS)

# Generate executables for tests
$(BUILD_DIR)/dictionary-tests.out: $(DICTIONARY_TEST_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/vec_deque-tests.out: $(VEC_DEQUE_TEST_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/tcp-test.out: $(TCP_TEST_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# Generate testing framework object file
$(BUILD_DIR)/unity.o: $(UNITY_SRC_DIR)/unity.c $(UNITY_SRC_DIR)/unity.h
	$(CC) $(CFLAGS) -c $< -o $@

# Generate object files for library modules
$(BUILD_DIR)/util.o: $(LIB_DIR)/util.c $(LIB_DIR)/util.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/collections.o: $(LIB_DIR)/collections.c $(LIB_DIR)/collections.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/tcp.o: $(LIB_DIR)/tcp.c $(LIB_DIR)/tcp.h $(LIB_DIR)/stopwatch.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/stopwatch.o: $(LIB_DIR)/stopwatch.c $(LIB_DIR)/stopwatch.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/vec_deque.o: $(LIB_DIR)/vec_deque.c $(LIB_DIR)/vec_deque.h
	$(CC) $(CFLAGS) -c $< -o $@

# Generate object files for tests
$(BUILD_DIR)/dictionary-tests.o: $(DICTIONARY_TEST_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/vec_deque-tests.o: $(VEC_DEQUE_TEST_SRC) $(LIB_DIR)/vec_deque.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/tcp-test.o: $(TCP_TEST_SRC) $(LIB_DIR)/util.h $(LIB_DIR)/tcp.h
	$(CC) $(CFLAGS) -c $< -o $@


# Command: Run unit tests
.PHONY: tests
tests: $(BUILD_DIR)/vec_deque-tests.out $(BUILD_DIR)/dictionary-tests.out
	valgrind --leak-check=full ./$(BUILD_DIR)/vec_deque-tests.out
	valgrind --leak-check=full ./$(BUILD_DIR)/dictionary-tests.out
