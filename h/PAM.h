#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Formater.h"
using namespace std;
class PAM:public Formater {
public:
	void open(string filename) override;
	void save(string filename) override;
private:
	int depth, maxval;
	string tupltype;
	void readheader(ifstream& file);
};