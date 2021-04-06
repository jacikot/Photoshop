#pragma once
#include <iostream>
#include <cmath>
using namespace std;
class PIX { 
public:
	int pixel[4];
	friend ostream& operator<<(ostream& out, const PIX& p);
	void setr(unsigned int c) {
		pixel[0] = c % 256;
	}
	void setg(unsigned int c) {
		pixel[1] = c % 256;
	}
	void setb(unsigned int c) {
		pixel[2] = c % 256;
	}
	void seta(unsigned int c=255) {
		pixel[3] = c % 256;
	}
	PIX(int r = 0, int g = 0, int b = 0, int a = 0) {
		setAll(r, g, b, a);
	}
	void setAll(int r = 0, int g = 0, int b = 0, int a = 0);
	unsigned int operator[](char c) const;
	PIX operator* (double d) const;
	PIX& operator=(const PIX& p);
	PIX(const PIX& p);
	
	friend PIX operator+(const PIX& p1, const PIX& p2);
	void operator()(PIX p, int opacity) {
		(*this)= p * ((double)(p['a'])*opacity / 25500) + (*this) * ((double)(255 - p['a'] * opacity/100)/255);
	}
	PIX operator %(int p);
private:
	void kopiraj(const PIX& p) {
		for (int i = 0; i < 4; i++) {
			pixel[i] = p.pixel[i];
		}
	}
	
};