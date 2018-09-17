#include <iostream>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <time.h>
#include "helper.h"

using namespace std;

void CSVRow::readNextRow(istream& str) {
    string line;
    getline(str, line);
    stringstream lineStream(line);
    string cell;

    data.clear();
    while(std::getline(lineStream, cell, ' ')) {
        data.push_back(stof(cell));
    }
}

void PointData::parseData(string filename) {
	std::ifstream file(filename);

	CSVRow row;
	while(file >> row) {
	    data.push_back(row.val());
	}
}

void PointData::printData() {
	for(auto ele : data) {
    	for (auto it : ele) {
    		cout << it << " ";
    	}
    	cout << endl;
    }
}