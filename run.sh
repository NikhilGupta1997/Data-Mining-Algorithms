#!/bin/bash

# Parse the dynamic options
case $1 in
    -kmeans)
    ./kmeans $2 $3
    shift
    ;;
    -dbscan)
    ./dbscan $2 $3 $4
    shift
    ;;
    -optics)
    ./optics $2 $3 $4
    shift
    ;;
    *)
    echo "$0 : what do you mean by $i ?"
    usage
    exit
    ;;
esac 
