import pandas as pd
import matplotlib.pyplot as plt

table = pd.read_csv("statistics1747274564728860.csv", sep = '\t')

data = dict()
for idx, row in table.iterrows():
	filename = row["filename"]
	algo = row["algo_name"]
	
	if data.get(filename) is None:
		data[filename] = dict()
	data[filename][algo] = int(row["count_steps"]), int(row["expanded_nodes"]), float(row["time"]), float(row["max_size"])

stats_node = dict()
stats_step = dict()
stats_time = dict()
stats_size = dict()
# allowed = [
# 	"PFS_PE with eps = 1.05; pFocal = 0.6, cBound = 2",
# 	"Probabilistic Focal Search with eps = 1.05; pFocal = 0.6",
# 	"PFS_VDMPE with eps = 1.05; pFocal = 0.6"
# ]
# BASE = allowed[1]
# BASE = "PFS_PE with eps = 1.05; pFocal = 0.6, cBound = 2"
BASE = "Probabilistic Focal Search with eps = 1.05; pFocal = 0.6"
for filename in data:
	tmp = data[filename]
	if (len(tmp) < 3): continue
	current_step, current_node, current_time, current_size = data[filename][BASE]
	if current_step < 55: continue
	for algo in tmp:
		# if algo not in allowed: continue
		if stats_step.get(algo) is None:
			stats_step[algo] = []
			stats_node[algo] = []
			stats_time[algo] = []
			stats_size[algo] = []
		step, node, time, size = data[filename][algo]
		stats_step[algo].append(step / current_step)
		stats_node[algo].append(node / current_node)
		stats_time[algo].append(time / current_time)
		stats_size[algo].append(size / current_size)
nodes = []
steps = []
times = []
sizes = []
for tmp in stats_node.values(): nodes.append(tmp)
for tmp in stats_step.values(): steps.append(tmp)
for tmp in stats_time.values(): times.append(tmp)
for tmp in stats_size.values(): sizes.append(tmp)
algoName = []
for key in stats_node.keys(): algoName.append(key)

print(len(nodes[0]))

# for i in range(len(nodes)):
# 	for j in range(len(nodes[i])):
# 		nodes[i][j] *= 1000
plt.plot(
	steps[1], nodes[1], "bs",
	steps[0], nodes[0], "ro", 
	# steps[2], times[2], "g^",
)
plt.show()

fig, axs = plt.subplots(nrows=1, ncols=4)
axs[0].set_title('Expanded nodes')
axs[0].boxplot(nodes, showfliers = False)
axs[1].set_title('Execution time')
axs[1].boxplot(times, showfliers = False)
axs[2].set_title('Solution length')
axs[2].boxplot(steps, showfliers = False)
axs[3].set_title('Max size')
axs[3].boxplot(sizes, showfliers = False)
for ax in axs:
	ax.set_xticks([y + 1 for y in range(len(nodes))],
			   labels=["PFS_PE", "PFS", "PFS_VDMPE"])#, "VDM_PE", "A*-PE", "FS"])
plt.show()