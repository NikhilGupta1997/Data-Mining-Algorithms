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

typedef KDTreeSingleIndexAdaptor<
		L2_Simple_Adaptor<float, PointCloud<float> > ,
		PointCloud<float>, 3 /* dim */ > my_kd_tree_t;

vector<std::pair<size_t,float> > radiusSearch(int index, float epsilon, PointCloud<float>& cloud, my_kd_tree_t &ind) {
	const float search_radius = static_cast<float>(epsilon);
	std::vector<std::pair<size_t,float> >   ret_matches;

	nanoflann::SearchParams params;
	//params.sorted = false;

	const float query_pt[3] = {cloud.pts[index].x, cloud.pts[index].y, cloud.pts[index].z};
	const size_t nMatches = ind.radiusSearch(&query_pt[0], search_radius, ret_matches, params);

	cout << "radiusSearch(): radius=" << search_radius << " -> " << nMatches << " matches\n";
	for (size_t i = 0; i < nMatches; i++)
		cout << "idx["<< i << "]=" << ret_matches[i].first << " dist["<< i << "]=" << ret_matches[i].second << endl;
	cout << "\n";
	return ret_matches;
}

class DBScan{
private:
	int minPts;
	double eps;
	vector<vector<int>> clusters;
	vector<int> point_to_cluster;
	set<int> noise;


public:
	DBScan(int minPts, double eps, PointCloud<float>& cloud, my_kd_tree_t& ind){
		this->minPts = minPts;
		this->eps = eps;
		// std::ifstream file(filename);

		// CSVRow row;
		// while(file >> row) {
	 //    	this->points.push_back(row.val());
		// }
		int size = cloud.pts.size();
		point_to_cluster.resize(size, UNCLASSIFIED);
		int clusterid = 0;
		// clusters.resize(1);
		for(int i=0;i<size;i++){
			if(point_to_cluster[i]==UNCLASSIFIED){
				if(expand_cluster(i, clusterid, cloud, ind))
					clusterid++;
			}
		}

	}

	bool expand_cluster(int index, int clusterid, PointCloud<float>& cloud, my_kd_tree_t& ind){
		vector<pair<size_t,float> > seeds = radiusSearch(index, eps, cloud, ind);
		if(seeds.size()<minPts){
			point_to_cluster[index] = NOISE;
			noise.insert(index);
		}
		else{
			int si = seeds.size();
			vector<int> clust;
			queue<int> values;
			for(int i=0;i<si;i++){
				int ind = seeds[i].first;
				point_to_cluster[ind] = clusterid;
				clust.push_back(ind);
				if(ind!=index)
					values.push(ind);
			}
			while(!values.empty()){
				int inde = values.front();
				values.pop();
				vector<pair<size_t,float> > result = radiusSearch(inde, eps, cloud, ind);
				for(auto pt:result){
					int val = point_to_cluster[pt.first];
					if(val==UNCLASSIFIED || val==NOISE){
						if(val==UNCLASSIFIED && pt.first!=inde)
							values.push(pt.first);
						point_to_cluster[pt.first] = clusterid;
						clust.push_back(pt.first);
					}
				}
			}
			clusters.push_back(clust);
			return true;
		}
	}

	void output() {
  		ofstream myfile ("dbscan.txt");
  		assert(myfile.is_open());
  		for (int i = 0; i < clusters.size(); i++) {
  			myfile << "#" << i << "\n";
  			for (auto ele : clusters[i]) {
  				myfile << ele << "\n";
  			}
  			myfile << "\n";
  		}
		myfile.close();
  	}



};


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
	DBScan db(minpoints, epsilon, cloud, index);
	db.output();
	

	return 0;
}