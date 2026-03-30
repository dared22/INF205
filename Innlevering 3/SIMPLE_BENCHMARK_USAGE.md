# Task 3.4 - Simple C++ Benchmarking Guide

## Building

```bash
make clean
make
```

This builds:
- `generate-graph` - for diamond path diamond queries
- `generate-scc` - for SCC algorithm testing
- `graph-benchmark-scc` - SCC algorithm benchmark
- `graph-benchmark-diamond` - diamond path query benchmark

## Running Benchmarks

### 1. Diamond Path Analysis (Problem 3.2)

Generate a test graph and query, then measure performance:

```bash
# Generate graph with 1000 nodes and query path length 10
./generate-graph 1000 10 test-graph.dat test-query.dat

# Run benchmark with LinkedGraph representation
./graph-benchmark-diamond test-graph.dat test-query.dat --linked

# Run benchmark with MatrixGraph representation
./graph-benchmark-diamond test-graph.dat test-query.dat --matrix

# Run in silent mode for automated timing collection
./graph-benchmark-diamond test-graph.dat test-query.dat --linked -s
./graph-benchmark-diamond test-graph.dat test-query.dat --matrix -s
```

### 2. SCC Algorithm Analysis (Problem 3.1)

Generate a test graph and measure performance:

```bash
# Generate graph with 1000 nodes and 10000 edges
./generate-scc 1000 10000 test-scc-graph.dat

# Run benchmark with LinkedGraph
./graph-benchmark-scc test-scc-graph.dat --linked

# Run benchmark with MatrixGraph
./graph-benchmark-scc test-scc-graph.dat --matrix

# Run in silent mode for automated timing collection
./graph-benchmark-scc test-scc-graph.dat --linked -s
./graph-benchmark-scc test-scc-graph.dat --matrix -s
```

## Scaling Experiments

To measure complexity scaling, test multiple graph sizes:

```bash
# Test 1: constant query size (m=5)
./generate-graph 1000 5 graph_1000.dat query_1000.dat
./graph-benchmark-diamond graph_1000.dat query_1000.dat --linked -s
./graph-benchmark-diamond graph_1000.dat query_1000.dat --matrix -s

./generate-graph 5000 5 graph_5000.dat query_5000.dat
./graph-benchmark-diamond graph_5000.dat query_5000.dat --linked -s
./graph-benchmark-diamond graph_5000.dat query_5000.dat --matrix -s

# Test 2: proportional query size (m = n/10)
./generate-graph 1000 100 graph_1000_prop.dat query_1000_prop.dat
./graph-benchmark-diamond graph_1000_prop.dat query_1000_prop.dat --linked -s
./graph-benchmark-diamond graph_1000_prop.dat query_1000_prop.dat --matrix -s

./generate-graph 5000 500 graph_5000_prop.dat query_5000_prop.dat
./graph-benchmark-diamond graph_5000_prop.dat query_5000_prop.dat --linked -s
./graph-benchmark-diamond graph_5000_prop.dat query_5000_prop.dat --matrix -s
```

## Example: Complete Scaling Test

Run this to test multiple graph sizes:

```bash
#!/bin/bash

# Test sizes
for n in 100 500 1000 5000; do
    echo "=== Testing n=$n ==="
    
    # Diamond: constant query size m=5
    ./generate-graph $n 5 diamond_const_$n.dat query_const_$n.dat
    echo "Diamond (const m=5):"
    ./graph-benchmark-diamond diamond_const_$n.dat query_const_$n.dat --linked -s
    ./graph-benchmark-diamond diamond_const_$n.dat query_const_$n.dat --matrix -s
    
    # Diamond: proportional query size m=n/10
    q=$((n / 10))
    ./generate-graph $n $q diamond_prop_$n.dat query_prop_$n.dat
    echo "Diamond (prop m=n/10):"
    ./graph-benchmark-diamond diamond_prop_$n.dat query_prop_$n.dat --linked -s
    ./graph-benchmark-diamond diamond_prop_$n.dat query_prop_$n.dat --matrix -s
    
    # SCC: edges = n * log(n) for reasonable density
    e=$((n * (int)log(n)))
    ./generate-scc $n $e scc_$n.dat
    echo "SCC:"
    ./graph-benchmark-scc scc_$n.dat --linked -s
    ./graph-benchmark-scc scc_$n.dat --matrix -s
    
    echo
done
```

## Output Interpretation

When running with `-s` (silent) flag, output is:
```
representation=linked loaded_nodes=1000 loaded_edges=5000 pairs=4 load_ms=5 diamond_ms=23
```

Key values:
- `loaded_nodes` - actual nodes in generated graph
- `loaded_edges` - actual edges in generated graph
- `load_ms` - time to load graph from file
- `diamond_ms` / `scc_ms` - algorithm execution time in milliseconds

## Manual Timing

To time multiple runs and average:

```bash
# Collect 5 runs
for i in {1..5}; do
    ./graph-benchmark-diamond diamond_1000.dat query_1000.dat --linked -s | grep diamond_ms
done

# Average the results manually or write them to a file
./graph-benchmark-diamond diamond_1000.dat query_1000.dat --linked -s >> results.txt
```

## Scaling Analysis

To analyze complexity, collect timing data at different sizes and plot:

**Expected behavior:**
- Diamond path (constant m): likely O(n) to O(n²) depending on graph density
- Diamond path (proportional m): likely O(n²) to O(n³)
- SCC (Tarjan): O(n + e) = O(n) for sparse graphs, O(n²) for dense

Use the timing data to:
1. Plot `time vs. n` on linear scale
2. Plot `time vs. n` on log-log scale
3. Measure slope in log-log space:
   - Slope ≈ 1.0 → O(n)
   - Slope ≈ 1.15 → O(n log n)
   - Slope ≈ 2.0 → O(n²)
   - Slope ≈ 3.0 → O(n³)

## Windows PowerShell Version

```powershell
# Test different sizes
foreach ($n in 100, 500, 1000, 5000) {
    Write-Host "=== Testing n=$n ==="
    
    # Generate and test diamond (constant)
    & ".\generate-graph.exe" $n 5 "diamond_const_$n.dat" "query_const_$n.dat"
    Write-Host "Diamond (const m=5):"
    & ".\graph-benchmark-diamond.exe" "diamond_const_$n.dat" "query_const_$n.dat" "--linked" "-s"
    & ".\graph-benchmark-diamond.exe" "diamond_const_$n.dat" "query_const_$n.dat" "--matrix" "-s"
    
    # Generate and test SCC
    $edges = [int]($n * [Math]::Log($n))
    & ".\generate-scc.exe" $n $edges "scc_$n.dat"
    Write-Host "SCC:"
    & ".\graph-benchmark-scc.exe" "scc_$n.dat" "--linked" "-s"
    & ".\graph-benchmark-scc.exe" "scc_$n.dat" "--matrix" "-s"
    
    Write-Host ""
}
```

## Quick Test

```bash
# Test with small graph
./generate-graph 10 2 tiny.dat tiny_query.dat
./graph-benchmark-diamond tiny.dat tiny_query.dat --linked

./generate-scc 10 20 tiny_scc.dat
./graph-benchmark-scc tiny_scc.dat --linked
```

This should complete in < 1 second and show the algorithm works.

## Notes

- Use `-s` flag for automated timing (no verbose output)
- For accurate timing, run multiple times and average
- Larger graphs take longer - adjust sizes based on your patience
- LinkedGraph typically faster for sparse graphs
- MatrixGraph typically faster for dense graphs
