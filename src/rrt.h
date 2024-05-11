#pragma once
#include <cmath>
#include <queue>
#include "rrt_graph.h"

#define PI acos(-1)

class RRT
{
public:
	//default graph set, it always use (0,0) as start, (width, height) as end
	//i don't commend to use this default constructor
	//because struct paras can change outside 
	//rrt or rrtstar class need to know the last paras
	RRT() { srand((unsigned)time(NULL)); };
	RRT(const rrtPara& user_rrt_graph_para):rrt_user_para(user_rrt_graph_para){
		start = new Point(0, 0);//receive specific graph set, use (0,0) as start, (width, height) as end
		end = new Point(user_rrt_graph_para.rrt_graph_width-1, user_rrt_graph_para.rrt_graph_height-1);
	}
	virtual ~RRT();//do nothing,because the code can run without errors,if errors happen,i will fix it
	RRT(const RRT&) {};
	virtual void rrt_algorithm();
	virtual void testAtan2Radian();
	virtual void getAtan2Node(Point*, const Point* node, float&);
	virtual void getPath(const Point*);
	virtual float getRandomFloat(const float&, const float&);
	virtual Point* getRRTNode(const Point*);
	virtual Point* getRandomRRTNode(int&);
	virtual void delayMs(const int&);
	virtual void changeAngle(float&);
	virtual void showParaBelow(const Point*);
	virtual int calcuDist(const Point*, const Point*);
	virtual Point* findMinDistPoint(const Point*);
	virtual bool isLegalPoint(const Point*);
	virtual bool isObstacle(const Point*);
	

protected:

	std::vector<Point*> rrt_path;
	rrtPara rrt_user_para;
	Point* start = nullptr;
	Point* end = nullptr;
};

