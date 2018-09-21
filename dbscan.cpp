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

int dim1(int minpoints, float epsilon, string filename) {
	typedef KDTreeSingleIndexAdaptor<
			L2_Simple_Adaptor<float, PointCloud1<float> > ,
			PointCloud1<float>, 1 /* dim */ > my_kd_tree_t;

	class DBScan{
	private:
		int minPts;
		double eps;
		vector<vector<int>> clusters;
		vector<int> point_to_cluster;
		set<int> noise;


	public:
		DBScan(int minPts, double eps, PointCloud1<float>& cloud, my_kd_tree_t& ind){
			this->minPts = minPts;
			this->eps = eps;
			int size = cloud.pts.size();
			point_to_cluster.resize(size, UNCLASSIFIED);
			int clusterid = 0;
			for(int i=0;i<size;i++){
				if(point_to_cluster[i]==UNCLASSIFIED){
					if(expand_cluster(i, clusterid, cloud, ind))
						clusterid++;
				}
			}
		}

		vector<std::pair<size_t,float> > radiusSearch(int index, float epsilon, PointCloud1<float>& cloud, my_kd_tree_t &ind) {
			const float search_radius = static_cast<float>(epsilon);
			std::vector<std::pair<size_t,float> >   ret_matches;

			nanoflann::SearchParams params;
			//params.sorted = false;

			const float query_pt[1] = {cloud.pts[index].x};
			const size_t nMatches = ind.radiusSearch(&query_pt[0], search_radius, ret_matches, params);

			// cout << "radiusSearch(): radius=" << search_radius << " -> " << nMatches << " matches\n";
			// for (size_t i = 0; i < nMatches; i++)
			// 	cout << "idx["<< i << "]=" << ret_matches[i].first << " dist["<< i << "]=" << ret_matches[i].second << endl;
			// cout << "\n";
			return ret_matches;
		}

		bool expand_cluster(int index, int clusterid, PointCloud1<float>& cloud, my_kd_tree_t& ind){
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
							if(val==NOISE) {
								noise.erase(pt.first);
							}
						}
					}
				}
				clusters.push_back(clust);
				return true;
			}
			return false;
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
	  		for (auto ele : noise) {
  				myfile << ele << "\n";
  			}
  			myfile << "\n";
			myfile.close();
	  	}

	};

    srand(static_cast<unsigned int>(time(nullptr)));
  	PointCloud1<float> cloud;
  	
	std::ifstream file(filename);
	
	vector<vector<coord>> points;
  	CSVRow row;
	while(file >> row) {
    	points.push_back(row.val());
	}
	addpoints1(cloud, points);

	my_kd_tree_t index(1 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(10 /* max leaf */) );	
	index.buildIndex();
	DBScan db(minpoints, epsilon, cloud, index);
	db.output();
	
	return 0;
}

int dim2(int minpoints, float epsilon, string filename) {
	typedef KDTreeSingleIndexAdaptor<
			L2_Simple_Adaptor<float, PointCloud2<float> > ,
			PointCloud2<float>, 2 /* dim */ > my_kd_tree_t;

	class DBScan{
	private:
		int minPts;
		double eps;
		vector<vector<int>> clusters;
		vector<int> point_to_cluster;
		set<int> noise;


	public:
		DBScan(int minPts, double eps, PointCloud2<float>& cloud, my_kd_tree_t& ind){
			this->minPts = minPts;
			this->eps = eps;
			int size = cloud.pts.size();
			point_to_cluster.resize(size, UNCLASSIFIED);
			int clusterid = 0;
			for(int i=0;i<size;i++){
				if(point_to_cluster[i]==UNCLASSIFIED){
					if(expand_cluster(i, clusterid, cloud, ind))
						clusterid++;
				}
			}
		}

		vector<std::pair<size_t,float> > radiusSearch(int index, float epsilon, PointCloud2<float>& cloud, my_kd_tree_t &ind) {
			const float search_radius = static_cast<float>(epsilon);
			std::vector<std::pair<size_t,float> >   ret_matches;

			nanoflann::SearchParams params;
			//params.sorted = false;

			const float query_pt[2] = {cloud.pts[index].x, cloud.pts[index].y};
			const size_t nMatches = ind.radiusSearch(&query_pt[0], search_radius, ret_matches, params);

			// cout << "radiusSearch(): radius=" << search_radius << " -> " << nMatches << " matches\n";
			// for (size_t i = 0; i < nMatches; i++)
			// 	cout << "idx["<< i << "]=" << ret_matches[i].first << " dist["<< i << "]=" << ret_matches[i].second << endl;
			// cout << "\n";
			return ret_matches;
		}

		bool expand_cluster(int index, int clusterid, PointCloud2<float>& cloud, my_kd_tree_t& ind){
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
							if(val==NOISE) {
								noise.erase(pt.first);
							}
						}
					}
				}
				clusters.push_back(clust);
				return true;
			}
			return false;
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
	  		for (auto ele : noise) {
  				myfile << ele << "\n";
  			}
  			myfile << "\n";
			myfile.close();
	  	}

	};

    srand(static_cast<unsigned int>(time(nullptr)));
  	PointCloud2<float> cloud;
  	
	std::ifstream file(filename);
	
	vector<vector<coord>> points;
  	CSVRow row;
	while(file >> row) {
    	points.push_back(row.val());
	}
	addpoints2(cloud, points);

	my_kd_tree_t index(2 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(10 /* max leaf */) );	
	index.buildIndex();
	DBScan db(minpoints, epsilon, cloud, index);
	db.output();
	
	return 0;
}

