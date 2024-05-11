#pragma once
#include "rrt.h"

typedef struct _rrt_star_para {
    int near_circle = 20;//can decide smooth degree of the path
    int iterator = 8000;
    int end2node = 5;


}rrtstarPara;

class RRTStar :
    public RRT
{

public:
    //default graph set, it always use (0,0) as start, (width, height) as end
    //i don't commend to use this default constructor
    //because struct paras can change outside 
    //rrt or rrtstar class need to know the last paras
    RRTStar() { srand((unsigned)time(NULL)); }
    RRTStar(const rrtPara& user_rrt_graph_para){
        //receive specific graph set, use (0,0) as start, (width, height) as end
        rrt_user_para = user_rrt_graph_para;
        start = new Point(0, 0);
        end = new Point(user_rrt_graph_para.rrt_graph_width-1, user_rrt_graph_para.rrt_graph_height-1);
    }
    RRTStar(const RRTStar&) {}
    virtual ~RRTStar();
    virtual void rrtstar_algorithm();
    virtual Point* rrt_algorithm(const Point*, const Point*);
    virtual Point* rrtstar_algorithm(const Point*, const Point*);//given specific start and end 
    virtual bool findOptimizationPoint(Point*, Point*);
    virtual bool findNearPoints(std::vector<Point*>&, Point*&, Point*&);
    virtual void changePrePoint(std::vector<Point*>&, Point*&);
    virtual bool isBlockedByObs(const Point*, const Point*);
    virtual Point* getOptimalPath(const Point*);
    virtual Point* getSearchNode(Point*&, Point*&);
    virtual void setIter(const int& cnt) { user_rrtstar_para.iterator = cnt; }
    virtual const std::vector<Point*> getRRTstarPath(Point*);
    virtual bool isLegalPoint(const Point*);

protected:
    //remember set it as a parameter, then give it to funcs to do things
    //so i can change elements in struct outside then pass them to funcs
    rrtstarPara user_rrtstar_para;
    int lx = 250;
    int ux = 560;;
    int ly = 190;
    int uy = 400;
};
