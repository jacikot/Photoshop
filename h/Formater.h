#pragma once
#include <string>
#include <vector>
#include "PIX.h"
using namespace std;
typedef vector<PIX> vec;

class Formater {
public:
	virtual void open(string filename)=0;
	virtual void save(string filename)=0;
	int getwidth() {
		return width;
	}
	int getheight() {
		return height;
	}
	vec& getpixels(){
		return pixels;
	}
	void setwidth(int w) {
		width = w;
	}
	void setheight(int h) {
		height = h;
	}
	void setPix(const vec& pix) {
		pixels = pix;
	}
	virtual string getName(string path) = 0;
	virtual ~Formater() {};
	//Formater():p(new vec()),pixels(*p),width(0),height(0){}
protected:
	int width, height;
	//!promeni ovo sa pikselima
	vec pixels; 
	void swap();
};