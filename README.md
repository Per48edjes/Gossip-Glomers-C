# Gossip Glomers in C

This repo houses solutions [Fly.io](https://fly.io/)'s Gossip Glomers challenges, implemented in C.

❓: **What's Gossip Glomers?** See [here](https://fly.io/blog/gossip-glomers/) for more information!


## Getting Set Up

The workflow to develop this project leverages Docker to provide the runtime for `maelstrom` (the testing framework for Gossip Glomers). For developer quality of life (dev QoL), we recommend developing locally and volume mounting
the container. This allows you to use your local development environment (e.g., choice of editor, LSP, debugger, etc.) while still having the runtime available for testing and program execution.

### Requirements

The following are what's needed to set up the development and execution environments for this project:

- [ ] Docker Desktop (namely, the Docker daemon will need to be running to run the container)

Note that the following two dependencies are already included in the runtime (see `Dockerfile`), but are worth installing locally for dev QoL (e.g., LSPs may require having `json-c` source on hand).

- [ ] `json-c` (e.g., on Mac, this can be installed using `brew install json-c`; see the [project Github](https://github.com/json-c/json-c) for more information)
- [ ] `pkg-config` (e.g., on Mac, this can be installed using `brew install pkg-config`) 

### Building the Project

The top-level `Makefile` governs the build dependency graph. For convenience, the entire project can be built by simply executing:

```bash
make
```

in the container's `/app` directory (read: the same directory, in the container, mounted to the local directory containing this project).


# Challenges

## Challenge 1

To run the solution to [Challenge 1: Echo](https://fly.io/dist-sys/1/), execute the following (in `/app`) in the container (after building the project):

```bash
maelstrom test -w echo --bin build/challenge-1.out --node-count 1 --time-limit 10
```

## Challenge 2

To run the solution to [Challenge 2: Unique ID Generation](https://fly.io/dist-sys/2/), execute the following (in `/app`) in the container (after building the project):

```bash
maelstrom test -w unique-ids --bin build/challenge-2.out --time-limit 30 --rate 1000 --node-count 3 --availability total --nemesis partition
```

## Challenge 3a

To run the solution to [Challenge 3a: Single Node Broadcast](https://fly.io/dist-sys/3a/), execute the following (in `/app`) in the container (after building the project):

```bash
maelstrom test -w broadcast --bin build/challenge-3a.out --node-count 1 --time-limit 20 --rate 10
```

## Challenges 3b & 3c

To run the solution to [Challenge 3b: Multi Node Broadcast](https://fly.io/dist-sys/3b/) and [Challenge 3b: Fault Tolerant Broadcast](https://fly.io/dist-sys/3c/),, execute the following (in `/app`) in the container (after building the project):

```bash
# Challenge 3b
maelstrom test -w broadcast --bin build/challenge-3bc.out --node-count 5 --time-limit 20 --rate 10

# Challenge 3c
maelstrom test -w broadcast --bin build/challenge-3bc.out --node-count 5 --time-limit 20 --rate 10 --nemesis partition
```

---

```bash
Everything looks good! ヽ(‘ー`)ノ
```
