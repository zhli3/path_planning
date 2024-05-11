#include <algorithm>   
#include "Bezier.h"
#include "Graph.h"

const std::vector<Point*> Bezier::formulaBezier()
{
	smooth_path.clear();

	//last several points may not be sampled
	int order = unsmooth_path.size()-1;
	if (order > 30) order = 30;
	std::vector<Point> sample_unsmooth_path = \
		getSamplePath(order/2);
	order = sample_unsmooth_path.size() - 1;
	long double n_factorial = static_cast<double>(factorial(order));

	double t(1.0);
	double w_n_i(0.0);
	while (t > 0) {
		double tmp_x = 0.0;
		double tmp_y = 0.0;
		Point* p = new Point();
		for (int i = 0; i <= order; i++) {
			w_n_i = myPow(1 - t, order - i) * myPow(t, i) * \
				 (n_factorial / static_cast<double>(factorial(i) * factorial(order -i)));

			tmp_x += w_n_i * sample_unsmooth_path[i].x;
			tmp_y += w_n_i * sample_unsmooth_path[i].y;
		}

		p->x = tmp_x;
		p->y = tmp_y;

		smooth_path.push_back(p);
		t -= 0.01;
	}

	//setfillcolor(RED);
	//solidcircle(275, 275, 70);
	//setlinecolor(RED);
	//setlinestyle(PS_SOLID, 5);
	//line(275, 275, 410, 275);
	//solidcircle(410, 275, 45);
	//solidcircle(550, 275, 60);
	for (int i = 0; i < smooth_path.size(); i++) {
		setfillcolor(BLUE);
		solidcircle(smooth_path[i]->x, smooth_path[i]->y, 3);
	}

	return smooth_path;
}

const double Bezier::myPow(const double& x, const int& n)
{
	if (n < 0) {
		return myPow(1 / x, -n);
	}
	else if (n == 0 || x == 1) {
		return 1;
	}
	else if (n == 2) {
		return x * x;
	}
	else if (n % 2 == 0) {
		return myPow(myPow(x, n / 2), 2);
	}
	else if (n % 2 != 0) {
		return x * myPow(myPow(x, n / 2), 2);
	}
}

const long long int Bezier::factorial(const int& n)
{
	long long int m;

	if (n <= 1){
		m = 1;
	}
	else{
		m = n * factorial(n - 1); 
	}

	return m;
}

bool Bezier::getUnsmoothPath(const std::vector<Point*>& p_set)
{
	for (int i = 0; i < p_set.size(); i++) {
		unsmooth_path.push_back(p_set[i]);
	}
	return true;
}

bool Bezier::getUnsmoothPath(Point*& p_tail)
{
	while (!unsmooth_path.empty())
	{
		delete unsmooth_path[0];
		unsmooth_path.erase(unsmooth_path.begin());
	}

	Point* p = nullptr;
	if (p_tail != nullptr) {
		p = p_tail;
		while (p != nullptr) {
			unsmooth_path.push_back(p);
			p = p->pre_point;
		}
	}
	else {
		return false;
	}

	return true;
}

const std::vector<Point> Bezier::getSamplePath(const int& m_order)
{
	int len_unsmooth = unsmooth_path.size();
	int sample_point = len_unsmooth / m_order;
	int redundant_point = len_unsmooth % m_order;

	std::vector<Point> sample_unsmooth;

	if (m_order + 1 > unsmooth_path.size()) {//unsmooth path points are not enough
		std::vector<Point> fail;

		for (int i = 0; i < unsmooth_path.size(); i++) {
			fail.push_back(*unsmooth_path[i]);
		}

		return fail;
	}

	if (m_order > 1) {
		if (sample_point > 1) {
			int fill_idx = 0;
			sample_unsmooth.push_back(*unsmooth_path[fill_idx]);
			fill_idx = sample_point - 1;
			sample_unsmooth.push_back(*unsmooth_path[fill_idx]);
			for (int i = 0; i < m_order - 1; i++) {
				fill_idx += sample_point;
				if (redundant_point != 0) {
					fill_idx += 1;
					redundant_point -= 1;
				}
				sample_unsmooth.push_back(*unsmooth_path[fill_idx]);
			}
		}
		else {
			for (int i = 0; i < m_order + 1; i++) {
				sample_unsmooth.push_back(*unsmooth_path[i]);
			}
		}
	}
	else {
		sample_unsmooth.push_back(*unsmooth_path[0]);
		sample_unsmooth.push_back(*unsmooth_path[unsmooth_path.size() - 1]);
	}

	return sample_unsmooth;
}

const std::vector<Point*> Bezier::iterateBezier(const int& m_order)
{
	//average sample
	std::vector<Point> sample_unsmooth = getSamplePath(m_order);

	double t(1.0);
	while (t >= 0){
		smooth_path.push_back(getIteratePoint(sample_unsmooth, t));
		t -= 0.005;
	}

	return smooth_path;

}

Point* Bezier::getIteratePoint( \
	const std::vector<Point> control_points, const double& t\
)
{
	//it may have accumulative loss
	//because if i return coordinate, it's x and y must be int, not exact one
	//but formula can do all the calculation in double 
	const int len_control = control_points.size();

	if (len_control > 1) {
		
		std::vector<Point> next_ctrl_points;
		Point p;
		for (int i = 0; i < len_control-1; i++) {

			double tmp_x = (1 - t) * static_cast<double>(control_points[i].x) \
				+ static_cast<double>(control_points[i + 1].x) * t;

			double tmp_y = (1 - t) * static_cast<double>(control_points[i].y) \
				+ static_cast<double>(control_points[i + 1].y) * t;

			Point p(static_cast<int>(tmp_x), static_cast<int>(tmp_y));
			
			next_ctrl_points.push_back(p);

		}
		return getIteratePoint(next_ctrl_points, t);

	}

	Point* final_p = new Point(control_points[0].x, control_points[0].y);
	
	return final_p;

}

Bezier::~Bezier()
{
	while (!smooth_path.empty())
	{
		delete smooth_path[0];
		smooth_path.erase(smooth_path.begin());
	}
	while (!unsmooth_path.empty())
	{
		delete unsmooth_path[0];
		unsmooth_path.erase(unsmooth_path.begin());
	}
}