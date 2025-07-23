import json

import os, sys, subprocess
import datetime

class TestSetting:
	def __init__(self, n, C):
		self.n = n; self.C = C
	def __hash__(self):
		return self.n * 10**30 + self.C
	def __eq__(self, value):
		return self.n == value.n and self.C == value.C

filenames = []
with open("supplement_inputs") as f:
	filenames = f.readlines()

TIME_LIMIT = 600
EXE = [
	"../../build/SearchingAlgorithmsV2"
]
def currentDirectory() -> str:
	return os.path.dirname(os.path.abspath(__file__))
os.chdir(currentDirectory())

def makeTest(n: int) -> str:
	command = ["../../build/DataCreation", "--n", str(n), "--m", "100000", "--edge_mode", "SINGLE"]
	process = subprocess.run(command, capture_output=True)
	return process.stderr.decode().split()[1]

sys.stdout = open("statistics"+str(int(datetime.datetime.now().timestamp() * 1000000))+".csv", "w")
columns = ["n", "C", "filename", "algo_name", "expanded_nodes", "iteration_count", "max_size", "time", "answer"]
print(*columns, sep="\t")
for inputFile in filenames:
	tmp = []
	n = int(inputFile.split('-')[1][1:])
	for PATH in EXE:
		fInput = open(inputFile, "r")
		process = subprocess.run([PATH, "--tl", str(TIME_LIMIT)], stdin=fInput, capture_output=True)
		output = process.stdout.decode().split(sep = '\n')
		for group in range(0, len(output), 7):
			if (group + 4 >= len(output)): break
			name = output[group][6:]
			expanded_nodes = output[group + 1].split()[-1]
			iteration_count = output[group + 2].split()[-1]
			max_size = output[group + 3].split()[-1]
			answer = float(output[group + 4].split()[-1])
			time = float(output[group + 6].split()[-1])
			data = [4, 0, inputFile, name, expanded_nodes, iteration_count, max_size, time, answer]
			tmp.append(time)
			print(*data, sep = '\t')
		print("done test {}".format(inputFile), file=sys.stderr)
	if len(tmp) > 0:
		print(inputFile, *tmp, file=sys.stderr)
