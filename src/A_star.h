#pragma once
#include "Graph.h"
#include <vector>

//#define Manhattan_Distance  0
#define Euclidean_Distance  1

#define MIN(x,y) ((x) >= (y) ? (y) : (x))

class Graph;

class A_star
{
	public:
		A_star() {};
		virtual ~A_star();
		A_star(const A_star&) {};
		void Astar_algorithm();
		bool judge_env_legal(const Point&);
		int calcu_Hn(const Point&);
		int calcu_Gn(const Point&);
		int calcu_cost(const Point&, const Point&);
		void set_start_end_obs_map(Graph&);
		void add_openset(Point* p) { openset.push_back(p); };
		void add_fatherset(Point* p) { fatherset.push_back(p); };
		void delete_openset(const int& idx) {openset.erase(openset.begin() + idx);};
		void search_env_block(Point*);
		void calcu_type_selection(int&, const int&, const int&);
		void show_para_below(Point*);
		bool is_alread_set(std::vector<Point*>&, Point*);
		void get_trajectory(const Point*);
		UserPara get_astar_userpara();
		Point* find_min_Fn(int& idx);

	protected:
		int cross_cost = 14;
		int plane_cost = 10;
		std::vector<Point*> openset;
		std::vector<Point*> fatherset;
		//std::vector<Point*> trajectory;
		std::vector<Point*> obstacleset;		
		Point start;
		Point end;
		UserPara user_trajectory;
		const int env_nums = 8;
		//int offset_x[4] = {0, 1, 0, -1};
		//int offset_y[4] = {-1, 0, 1, 0};
		int offset_x[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
		int offset_y[8] = { -1, -1, -1, 0, 1, 1, 1, 0};
		
};


