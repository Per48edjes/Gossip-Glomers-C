# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build and Test Commands

### Building the Project

```bash
# Build the entire project
make

# Build Docker image (contains Maelstrom testing framework)
make build

# Run Docker container (mounts the project directory)
make run
```

### Testing

```bash
# Build and run specific tests
cd tests
make all
./build/dictionary-tests.out
./build/vec_deque-tests.out
./build/tcp-test.out

# Run tests with memory leak detection
cd tests
make tests  # Uses valgrind to check for memory leaks
```

### Running Challenge Solutions

The following commands should be run inside the Docker container:

```bash
# Challenge 1: Echo
maelstrom test -w echo --bin build/challenge-1.out --node-count 1 --time-limit 10

# Challenge 2: Unique ID Generation
maelstrom test -w unique-ids --bin build/challenge-2.out --time-limit 30 --rate 1000 --node-count 3 --availability total --nemesis partition

# Challenge 3a: Single Node Broadcast
maelstrom test -w broadcast --bin build/challenge-3a.out --node-count 1 --time-limit 20 --rate 10

# Challenge 3b: Multi Node Broadcast
maelstrom test -w broadcast --bin build/challenge-3bc.out --node-count 5 --time-limit 20 --rate 10

# Challenge 3c: Fault Tolerant Broadcast
maelstrom test -w broadcast --bin build/challenge-3bc.out --node-count 5 --time-limit 20 --rate 10 --nemesis partition
```

## Code Architecture

### Library Components

The project is organized around several key library components:

1. **Collections Library** (`lib/collections.c/h`)
   - Dictionary: Hash table implementation with open addressing
   - List: Dynamic array with automatic resizing
   - Queue: Linked list-based queue implementation

2. **VecDeque** (`lib/vec_deque.c/h`)
   - Double-ended queue implemented with a circular buffer

3. **TCP Library** (`lib/tcp.c/h`)
   - Reliable messaging layer with sequencing and retransmission
   - Channel state management for peers

4. **Utility Library** (`lib/util.c/h`)
   - JSON message handling (parsing, creation)
   - Node management and topology functions

5. **Stopwatch** (`lib/stopwatch.c/h`)
   - Time measurement utilities

### Challenge Implementations

Each challenge builds on these core components:

- **Challenge 1** (`src/challenge-1/`): Simple echo server using basic message handling
- **Challenge 2** (`src/challenge-2/`): Unique ID generation with node-specific IDs
- **Challenge 3a** (`src/challenge-3a/`): Single node broadcast implementation
- **Challenge 3bc** (`src/challenge-3bc/`): Multi-node broadcast with fault tolerance

### Key Architectural Patterns

1. **Message-Passing Architecture**
   - JSON-based messages for all communication
   - Request-response patterns
   - Event loop pattern in all challenge implementations

2. **Memory Management**
   - Clear ownership semantics throughout the codebase
   - Functions for element cleanup passed to collections
   - Careful memory allocation and deallocation

3. **Error Handling**
   - Extensive error checking with descriptive messages
   - Abort-on-error pattern in critical sections

4. **Reliable Communication**
   - TCP layer implements sequence numbers and acknowledgments
   - Automatic retransmission of unacknowledged messages
   - Message deduplication using sequence numbers