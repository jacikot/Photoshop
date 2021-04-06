#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Errors.h"
#include "Formater.h"
#include "PAM.h"

void PAM::readheader(ifstream& file) {
	string s;
	file >> s >> s;
	width = atoi(s.c_str());
	file >> s >> s;
	height = atoi(s.c_str());
	file >> s >> s;
	depth = atoi(s.c_str());
	file >> s >> s;
	maxval = atoi(s.c_str());
	file >> s >> s;
	tupltype = s;
	file >> s;
}
string ss;
void PAM::open(string filename) {
	ifstream file;
	try {
		file.open(filename, std::ios::binary);
		if (!file) throw OpeningFileFail();
		string s;
		file >> s;
		if (s != "P7") throw FormatError();
		readheader(file);

		cout << width << " " << height << " " << depth << " " << maxval << " " << tupltype << endl;

		char c;
		int br = 0;
		PIX p;
		unsigned short sign;
		char r, g, b,a;
		//int word = maxval / 255 + ((maxval / 255 * 255 == maxval) ? 0 : 1);
		file.read(&c, 1);
		//omoguciti vise reci!
		for (br = 0; br < height*width; br++) {
			file.read(&r, 1);
			p.setr(r);
			file.read(&g, 1);
			p.setg(g);
			file.read(&b, 1);
			p.setb(b);
			if (depth == 4) {
				file.read(&a, 1);
				p.seta(a);
			}
			else p.seta();
			pixels.push_back(p);
		}
		file.read(&c, 1);
		if (!file.eof()) throw ReadingFail();
		else cout << "uspesno ucitavanje PAM formata" << endl;
		cout << height << " " << width<<endl;
		
	}
	catch (FormatError& fail) {
		file.close();
		throw;
	}
	catch (ReadingFail& fail) {
		file.close();
		throw;
	}
}


void PAM::save(string filename) {
	ofstream file;
	try {
		file.open(filename, std::ios::binary);
		if (!file) throw OpeningFileFail();
		unsigned char c = 0x0a;
		file << "P7" << c;
		file << "WIDTH " << width << c;
		file << "HEIGHT " << height << c;
		file << "DEPTH " << 4 << c;
		file << "MAXVAL " << 255 << c;
		file << "TUPLTYPE " << "RGB_ALPHA" << c;
		file << "ENDHDR" << endl;
		for (PIX p : pixels) {
			file << (unsigned char)p.pixel[0] <<(unsigned char) p.pixel[1] << (unsigned char)p.pixel[2] <<(unsigned char)p.pixel[3];
		}
		file.close();
	}
	catch (OpeningFileFail& fail) {
		cout << fail;
	}
}

