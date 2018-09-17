#include <iostream>
#include <string>
#include <assert.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include "helper.cpp"

using namespace std;

int myrandom (int i) { return std::rand()%i;}

class KMeans {
  private: 
  	vector<vector <coord> > clusters;

  public:
  	void add_cluster(vector<coord> point) {
  		clusters.push_back(point);
  	}

  	void update_cluster(int index, vector<coord> point) {
  		assert(index < clusters.size());
  		clusters[index] = point;
  	}

  	KMeans(PointData data, int k) {
  		vector<int> index;
  		for(int i = 0; i < data.size(); i++) {
  			index.push_back(i);
  		}
		random_shuffle(index.begin(), index.end(), myrandom);
		for(int i = 0; i < k; i++) {
	  		add_cluster(data[index[i]]);
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

};

int main(int argc, char **argv){
	srand(time(0)); 
    int k = stoi(argv[1]);
    cout << "Number of Clusters = " << k << endl;
    
    PointData data(argv[2]);
    cout << "Number of Points = " << data.size() << endl;
    
    KMeans clusters(data, k);
    clusters.print();
	return 0;
}