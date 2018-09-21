import time
import warnings
import sys

import numpy as np
import matplotlib.pyplot as plt

from sklearn import cluster, datasets, mixture
from sklearn.neighbors import kneighbors_graph
from sklearn.preprocessing import StandardScaler
from itertools import cycle, islice

file = sys.argv[1]
size = int(sys.argv[2])

with open(file, 'w') as f:

    np.random.seed(0)

    X, y = datasets.make_circles(n_samples=int(0.85*size), noise=.1, factor=0.5)
    for val in X:
        f.write(str(val[0]*1200) + ' ' + str(val[1]*1000) + '\n')

    plt.figure()
    plt.title("Original space")
    reds = y == 0
    blues = y == 1

    x_val = 1200
    y_val = 1000
    plt.scatter(X[reds, 0]*x_val, X[reds, 1]*y_val, c="red",
                s=2, edgecolor='k')
    plt.scatter(X[blues, 0]*x_val, X[blues, 1]*y_val, c="blue",
                s=2, edgecolor='k')
    plt.xlabel("$x_1$")
    plt.ylabel("$x_2$")

    
    X, y = datasets.make_blobs(n_samples=int(0.15*size), centers=1, n_features=2, random_state=0, cluster_std=50, center_box=(79.0, 80.0))
    reds = y == 0
    for val in X:
        f.write(str(val[0]) + ' ' + str(val[1]) + '\n')

    x_val = 1
    y_val = 1
    plt.scatter(X[reds, 0]*x_val, X[reds, 1]*y_val, c="red",
                s=2, edgecolor='k') 
    plt.xlabel("$x_1$")
    plt.ylabel("$x_2$")

    # X, y = datasets.make_blobs(n_samples=int(0.04*size), centers=1, n_features=2, random_state=0, cluster_std=20, center_box=(-100.0, -99.0))
    # reds = y == 0
    # for val in X:
    #     f.write(str(val[0]) + ' ' + str(val[1]) + '\n')

    # x_val = 1
    # y_val = 1
    # plt.scatter(X[reds, 0]*x_val, X[reds, 1]*y_val, c="red",
    #             s=2, edgecolor='k') 
    # plt.xlabel("$x_1$")
    # plt.ylabel("$x_2$")


    # X, y = datasets.make_blobs(n_samples=int(0.04*size), centers=1, n_features=2, random_state=1, cluster_std=30, center_box=(80.0, 81.0))
    # reds = y == 0
    # for val in X:
    #     f.write(str(val[0]) + ' ' + str(val[1]) + '\n')
    # x_val = 1
    # y_val = 1
    # plt.scatter(X[reds, 0]*x_val, X[reds, 1]*y_val, c="blue",
    #             s=2, edgecolor='k')
    # plt.xlabel("$x_1$")
    # plt.ylabel("$x_2$")

    # X = np.random.rand(int(0.07*size), 2) - 0.5
    # plt.scatter(X[:, 0]*3000, X[:, 1]*2000, c="green",
    #             s=2, edgecolor='k')
    # for val in X:
    #     f.write(str(val[0]*3000) + ' ' + str(val[1]*2000) + '\n')

    plt.show()
