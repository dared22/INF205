CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

COMMON_SOURCES := \
	Algorithms.cpp \
	Cli.cpp \
	Edge.cpp \
	LinkedGraph.cpp \
	MatrixGraph.cpp \
	Node.cpp

all: graph-benchmark-scc graph-benchmark-diamond generate-graph generate-scc benchmark-runner

graph-benchmark-scc: graph-benchmark-scc.cpp $(COMMON_SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ graph-benchmark-scc.cpp $(COMMON_SOURCES)

graph-benchmark-diamond: graph-benchmark-diamond.cpp $(COMMON_SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ graph-benchmark-diamond.cpp $(COMMON_SOURCES)

generate-graph: generate-graph.cpp
	$(CXX) $(CXXFLAGS) -o $@ generate-graph.cpp

generate-scc: generate-scc.cpp
	$(CXX) $(CXXFLAGS) -o $@ generate-scc.cpp

benchmark-runner: benchmark-runner.cpp
	$(CXX) $(CXXFLAGS) -o $@ benchmark-runner.cpp

graph-demo-pro2: graph-demo-pro2.cpp $(COMMON_SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ graph-demo-pro2.cpp $(COMMON_SOURCES)

pro21: graph-demo-pro2
	./graph-demo-pro2 pro21

pro22: graph-demo-pro2
	./graph-demo-pro2 pro22

pro23: graph-demo-pro2
	./graph-demo-pro2 pro23

pro24: graph-demo-pro2
	./graph-demo-pro2 pro24

pro25: graph-demo-pro2
	./graph-demo-pro2 pro25

pro26: graph-demo-pro2
	./graph-demo-pro2 pro26

pro31: graph-benchmark-scc
	./graph-benchmark-scc scc-test-data/scc02.dat --silent --linked

pro32: graph-benchmark-diamond
	./graph-benchmark-diamond knowledge-test-data/kb03a.dat knowledge-test-data/qy03a.dat --silent --linked

clean:
	rm -f graph-benchmark-scc graph-benchmark-diamond graph-demo-pro2 \
		generate-graph generate-scc benchmark-runner \
		.demo-pro22-input.txt .demo-pro22-output.txt

.PHONY: all clean pro21 pro22 pro23 pro24 pro25 pro26 pro31 pro32
