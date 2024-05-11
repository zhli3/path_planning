#include <stdio.h>
#include <direct.h>
#include "Point.h"
#include "A_star.h"
#include "rrt.h"
#include "RRTStar.h"
#include "Bezier.h"
#include "DataProc.h"

using namespace std;

#define A_STAR 0
//#define _RRT 1
//#define _RRTSTAR 2
//#define _RRTTSAR_USERGRAPH 3 //can not use
//#define _BEZIER_TEST 4
//#define _GENEGATE_RRTSTAR_BEZIER_PATH 5


IMAGE img;
int main()
{
	while(1) {

#ifdef A_STAR
		UserPara user_para;
		Graph graph(user_para);
		A_star astar;
		astar.set_start_end_obs_map(graph);
		astar.Astar_algorithm();
#elif _RRT
		rrtGraph rrt_graph(480, 360);
		RRT rrt_user(rrt_graph.getRRTGraphPara());
		rrt_user.rrt_algorithm();
#elif _RRTSTAR
		rrtGraph rrt_graph(480, 360);
		RRTStar rrtstar(rrt_graph.getRRTGraphPara());
		rrtstar.rrtstar_algorithm();
#elif _RRTTSAR_USERGRAPH

		vector<Point*> unsmooth_path;
		string now_path = _getcwd(NULL, 0);
		string path2_dir = now_path + "\\csvdata\\reverse_path2.csv";

		Point start(40, 90);
		Point end(420, 90);

		rrtGraph rrt_graph_img(&img);
		RRTStar rrtstar(rrt_graph_img.getRRTGraphPara());

		//start.x = 320;
		//start.y = 90;
		//end.x = 240;
		//end.y = 300;

		end.x = 420;
		end.y = 130;

		Point* p_tail = rrtstar.rrtstar_algorithm(&start, &end);
		Bezier bezier;
		bezier.getUnsmoothPath(p_tail);
		std::vector<Point*> path2 = bezier.formulaBezier(3);
		//DataProc read_csv;
		//std::vector<Point> path2;
		//read_csv.readCSV(path2, path2_dir, 2);

		Point* p = new Point();
		for (int i = 0; i < path2.size(); i++) {
			p->x = path2[i]->x;
			p->y = path2[i]->y;
			rrt_graph_img.drawSolidCircle(p, 1, BLUE);
			//rrt_graph_img.drawSolidLine(smooth_path[i], smooth_path[i + 1], GREEN, 4);
		}
		delete p;

		DataProc wirte_csv;
		
		
		//wirte_csv.saveCSV(path2, path2_dir);


#elif _BEZIER_TEST

		string now_path = _getcwd(NULL, 0);

		string unsmooth_path_rdir = (now_path + "\\csvdata\\reverse_unsmooth.csv");
		vector<Point> unsmooth_path_body;
		DataProc read_data;
		read_data.readCSV(unsmooth_path_body, unsmooth_path_rdir, 2);

		vector<Point*> unsmooth_path;
		for (int i = 0; i < unsmooth_path_body.size(); i++) {
			Point* p = new Point(unsmooth_path_body[i].x, unsmooth_path_body[i].y);
			unsmooth_path.push_back(p);
		}

		Bezier bezier;
		bezier.getUnsmoothPath(unsmooth_path);
		
		
		//unsmooth_path.clear();
		//srand((unsigned)time(NULL));
		//for (int i = 0; i < 30; i++) {
		//	Point* p = new Point(RANDOM_RANGE(300, 300), RANDOM_RANGE(300, 300));
		//	unsmooth_path.push_back(p);
		//}


		vector<Point*> smooth_path = bezier.iterateBezier(4);
		vector<Point*> formula_smooth_path = bezier.formulaBezier(4);

		DataProc write_data;
		string smooth_path_dir = (now_path + "\\csvdata\\reverse_smooth.csv");
		write_data.saveCSV(smooth_path, smooth_path_dir);

		string formula_smooth_path_dir = (now_path + "\\csvdata\\reverse_formula_smooth.csv");
		write_data.saveCSV(formula_smooth_path, formula_smooth_path_dir);

		rrtGraph rrt_graph_img(&img);
		for (int i = 1; i < unsmooth_path.size(); i++) {
			rrt_graph_img.drawSolidLine(unsmooth_path[i], unsmooth_path[i - 1], RED, 2);
			//rrt_graph_img.drawSolidCircle(unsmooth_path[i], 1, RED);
		}

		for (int i = 0; i < smooth_path.size(); i++) {
			rrt_graph_img.drawSolidCircle(smooth_path[i], 1, GREEN);
			//rrt_graph_img.drawSolidLine(smooth_path[i], smooth_path[i + 1], GREEN, 4);
		}

		for (int i = 0; i < formula_smooth_path.size(); i++) {
			rrt_graph_img.drawSolidCircle(formula_smooth_path[i], 1, BROWN);
			//rrt_graph_img.drawSolidLine(smooth_path[i], smooth_path[i + 1], GREEN, 4);
		}

		while (!unsmooth_path.empty()) {
			delete unsmooth_path[0];
			unsmooth_path.erase(unsmooth_path.begin());
		}

#elif _GENEGATE_RRTSTAR_BEZIER_PATH

//set first path start and end
	Point start(550, 200);
	Point end(305, 335);
	int smooth_len(0);
	string now_path = _getcwd(NULL, 0);
	DataProc w_csv;

	{
		//set up sample based graph
		rrtGraph rrt_graph_img(&img);
		RRTStar rrtstar(rrt_graph_img.getRRTGraphPara());
		//run algorithm
		rrtstar.setIter(1500);

		Point* p_tail = rrtstar.rrtstar_algorithm(&start, &end);
		//run n-1 order bezier to smooth the path
		Bezier bezier;
		bezier.getUnsmoothPath(p_tail);
		std::vector<Point*> path1 = bezier.formulaBezier();
		//save smooth path
		string path1_dir = now_path + "\\csvdata\\smooth_path1.csv";
		//std::vector<Point*> path1 = rrtstar.getRRTstarPath(p_tail);
		w_csv.saveCSV(path1, path1_dir);

		//smooth_len = path1.size();
		//start.x = path1[smooth_len - 1]->x;
		//start.y = path1[smooth_len - 1]->y;

		cin.get();
	}

	//{
	//	end.x = 400;
	//	end.y = 335;

	//	closegraph();
	//	rrtGraph rrt_graph_img2(&img);
	//	RRTStar rrtstar2(rrt_graph_img2.getRRTGraphPara());
	//	//rrtstar2.setIter(4000);
	//	Point* p_tail2 = rrtstar2.rrtstar_algorithm(&start, &end);
	//	//Bezier bezier2;
	//	//bezier2.getUnsmoothPath(p_tail2);
	//	//std::vector<Point*> path2 = bezier2.formulaBezier(6);

	//	string path2_dir = now_path + "\\csvdata\\smooth_path2.csv";
	//	std::vector<Point*> path2 = rrtstar2.getRRTstarPath(p_tail2);
	//	w_csv.saveCSV(path2, path2_dir);
	//}


#endif
		cin.get();
		if (cin.get() == 'q') { break; }
	}

	closegraph();
	return 0;
}

