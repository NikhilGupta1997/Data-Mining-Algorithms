import numpy as np 
import sys
import matplotlib.pyplot as plt

dataset = sys.argv[1]
clusters = sys.argv[2]

data = np.array([val.strip().split() for val in open(dataset, 'r').readlines()])
X = data[:, 0]
Y = data[:, 1]

cluster_inds = []
inds = []
for line in open(clusters, 'r').readlines():
	if line == '\n':
		cluster_inds.append(inds)
		inds = []
	elif '#' not in line:
		inds.append(int(line.strip()))

color = {4: 'red', 1: 'blue', 2: 'green', 3: 'yellow', 6: 'black', 0: 'cyan', 5: 'magenta', }
plt.figure()
count = 0
for i, inds in enumerate(cluster_inds):
	count += len(inds)
	print count, len(inds)
	x_val = X[inds]
	y_val = Y[inds]
	plt.scatter(x_val, y_val, c=color[(i%7)], s=2, edgecolor=color[(i%6+1)])

plt.show()