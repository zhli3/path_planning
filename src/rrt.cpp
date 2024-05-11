#include "rrt.h"

RRT::~RRT()
{
	delete start;
	delete end;
}

void RRT::rrt_algorithm()
{
	setfillcolor(RED);
	solidcircle(start->x, start->y, rrt_user_para.block / 2);
	setfillcolor(GREEN);
	solidcircle(end->x, end->y, rrt_user_para.block / 2);

	srand((unsigned)time(NULL));
	setlinecolor(GREEN);
	setlinestyle(PS_SOLID, rrt_user_para.rrt_line_width);

	int min_dist_random_idx = 0;
	Point* search_p = new Point(start->x,start->y);
	Point* search_next_p = nullptr;
	rrt_path.push_back(search_p);

	//testAtan2Radian();
	
	while (1) {
		//showParaBelow(end);
		delayMs(1);
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
		else{
			delete search_next_p;
		}

		search_p = findMinDistPoint(end);
		if (calcuDist(search_p, end) < rrt_user_para.rrt_edge_len) { break; }
	}

	getPath(search_p);

}

void RRT::delayMs(const int& cnt)
{
	clock_t start = clock(); while ((clock() - start)<cnt);
}

Point* RRT::getRandomRRTNode(int &idx)
{

	Point* p_new = new Point();
	//if error happens, code may not get picture
	Point p(RANDOM_RANGE(0,rrt_user_para.rrt_graph_width-1),
		RANDOM_RANGE(0,rrt_user_para.rrt_graph_height-1));

	int temp_dist = 0;
	int min_dist = INT_MAX;
	for (int i = 0; i < rrt_path.size(); i++) {
		temp_dist = calcuDist(rrt_path[i], &p);
		if (temp_dist < min_dist) {
			min_dist = temp_dist;
			idx = i;
		}
	}

	Point* p_node = rrt_path[idx];

	if (p.x == p_node->x) {
		p_new->x = p.x;
		int symbol = (p.y - p_node->y) > 0 ? 1 : -1;
		int p_y = p_node->y;
		p_new->y = p_y + symbol * rrt_user_para.rrt_edge_len;
	}
	else {
		float theta = atan2((p.x - p_node->x), (p.y - p_node->y));
		getAtan2Node(p_new, p_node, theta);
	}
	
	return p_new;
}

bool RRT::isLegalPoint(const Point* p)
{
	if (p->x >= 0 && p->x < rrt_user_para.rrt_graph_width && 
		p->y >= 0 && p->y < rrt_user_para.rrt_graph_height) {
		return true;
	}
	else {
		return false;
	}

	return false;
}

float RRT::getRandomFloat(const float& x, const float& y)
{
	return ((x) + (1.0 * rand()) / RAND_MAX * ((y)-(x)));//[x, y]
}

void RRT::getAtan2Node(Point* p, const Point* node, float& theta)
{
	changeAngle(theta);
	p->x = (cos(theta) * rrt_user_para.rrt_edge_len) + node->x;
	p->y = (sin(theta) * rrt_user_para.rrt_edge_len) + node->y;
}

Point* RRT::findMinDistPoint(const Point* end)
{
	int min_dist = INT_MAX;
	int temp_dist = 0;
	int min_dist_idx = 0;

	for (int i = 0; i < rrt_path.size(); i++) {
		temp_dist = calcuDist(rrt_path[i], end);
		if (temp_dist < min_dist) {
			min_dist = temp_dist;
			min_dist_idx = i;
		}
	}

	return rrt_path[min_dist_idx];
}

bool RRT::isObstacle(const Point* p)
{
	for (int i = -2; i < rrt_user_para.avoid_obstacles_cnt; i++) {
		for (int j = -2; j < rrt_user_para.avoid_obstacles_cnt; j++) {
			Point tmp_p(p->x + i, p->y + j);
			if (isLegalPoint(&tmp_p)) {
				if (getpixel(tmp_p.x, tmp_p.y) == BLACK) {
					return true;
				}
			}
		}
	}
	return false;
}

Point* RRT::getRRTNode(const Point* node)
{
	Point* p = new Point();

	if (node->x != end->x) {
		float theta = atan2((end->x - node->x), (end->y - node->y));
		getAtan2Node(p, node, theta);
	}
	else {
		p->x = node->x;
		p->y = node->y + rrt_user_para.rrt_edge_len;
	}

	return p;
}

void RRT::changeAngle(float& angle)
{
	if (angle < -PI / 2 && angle > -PI) { angle = -3 * PI / 2 - angle; }
	else { angle = PI / 2 - angle; }
}

int RRT::calcuDist(const Point* p1, const Point* p2)
{
	int dx = abs(p1->x - p2->x);
	int dy = abs(p1->y - p2->y);
	return int(sqrt(dx * dx + dy * dy));
}

void RRT::getPath(const Point* p)
{
	setlinecolor(BROWN);
	setlinestyle(PS_SOLID, rrt_user_para.rrt_line_width*2);

	if (p != nullptr) {
		while (p->pre_point != nullptr) {
			line(p->x, p->y, p->pre_point->x, p->pre_point->y);
			p = p->pre_point;
		}
	}
}

void RRT::testAtan2Radian()
{
	ExMessage mse;

	while (1) {
		mse = getmessage(EM_MOUSE);
		if (mse.message == WM_LBUTTONDOWN) {
			line(480, 240, mse.x, mse.y);
			float theta = atan2((mse.x - 480), (mse.y - 240));
			changeAngle(theta);
			TCHAR s1[5];
			_stprintf_s(s1, sizeof(s1), _T("%.4f"), theta);
			outtextxy(mse.x, mse.y, s1);
		}
	}
}

void RRT::showParaBelow(const Point* search_end)
{
	ExMessage mse;

	while (1) {
		mse = getmessage(EM_MOUSE);

		wchar_t s1[7];
		_stprintf_s(s1, sizeof(s1), _T("(%d,"), mse.x);
		outtextxy(100, 500, s1);
		wchar_t s2[7];
		_stprintf_s(s2, sizeof(s2), _T("%d)"), mse.y);
		outtextxy(150, 500, s2);

		Point p(mse.x, mse.y);
		search_end = getRRTNode(&p);

		_stprintf_s(s1, sizeof(s1), _T("(%d,"), search_end->x);
		outtextxy(300, 500, s1);
		_stprintf_s(s2, sizeof(s2), _T("%d)"), search_end->y);
		outtextxy(350, 500, s2);

		if (mse.message == WM_LBUTTONDOWN)
			line(p.x, p.y, search_end->x, search_end->y);
	}
}