#pragma once

#include <vector>
#include <iostream>
#include <conio.h>
#include <graphics.h>
#include "Point.h"

#define RANDOM(a) (rand()%(a)) 
#define RANDOM_RANGE(a,b) (rand()%(((b)-(a)+1)+(a)))  //[i, j]

//#define	BLACK			0
//#define	BLUE			0xAA0000
//#define	GREEN			0x00AA00
//#define	CYAN			0xAAAA00
//#define	RED				0x0000AA
//#define	MAGENTA			0xAA00AA
//#define	BROWN			0x0055AA
//#define	LIGHTGRAY		0xAAAAAA
//#define	DARKGRAY		0x555555
//#define	LIGHTBLUE		0xFF5555
//#define	LIGHTGREEN		0x55FF55
//#define	LIGHTCYAN		0xFFFF55
//#define	LIGHTRED		0x5555FF
//#define	LIGHTMAGENTA	0xFF55FF
//#define	YELLOW			0x55FFFF
//#define	WHITE			0xFFFFFF

#define PATH			0xFFFF55//LIGHTCYAN
#define OBSTACLE		0x000000//BLACK
#define START			0x0000AA//RED
#define END				0x00AA00//GREEN
#define GENERATE		0x55FFFF//YELLOW
#define SEARCH			0x55FF55//LIGHTGREEN
#define PASS_SEARCH		0xAAAAAA//LIGHTGRAY

const int map_line = 60;
const int map_col  = 30;
const int block = 16;
const int block_division = 3;

typedef struct _UserPara
{

public:
	int block_size = block;
	int win_width = map_line * block_size;
	int win_height = map_col * block_size;
	int win_real_height = win_height + block_size * 3;
	int map_x = win_width / block_size - 1;
	int map_y = win_height / block_size - 1;
	int obstacle_num = map_line * map_col / block_division;
	int map[map_line][map_col] = { 0 };

}UserPara;

class A_star;

class Graph
{
	public:
		Graph(UserPara user_para);
		~Graph();
		friend class A_star;
		static void show_map(const UserPara&);
		void set_obstacles_start_end();
		Point get_start();
		Point get_end();
		UserPara get_userPara();
		std::vector<Point*> get_obstacle();
		

	private:
		UserPara graph_user_para;
		Point start;
		Point end;
		std::vector<Point*> obstacle_vector;
		std::vector<Point*> obstacle;

};

