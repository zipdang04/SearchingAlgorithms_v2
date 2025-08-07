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

TIME_LIMIT = 600
TEST_SETTINGS = {
	TestSetting(n=8, C=8): 70,
	TestSetting(n=10, C=8): 70,
	TestSetting(n=12, C=8): 70,
	TestSetting(n=14, C=9): 70,
	TestSetting(n=15, C=9): 70,
	# TestSetting(n=18, C=9): 125,
	# TestSetting(n=19, C=9): 125,
	# TestSetting(n=20, C=9): 125,
}
# TEST_SETTINGS = {
# 	TestSetting(n=20, C=5): 125,
# 	TestSetting(n=25, C=10): 125,
# 	TestSetting(n=27, C=11): 125,
# 	TestSetting(n=28, C=12): 125,
# 	TestSetting(n=29, C=13): 125,
# 	TestSetting(n=30, C=15): 125,
# }
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
for key in TEST_SETTINGS:
	n, C = key.n, key.C
	COUNT = TEST_SETTINGS[key]
	for i in range(COUNT):
		inputFile = makeTest(n)
		tmp = []
		for PATH in EXE:
			try:
				fInput = open(inputFile, "r")
				process = subprocess.run([PATH, "--C", str(C), "--tl", str(TIME_LIMIT)], stdin=fInput, capture_output=True)
				output = process.stdout.decode().split(sep = '\n')
				for group in range(0, len(output), 7):
					if (group + 4 >= len(output)): break
					name = output[group][6:]
					expanded_nodes = output[group + 1].split()[-1]
					iteration_count = output[group + 2].split()[-1]
					max_size = output[group + 3].split()[-1]
					answer = float(output[group + 4].split()[-1])
					time = float(output[group + 6].split()[-1])
					data = [4, C, inputFile, name, expanded_nodes, iteration_count, max_size, time, answer]
					tmp.append(time)
					print(*data, sep = '\t')
				print("C = {} | done test {}".format(C, i + 1), file=sys.stderr)
			except subprocess.TimeoutExpired:
				tmp.append(-1)
				print("C = {} | test {} is too hard ({})".format(C, i + 1, inputFile), file=sys.stderr)
		if len(tmp) > 0:
			print(inputFile, *tmp, file=sys.stderr)
