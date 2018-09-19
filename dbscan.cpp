#include<iostream>

using namespace std;

const int NOISE = -1;
const int UNCLASSIFIED = -2;

class DBScan{
private:
	int minPts;
	double eps;
	vector<vector<coord>> points;
	vector<vector<int>> clusters;
	vector<int> point_to_cluster;
	set<int> noise;

public:
	DBSCan(int minPts, double eps, string filename){
		this->minPts = minPts;
		this->eps = eps;
		std::ifstream file(filename);

		CSVRow row;
		while(file >> row) {
	    	this->points.push_back(row.val());
		}
		int size = points.size();
		point_to_cluster.resize(size, UNCLASSIFIED);
		int clusterid = 0;
		// clusters.resize(1);
		for(int i=0;i<size;i++){
			if(point_to_cluster[i]==UNCLASSIFIED){
				if(expand_cluster(i, clusterid))
					clusterid++;
			}
		}

	}

	bool expand_cluster(int index, int clusterid){
		vector<int> seeds = regionQuery(index, eps, points);
		if(seeds.size()<minPts){
			point_to_cluster[index] = NOISE;
			noise.insert(index);
		}
		else{
			int si = seeds.size();
			vector<int> clust;
			queue<int> values;
			for(int i=0;i<si;i++){
				int ind = seeds[i];
				point_to_cluster[ind] = clusterid;
				clust.push_back(ind);
				if(ind!=index)
					values.push(ind);
			}
			while(!values.empty()){
				int ind = values.top();
				values.pop();
				vector<int> result = regionQuery(ind, eps, points);
				for(auto pt:result){
					int val = point_to_cluster[pt];
					if(val==UNCLASSIFIED || val==NOISE){
						if(val==UNCLASSIFIED && pt!=ind)
							values.push(pt);
						point_to_cluster[pt] = clusterid;
						clust.push_back(pt);
					}
				}
			}
			clusters.push_back(clust);
		}
	}



};

int main(int argc, char **argv){
	cout << "You have entered " << argc 
         << " arguments:" << "\n"; 
  
    for (int i = 0; i < argc; ++i) 
        cout << argv[i] << "\n"; 
	return 0;
}