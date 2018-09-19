import time
import warnings

import numpy as np
import matplotlib.pyplot as plt

from sklearn import cluster, datasets, mixture
from sklearn.neighbors import kneighbors_graph
from sklearn.preprocessing import StandardScaler
from itertools import cycle, islice

np.random.seed(0)

X, y = datasets.make_circles(n_samples=8000, noise=.1, factor=0.5)

# kpca = KernelPCA(kernel="rbf", fit_inverse_transform=True, gamma=10)
# X_kpca = kpca.fit_transform(X)
# X_back = kpca.inverse_transform(X_kpca)
# pca = PCA()
# X_pca = pca.fit_transform(X)

# Plot results

plt.figure()
plt.title("Original space")
reds = y == 0
blues = y == 1

x_val = 400
y_val = 200
plt.scatter(X[reds, 0]*x_val, X[reds, 1]*y_val, c="red",
            s=20, edgecolor='k')
plt.scatter(X[blues, 0]*x_val, X[blues, 1]*y_val, c="blue",
            s=20, edgecolor='k')
plt.xlabel("$x_1$")
plt.ylabel("$x_2$")

mean = np.array([-50,0])
cov = np.array([[100, 50], [50, 50]])
data = np.random.multivariate_normal(mean, cov, 200)

x = data[:,0]
y = data[:,1]

plt.scatter(x, y)

mean = np.array([-50,0])
cov = np.array([[100, 50], [50, 50]])
data2 = np.random.multivariate_normal(mean, cov, 200)


x = data2[:,0]
y = data2[:,1]*-1

plt.scatter(x, y, color='red')

mean = np.array([50,0])
cov = np.array([[100, 50], [50, 50]])
data = np.random.multivariate_normal(mean, cov, 100)

x = data[:,0]
y = data[:,1]

plt.scatter(x, y)

mean = np.array([50,0])
cov = np.array([[100, 50], [50, 50]])
data2 = np.random.multivariate_normal(mean, cov, 100)


x = data2[:,0]
y = data2[:,1]*-1

plt.scatter(x, y, color='red')


plt.show()
