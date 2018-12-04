#ifndef HELPER_H
#define HELPER_H

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

    void readNextRow(istream& str);
};

istream& operator>>(istream& str, CSVRow& data) {
    data.readNextRow(str);
    return str;
} 

class PointData {
  private:
  	vector< vector<coord> > data;

  public:
  	vector<coord> const& operator[](size_t index) const {
        return data[index];
    }

  	PointData(string filename) {
  		parseData(filename);
  	}

  	vector< vector<coord> > val() const {
    	return data;
    }

    size_t size() const {
        return data.size();
    }

  	void parseData(string filename);

  	void printData();
};

#endif