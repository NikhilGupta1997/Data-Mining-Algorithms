#!/bin/bash

# Parse the dynamic options
case $1 in
    -kmeans)
    ./kmeans $2 $3
    python plotter.py $3 'kmeans.txt'
    shift
    ;;
    -dbscan)
    ./dbscan $2 $3 $4
    python plotter.py $4 'dbscan.txt'
    shift
    ;;
    -optics)
    ./optics $2 $3 $4
    python create_chart.py $2 $3 $4
    shift
    ;;
    *)
    echo "$0 : what do you mean by $i ?"
    usage
    exit
    ;;
esac 
