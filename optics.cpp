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
#include <algorithm>

using namespace std;
using namespace nanoflann;

const float UNDEFINED = -1.0;
ofstream myfile ("optics.txt");

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

struct Compare{
	constexpr bool operator()(pair<int, float> const & t1, pair<int, float> const & t2) const noexcept{
		return t1.second>t2.second;
	}
};

class Optics{
private:
	int minPts;
	double eps;
	vector<float> core_dist;
	vector<bool> point_to_processed;
	vector<float> reachability_dist;

public:
	Optics(int minPts, double eps, PointCloud<float>& cloud, my_kd_tree_t& ind){
		this->minPts = minPts;
		this->eps = eps;
		int size = cloud.pts.size();
		point_to_processed.resize(size, false);
		core_dist.resize(size, UNDEFINED);
		reachability_dist.resize(size, UNDEFINED);
		
		assert(myfile.is_open());
		// clusters.resize(1);
		for(int i=0;i<size;i++){
			if(!point_to_processed[i]){
				expand_cluster(i, cloud, ind);
			}
		}

	}

	// write custom operator for queue as well
	bool expand_cluster(int index, PointCloud<float>& cloud, my_kd_tree_t& ind){
		vector<pair<size_t,float> > seeds = radiusSearch(index, eps, cloud, ind);
		point_to_processed[index] = true;
		reachability_dist[index] = UNDEFINED;

		// assumption - distances returned are sorted
		core_dist[index] = seeds[minPts-1].second;

		myfile<<index<<"\n";
		myfile<<core_dist[index]<<"\n";
		myfile<<reachability_dist[index]<<"\n";
		myfile<<"\n";
		priority_queue<pair<int, float>, vector<pair<int, float>>, Compare> pq;

		if(core_dist[index]!=UNDEFINED){
			update(seeds, index, pq, cloud, ind);
			while(!pq.empty()){
				pair<size_t,float> elem;
				elem = pq.top();
				pq.pop();
				// check this as well
				while(point_to_processed[elem.first] && !pq.empty()){
					elem = pq.top();
					pq.pop();
				}
				if(!point_to_processed[elem.first]){
					int inde = elem.first;
					vector<pair<size_t,float> > result = radiusSearch(inde, eps, cloud, ind);
					point_to_processed[inde] = true;

					// assumptions - distances returned are sorted
					core_dist[inde] = result[minPts-1].second;

					myfile<<inde<<"\n";
					myfile<<core_dist[inde]<<"\n";
					myfile<<reachability_dist[inde]<<"\n";
					myfile<<"\n";
					if(core_dist[inde]!=UNDEFINED)
						update(result, inde, pq, cloud, ind);
				}
			}
		}
		
	}


	void update(vector<pair<size_t,float> > seeds, int index, priority_queue<pair<int, float>, vector<pair<int, float>>, Compare>& pq, PointCloud<float>& cloud, my_kd_tree_t& ind){
		float c_dist = core_dist[index];
		for (auto &elem:seeds){
			if(!point_to_processed[index]){
				float new_r_dist = max(c_dist, elem.second);
				if(reachability_dist[elem.first]==UNDEFINED){
					reachability_dist[elem.first] = new_r_dist;
					pair<int, float> tem = make_pair(elem.first, new_r_dist);
					pq.push(tem);
				}
				else{
					if(new_r_dist<reachability_dist[elem.first]){
						reachability_dist[elem.first] = new_r_dist;
						pair<int, float> tem = make_pair(elem.first, new_r_dist);
						pq.push(tem);
					}
				}
			}
		}
	}

	// void output() {
 //  		ofstream myfile ("dbscan.txt");
 //  		assert(myfile.is_open());
 //  		for (int i = 0; i < clusters.size(); i++) {
 //  			myfile << "#" << i << "\n";
 //  			for (auto ele : clusters[i]) {
 //  				myfile << ele << "\n";
 //  			}
 //  			myfile << "\n";
 //  		}
	// 	myfile.close();
 //  	}



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
	
	Optics op(minpoints, epsilon, cloud, index);
	myfile.close();

	

	return 0;
}