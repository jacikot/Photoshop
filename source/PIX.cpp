#include"PIX.h"
#include <iostream>
using namespace std;
ostream& operator<<(ostream& out,const PIX& p) {
	out << "(";
	for (int i = 0; i < 4; i++) {
		out << (unsigned short)p.pixel[i];
		if (i != 3) out << ",";
		else out << ")";
	}
	out << endl;
	return out;
}

void PIX:: setAll(int r, int g, int b, int a) {
	pixel[0] = r % 256;
	pixel[1] = g % 256;
	pixel[2] = b % 256;
	pixel[3] = a % 256;
}

unsigned int PIX:: operator[](char c) const {
	switch (c) {
	case 'r': return  pixel[0]; break;
	case 'g': return  pixel[1]; break;
	case 'b': return  pixel[2]; break;
	case 'a': return  pixel[3]; break;
	}
}

PIX PIX::operator* (double d) const {
	PIX p;
	if (d > 1) return*this; //razmotri
	p.pixel[0] = (unsigned char)trunc(d*pixel[0]);
	p.pixel[1] = (unsigned char)trunc(d*pixel[1]);
	p.pixel[2] = (unsigned char)trunc(d*pixel[2]);
	return p;
}

PIX& PIX::operator=(const PIX& p) {
	if (this != &p) {
		kopiraj(p);
	}
	return *this;
}

PIX::PIX(const PIX& p) {
	for (int i = 0; i < 4; i++) {
		pixel[i] = p.pixel[i];
	}
}

PIX operator+(const PIX& p1, const PIX& p2) {
	PIX p;
	p.pixel[0] = (p1['r'] + p2['r']) % 256; 
	p.pixel[1] = (p1['g'] + p2['g']) % 256;
	p.pixel[2] = (p1['b'] + p2['b']) % 256;
	return p;
}

PIX PIX:: operator %(int p) {
	PIX pix;
	for (int i = 0; i < 4; i++) {
		pix.pixel[i] = pixel[i] % p;
		if (pix.pixel[i] < 0) pix.pixel[i] + p;
	}
	return pix;
}