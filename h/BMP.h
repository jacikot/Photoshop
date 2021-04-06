#pragma once
#include "Formater.h"
#include <iostream>
#include <string>
#include <fstream>
typedef unsigned int dword;
typedef unsigned short word;
typedef class h {
	word type; //
	dword size; //
	dword offpix; //!!!obavezno menjaj kad menjas sliku
	friend class BMP;
public:
	void readheader(ifstream&file);
} header;
typedef class s1 {
public:
	dword size;
	dword width; //
	dword height; //
	word planes;
	word bitCount;
	dword imagesize; //
	friend class BMP;
public:
	void readinfo(ifstream&file);

}info;
using namespace std;
class BMP :public Formater {
public:
	void open(string filename);
	void save(string filename);
private:
	header h;
	info i;
};