#include"Selection.h"
#include <algorithm>
#include "Rectangle.h"
#include"Image.h"
#include <fstream>
using namespace std;



bool Selection:: isInActiveSelection(int x, int y) const {
	if (!changeable) return false;
	return any_of(rectangles.begin(), rectangles.end(), [x,y](rectangle*r) {
		return (x >= r->getX()) && 
			(x <= r->getW() + r->getX()) && 
			(y >= r->getY()) && 
			(y <= r->getH() + r->getY());
	});
}

bool Selection::isValid(Image*i) const {
	return all_of(rectangles.begin(), rectangles.end(), [i](rectangle* r)->bool {
		return (r->getX() >= 0) && (r->getY() >= 0) &&
			((r->getX() + r->getW()) <= i->getW()) &&
			((r->getY() + r->getH()) <= i->getH());
	});
}
;