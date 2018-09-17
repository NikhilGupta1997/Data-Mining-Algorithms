#include <iostream>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <time.h>

using namespace std;

typedef float coord;

class CSVRow {
  private:
    vector<coord> data;

  public:
    coord const& operator[](size_t index) const {
        return data[index];
    }

    size_t size() const {
        return data.size();
    }

    vector<coord> val() const {
    	return data;
    }

    void readNextRow(istream& str) {
        string line;
        getline(str, line);
        stringstream lineStream(line);
        string cell;

        data.clear();
        while(std::getline(lineStream, cell, ' ')) {
            data.push_back(stof(cell));
        }
    }
};

istream& operator>>(istream& str, CSVRow& data) {
    data.readNextRow(str);
    return str;
} 

class PointData {
  private:
  	vector< vector<coord> > data;

  public:
  	PointData(string filename) {
  		parseData(filename);
  	}

  	vector< vector<coord> > val() const {
    	return data;
    }

  	void parseData(string filename) {
  		std::ifstream file(filename);

	    CSVRow row;
	    while(file >> row) {
	        data.push_back(row.val());
	    }
  	}
};

int main() {
    PointData cluster("input.txt");
    for(auto val : cluster.val()) {
    	for (auto it : val) {
    		cout << it << " ";
    	}
    	cout << endl;
    }
}