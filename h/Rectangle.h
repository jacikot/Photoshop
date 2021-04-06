#pragma once
#include <utility>
using namespace std;
class rectangle { //probaj da iskljucis rectangle ugradjeni
public:

	rectangle(int x=0, int y=0, int w=1, int h=1): coordinates(x,y),width(w),height(h){}
	int getW() {
		return width;
	}
	int getH() {
		return height;
	}
	int getX() {
		return coordinates.first;
	}
	int getY() {
		return coordinates.second;
	}
private:
	int width, height;
	pair<int, int> coordinates;
 };