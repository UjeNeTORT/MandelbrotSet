import matplotlib.pyplot as plt
import numpy as np
import sys

CMD_LINE_ERR_MSG = "not enough command line arguments provided. (Use python3 analyze_results.py <path_to_graph.png> <test_results_path.csv> <test_results_name>)"
assert len (sys.argv) == 4, CMD_LINE_ERR_MSG

graph_path        = str (sys.argv[1])
test_results_path = str (sys.argv[2])
test_results_name = str (sys.argv[3])

brute_force_results = []
vectorized_results  = []

with open (test_results_path, "r") as test_results_file:
    lines = test_results_file.readlines ()
    brute_force_results = lines[0].split(',')[1:]
    vectorized_results  = lines[1].split(',')[1:]

bf_i = brute_force_results.index(' \n')
brute_force_results = brute_force_results[:bf_i] + brute_force_results[bf_i + 1:]
v_i = vectorized_results.index(' \n')
vectorized_results = vectorized_results[:v_i] + vectorized_results[v_i + 1:]

brute_force_results = [int(res) for res in brute_force_results]
vectorized_results  = [int(res) for res in vectorized_results]

brute_mean = np.mean (brute_force_results)
brute_std  = np.std (brute_force_results)
brute_eps  = round (brute_std / brute_mean * 100, 2)

vec_mean = np.mean (vectorized_results)
vec_std  = np.std (vectorized_results)
vec_eps  = round (vec_std / vec_mean * 100, 2)

plt.figure()
plt.xlabel("test No")
plt.ylabel("performance, CPU clock cycles")
plt.title(test_results_name)
plt.grid(True, linestyle = '--')
plt.scatter(range (0, len (brute_force_results)), brute_force_results, s=3, c = "blue")
plt.scatter(range (0, len (vectorized_results)), vectorized_results, s=3, c = "red")

plt.axhline (y=brute_mean, label = f"brute force: {round (brute_mean / 1e10, 2)} $\cdot$ 1e10 ($\pm$ {brute_eps} %)", color="blue")
plt.axhline (y=vec_mean, label = f"vectorized: {round (vec_mean / 1e10, 2)} $\cdot$ 1e10 ($\pm$ {vec_eps} %)", color = "red")

plt.savefig (graph_path, bbox_extra_artists=(plt.legend(),), bbox_inches='tight')

print ("graph saved: " + graph_path)