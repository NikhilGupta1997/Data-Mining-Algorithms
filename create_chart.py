import numpy as np 
import sys
import matplotlib.pyplot as plt

file = 'optics.txt'
minpts = int(sys.argv[1])
epsilon = float(sys.argv[2])

X = []
Y = []
cluster_inds = []
inds = []
noise = []
buff = []
counter = 0
for i, line in enumerate(open(file).readlines()):
	counter += 1
	val = line.strip().split()
	idx = int(val[0])
	dist = float(val[1])
	if dist < 0.0:
		dist = epsilon*epsilon
		buff.append(idx)
		if len(inds) > 100*minpts:
			cluster_inds.append(inds)
			noise.extend(buff)
			buff = []
			inds = []
	else:
		inds.append(idx)
	X.append(i)
	Y.append(dist)
noise.extend(buff)
noise.extend(inds)
# if len(inds) >= 0:
# 	cluster_inds.append(inds)
# cluster_inds.append(noise)

plt.figure()
plt.plot(X, Y)
plt.legend()
plt.xlabel('Point ID')
plt.ylabel('Reachability Distance')
plt.xticks([])

plt.title('Reachability Graph')

# plt.show()

dataset = sys.argv[3]
data = np.array([val.strip().split() for val in open(dataset, 'r').readlines()])
if data.shape[1] == 2:
	X = data[:, 0]
	Y = data[:, 1]

	color = {4: 'red', 1: 'blue', 2: 'green', 3: 'yellow', 0: 'black', 5: 'cyan', 6: 'magenta', }
	plt.figure()
	count = 0
	for i, inds in enumerate(cluster_inds):
		count += len(inds)
		print(count, len(inds))
		x_val = X[inds]
		y_val = Y[inds]
		plt.scatter(x_val, y_val, c=color[(i%6+1)], s=2, edgecolor=color[(i%6+1)])

	# print noise
	count += len(noise)
	print(count, len(noise))
	x_val = X[noise]
	y_val = Y[noise]
	plt.scatter(x_val, y_val, c='black', s=2)

plt.show()