# Innlevering 3

This repository contains the graph implementations from problem 2, the
algorithms for problems 3.1 and 3.2, and GNU make demo targets for problems
2.1 through 3.2.

Completed in code:

1. Problems 2.1 to 2.6: graph data structures, file I/O, abstract interface,
   matrix representation, deletion operations, and rule-of-five support.
2. Problem 3.1: strongly connected components using Tarjan's algorithm.
3. Problem 3.2: query for diamond-shaped labeled paths.
4. Problem 3.3: GNU make demo targets for the implemented problems.

Not yet completed as deliverables:

1. Problem 3.4: scaling experiments, averaged runtime measurements, plots, and
   analysis.
2. Problem 3.5: entity-relationship diagram.
3. Problem 3.6: final PDF report with sections 2.1 through 3.5.

The code supports two graph representations, reads graph data in triple
notation, and exposes a shared abstract interface so the same high-level
algorithms can run on both graph types. This README explains the codebase for a
collaborator who needs both the big picture and the implementation details,
and it also documents what remains to finish the assignment.

## Assignment status overview

This section gives a direct mapping from assignment items to repository state.

- Problem 2.1
  Implemented. The linked incidence-list graph is `LinkedGraph`.
- Problem 2.2
  Implemented. Graphs can be loaded from and saved to triple notation.
- Problem 2.3
  Implemented. `AbstractGraph` is the shared interface.
- Problem 2.4
  Implemented. `MatrixGraph` is the second concrete implementation.
- Problem 2.5
  Implemented. `disconnect` and `remove_node` exist in both graph classes.
- Problem 2.6
  Implemented. Both graph classes define destructor, copy constructor, copy
  assignment, move constructor, and move assignment.
- Problem 3.1
  Implemented. Tarjan's SCC algorithm is in `Algorithms.cpp`.
- Problem 3.2
  Implemented. The diamond-path query is in `Algorithms.cpp`.
- Problem 3.3
  Implemented. `Makefile` contains demo targets `pro21` through `pro32`.
- Problem 3.4
  Partly prepared. Benchmark executables and timing output exist, but the
  scaling study, averaging, plots, and written analysis still need to be done.
- Problem 3.5
  Not yet done.
- Problem 3.6
  Not yet done.

## What the repository contains

This section maps each file to its purpose. Read this section first if you want
to understand how the codebase is organized before reading the algorithms.

- `AbstractGraph.h`
  Defines the common interface shared by both graph representations. The key
  idea is that code outside the graph implementation does not need to know
  whether the graph is stored as a linked structure or as a matrix.
- `Node.h` and `Node.cpp`
  Implement graph nodes. Each node has a unique label and stores the set of
  incident edges touching that node.
- `Edge.h` and `Edge.cpp`
  Implement directed labeled edges. Each edge stores its label, source node,
  target node, and the two incident nodes.
- `LinkedGraph.h` and `LinkedGraph.cpp`
  Implement the linked representation. This class is the current project's
  equivalent of the `Graph` class from `Innlevering 2`.
- `MatrixGraph.h` and `MatrixGraph.cpp`
  Implement the matrix-based representation. It stores edge labels in a matrix
  cell structure and also stores `Edge` objects in a separate edge list.
- `Algorithms.h` and `Algorithms.cpp`
  Implement the assignment algorithms. This file contains Tarjan's algorithm
  for strongly connected components and the query algorithm for diamond-shaped
  paths.
- `Cli.h` and `Cli.cpp`
  Parse command-line arguments, parse the query file for problem 3.2, and
  create the requested graph representation.
- `graph-benchmark-scc.cpp`
  Main program for problem 3.1. It loads a graph, runs Tarjan's algorithm, and
  prints either benchmark output or a detailed component listing.
- `graph-benchmark-diamond.cpp`
  Main program for problem 3.2. It loads a graph, reads the query file, runs
  the labeled path query, and prints either benchmark output or all matching
  node pairs.
- `graph-demo-pro2.cpp`
  Small demonstration program used by the `make pro21` to `make pro26`
  targets. It gives fast, readable demos for the problem 2 requirements.
- `Makefile`
  Builds the benchmark executables and provides demo targets for problems 2.1
  to 3.2.
- `knowledge-test-data/`
  Input files used for the diamond query benchmark.
- `scc-test-data/`
  Input files used for the SCC benchmark.
- `graph-benchmark-scc`, `graph-benchmark-diamond`, and `graph-demo-pro2`
  Compiled binaries produced by `make`.

## How the graph layer is designed

