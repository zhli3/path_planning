#include "rrt_graph.h"


rrtGraph::rrtGraph(IMAGE* img)
{

	loadimage(img, _T("D:\\parallel_map.png"));
	int imagHeight = img->getheight();
	int imagWidth = img->getwidth();
	initgraph(imagWidth, imagHeight);
	putimage(0, 0, img);

	rrt_graph_para->rrt_graph_height = imagHeight;
	rrt_graph_para->rrt_graph_width = imagWidth;
	rrt_graph_para->rrt_graph_real_height = \
		rrt_graph_para->rrt_graph_height + rrt_graph_para->block * 3;
	rrt_graph_para->rrt_line_width = 2;
	rrt_graph_para->rrt_edge_len = 15;

	//this is only use in parking
	//in other condition can be ignored
	setfillcolor(BLACK);
	solidcircle(250, 275, 70);
	int off_set_x = 230;
	int off_set_y = 50;
	solidellipse(600-off_set_x, 275-off_set_y, 600+off_set_x,  275+off_set_y);

	std::cin.get();
	settextstyle(16, 8, _T("Consolas"));
	settextcolor(BLACK);
	setlinecolor(BLACK);
	setfillstyle(BS_SOLID);
	setfillcolor(GREEN);
	setlinestyle(PS_SOLID, 3);

}

rrtGraph::rrtGraph()
{
	
	init_obs_start_end();

	//while (1) {
	//	HWND wnd = GetHWnd();
	//	if (MessageBox(wnd, _T("rrt is over.\ndo it again£¿"), _T("ask"), MB_YESNO | MB_ICONQUESTION) == IDYES)
	//		init_obs_start_end();
	//}
	//else
	//	Quit();

}

rrtGraph::rrtGraph(const int& width, const int& height)
{
	rrt_graph_para->rrt_graph_height = height;
	rrt_graph_para->rrt_graph_width = width;
	rrt_graph_para->rrt_graph_real_height = rrt_graph_para->rrt_graph_height + block * 3;
	init_obs_start_end();
		
}

rrtGraph::~rrtGraph()
{
	delete rrt_graph_para;
}



void rrtGraph::init_obs_start_end()
{
	initgraph(rrt_graph_para->rrt_graph_width, rrt_graph_para->rrt_graph_real_height);
	settextstyle(16, 8, _T("Consolas"));
	setbkcolor(WHITE);
	settextcolor(BLACK);
	setlinestyle(PS_DASHDOT, rrt_graph_para->separating_line);
	setlinecolor(BLACK);
	setfillstyle(BS_SOLID);
	cleardevice();

	setfillcolor(BLACK);

	//draw the separating line between graph set and paras set
	line(0, rrt_graph_para->rrt_graph_height + rrt_graph_para->separating_line,
		rrt_graph_para->rrt_graph_width,
		rrt_graph_para->rrt_graph_height + rrt_graph_para->separating_line);

	bool mouse_down_flag = false;
	ExMessage mse;

	while (1) {
		mse = getmessage(EM_MOUSE | EM_KEY);

		if (mouse_down_flag == false && mse.message == WM_RBUTTONDOWN) {
			mouse_down_flag = true;
			while (1) {
				mse = getmessage(EM_MOUSE);
				if (mse.message == WM_RBUTTONUP) {
					mouse_down_flag = false;
					break;
				}
				else {solidcircle(mse.x, mse.y, rrt_graph_para->obstacle_r);}
			}
		}
		else if (mse.message == WM_LBUTTONUP) {
			setfillcolor(GREEN);
			solidcircle(0 + rrt_graph_para->block, rrt_graph_para->rrt_graph_height+block, 
				rrt_graph_para->obstacle_r/4);
			break;
		}
	}


}

void rrtGraph::drawSolidCircle(Point*& p, const int& r, const int& color)
{
	setfillcolor(color);
	solidcircle(p->x, p->y, r);
}

void rrtGraph::drawSolidLine(Point*& p1, Point*& p2, const int& color, const int& line_width)
{
	setlinecolor(color);
	setlinestyle(PS_SOLID, line_width);
	line(p1->x, p1->y, p2->x, p2->y);
}