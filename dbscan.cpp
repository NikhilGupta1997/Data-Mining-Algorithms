#include "nanoflann.hpp"
#include "utils.h"
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "helper.cpp"
#include <time.h> 
#include <math.h>
#include <fstream>
#include <set>
#include <queue>

using namespace std;
using namespace nanoflann;

const int NOISE = -1;
const int UNCLASSIFIED = -2;

// class DBScan{
// private:
// 	int minPts;
// 	double eps;
// 	vector<vector<coord>> points;
// 	vector<vector<int>> clusters;
// 	vector<int> point_to_cluster;
// 	set<int> noise;

// public:
// 	DBScan(int minPts, double eps, string filename){
// 		this->minPts = minPts;
// 		this->eps = eps;
// 		std::ifstream file(filename);

// 		CSVRow row;
// 		while(file >> row) {
// 	    	this->points.push_back(row.val());
// 		}
// 		int size = points.size();
// 		point_to_cluster.resize(size, UNCLASSIFIED);
// 		int clusterid = 0;
// 		// clusters.resize(1);
// 		for(int i=0;i<size;i++){
// 			if(point_to_cluster[i]==UNCLASSIFIED){
// 				if(expand_cluster(i, clusterid))
// 					clusterid++;
// 			}
// 		}

// 	}

// 	bool expand_cluster(int index, int clusterid){
// 		vector<int> seeds = regionQuery(index, eps, points);
// 		if(seeds.size()<minPts){
// 			point_to_cluster[index] = NOISE;
// 			noise.insert(index);
// 		}
// 		else{
// 			int si = seeds.size();
// 			vector<int> clust;
// 			queue<int> values;
// 			for(int i=0;i<si;i++){
// 				int ind = seeds[i];
// 				point_to_cluster[ind] = clusterid;
// 				clust.push_back(ind);
// 				if(ind!=index)
// 					values.push(ind);
// 			}
// 			while(!values.empty()){
// 				int ind = values.front();
// 				values.pop();
// 				vector<int> result = regionQuery(ind, eps, points);
// 				for(auto pt:result){
// 					int val = point_to_cluster[pt];
// 					if(val==UNCLASSIFIED || val==NOISE){
// 						if(val==UNCLASSIFIED && pt!=ind)
// 							values.push(pt);
// 						point_to_cluster[pt] = clusterid;
// 						clust.push_back(pt);
// 					}
// 				}
// 			}
// 			clusters.push_back(clust);
// 		}
// 	}



// };

typedef KDTreeSingleIndexAdaptor<
		L2_Simple_Adaptor<float, PointCloud<float> > ,
		PointCloud<float>, 3 /* dim */ > my_kd_tree_t;

vector<std::pair<size_t,float> > radiusSearch(my_kd_tree_t &index, float epsilon) {
	const float search_radius = static_cast<float>(epsilon);
	std::vector<std::pair<size_t,float> >   ret_matches;

	nanoflann::SearchParams params;
	//params.sorted = false;

	const float query_pt[3] = {0.5, 0.5, 0.5};
	const size_t nMatches = index.radiusSearch(&query_pt[0], search_radius, ret_matches, params);

	cout << "radiusSearch(): radius=" << search_radius << " -> " << nMatches << " matches\n";
	for (size_t i = 0; i < nMatches; i++)
		cout << "idx["<< i << "]=" << ret_matches[i].first << " dist["<< i << "]=" << ret_matches[i].second << endl;
	cout << "\n";
	return ret_matches;
}

int main(int argc, char **argv){
	cout << "You have entered " << argc 
         << " arguments:" << "\n"; 
  
    for (int i = 0; i < argc; ++i) 
        cout << argv[i] << "\n"; 

	int minpoints = stoi(argv[1]);
	float epsilon = stof(argv[2]);
	string filename = argv[3];

    srand(static_cast<unsigned int>(time(nullptr)));
  	PointCloud<float> cloud;
  	
	std::ifstream file(filename);
	vector<vector<coord>> points;
  	CSVRow row;
	while(file >> row) {
    	points.push_back(row.val());
	}
	addpoints(cloud, points);

	my_kd_tree_t index(3 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(10 /* max leaf */) );	
	index.buildIndex();

	vector<std::pair<size_t,float> > results = radiusSearch(index, epsilon);

	return 0;
}