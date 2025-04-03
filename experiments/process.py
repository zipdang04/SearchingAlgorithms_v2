import pandas as pd
import matplotlib.pyplot as plt

table = pd.read_csv("stats.csv", sep = '\t')

data = dict()
for idx, row in table.iterrows():
	filename = row["filename"]
	algo = row["algo_name"]
	
	if data.get(filename) is None:
		data[filename] = dict()
	data[filename][algo] = int(row["count_steps"]), int(row["expanded_nodes"]), float(row["time"])

stats_node = dict()
stats_step = dict()
stats_time = dict()
BASE = "Focal Search"
for filename in data:
	tmp = data[filename]
	current_step, current_node, current_time = data[filename][BASE]
	for algo in tmp:
		if stats_step.get(algo) is None:
			stats_step[algo] = []
			stats_node[algo] = []
			stats_time[algo] = []
		step, node, time = data[filename][algo]
		stats_step[algo].append(step / current_step)
		stats_node[algo].append(node / current_node)
		stats_time[algo].append(time / current_time)

nodes = []
steps = []
times = []
for tmp in stats_node.values(): nodes.append(tmp)
for tmp in stats_step.values(): steps.append(tmp)
for tmp in stats_time.values(): times.append(tmp)
algoName = []
for key in stats_node.keys(): algoName.append(key)

fig, axs = plt.subplots(nrows=1, ncols=3)
axs[0].set_title('Expanded nodes')
axs[0].boxplot(nodes)
axs[1].set_title('Execution time')
axs[1].boxplot(times)
axs[2].set_title('Solution length')
axs[2].boxplot(steps)
for ax in axs:
	ax.set_xticks([y + 1 for y in range(len(nodes))],
			   labels=["FS", "PFS-0.6", "A*-PE", "PFS-0.95"])
plt.show()