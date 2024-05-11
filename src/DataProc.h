#pragma once
#include <vector>
#include <string>
#include "Point.h"

class DataProc
{
public:
	DataProc(){}
	DataProc(const DataProc&){}
	~DataProc(){}
	//input must be a vector of body, can't be vector of point
	//because i can't find a appropriate place to delete them
	virtual bool saveCSV(std::vector<Point*>&, const std::string&);
	virtual bool readCSV(std::vector<Point>&, const std::string&, const int&);

private:

protected:

};

