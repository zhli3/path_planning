#include <time.h>
#include <random>
#include "Graph.h"

Graph::Graph(UserPara user_para)
{
	graph_user_para = user_para;
	start = Point(1, 1);
	end = Point(graph_user_para.map_x-1, graph_user_para.map_y-1);
	set_obstacles_start_end();
	srand((unsigned)time(NULL));
	initgraph(graph_user_para.win_width, graph_user_para.win_real_height);
	settextstyle(16, 8, _T("Courier"));
	setbkcolor(WHITE);
	settextcolor(BLACK);
	cleardevice();
}

std::vector<Point*> Graph::get_obstacle()
{
	return obstacle;
}

UserPara Graph::get_userPara()
{
	return graph_user_para;
}

Point Graph::get_end()
{
	return end;
}

Point Graph::get_start()
{
	return start;
}

void Graph::set_obstacles_start_end()
{

	for (int i = 0; i <= graph_user_para.map_x; i++) {
		for (int j = 0; j <= graph_user_para.map_y; j++) {
			Point* obstacle = new Point(i, j);
			if (*obstacle != start && *obstacle != end) {
				obstacle_vector.push_back(obstacle);
			}
			else { delete obstacle; }
		}
	}

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(obstacle_vector.begin(), obstacle_vector.end(), rd);

	for (int i = 0; i < graph_user_para.obstacle_num; i++) {
		obstacle.push_back(obstacle_vector[0]);
		obstacle_vector.erase(obstacle_vector.begin());
	}

}
	
void Graph::show_map(const UserPara& para)
{
	for (int i = 0; i <= para.map_x; i++) {
		for (int j = 0; j <= para.map_y; j++) {
			setfillcolor(para.map[i][j]);
			fillrectangle(i * para.block_size, j * para.block_size, \
				(i + 1) * para.block_size, (j + 1) * para.block_size);
		}
	}
}

Graph::~Graph()
{
	for (int i = 0; i < obstacle_vector.size(); i++) {
		delete obstacle_vector[i];
		obstacle_vector.erase(obstacle_vector.begin());
	}


	for (int i = 0; i < obstacle.size(); i++) {
		delete obstacle[i];
		obstacle.erase(obstacle.begin());
	}
}

