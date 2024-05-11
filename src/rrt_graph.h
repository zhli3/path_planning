#pragma once

#include <string>
#include "Graph.h"

//remember do not user one struct  variable to decide another variable
//because i always forget to update their values when code gets bigger
typedef struct _rrt_para
{
	int rrt_graph_width = 960;
	int rrt_graph_height = 480;
	int block = 16;
	int rrt_graph_real_height = rrt_graph_height + block * 3;
	int obstacle_r = 30;
	int separating_line = 5;
	int rrt_line_width = 2;
	int rrt_edge_len = 15;
	float beta = 0.99;
	int avoid_obstacles_cnt = 5;

}rrtPara;

class rrtGraph
{
public:
    rrtGraph();//make default graph set, it always use (0,0) as start, (width, height) as end
	rrtGraph(const int&, const int&);//make specific graph set
	rrtGraph(IMAGE*);
    ~rrtGraph();
	virtual rrtPara getRRTGraphPara() const { return *rrt_graph_para; }
	virtual void drawSolidCircle(Point*& p, const int& r, const int& color);
	virtual void drawSolidLine(Point*&, Point*&, const int&, const int&);
    void init_obs_start_end();
	

private:
    rrtPara* rrt_graph_para = new rrtPara;
};

