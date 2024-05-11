#include "RRTStar.h"

RRTStar::~RRTStar()
{
	while (!rrt_path.empty()) {
		delete rrt_path[0];
		rrt_path.erase(rrt_path.begin());
	}
}

Point* RRTStar::rrt_algorithm(const Point* start, const Point* end)
{
	setfillcolor(RED);
	solidcircle(start->x, start->y, rrt_user_para.block / 2);
	setfillcolor(GREEN);
	solidcircle(end->x, end->y, rrt_user_para.block / 2);

	srand((unsigned)time(NULL));
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, rrt_user_para.rrt_line_width);

	int min_dist_random_idx = 0;
	Point* search_p = new Point(start->x, start->y);
	Point* search_next_p = nullptr;
	rrt_path.push_back(search_p);

	//testAtan2Radian();

	while (1) {
		//showParaBelow(end);
		//delayMs(1);
		if (getRandomFloat(0, 1) > rrt_user_para.beta) {
			search_p = findMinDistPoint(end);
			search_next_p = getRRTNode(search_p);
		}
		else {
			search_next_p = getRandomRRTNode(min_dist_random_idx);
			search_p = rrt_path[min_dist_random_idx];
		}


		if (!isObstacle(search_next_p)) {
			search_next_p->pre_point = search_p;

			rrt_path.push_back(search_next_p);

			line(search_p->x, search_p->y, search_next_p->x, search_next_p->y);
			solidcircle(search_next_p->x, search_next_p->y, rrt_user_para.rrt_line_width);

		}
		else {
			delete search_next_p;
		}

		search_p = findMinDistPoint(end);
		if (calcuDist(search_p, end) < rrt_user_para.rrt_edge_len) { break; }
	}

	getPath(search_p);
	return search_p;

}

const std::vector<Point*> RRTStar::getRRTstarPath(Point* p_tail)
{
	std::vector<Point*> p;
	p.push_back(p_tail);
	while (p_tail != nullptr) {
		p.push_back(p_tail);
		p_tail = p_tail->pre_point;
	}
	return p;
}

Point* RRTStar::rrtstar_algorithm(const Point* start, const Point* end)
{
	setfillcolor(RED);
	solidcircle(start->x, start->y, rrt_user_para.block / 2);
	setfillcolor(BLUE);
	solidcircle(end->x, end->y, rrt_user_para.block / 2);

	setfillcolor(GREEN);
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, rrt_user_para.rrt_line_width);

	std::cin.get();

	int min_dist_random_idx = 0;
	int iterator_cnt = 0;
	Point* search_p = new Point(start->x, start->y);
	search_p->Fn = 0;
	Point* search_next_p = nullptr;
	rrt_path.push_back(search_p);

	while (iterator_cnt < user_rrtstar_para.iterator) {
		
		if (getRandomFloat(0, 1) > rrt_user_para.beta) {
			search_p = findMinDistPoint(end);
			search_next_p = getRRTNode(search_p);
		}
		else {
			search_next_p = getRandomRRTNode(min_dist_random_idx);
			search_p = rrt_path[min_dist_random_idx];
		}

		//search_p is only used to find temp search_next_p;
		//once search_next_p is found, search_p is userless;
		//because search_p usually need to be update because of the cost
		if (findOptimizationPoint(search_p, search_next_p)) {
			iterator_cnt++;
			rrt_path.push_back(search_next_p);
			setlinecolor(GREEN);
			line(search_p->x, search_p->y, search_next_p->x, search_next_p->y);
			solidcircle(search_next_p->x, search_next_p->y, rrt_user_para.rrt_line_width);
		}

	}

	Point* p_tail = getOptimalPath(end);
	getPath(p_tail);
	std::cin.get();
	return p_tail;
}

void RRTStar::rrtstar_algorithm()
{
	
	setfillcolor(RED);
	solidcircle(start->x, start->y, rrt_user_para.block/2);
	setfillcolor(GREEN);
	solidcircle(end->x, end->y, rrt_user_para.block / 2);

	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, rrt_user_para.rrt_line_width);

	int min_dist_random_idx = 0;
	int iterator_cnt = 0;
	Point* search_p = new Point(start->x, start->y);
	search_p->Fn = 0;
	Point* search_next_p = nullptr;
	rrt_path.push_back(search_p);

	//Point p1(100, 100);
	//Point p2(100, 400);

	//if (isBlockedByObs(&p1, &p2)) {
	//	int i = 0;
	//	i++;
	//}

	while (iterator_cnt++ < user_rrtstar_para.iterator) {

		search_next_p = getRandomRRTNode(min_dist_random_idx);
		search_p = rrt_path[min_dist_random_idx];

		//search_p is only used to find temp search_next_p;
		//once search_next_p is found, search_p is userless;
		//because search_p usually need to be update because of the cost
		if (findOptimizationPoint(search_p, search_next_p)) {
			rrt_path.push_back(search_next_p);
			setlinecolor(GREEN);
			line(search_p->x, search_p->y, search_next_p->x, search_next_p->y);
			solidcircle(search_next_p->x, search_next_p->y, rrt_user_para.rrt_line_width);
		}


		//showParameters();

	}

	getPath(getOptimalPath(end));

}