The graph layer is intentionally separated from the algorithms. This lets you
write the algorithm once and apply it to both graph implementations.

The abstraction lives in `AbstractGraph.h`. Every graph must implement:

- `insert_edge(...)`
- `disconnect(...)`
- `remove_node(...)`
- `find_node(...)`
- `nodes()`
- `edges()`
- `save(...)`
- `load(...)`

The most important methods for the assignment itself are `load`, `nodes`, and
`edges`. Those are the methods used directly by the benchmark programs and by
the algorithm layer.

### Why the current graph matches Innlevering 2

The current graph code was aligned with the design from `Innlevering 2`.
Concretely, that means:

- Nodes store incident edges, not a dedicated outgoing-edge adjacency list.
- Edges use the `sourceNode()` and `targetNode()` naming style.
- Both graph types expose editing operations such as `disconnect` and
  `remove_node`.
- The linked graph uses a map from label to node.
- The matrix graph stores label sequences in matrix cells.

The only intentional deviation is naming: this repository keeps the class name
`LinkedGraph` so the current benchmark and CLI code do not need to be renamed.
Functionally, it now plays the same role as `Graph` in `Innlevering 2`.

## How input is read

The assignment requires triple notation, which means each graph edge is stored
as one line with three tokens:

```txt
nodeA edgeLabel nodeB.
```

Examples:

```txt
n0 r0 n7.
n0 r1 n11.
```

Both graph implementations read input through `load(...)`.

- In `LinkedGraph.cpp`, `load` reads triples and inserts one edge at a time.
- In `MatrixGraph.cpp`, `load` reads the same triples and inserts them into the
  matrix representation.

For problem 3.2, the query file contains exactly two sequences, one per line:

```txt
r0 r2 r4 r6.
r1 r3 r5 r7.
```

`Cli.cpp` parses those two sequences and returns them as vectors of labels.

## How problem 3.1 is solved

Problem 3.1 asks for Tarjan's algorithm for strongly connected components in a
directed graph.

A strongly connected component is a maximal set of nodes where every node can
reach every other node by following directed edges.

### Where the implementation lives

The SCC implementation lives in `Algorithms.cpp` in two main pieces:

- `build_outgoing_edges(...)`
- `tarjan_strongly_connected_components(...)`

The helper `build_outgoing_edges(...)` exists because the transferred graph
design from `Innlevering 2` stores only incident edges on each node. Tarjan
needs outgoing traversal, so the algorithm first scans `graph.edges()` and
builds an outgoing adjacency map from each source node to its outgoing edges.

### Tarjan state

Tarjan's algorithm uses a DFS and keeps the following state:

- `index_by_node`
  The DFS discovery index assigned when a node is first visited.
- `lowlink_by_node`
  The smallest discovery index reachable from that node while staying within
  the active DFS stack.
- `on_stack`
  Whether a node is currently on Tarjan's stack.
- `stack`
  The active DFS stack.
- `components`
  The strongly connected components collected so far.

### Tarjan flow

For each unvisited node, the algorithm runs `strongconnect(...)`.

That function does the following:

1. It assigns the node a discovery index and initial lowlink.
2. It pushes the node onto the stack.
3. It explores every outgoing edge from that node.
4. If the target node is unvisited, it recurses and then updates the current
   node's lowlink using the child's lowlink.
5. If the target node is already on the stack, it updates the current node's
   lowlink using the target node's discovery index.
6. If the node's lowlink equals its discovery index, the node is the root of a
   strongly connected component. The algorithm then pops nodes from the stack
   until it reaches that root.

This is the standard Tarjan condition for detecting the boundary of one SCC.

### Output formatting

After the SCCs are found, the code sorts:

- the nodes within each component by label
- the component list by the first label in each component

This sorting is not part of Tarjan's correctness. It exists to make output
stable and easier to compare.

## How problem 3.2 is solved

Problem 3.2 asks for all node pairs `(ni, nj)` such that there are two labeled
paths from `ni` to `nj`:

- one matching the first sequence, such as `r0 r2 r4 ...`
- one matching the second sequence, such as `r1 r3 r5 ...`

### Core idea

The implementation is based on exact labeled reachability.

For each start node, the algorithm computes all target nodes reachable by:

- the first label sequence
- the second label sequence

Then it intersects those target sets. Any target that appears in both sets
produces one valid `(start, target)` pair.

### Reachability algorithm

The helper `compute_sequence_reachability(...)` performs the sequence match.

For each start node:

1. It begins with a frontier containing only the start node.
2. For each label in the sequence, it expands the frontier by following only
   outgoing edges with that exact label.
