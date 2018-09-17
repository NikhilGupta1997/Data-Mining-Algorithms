#include <iostream>
#include <string>
#include <assert.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <math.h>
#include <fstream>
#include "helper.cpp"

using namespace std;

int myrandom (int i) { return std::rand()%i;}

class KMeans {
  private: 
  	vector<vector <coord> > clusters;
  	vector<vector<int> > closest;
  	double error, threshold = 0.00001;

  public:
  	void add_cluster(vector<coord> point) {
  		assert(!point.empty());
  		clusters.push_back(point);
  	}

  	void update_cluster(int index, vector<coord> point) {
  		assert(index < clusters.size());
  		assert(!point.empty());
  		clusters[index] = point;
  	}

  	double eu_distance(vector<coord> point, vector<coord> cluster) {
  		double val = 0.0;
  		for (int i = 0; i < point.size(); i++) {
  			val += pow(point[i] - cluster[i], 2);
  		}
  		return val;
  	}

  	void map_points(PointData data) {
  		for(int i = 0; i < clusters.size(); i++) {
  			closest[i].clear();
  		}

  		for(int i = 0; i < data.size(); i++) {
  			vector<coord> point = data[i];
  			double distance = -1.0, dist;
  			int index = 0;
  			for(int j = 0; j < clusters.size(); j++) {
  				dist = eu_distance(point, clusters[j]);
  				if(distance == -1.0 || dist < distance) {
  					distance = dist;
  					index = j;
  				}
  			}
  			closest[index].push_back(i);
  		}
  	}

  	void vector_add(vector <coord>& a, vector <coord> b) {
  		for(int i = 0; i < a.size(); i++) {
  			a[i] = a[i] + b[i];
  		}
  	}

  	void vector_divide(vector <coord>& a, int b) {
  		for(int i = 0; i < a.size(); i++) {
  			a[i] = a[i] / double(b);
  		}
  	}

  	double calculate_clusters(PointData data) {
  		double total_error = 0.0;
  		for(int i = 0; i < clusters.size(); i++) {
  			vector<coord> v;
  			for(auto point : closest[i]) {
  				if(v.empty()) {
  					v = data[point];
  				}
  				else {
  					vector_add(v, data[point]);
  				}
  			}
  			if (v.empty()) {
  				continue;
  			}
  			vector_divide(v, closest[i].size());
  			total_error += eu_distance(v, clusters[i]);
  			update_cluster(i, v);
  		}
  		return total_error;
  	}

  	KMeans(PointData data, int k) {
  		vector<int> index;
  		for(int i = 0; i < data.size(); i++) {
  			index.push_back(i);
  		}
		random_shuffle(index.begin(), index.end(), myrandom);
		for(int i = 0; i < k; i++) {
	  		add_cluster(data[index[i]]);
	  		vector<int> row;
		    closest.push_back(row);
		}
		map_points(data);
		error = calculate_clusters(data);
		while(threshold < error) {
			map_points(data);
			error = calculate_clusters(data);
		}
  	}

  	void print() {
		for(auto ele : clusters) {
	    	for (auto it : ele) {
	    		cout << it << " ";
	    	}
	    	cout << endl;
	    }
  	}

  	void output() {
  		ofstream myfile ("kmeans.txt");
  		assert(myfile.is_open());
  		for (int i = 0; i < clusters.size(); i++) {
  			myfile << "#" << i << "\n";
  			for (auto ele : closest[i]) {
  				myfile << ele << "\n";
  			}
  			myfile << "\n";
  		}
		myfile.close();
  	}

};

int main(int argc, char **argv){
	srand(time(0)); 
    int k = stoi(argv[1]);
    cout << "Number of Clusters = " << k << endl;
    
    PointData data(argv[2]);
    cout << "Number of Points = " << data.size() << endl;
    
    KMeans clusters(data, k);
    clusters.print();
    clusters.output();
	return 0;
}