int dim3(int minpoints, float epsilon, string filename) {
	typedef KDTreeSingleIndexAdaptor<
			L2_Simple_Adaptor<float, PointCloud3<float> > ,
			PointCloud3<float>, 3 /* dim */ > my_kd_tree_t;

	class DBScan{
	private:
		int minPts;
		double eps;
		vector<vector<int>> clusters;
		vector<int> point_to_cluster;
		set<int> noise;


	public:
		DBScan(int minPts, double eps, PointCloud3<float>& cloud, my_kd_tree_t& ind){
			this->minPts = minPts;
			this->eps = eps;
			int size = cloud.pts.size();
			point_to_cluster.resize(size, UNCLASSIFIED);
			int clusterid = 0;
			for(int i=0;i<size;i++){
				if(point_to_cluster[i]==UNCLASSIFIED){
					if(expand_cluster(i, clusterid, cloud, ind))
						clusterid++;
				}
			}
		}

		vector<std::pair<size_t,float> > radiusSearch(int index, float epsilon, PointCloud3<float>& cloud, my_kd_tree_t &ind) {
			const float search_radius = static_cast<float>(epsilon);
			std::vector<std::pair<size_t,float> >   ret_matches;

			nanoflann::SearchParams params;
			//params.sorted = false;

			const float query_pt[3] = {cloud.pts[index].x, cloud.pts[index].y, cloud.pts[index].z};
			const size_t nMatches = ind.radiusSearch(&query_pt[0], search_radius, ret_matches, params);

			// cout << "radiusSearch(): radius=" << search_radius << " -> " << nMatches << " matches\n";
			// for (size_t i = 0; i < nMatches; i++)
			// 	cout << "idx["<< i << "]=" << ret_matches[i].first << " dist["<< i << "]=" << ret_matches[i].second << endl;
			// cout << "\n";
			return ret_matches;
		}

		bool expand_cluster(int index, int clusterid, PointCloud3<float>& cloud, my_kd_tree_t& ind){
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
							if(val==NOISE) {
								noise.erase(pt.first);
							}
						}
					}
				}
				clusters.push_back(clust);
				return true;
			}
			return false;
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
	  		myfile << "#" << clusters.size() << "\n";
  			for (auto ele : noise) {
  				myfile << ele << "\n";
  			}
  			myfile << "\n";
			myfile.close();
	  	}

	};

    srand(static_cast<unsigned int>(time(nullptr)));
  	PointCloud3<float> cloud;
  	
	std::ifstream file(filename);
	
	vector<vector<coord>> points;
  	CSVRow row;
	while(file >> row) {
    	points.push_back(row.val());
	}
	addpoints3(cloud, points);

	my_kd_tree_t index(3 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(10 /* max leaf */) );	
	index.buildIndex();
	DBScan db(minpoints, epsilon, cloud, index);
	db.output();
	
	return 0;
}

int dim4(int minpoints, float epsilon, string filename) {
	typedef KDTreeSingleIndexAdaptor<
			L2_Simple_Adaptor<float, PointCloud4<float> > ,
			PointCloud4<float>, 4 /* dim */ > my_kd_tree_t;

	class DBScan{
	private:
		int minPts;
		double eps;
		vector<vector<int>> clusters;
		vector<int> point_to_cluster;
		set<int> noise;


	public:
		DBScan(int minPts, double eps, PointCloud4<float>& cloud, my_kd_tree_t& ind){
			this->minPts = minPts;
			this->eps = eps;
			int size = cloud.pts.size();
			point_to_cluster.resize(size, UNCLASSIFIED);
			int clusterid = 0;
			for(int i=0;i<size;i++){
				if(point_to_cluster[i]==UNCLASSIFIED){
					if(expand_cluster(i, clusterid, cloud, ind))
						clusterid++;
				}
			}
		}

		vector<std::pair<size_t,float> > radiusSearch(int index, float epsilon, PointCloud4<float>& cloud, my_kd_tree_t &ind) {
			const float search_radius = static_cast<float>(epsilon);
			std::vector<std::pair<size_t,float> >   ret_matches;

			nanoflann::SearchParams params;
			//params.sorted = false;

			const float query_pt[4] = {cloud.pts[index].w, cloud.pts[index].x, cloud.pts[index].y, cloud.pts[index].z};
			const size_t nMatches = ind.radiusSearch(&query_pt[0], search_radius, ret_matches, params);

			// cout << "radiusSearch(): radius=" << search_radius << " -> " << nMatches << " matches\n";
			// for (size_t i = 0; i < nMatches; i++)
			// 	cout << "idx["<< i << "]=" << ret_matches[i].first << " dist["<< i << "]=" << ret_matches[i].second << endl;
			// cout << "\n";
			return ret_matches;
		}

		bool expand_cluster(int index, int clusterid, PointCloud4<float>& cloud, my_kd_tree_t& ind){
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
							if(val==NOISE) {
								noise.erase(pt.first);
							}
						}
					}
				}
				clusters.push_back(clust);
				return true;
			}
			return false;
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
	  		for (auto ele : noise) {
  				myfile << ele << "\n";
  			}
  			myfile << "\n";
			myfile.close();
	  	}

	};

    srand(static_cast<unsigned int>(time(nullptr)));
  	PointCloud4<float> cloud;
  	
	std::ifstream file(filename);
	
	vector<vector<coord>> points;
  	CSVRow row;
	while(file >> row) {
    	points.push_back(row.val());
	}
	addpoints4(cloud, points);

	my_kd_tree_t index(4 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(10 /* max leaf */) );	
	index.buildIndex();
	DBScan db(minpoints, epsilon, cloud, index);
	db.output();
	
	return 0;
}

