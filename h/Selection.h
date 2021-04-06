#pragma once
#include <set>
#include<vector>
#include "Rectangle.h"
//#include "Image.h"

extern class Image;
//using namespace std;
class Selection {
public:
	Selection(string n, vector<rectangle*>v):name(n) {
		for (rectangle* r : v) {
			rectangles.insert(r);
		}
	}
	~Selection(){
		for (rectangle* r : rectangles) {
			delete r;
		}
	}
	void activate() {
		changeable = true;
	}
	void inactivate() {
		changeable = false;
	}
	bool isActive() {
		return changeable;
	}
	bool isInActiveSelection(int x, int y) const;
	void insertRectangle(rectangle*r) {
		rectangles.insert(r);
	}
	void deleteRectangle(rectangle* r) {
		rectangles.erase(r);
	}
	string getName() const {
		return name;
	}
	bool isValid(Image*i) const;
	set<rectangle*>&getRectangles() {
		return rectangles;
	}
private:
	set<rectangle*> rectangles;
	string name;
	bool changeable;
};