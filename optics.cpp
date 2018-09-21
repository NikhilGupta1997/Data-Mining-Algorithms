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
ofstream myfile("optics.txt");

typedef KDTreeSingleIndexAdaptor<
		L2_Simple_Adaptor<float, PointCloud5<float> > ,
		PointCloud5<float>, 5 /* dim */ > my_kd_tree_t;

vector<std::pair<size_t,float> > radiusSearch(int index, float epsilon, PointCloud5<float>& cloud, my_kd_tree_t &ind) {
	const float search_radius = static_cast<float>(epsilon);
	std::vector<std::pair<size_t,float> >   ret_matches;

	nanoflann::SearchParams params;

	const float query_pt[5] = {cloud.pts[index].v, cloud.pts[index].w, cloud.pts[index].x, cloud.pts[index].y, cloud.pts[index].z};
	const size_t nMatches = ind.radiusSearch(&query_pt[0], search_radius, ret_matches, params);

	return ret_matches;
}

struct Compare{
	constexpr bool operator()(pair<int, float> const & t1, pair<int, float> const & t2) const noexcept{
		return t1.second>t2.second;
	}
};

class Optics{
private:
	int minPts, core_index;
	double eps;
	vector<float> core_dist;
	vector<bool> point_to_processed;
	vector<float> reachability_dist;

public:
	Optics(int minPts, double eps, PointCloud5<float>& cloud, my_kd_tree_t& ind){
		this->minPts = minPts;
		this->eps = eps;
		this->core_index = minPts;
		int size = cloud.pts.size();
		point_to_processed.resize(size, false);
		core_dist.resize(size, UNDEFINED);
		reachability_dist.resize(size, UNDEFINED);
		
		assert(myfile.is_open());
		for(int i=0;i<size;i++){
			if(!point_to_processed[i]){
				expand_cluster(i, cloud, ind);
			}
		}
	}

	// write custom operator for queue as well
	void expand_cluster(int index, PointCloud5<float>& cloud, my_kd_tree_t& ind){
		vector<pair<size_t,float> > seeds = radiusSearch(index, eps, cloud, ind);
		point_to_processed[index] = true;
		reachability_dist[index] = UNDEFINED;
		// assumption - distances returned are sorted
		// cout<<"seeds is "<<endl;
		// for(int i=0;i<seeds.size();i++){
		// 	cout<<seeds[i].first<<" "<<seeds[i].second<<endl;
		// }
		// cout<<endl;
		if(seeds.size()>=minPts){
			core_dist[index] = seeds[core_index-1].second;
		}
		else
			core_dist[index] = UNDEFINED;

		myfile<<index<<" ";
		// myfile<<core_dist[index]<<"\n";
		myfile<<reachability_dist[index]<<"\n";
		priority_queue<pair<int, float>, vector<pair<int, float>>, Compare> pq;
		string v = (core_dist[index]!=UNDEFINED?"1":"0");
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
					// cout<<"result is "<<endl;
					// for(int i=0;i<result.size();i++){
					// 	cout<<result[i].first<<" "<<result[i].second<<endl;
					// }
					// cout<<endl;

					// assumptions - distances returned are sorted
					if(result.size()>=minPts){
						core_dist[inde] = result[core_index-1].second;
					}
					else
						core_dist[inde] = UNDEFINED;

					myfile<<inde<<" ";
					// myfile<<core_dist[inde]<<"\n";
					myfile<<reachability_dist[inde]<<"\n";
					if(core_dist[inde]!=UNDEFINED)
						update(result, inde, pq, cloud, ind);
				}
			}
		} 
	}

	void update(vector<pair<size_t,float> >& seeds, int index, priority_queue<pair<int, float>, vector<pair<int, float>>, Compare>& pq, PointCloud5<float>& cloud, my_kd_tree_t& ind){
		float c_dist = core_dist[index];
		for (auto elem:seeds){
			if(!point_to_processed[elem.first]){
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

vector<coord> pad(vector<coord> v) {
	int len = v.size();
	for(int i = 0; i < 5 - len; i++) {
		v.push_back(0);
	}
	return v;
}


int main(int argc, char **argv){
	cout << "You have entered " << argc 
         << " arguments:" << "\n"; 
  
    for (int i = 0; i < argc; ++i) 
        cout << argv[i] << "\n"; 

	int minpoints = stoi(argv[1]);
	float epsilon = stof(argv[2]);
	epsilon = epsilon*epsilon;
	string filename = argv[3];

    srand(static_cast<unsigned int>(time(nullptr)));
  	PointCloud5<float> cloud;
  	
	std::ifstream file(filename);
	
	vector<vector<coord>> points;
  	CSVRow row;
	while(file >> row) {
    	points.push_back(pad(row.val()));
	}
	addpoints5(cloud, points);

	my_kd_tree_t index(5 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(10 /* max leaf */) );	
	index.buildIndex();
	
	Optics op(minpoints, epsilon, cloud, index);
	myfile.close();

	return 0;
}