int dim5(int minpoints, float epsilon, string filename) {
	typedef KDTreeSingleIndexAdaptor<
			L2_Simple_Adaptor<float, PointCloud5<float> > ,
			PointCloud5<float>, 5 /* dim */ > my_kd_tree_t;

	class DBScan{
	private:
		int minPts;
		double eps;
		vector<vector<int>> clusters;
		vector<int> point_to_cluster;
		set<int> noise;


	public:
		DBScan(int minPts, double eps, PointCloud5<float>& cloud, my_kd_tree_t& ind){
			this->minPts = minPts;
			this->eps = eps;
			int size = cloud.pts.size();
			point_to_cluster.resize(size, UNCLASSIFIED);
			int clusterid = 0;
			for(int i=0;i<size;i++){
				if(point_to_cluster[i]==UNCLASSIFIED){
					if(expand_cluster(i, clusterid, cloud, ind))
						clusterid++;
				}
			}
		}

		vector<std::pair<size_t,float> > radiusSearch(int index, float epsilon, PointCloud5<float>& cloud, my_kd_tree_t &ind) {
			const float search_radius = static_cast<float>(epsilon);
			std::vector<std::pair<size_t,float> >   ret_matches;

			nanoflann::SearchParams params;
			//params.sorted = false;

			const float query_pt[5] = {cloud.pts[index].v, cloud.pts[index].w, cloud.pts[index].x, cloud.pts[index].y, cloud.pts[index].z};
			const size_t nMatches = ind.radiusSearch(&query_pt[0], search_radius, ret_matches, params);

			// cout << "radiusSearch(): radius=" << search_radius << " -> " << nMatches << " matches\n";
			// for (size_t i = 0; i < nMatches; i++)
			// 	cout << "idx["<< i << "]=" << ret_matches[i].first << " dist["<< i << "]=" << ret_matches[i].second << endl;
			// cout << "\n";
			return ret_matches;
		}

		bool expand_cluster(int index, int clusterid, PointCloud5<float>& cloud, my_kd_tree_t& ind){
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
							if(val==NOISE) {
								noise.erase(pt.first);
							}
						}
					}
				}
				clusters.push_back(clust);
				return true;
			}
			return false;
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
	  		for (auto ele : noise) {
  				myfile << ele << "\n";
  			}
  			myfile << "\n";
			myfile.close();
	  	}

	};

    srand(static_cast<unsigned int>(time(nullptr)));
  	PointCloud5<float> cloud;
  	
	std::ifstream file(filename);
	
	vector<vector<coord>> points;
  	CSVRow row;
	while(file >> row) {
    	points.push_back(row.val());
	}
	addpoints5(cloud, points);

	my_kd_tree_t index(5 /*dim*/, cloud, KDTreeSingleIndexAdaptorParams(10 /* max leaf */) );	
	index.buildIndex();
	DBScan db(minpoints, epsilon, cloud, index);
	db.output();
	
	return 0;
}

int main(int argc, char **argv) {
	cout << "You have entered " << argc 
         << " arguments:" << "\n"; 
  
    for (int i = 0; i < argc; ++i) 
        cout << argv[i] << "\n"; 

	int minpoints = stoi(argv[1]);
	float epsilon = stof(argv[2]);
	epsilon = epsilon*epsilon;
	string filename = argv[3];

	std::ifstream file(filename);
  	CSVRow row;
  	file >> row;
  	int length = row.val().size();

	if(length == 1) {
		cout << "Length 1" << endl;
		dim1(minpoints, epsilon, filename);
	}
	else if(length == 2) {
		cout << "Length 2" << endl;
		dim2(minpoints, epsilon, filename);
	}
	else if(length == 3) {
		cout << "Length 3" << endl;
		dim3(minpoints, epsilon, filename);
	}
	else if(length == 4) {
		cout << "Length 4" << endl;
		dim4(minpoints, epsilon, filename);
	}
	else if(length >= 5) {
		cout << "Length 5" << endl;
		dim5(minpoints, epsilon, filename);
	}
	return 0;
}