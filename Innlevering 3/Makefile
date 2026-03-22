CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -pedantic

COMMON_SOURCES := \
	Algorithms.cpp \
	Cli.cpp \
	Edge.cpp \
	LinkedGraph.cpp \
	MatrixGraph.cpp \
	Node.cpp

all: graph-benchmark-scc graph-benchmark-diamond

graph-benchmark-scc: graph-benchmark-scc.cpp $(COMMON_SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ graph-benchmark-scc.cpp $(COMMON_SOURCES)

graph-benchmark-diamond: graph-benchmark-diamond.cpp $(COMMON_SOURCES)
	$(CXX) $(CXXFLAGS) -o $@ graph-benchmark-diamond.cpp $(COMMON_SOURCES)

clean:
	rm -f graph-benchmark-scc graph-benchmark-diamond

.PHONY: all clean
