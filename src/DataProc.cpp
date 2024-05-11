#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "DataProc.h"

using std::string;
using std::vector;

bool DataProc::readCSV(std::vector<Point>& data, const std::string& path, const int& col_cnt)
{
	std::ifstream in_file(path);

	std::string line;
	while (getline(in_file, line)) {
		string num;
		std::istringstream readstr(line);
		vector<int> temp_coordinate;
		for (int i = 0; i < col_cnt; i++) {
			getline(readstr, num, ',');
			temp_coordinate.push_back(atoi(num.c_str()));
		}
		Point p;
		p.x = temp_coordinate[0];
		p.y = temp_coordinate[col_cnt-1];
		data.push_back(p);
	}

	in_file.close();

	return true;
}

bool DataProc::saveCSV(std::vector<Point*>& data, const std::string& path)
{
	std::ofstream out_file;

	out_file.open(path);

	for (int i = 0; i < data.size(); i++) {
		out_file << data[i]->x << ',' << data[i]->y << std::endl;
	}

	out_file.close();
	return true;
}