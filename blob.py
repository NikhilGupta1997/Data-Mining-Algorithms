import time
import warnings

import numpy as np
import matplotlib.pyplot as plt

from sklearn import cluster, datasets, mixture
from sklearn.neighbors import kneighbors_graph
from sklearn.preprocessing import StandardScaler
from itertools import cycle, islice

with open('dataset.txt', 'w') as f:

    np.random.seed(0)

    X, y = datasets.make_circles(n_samples=85000, noise=.1, factor=0.5)
    for val in X:
        f.write(str(val[0]*1000) + ' ' + str(val[1]*600) + '\n')

    plt.figure()
    plt.title("Original space")
    reds = y == 0
    blues = y == 1

    x_val = 1000
    y_val = 600
    plt.scatter(X[reds, 0]*x_val, X[reds, 1]*y_val, c="red",
                s=2, edgecolor='k')
    plt.scatter(X[blues, 0]*x_val, X[blues, 1]*y_val, c="blue",
                s=2, edgecolor='k')
    plt.xlabel("$x_1$")
    plt.ylabel("$x_2$")

    X, y = datasets.make_blobs(n_samples=4000, centers=1, n_features=2, random_state=0, cluster_std=20, center_box=(-80.0, -79.0))
    reds = y == 0
    for val in X:
        f.write(str(val[0]) + ' ' + str(val[1]) + '\n')

    x_val = 1
    y_val = 1
    plt.scatter(X[reds, 0]*x_val, X[reds, 1]*y_val, c="red",
                s=2, edgecolor='k') 
    # plt.scatter(X[blues, 0]*x_val, X[blues, 1]*y_val, c="blue",
    #             s=20, edgecolor='k')
    plt.xlabel("$x_1$")
    plt.ylabel("$x_2$")


    X, y = datasets.make_blobs(n_samples=4000, centers=1, n_features=2, random_state=1, cluster_std=50, center_box=(80.0, 81.0))
    reds = y == 0
    for val in X:
        f.write(str(val[0]) + ' ' + str(val[1]) + '\n')
    x_val = 1
    y_val = 1
    plt.scatter(X[reds, 0]*x_val, X[reds, 1]*y_val, c="blue",
                s=2, edgecolor='k')
    # plt.scatter(X[blues, 0]*x_val, X[blues, 1]*y_val, c="blue",
    #             s=20, edgecolor='k')
    plt.xlabel("$x_1$")
    plt.ylabel("$x_2$")

    X = np.random.rand(7000, 2) - 0.5
    plt.scatter(X[:, 0]*3000, X[:, 1]*2000, c="green",
                s=2, edgecolor='k')
    for val in X:
        f.write(str(val[0]*3000) + ' ' + str(val[1]*2000) + '\n')
    # mean = np.array([-50,0])
    # cov = np.array([[200, 50], [50, 50]])
    # data = np.random.multivariate_normal(mean, cov, 150)

    # x = data[:,0]
    # y = data[:,1]

    # plt.scatter(x, y)

    # mean = np.array([-50,0])
    # cov = np.array([[200, 50], [50, 50]])
    # data2 = np.random.multivariate_normal(mean, cov, 150)


    # x = data2[:,0]
    # y = data2[:,1]*-1

    # plt.scatter(x, y, color='red')

    # mean = np.array([70,0])
    # cov = np.array([[200, 70], [70, 70]])
    # data = np.random.multivariate_normal(mean, cov, 100)

    # x = data[:,0]
    # y = data[:,1]

    # plt.scatter(x, y)

    # mean = np.array([70,0])
    # cov = np.array([[200, 70], [70, 70]])
    # data2 = np.random.multivariate_normal(mean, cov, 100)


    # x = data2[:,0]
    # y = data2[:,1]*-1

    # plt.scatter(x, y, color='red')




    plt.show()
