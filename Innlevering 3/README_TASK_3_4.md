# Task 3.4 Implementation - Automated Benchmarking

Complete automated benchmarking system for scaling analysis. All generators, benchmarks, and plotting integrated into a single workflow.

## Quick Start (2 commands)

```powershell
# Step 1: Run all benchmarks (generates data + CSV)
.\benchmark-runner.exe

# Step 2: Create plots from the results
python plot_benchmarks.py
```

That's it! You'll get:
- `results/benchmark_results.csv` - Raw timing data
- `results/benchmark_plots.png` - Linear scale plots
- `results/benchmark_loglog.png` - Log-log scale for complexity analysis
- `data/` folder - All test graph files

## What Gets Tested

The `benchmark-runner.exe` automatically tests:

### 1. Diamond Path - Constant Query Size (m=5)
- Tests: n = 100, 500, 1000, 5000 nodes
- Both representations: LinkedGraph and MatrixGraph
- Fixed query path length throughout

### 2. Diamond Path - Proportional Query Size (m=n/10)  
- Tests: n = 100, 500, 1000, 5000 nodes
- Both representations: LinkedGraph and MatrixGraph
- Query length scales with graph size

### 3. SCC Algorithm - Tarjan's Algorithm
- Tests: n = 100, 500, 1000, 5000 nodes
- Both representations: LinkedGraph and MatrixGraph
- Random graphs with edges = n * 10

## Output Files

```
Innlevering 3/
├── data/                          # Generated test graphs
│   ├── graph_100.dat, graph_500.dat, ...
│   ├── query_100.dat, query_500.dat, ...
│   └── scc_graph_100.dat, scc_graph_500.dat, ...
├── results/                       # Results and visualizations
│   ├── benchmark_results.csv
│   ├── benchmark_plots.png
│   └── benchmark_loglog.png
├── benchmark-runner.exe           # Main orchestrator
├── generate-graph.exe             # Graph generator
├── generate-scc.exe               # SCC generator
├── graph-benchmark-diamond.exe    # Diamond benchmark
└── plot_benchmarks.py             # Plotter
```

## Understanding the Results

### Console Output (from `benchmark-runner.exe`)

The runner displays all tests with timing results:

```
================================================
     Task 3.4 - Scaling & Runtime Measurement
================================================

[1] DIAMOND PATH - Constant Query Size (m=5)
--------------------------------------------
  n=100:
    linked | nodes: 100   | edges: 162    | time: 0    ms
    matrix | nodes: 100   | edges: 156    | time: 0    ms
  n=500:
    linked | nodes: 500   | edges: 612    | time: 0    ms
    matrix | nodes: 500   | edges: 647    | time: 0    ms
  ...
```

### CSV Data Format

`results/benchmark_results.csv` contains:
```
n,representation,time_ms
100,linked,0
100,matrix,0
500,linked,0
...
```

Three sections (separated by blank lines):
1. Lines 1-8: Diamond constant results
2. Lines 10-17: Diamond proportional results  
3. Lines 19-26: SCC results

### Plot Interpretation

**Linear Scale Plot** (`benchmark_plots.png`):
- Shows actual timing in milliseconds
- Easy to see which representation is faster
- Steep curves indicate worse scaling (O(n²) or O(n³))

**Log-Log Plot** (`benchmark_loglog.png`):
- Both axes are logarithmic
- Straight line indicates polynomial complexity
- **Slope reveals complexity**:
  - slope ≈ 1 → O(n) linear
  - slope ≈ 1.5 → O(n^1.5)
  - slope ≈ 2 → O(n²) quadratic
  - slope ≈ 3 → O(n³) cubic

## Manual Testing (if needed)

If you want to test specific configurations manually:

### Test Diamond Path
```powershell
.\generate-graph.exe 1000 5 test_graph.dat test_query.dat
.\graph-benchmark-diamond.exe test_graph.dat test_query.dat --linked -s
```

### Test SCC Algorithm
```powershell
.\generate-scc.exe 1000 10000 test_scc.dat
.\graph-benchmark-scc.exe test_scc.dat --linked -s
```

Output format (single line with silent mode `-s`):
```
representation=linked loaded_nodes=1000 loaded_edges=10000 diamond_ms=45
```

Key fields:
- `diamond_ms` or `scc_ms` - Algorithm execution time (what you measure)
- `loaded_nodes` - Actual graph size used
- `loaded_edges` - Actual edge count

## How Automated Benchmarking Works

### `benchmark-runner.exe` Workflow