3. It deduplicates nodes reached at the same step.
4. After the final label, the remaining frontier contains all targets that
   match the full sequence exactly.

This procedure is repeated once for the first sequence and once for the second
sequence.

### Why this matches the assignment

The assignment describes paths labeled `r0-r2-r4-...` and `r1-r3-r5-...`.
The current implementation accepts those sequences from a query file rather
than hard-coding them. That is more general but still satisfies the assignment,
because the benchmark query files use exactly the required pattern.

## How the CLI works

The CLI layer is in `Cli.cpp`. It handles both programs.

### SCC program

You can run problem 3.1 in either of these forms:

```bash
./graph-benchmark-scc <nodes> <edges> <graph-file> [flags]
./graph-benchmark-scc <graph-file> [flags]
```

### Diamond program

You can run problem 3.2 in either of these forms:

```bash
./graph-benchmark-diamond <nodes> <edges> <graph-file> <query-file> [flags]
./graph-benchmark-diamond <graph-file> <query-file> [flags]
```

### Flags

The supported flags are:

- `--silent` or `-s`
- `--verbose` or `-v`
- `--linked`
- `--matrix`

### Silent versus verbose mode

Silent mode prints one compact benchmark line. This is useful when measuring
performance or comparing linked and matrix representations.

Verbose mode prints human-readable details:

- representation
- output mode
- expected or inferred counts
- loaded counts
- timing
- SCC listing or diamond pair listing

## GNU make demos

Problem 3.3 asks for demo targets runnable directly through GNU make. This
repository now provides the following targets:

- `make pro21`
  Demonstrates the linked incidence-list graph from problem 2.1.
- `make pro22`
  Demonstrates file input and output in triple notation.
- `make pro23`
  Demonstrates the shared abstract interface with both graph implementations.
- `make pro24`
  Demonstrates the matrix-based graph implementation.
- `make pro25`
  Demonstrates `disconnect` and `remove_node`, including isolated-node cleanup.
- `make pro26`
  Demonstrates the rule of five for both graph classes.
- `make pro31`
  Runs a fast SCC demo on a small input file.
- `make pro32`
  Runs a fast diamond-query demo on a small graph and query file.

These demos are intentionally small so they finish immediately and are suitable
for grading demonstrations.

## Design choices

This section explains the main architectural decisions and why they were made.

### Shared abstract interface

The algorithms take `const AbstractGraph&` instead of a concrete graph type.
This keeps the algorithm code independent of storage details.

### Reuse of the Innlevering 2 graph model

The graph model was aligned with the earlier assignment so that the current
submission remains consistent with the previous work. This reduces cognitive
overhead for anyone already familiar with `Innlevering 2`.

### Derived outgoing adjacency in the algorithms

Because the transferred graph model stores only incident edges on each node,
the algorithms derive outgoing adjacency from `graph.edges()`. This keeps the
graph layer faithful to `Innlevering 2` while still giving Tarjan and the
diamond query the directed traversal structure they need.

### Deterministic output

The algorithms sort components and pairs before printing. Deterministic output
helps benchmarking, debugging, and grading because runs are easier to compare.

### Optional automatic counts

The original benchmark-style command format includes node and edge counts on
the command line. This repository still supports that form, but it also lets
you omit those counts. When omitted, the program prints `auto` in verbose mode
and reports the loaded counts after parsing the graph.

## Review findings from the strict pass

This section documents issues that a strict grader could plausibly comment on.
These findings are not blockers for the basic benchmark runs, but they are the
main weaknesses in the current code.

### Finding 1: graph loading does not reject missing files explicitly

`LinkedGraph::load` and `MatrixGraph::load` open the file and immediately start
reading triples, but they do not check whether the file opened successfully.

Practical effect:

- If the path is wrong, the graph can remain empty without a clear graph-file
  error.
- A strict grader may expect the program to fail fast with a descriptive
  message, especially because `Cli.cpp` already does this for the query file.

### Finding 2: graph loading does not validate triple notation strictly

The current `load` functions read triples with stream extraction and only strip
the trailing period when present. They do not reject a malformed third token
that lacks the required final `.`.

Practical effect:

- A malformed line can still be accepted as if it were valid input.
- A strict grader may consider this too permissive because the assignment
  explicitly states triple notation.

### Finding 3: matrix node removal can leave dangling incident-edge pointers

`MatrixGraph::removeNodeByIndex(...)` deletes edges touching the removed node,
but it does not clean those deleted edge pointers out of the `incidentEdges`
vectors of the surviving neighbor nodes.

Practical effect:

