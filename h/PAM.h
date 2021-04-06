#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Formater.h"
#include <regex>
using namespace std;
class PAM:public Formater {
public:
	void open(string filename) override;
	void save(string filename) override;
	string getName(string path) override {
		regex r(".*([^\\\.]*)\.pam");
		smatch res;
		string name;
		if (regex_match(path, res, r)) {
			return res.str(1);
		}
		return 0;
	}
private:
	int depth, maxval;
	string tupltype;
	void readheader(ifstream& file);

};