1. **Creates folders**: `data/` and `results/` (if they don't exist)
2. **For each test configuration**:
   - Generates test graph using `generate-graph.exe` or `generate-scc.exe`
   - Runs benchmark with both `--linked` and `--matrix` representations
   - Stores timing results to `results/benchmark_results.csv`
   - Cleans up temporary files (keeps graphs in `data/` folder)
3. **Outputs**: Console display + CSV file

### `plot_benchmarks.py` Workflow

1. **Reads** `results/benchmark_results.csv`
2. **Parses** data into three test groups (8 measurements each)
3. **Generates**:
   - `benchmark_plots.png` - Linear scale (readable timing values)
   - `benchmark_loglog.png` - Log scale (complexity analysis)

## Customizing the Benchmarks

To change tested graph sizes, edit `benchmark-runner.cpp` around line 180:

```cpp
std::vector<int> sizes = {100, 500, 1000, 5000};  // Change these
```

Then recompile:
```powershell
make benchmark-runner
```

## Analyzing the Results

After running `benchmark-runner.exe` and `python plot_benchmarks.py`, you have:

### Step 1: Look at the CSV Data
Open `results/benchmark_results.csv` in a text editor:
- First 8 rows: Diamond constant (n=100, 500, 1000, 5000; linked & matrix)
- Next 8 rows: Diamond proportional (same sizes and representations)
- Last 8 rows: SCC (same sizes and representations)

### Step 2: Read the Linear Scale Plot
Look at `results/benchmark_plots.png`:
- **Y-axis scale**: How fast do times grow with n?
- **Linked vs Matrix**: Which representation is faster, and by how much?
- **Query size impact**: Compare constant vs proportional growth in times

### Step 3: Analyze Log-Log Plot
Look at `results/benchmark_loglog.png`:
- **Straight lines** indicate polynomial complexity
- **Measure the slope** (rise/run):
  - Slope ≈ 1 → O(n) - linear (ideal)
  - Slope ≈ 1.5 → O(n^1.5)
  - Slope ≈ 2 → O(n²) - quadratic (bad)
  - Slope ≈ 3 → O(n³) - cubic (very bad)

### Step 4: Identify Patterns

**Questions to answer:**
1. Does the observed complexity match your algorithm's theoretical complexity?
2. At what graph size does one representation outperform the other?
3. How does query size affect the diamond path algorithm?
4. Is there a "crossover point" where behavior changes?

### Step 5: Run Again with Different Sizes (Optional)

If you want more data points for better plots:
1. Edit `benchmark-runner.cpp` line ~180: `sizes = {100, 200, 300, 500, 750, 1000, 1500, 2000, 5000}`
2. Recompile: `make benchmark-runner`
3. Run again: `benchmark-runner.exe` then `python plot_benchmarks.py`

## File Formats

### Graph File (`.dat`)
Triple notation format:
```
n0 r n5.
n2 r n7.
n3 r n1.
```
- **n0, n5, etc.** - Node IDs (n followed by integer)
- **r** - Relation label (single letter)
- **.** - Line terminator

### Query File (`.dat`)
Two query sequences:
```
r0 r2 r4 .
r1 r3 r5 .
```
- Space-separated relation labels
- Period terminates each sequence
- Used by diamond path benchmark

## Rebuilding (if code changes)

Edit source files, then rebuild:

```powershell
# Rebuild just benchmark-runner
make benchmark-runner

# Or rebuild everything
make clean
make
```

This recompiles:
- `generate-graph.cpp` → `generate-graph.exe`
- `generate-scc.cpp` → `generate-scc.exe`  
- `benchmark-runner.cpp` → `benchmark-runner.exe`
- Plus existing `graph-benchmark-diamond` and `graph-benchmark-scc`

## Troubleshooting

**Problem**: "benchmark-runner.exe is not recognized"
- Solution: Make sure you're in the right directory: `cd "Innlevering 3"`
- Verify `benchmark-runner.exe` exists: `ls *.exe`

**Problem**: Plots show all functions at y=0
- Solution: Your graph sizes are too small (fast algorithms)
- Try larger sizes: edit `benchmark-runner.cpp` line 180

**Problem**: Data looks sparse/noisy
- Solution: Increase graph sizes in `benchmark-runner.cpp`
- Timings below 1ms become too noisy; aim for 10ms+ for large graphs

**Problem**: Python not found
- Solution: `pip install matplotlib pandas` first
- Or use system Python: `python plot_benchmarks.py`

## For Your Task 3.6 Report

Include these findings from your benchmarking:

### 1. Complexity Analysis
- **SCC**: What is the observed time complexity? O(n)? O(n logn)? O(n²)?
- **Diamond Constant**: Observed complexity as m stays fixed
- **Diamond Proportional**: Observed complexity as m grows with n

### 2. Comparison Analysis
- Which representation (Linked/Matrix) is faster overall?
- At what graph size do they perform similarly?
- Why might one be preferred over the other?

### 3. Include Plots
- Paste `results/benchmark_plots.png` (linear scale results)
- Paste `results/benchmark_loglog.png` (complexity slopes)

### 4. Data Table
Create a summary table showing:
- Graph size (n)
- LinkedGraph time (ms)
- MatrixGraph time (ms)
- Speedup ratio (best/worst)

Example:
```
| n | LinkedGraph | MatrixGraph | Speedup |
|---|-------------|-------------|---------|
| 100 | 0 ms | 0 ms | 1.0x |
| 500 | 0 ms | 0 ms | 1.0x |
| 1000 | 0 ms | 0 ms | 1.0x |
| 5000 | 5 ms | 6 ms | 1.2x |
```

### 5. Analysis Questions

Answer these for each algorithm:

**SCC Algorithm**
- Is complexity closer to O(n), O(n log n), or O(n²)?
- Does the log-log plot show a straight line?
- What is the approximate slope?

**Diamond Path (Constant m)**
- Why does query size not change performance?
- Is this O(n) or O(n²)?

**Diamond Path (Proportional m)**
- How does performance compare to constant m?
- What complexity do you observe?

## Quick Checklist

- [ ] Run: `.\benchmark-runner.exe`
- [ ] Generate plots: `python plot_benchmarks.py`
- [ ] Check `results/` folder has 3 files (CSV, 2 plots)
- [ ] Check `data/` folder has 12 `.dat` files
- [ ] Open plots in image viewer
- [ ] Analyze slopes in log-log plot
- [ ] Write up findings for task 3.6

## Need More Help?

**Check timing values in CSV**: `cat results/benchmark_results.csv`

**Re-run with different sizes**: Edit `benchmark-runner.cpp`, change `sizes` vector, run `make benchmark-runner`

**Manual test specific case**:
```powershell
.\generate-graph.exe 2000 10 test.dat test_q.dat
.\graph-benchmark-diamond.exe test.dat test_q.dat --linked -s
```