- If later code traverses or mutates those incident-edge lists, it can observe
  dangling pointers.
- This matters mainly for editing operations such as repeated `remove_node` or
  `disconnect` calls, not for the benchmark path where graphs are only loaded
  and queried.

## File-by-file guide for a collaborator

This section is a condensed reading plan for someone joining the project.

If you want to understand the project quickly, read the files in this order:

1. Start with `README.md`.
2. Read `AbstractGraph.h` to understand the common graph contract.
3. Read `Node.h` and `Edge.h` to understand the base data model.
4. Read `LinkedGraph.h` and `MatrixGraph.h` to see the two representations.
5. Read `Cli.h` and `Cli.cpp` to understand how programs are started.
6. Read `Algorithms.h` and `Algorithms.cpp` to understand the two assignment
   solutions.
7. Read `graph-benchmark-scc.cpp` and `graph-benchmark-diamond.cpp` to see how
   the benchmark drivers connect input, graph creation, algorithms, and output.

## Build and run

Build the project with:

```bash
make
```

Run the demo targets with:

```bash
make pro21
make pro22
make pro23
make pro24
make pro25
make pro26
make pro31
make pro32
```

Run SCC in linked mode:

```bash
./graph-benchmark-scc scc-test-data/<file> --verbose --linked
```

Run SCC in matrix mode:

```bash
./graph-benchmark-scc scc-test-data/<file> --verbose --matrix
```

Run the diamond query in linked mode:

```bash
./graph-benchmark-diamond knowledge-test-data/kb13a.dat \
  knowledge-test-data/qy13a.dat --verbose --linked
```

Run the diamond query in matrix mode:

```bash
./graph-benchmark-diamond knowledge-test-data/kb13a.dat \
  knowledge-test-data/qy13a.dat --verbose --matrix
```

## What remains for the rest of the assignment

This section describes the remaining work for problems 3.4, 3.5, and 3.6.
These are not just code tasks; they are deliverable tasks.

### Problem 3.4: scaling and runtime measurement

Part of the infrastructure is already present:

- `graph-benchmark-scc.cpp` measures graph load time and SCC runtime.
- `graph-benchmark-diamond.cpp` measures graph load time and diamond-query
  runtime.
- Both programs support `--silent`, which is suitable for collecting data.
- Both programs support both graph representations, so linked and matrix runs
  can be compared directly.

What still needs to be done:

1. Choose a range of graph sizes `n`.
2. Run both graph representations for problem 3.1 across those sizes.
3. Repeat each run several times and compute average runtime.
4. Run both graph representations for problem 3.2 under two scenarios:
   - constant query size `m`
   - query size `m` proportional to `n`
5. Store the results in a structured format such as CSV.
6. Create diagrams showing runtime as a function of `n`.
7. Write a short analysis of the observed scaling behavior.

Suggested interpretation of what is already done versus missing:

- Already done:
  benchmark executables, silent timing output, representation switch.
- Missing:
  experimental methodology, averaged measurements, plots, and written
  discussion.

### Problem 3.5: entity-relationship diagram

This still needs to be produced manually as a diagram. The code architecture
already gives the content that the diagram should show.

The diagram should at least include:

- `AbstractGraph`
- `LinkedGraph`
- `MatrixGraph`
- `Node`
- `Edge`
- inheritance from `AbstractGraph` to the two concrete graph classes
- associations between graph objects and nodes/edges
- the main data relationships:
  - linked graph owns node and edge objects
  - matrix graph owns node objects, edge objects, and matrix cells
  - edge connects a source node and a target node
  - node stores incident edges

### Problem 3.6: report/summary

The final PDF report still needs to be written. The assignment asks for a brief
summary with section titles corresponding to problems 2.1 through 3.5 and a
maximum size of 3 pages.

A practical structure would be:

1. Page 1: problems 2.1 to 2.6
2. Page 2: problems 3.1 to 3.3
3. Page 3: problems 3.4 and 3.5

For each section, the report should briefly state:

- what was implemented
- the main design choice
- any important tradeoff or limitation
- for 3.4, the measurement setup and scaling conclusions
- for 3.5, the class/data architecture shown in the ER diagram

## Next steps

If you want to finish the remaining assignment work in a sensible order, the
best sequence is:

1. Add strict graph-file validation to both `load` functions.
2. Fix the dangling-pointer issue in `MatrixGraph::removeNodeByIndex(...)`.
3. Run and record the scaling experiments for problem 3.4.
4. Produce the ER diagram for problem 3.5.
5. Write the 3-page PDF summary for problem 3.6.
