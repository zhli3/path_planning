#pragma once
#include <vector>
#include "Point.h"

class Bezier
{
public:
	Bezier(){}
	Bezier(const int& order) :m_order(order){}
	Bezier(const Bezier&){}
	virtual ~Bezier();
	//how to const a reference?must have exactly body, temp value is not supported
	virtual bool getUnsmoothPath(Point*&);
	virtual bool getUnsmoothPath(const std::vector<Point*>&);
	virtual Point* getIteratePoint(const std::vector<Point>, const double&);
	virtual const std::vector<Point*> iterateBezier(const int&);
	virtual const std::vector<Point*> formulaBezier();
	virtual const std::vector<Point> getSamplePath(const int&);
	const long long int factorial(const int&);
	const double myPow(const double&, const int&);
	

protected:

private:
	int m_order;
	std::vector<Point*> smooth_path;
	std::vector<Point*> unsmooth_path;
};

