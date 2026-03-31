import matplotlib.pyplot as plt


with open('results/benchmark_results.csv') as f:
    lines = f.readlines()


data = []
for line in lines[1:]:  # Skip header
    line = line.strip()
    if line:  # Skip empty lines
        parts = line.split(',')
        n = int(parts[0])
        rep = parts[1]
        time = int(parts[2])
        data.append((n, rep, time))


test1 = data[0:10]    # Diamond constant
test2 = data[10:20]   # Diamond proportional
test3 = data[20:30]   # SCC


def get_curves(test_data):
    linked_n = []
    linked_t = []
    matrix_n = []
    matrix_t = []
    
    for n, rep, time in test_data:
        if rep == 'linked':
            linked_n.append(n)
            linked_t.append(time)
        else:
            matrix_n.append(n)
            matrix_t.append(time)
    
    return linked_n, linked_t, matrix_n, matrix_t


test_configs = [
    (test1, "Diamond: Constant (m=5)"),
    (test2, "Diamond: Proportional (m=n/10)"),
    (test3, "SCC Algorithm")
]


fig, ax = plt.subplots(1, 3, figsize=(15, 4))
fig.suptitle('Task 3.4 - Scaling  Runtime', fontsize=14, fontweight='bold')

for idx, (test_data, title) in enumerate(test_configs):
    ln, lt, mn, mt = get_curves(test_data)
    
    ax[idx].plot(ln, lt, 'o-', label='linked', linewidth=2, markersize=6)
    ax[idx].plot(mn, mt, 's-', label='matrix', linewidth=2, markersize=6)
    ax[idx].set_xlabel('Nodes (n)')
    ax[idx].set_ylabel('Time (ms)')
    ax[idx].set_title(title)
    ax[idx].legend()
    ax[idx].grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('results/benchmark_plots.png', dpi=150)
print("Saved: results/benchmark_plots.png")


fig, ax = plt.subplots(1, 3, figsize=(15, 4))
fig.suptitle('Log-Log Plots (Complexity)', fontsize=14, fontweight='bold')

for idx, (test_data, title) in enumerate(test_configs):
    ln, lt, mn, mt = get_curves(test_data)
    
    
    lt = [max(0.1, t) for t in lt]
    mt = [max(0.1, t) for t in mt]
    
    ax[idx].loglog(ln, lt, 'o-', label='linked', linewidth=2, markersize=6)
    ax[idx].loglog(mn, mt, 's-', label='matrix', linewidth=2, markersize=6)
    ax[idx].set_xlabel('Nodes (n)')
    ax[idx].set_ylabel('Time (ms)')
    ax[idx].set_title(title)
    ax[idx].legend()
    ax[idx].grid(True, alpha=0.3, which='both')

plt.tight_layout()
plt.savefig('results/benchmark_loglog.png', dpi=150)
print("Saved: results/benchmark_loglog.png")
print("\nBoth plots generated!")
