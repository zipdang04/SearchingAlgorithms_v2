import json

import os, sys, subprocess
import datetime
datetime.datetime.now().timestamp()

class TestSetting:
	def __init__(self, n, ctrl):
		self.n = n; self.ctrl = ctrl
	def __hash__(self):
		return self.n * 10**30 + self.ctrl
	def __eq__(self, value):
		return self.n == value.n and self.ctrl == value.ctrl

TEST_SETTINGS = {
	TestSetting(n=15, ctrl=50): 50,
	TestSetting(n=15, ctrl=100): 50,
	TestSetting(n=15, ctrl=150): 50,
	TestSetting(n=15, ctrl=200): 50,
	TestSetting(n=20, ctrl=50): 50,
	TestSetting(n=20, ctrl=100): 50,
	TestSetting(n=20, ctrl=150): 50,
	TestSetting(n=20, ctrl=200): 50,
	TestSetting(n=50, ctrl=10): 100,
	TestSetting(n=50, ctrl=15): 100,
	TestSetting(n=50, ctrl=20): 100,
	TestSetting(n=50, ctrl=25): 100,
	TestSetting(n=10, ctrl=0): 200,
	TestSetting(n=15, ctrl=0): 200,
	TestSetting(n=20, ctrl=0): 200,
	TestSetting(n=25, ctrl=0): 200,
}
EXE = [
	"../../build/SearchingAlgorithmsV2"
]
def currentDirectory() -> str:
	return os.path.dirname(os.path.abspath(__file__))
os.chdir(currentDirectory())

def makeTest(n: int, ctrl: int) -> str:
	command = ["../../build/DataCreation", "--n", str(n)]
	if ctrl > 0:
		command.append("--ctrl")
		command.append(str(ctrl))
	process = subprocess.run(command, capture_output=True)
	return process.stderr.decode().split()[1]

sys.stdout = open("statistics"+str(int(datetime.datetime.now().timestamp() * 1000000))+".csv", "w")
columns = ["n", "ctrl", "filename", "algo_name", "expanded_nodes", "iteration_count", "max_size", "time", "count_steps", "steps"]
print(*columns, sep="\t")
for key in TEST_SETTINGS:
	n, ctrl = key.n, key.ctrl
	COUNT = TEST_SETTINGS[key]
	for i in range(COUNT):
		inputFile = makeTest(n, ctrl)
		tmp = []
		for PATH in EXE:
			try:
				fInput = open(inputFile, "r")
				process = subprocess.run([PATH], stdin=fInput, capture_output=True, timeout=600)
				output = process.stdout.decode().split(sep = '\n')
				for group in range(0, len(output), 6):
					if (group + 4 >= len(output)): break
					name = output[group][6:]
					expanded_nodes = output[group + 1].split()[-1]
					iteration_count = output[group + 2].split()[-1]
					max_size = output[group + 3].split()[-1]
					steps = output[group + 4].split()[-1]
					count_steps = len(steps)
					time = float(output[group + 6].split()[-1])
					data = [4, ctrl, inputFile, name, expanded_nodes, iteration_count, max_size, time, count_steps, steps]
					tmp.append(time)
					print(*data, sep = '\t')
				print("ctrl = {} | done test {}".format(ctrl, i + 1), file=sys.stderr)
			except subprocess.TimeoutExpired:
				tmp.append(-1)
				print("ctrl = {} | test {} is too hard ({})".format(ctrl, i + 1, inputFile), file=sys.stderr)
		if len(tmp) > 0:
			print(inputFile, *tmp, file=sys.stderr)
