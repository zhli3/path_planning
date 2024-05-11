#pragma once
#include <limits>

#define FINAL_NODE		0
#define NOT_FINAL_NODE  1

class Point
{
	public:
		Point();
		Point(int x1, int y1);

		int x;
		int y;

		int Gn = 0;
		int Fn = 0;

		Point* pre_point = nullptr;

		bool operator!=(const Point& p1) 
		{
			if (this->x != p1.x || this->y != p1.y) {
				return true;
			}
			return false;
		}

		bool operator==(const Point& p1)
		{
			if (this->x == p1.x && this->y == p1.y) {
				return true;
			}
			return false;
		}

};

