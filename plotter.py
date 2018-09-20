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

noise = range(0, len(X))


color = {4: 'red', 1: 'blue', 2: 'green', 3: 'yellow', 0: 'black', 5: 'cyan', 6: 'magenta', }
plt.figure()
count = 0
for i, inds in enumerate(cluster_inds):
	for ind in inds:
		noise.remove(ind)
	count += len(inds)
	print count, len(inds)
	x_val = X[inds]
	y_val = Y[inds]
	print len(x_val), len(y_val)
	plt.scatter(x_val, y_val, c=color[(i%6+1)], s=2, edgecolor=color[(i%6+1)])

x_val = X[noise]
y_val = Y[noise]
print len(x_val), len(y_val)
plt.scatter(x_val, y_val, c=color[0], s=2, edgecolor='k')
plt.show()