bool RRTStar::findOptimizationPoint(Point* now_point, Point*  new_point)
{
	std::vector<Point*> near_points;

	if (findNearPoints(near_points, now_point, new_point) ) {
		changePrePoint(near_points, new_point);
	}
	else {
		return false;
	}

}

bool RRTStar::findNearPoints(std::vector<Point*>& near_vector, Point*& now_point, Point*& new_point)
{
	int min_dist = INT_MAX;
	int dist = 0;

	for (int i = 0; i < rrt_path.size(); i++) {
		dist = calcuDist(rrt_path[i], new_point);
		if (calcuDist(rrt_path[i], new_point) < user_rrtstar_para.near_circle) {
			//in the near circle then judge is blocked by obs
			if (!isBlockedByObs(rrt_path[i], new_point)) {
				//this can ensure near_vector don't exist obs between new_point and near_vector
				near_vector.push_back(rrt_path[i]);
				rrt_path[i]->Gn = rrt_path[i]->Fn + dist;
				if (rrt_path[i]->Gn < min_dist) {
					now_point = rrt_path[i];//now_point is pre point
					min_dist = rrt_path[i]->Gn;
				}
				rrt_path[i]->Gn = 0;
			}
		}

	}

	if (near_vector.size() != 0 && isLegalPoint(new_point) && !isObstacle(new_point)) {
		// two conditions, new_point can connect other point in this circle
		//and new_point must in the graph set 
		new_point->pre_point = now_point;
		new_point->Fn = calcuDist(now_point, new_point) + now_point->Fn;
	}
	else {
		delete new_point;
		new_point = nullptr;
		return false;
	}

	return true;

}

Point* RRTStar::getSearchNode(Point*& p_origin, Point*& p_target)
{
	Point* p_new_node = new Point();

	if (p_origin->x != p_target->x) {
		float theta = atan2((p_target->x - p_origin->x), (p_target->y - p_origin->y));
		getAtan2Node(p_new_node, p_origin, theta);
	}
	else {
		p_new_node->x = p_origin->x;
		p_new_node->y = p_origin->y + rrt_user_para.rrt_edge_len;
	}

	return p_new_node;
}

void RRTStar::changePrePoint(std::vector<Point*>& near_vector, Point*& new_point)
{
	int dist = 0;
	for (int i = 0; i < near_vector.size(); i++) {
		dist = new_point->Fn + calcuDist(new_point, near_vector[i]);
		if (dist < near_vector[i]->Fn) {//reconnect the line
			setlinecolor(LIGHTGRAY);
			line(near_vector[i]->pre_point->x, near_vector[i]->pre_point->y, \
				near_vector[i]->x, near_vector[i]->y);
			solidcircle(near_vector[i]->x, near_vector[i]->y, rrt_user_para.rrt_line_width);

			near_vector[i]->pre_point = new_point;
			near_vector[i]->Fn = dist;
		}
	}
}

bool RRTStar::isBlockedByObs(const Point* p1, const Point* p2)
{
	if (getpixel(p1->x, p1->y) == BLACK || getpixel(p2->x, p2->y) == BLACK \
		|| !isLegalPoint(p1) || !isLegalPoint(p2)) {
		return true;
	}

	int x1 = p1->x;
	int x2 = p2->x;
	int y1 = p1->y;
	int y2 = p2->y;
	int incremental_step = 2;
	int slope_len = 0;
	int convert = 0;

	Point p;

	if (x1 != x2) {
		//incremental steps can increase to 2 or 3 ...
		//then it can decrease computation time dramatically
		//but if incremental steps is too large,path can not avoid obstacles efficiently
		double k = double(y2 - y1) / (x2 - x1);//use y = kx + b model
		double b = double(x1 * y2 - x2 * y1) / (x1 - x2);
		convert = x1 < x2 ? incremental_step : -1 * incremental_step;
		slope_len = abs(x1 - x2) / incremental_step;

		for (int i = 1; i <= slope_len ; i++) {//judge without the start
			p.x = x1 + i * convert;
			p.y = k * p.x + b;
			
			if (getpixel(p.x, p.y) == BLACK || !isLegalPoint(&p)) {
				return true;
			}

		}
	}
	else {
		convert = y1 < y2 ? incremental_step : -1 * incremental_step;
		p.x = x1;
		slope_len = abs(y1 - y2) / incremental_step;

		for (int i = 1; i <= slope_len ; i++) {//judge without the start
			p.y = y1 + i * convert;

			if (getpixel(p.x, p.y) == BLACK || !isLegalPoint(&p)) {
				return true;
			}
		}
	}

	return false;
}

Point* RRTStar::getOptimalPath(const Point* end)
{
	int min_cost = INT_MAX;
	Point* p = nullptr;

	for (int i = 0; i < rrt_path.size(); i++) {
		if (calcuDist(rrt_path[i], end) < user_rrtstar_para.end2node) {
			if (rrt_path[i]->Fn < min_cost) {
				min_cost = rrt_path[i]->Fn;
				p = rrt_path[i];
			}
		}
	}

	return p;
}

bool RRTStar::isLegalPoint(const Point* p)
{
	if (p->x >= lx && p->x < ux &&
		p->y >= ly && p->y < uy) {
		return true;
	}
	else {
		return false;
	}

	return false;
}
