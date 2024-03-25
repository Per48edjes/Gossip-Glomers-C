CFLAGS += $(shell pkg-config --cflags json-c)
CFLAGS += -Wall -Wextra -Wpedantic
LDFLAGS += $(shell pkg-config --libs json-c)
SRC_DIR := src
BUILD_DIR:= build
LIB_DIR := lib

# Challenges executables
CHALLENGES := challenge-1 challenge-2 challenge-3a challenge-3b
CHALLENGE_EXECS := $(patsubst %, $(BUILD_DIR)/%.out, $(CHALLENGES))

# Challenges source files 
CHALLENGE_1_SRC := $(SRC_DIR)/challenge-1/challenge-1.c
CHALLENGE_2_SRC := $(SRC_DIR)/challenge-2/challenge-2.c
CHALLENGE_3a_SRC := $(SRC_DIR)/challenge-3a/challenge-3a.c
CHALLENGE_3b_SRC := $(SRC_DIR)/challenge-3b/challenge-3b.c

# Challenges object files
CHALLENGE_1_OBJS := $(patsubst $(SRC_DIR)/challenge-1/%.c, $(BUILD_DIR)/%.o, $(CHALLENGE_1_SRC))
CHALLENGE_2_OBJS := $(patsubst $(SRC_DIR)/challenge-2/%.c, $(BUILD_DIR)/%.o, $(CHALLENGE_2_SRC))
CHALLENGE_3a_OBJS := $(patsubst $(SRC_DIR)/challenge-3a/%.c, $(BUILD_DIR)/%.o, $(CHALLENGE_3a_SRC))
CHALLENGE_3b_OBJS := $(patsubst $(SRC_DIR)/challenge-3b/%.c, $(BUILD_DIR)/%.o, $(CHALLENGE_3b_SRC))


all: $(CHALLENGE_EXECS)

# Generate executables for challenges
$(BUILD_DIR)/challenge-1.out: $(CHALLENGE_1_OBJS) $(BUILD_DIR)/util.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/challenge-2.out: $(CHALLENGE_2_OBJS) $(BUILD_DIR)/util.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/challenge-3a.out: $(CHALLENGE_3a_OBJS) $(BUILD_DIR)/util.o $(BUILD_DIR)/collections.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/challenge-3b.out: $(CHALLENGE_3b_OBJS) $(BUILD_DIR)/util.o $(BUILD_DIR)/collections.o $(BUILD_DIR)/tcp.o $(BUILD_DIR)/stopwatch.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

# Generate object files for challenges
$(BUILD_DIR)/challenge-1.o: $(CHALLENGE_1_SRC) $(LIB_DIR)/util.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/challenge-2.o: $(CHALLENGE_2_SRC) $(LIB_DIR)/util.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/challenge-3a.o: $(CHALLENGE_3a_SRC) $(LIB_DIR)/util.h $(LIB_DIR)/collections.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/challenge-3b.o: $(CHALLENGE_3b_SRC) $(LIB_DIR)/util.h $(LIB_DIR)/collections.h $(LIB_DIR)/tcp.h $(LIB_DIR)/stopwatch.h
	$(CC) $(CFLAGS) -c $< -o $@

# Generate object files for library modules
$(BUILD_DIR)/util.o: $(LIB_DIR)/util.c $(LIB_DIR)/util.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/collections.o: $(LIB_DIR)/collections.c $(LIB_DIR)/collections.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/vec_deque.o: $(LIB_DIR)/vec_deque.c $(LIB_DIR)/vec_deque.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/tcp.o: $(LIB_DIR)/tcp.c $(LIB_DIR)/tcp.h $(LIB_DIR)/stopwatch.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/stopwatch.o: $(LIB_DIR)/stopwatch.c $(LIB_DIR)/stopwatch.h
	$(CC) $(CFLAGS) -c $< -o $@


# Command: Build Docker image
.PHONY: build
build:
	docker build -t ggc .
	mkdir -p build/

# Command: Run Docker container from image
.PHONY: run
run:
	docker run --rm -it -p 8080:8080 -v $(PWD):/app ggc

# Command: Clean build directory
.PHONY: clean
clean:
	rm -rf build/*
