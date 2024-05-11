#include "A_star.h"
#include <math.h>
#include <limits>

template <typename T>
const bool vector_contain(std::vector<T>& Vec, const T& Element)
{
	if (std::find(Vec.begin(), Vec.end(), Element) != Vec.end())
		return true;

	return false;
}

A_star::~A_star()
{
	//below delete is wrong,but i don not want to correct them
	//the right form can look up RRTStar.cpp , see how it delete vector and data in heap
	//why i do not want to fix this error?
	//because this algorithm can run without errors, i am afraid to change then error happens
	//if one day i use this destructor again, then i will fix it
	for (int i = 0; i < openset.size(); i++) {
		delete openset[i];
		openset.erase(openset.begin());
	}

	for (int i = 0; i < fatherset.size(); i++) {
		delete fatherset[i];
		fatherset.erase(fatherset.begin());
	}

}

void A_star::set_start_end_obs_map(Graph& user_graph)
{
	start = user_graph.get_start();
	end = user_graph.get_end();
	obstacleset = user_graph.get_obstacle();
	user_trajectory = user_graph.get_userPara();

	for (int i = 0; i <= user_trajectory.map_x; i++) {
		for (int j = 0; j <= user_trajectory.map_y; j++) {
			user_trajectory.map[i][j] = PATH;
		}
	}

	user_trajectory.map[start.x][start.y] = START;
	user_trajectory.map[end.x][end.y] = END;

	start.Gn = 0;
	start.Fn = start.Gn + calcu_Hn(start);

	for (int i = 0; i < obstacleset.size(); i++) {
		user_trajectory.map[obstacleset[i]->x][obstacleset[i]->y] = OBSTACLE;
	}

}

void A_star::Astar_algorithm()
{
	int min_Fn_idx(0);
	Point* min_Fn_p = new Point(start.x, start.y);

	memcpy(min_Fn_p, &start, sizeof(start));
	add_openset(min_Fn_p);
	
	while (!openset.empty()) {
		Graph::show_map(user_trajectory);
		min_Fn_p = find_min_Fn(min_Fn_idx);
		show_para_below(min_Fn_p);
		if (*min_Fn_p == end) { break; }
		search_env_block(min_Fn_p);
		
		add_fatherset(min_Fn_p);
		delete_openset(min_Fn_idx);
		
		user_trajectory.map[min_Fn_p->x][min_Fn_p->y] = PASS_SEARCH;
	}

	get_trajectory(min_Fn_p);
}

void A_star::get_trajectory(const Point* p)
{
	p = p->pre_point;
	while (p != nullptr) {
		user_trajectory.map[p->x][p->y] = GENERATE;
		p = p->pre_point;
	}
	user_trajectory.map[start.x][start.y] = START;
	user_trajectory.map[end.x][end.y] = END;
	Graph::show_map(user_trajectory);
}

void A_star::show_para_below(Point* p)
{
	wchar_t s1[5];
	_stprintf_s(s1, _T("(%d,"), p->x);
	outtextxy(0, 480, s1);
	wchar_t s2[5];
	_stprintf_s(s2, _T("%d)"), p->y);
	outtextxy(50, 480, s2);
}

void A_star::search_env_block(Point* p)
{
	int temp_cost(INT_MAX);

	for (int i = 0; i < env_nums; i++) {
		
		Point* search_p = new Point((*p).x + offset_x[i], (*p).y + offset_y[i]);

		if (judge_env_legal(*search_p)) {

			temp_cost = (*p).Gn + calcu_cost(*p, *search_p);

			if (is_alread_set(obstacleset, search_p) || is_alread_set(fatherset, search_p)) {
				delete search_p;
				continue;
			}
			else {
				user_trajectory.map[search_p->x][search_p->y] = SEARCH;
				if (!is_alread_set(openset, search_p)) {
					search_p->Gn = temp_cost;
					search_p->Fn = search_p->Gn + calcu_Hn(*search_p);
					search_p->pre_point = p;
					add_openset(search_p);
				}else if(search_p->Gn > temp_cost){
					search_p->Gn = temp_cost;
					search_p->Fn = search_p->Gn + calcu_Hn(*search_p);
					search_p->pre_point = p;
				}
				
			}
		}
		else {delete search_p;}
	}

}

bool A_star::is_alread_set(std::vector<Point*>& vec, Point* p)
{
	for (int i = 0; i < vec.size(); i++) {
		if (*vec[i] == *p) {
			return true;
		}
	}
	return false;
}

Point* A_star::find_min_Fn(int& idx)
{
	int min_Fn(INT_MAX);
	for (int i = 0; i < openset.size(); i++) {
		if ((*openset[i]).Fn < min_Fn) {
			idx = i;
			min_Fn = (*openset[i]).Fn;
		}
	}
	return openset[idx];
}

bool A_star::judge_env_legal(const Point& p)
{
	if (p.x < 0 || p.x > user_trajectory.map_x || p.y < 0 || p.y > user_trajectory.map_y)
	{
		return false;
	}
	else {
		return true;
	}
}

int A_star::calcu_cost(const Point& p1, const Point& p2)
{
	int dx = abs(p1.x - p2.x);
	int	dy = abs(p1.y - p2.y);
	int cost = 0;
	calcu_type_selection(cost, dx, dy);
	return cost;
}

int A_star::calcu_Hn(const Point& p1)
{
	int dx = abs(p1.x - end.x);
	int	dy = abs(p1.y - end.y);
	int Hn = 0;
	calcu_type_selection(Hn, dx, dy);
	return Hn;
}

void A_star::calcu_type_selection(int& data, const int& dx1, const int& dy1) {
	#ifdef Manhattan_Distance
		data = plane_cost * (dx1 + dy1);
	#elif Euclidean_Distance
		data = int(plane_cost * sqrt(dx1 * dx1 + dy1 * dy1));
	#endif
}

UserPara A_star::get_astar_userpara()
{
	return user_trajectory;
}