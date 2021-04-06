#include "BMP.h"
#include<fstream>
#include <iostream>
#include <algorithm>
#include"Errors.h"
#include "PAM.h"
#include"Image.h"
using namespace std;

void s1::readinfo(ifstream&file) {
	int i;
	file.read(reinterpret_cast<char*>(&size), 4);
	file.read(reinterpret_cast<char*>(&width), 4);
	file.read(reinterpret_cast<char*>(&height), 4);
	file.read(reinterpret_cast<char*>(&planes), 2);
	file.read(reinterpret_cast<char*>(&bitCount), 2);
	file.read(reinterpret_cast<char*>(&i), 4);
	file.read(reinterpret_cast<char*>(&imagesize), 4);
	cout << size << " " << width << " " << height << " " << bitCount << " " << imagesize << endl;
}


void h::readheader(ifstream&file) {
	int p;
	file.read(reinterpret_cast<char*>(&type), 2);
	file.read(reinterpret_cast<char*>(&size), 4);
	file.read(reinterpret_cast<char*>(&p), 4);
	file.read(reinterpret_cast<char*>(&offpix), 4);
}



void BMP::open(string filename) {
	
	ifstream file;
	try {
		file.open(filename, std::ios::binary);
		if (!file) throw OpeningFileFail();
		h.readheader(file);
		if (h.type != 0x4D42) throw FormatError();
		i.readinfo(file);
		width = i.width;
		height = i.height;
		char*c = new char[i.size - 24];
		file.read(c, i.size - 24);
		delete c;
		PIX p;
		for (int j = 0; j < i.height; j++) {
			for (int k = 0; k < i.width; k++) {
				char c;
				file.read(&c, 1);
				p.setb(c);
				file.read(&c, 1);
				p.setg(c);
				file.read(&c, 1);
				p.setr(c);
				if (i.bitCount == 32) {
					file.read(&c, 1);
					p.seta(c);
				}
				else p.seta();
				pixels.push_back(p);
			}
			if (i.bitCount == 24) { //padding
				if (i.width % 4) {
					char*c = new char[4 - i.width % 4];
					file.read(c, 4 - i.width % 4);
				}
			}
		}
		char cc;
		file.read(&cc, 1);
		if (!file.eof()) throw ReadingFail();
		cout << "uspesno citanje" << endl;
		swap();
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

void BMP::save(string filename) {
	swap();
	ofstream file;
	try {
		file.open(filename, std::ios::binary);
		if (!file) throw OpeningFileFail();
		short t = 0x4d42;
		int k;
		//header
		cout << "bmp save" << height << " " << width << endl;
		file.write(reinterpret_cast<char*>(&t), 2);
		k = height*width * 4 + 122;
		t = 0;
		file.write(reinterpret_cast<char*>(&k), 4)
			.write(reinterpret_cast<char*>(&t), 2)
			.write(reinterpret_cast<char*>(&t), 2);
		k = 122;
		file.write(reinterpret_cast<char*>(&k), 4);
		//info
		k = 108;
		file.write(reinterpret_cast<char*>(&k), 4)
			.write(reinterpret_cast<char*>(&width), 4)
			.write(reinterpret_cast<char*>(&height), 4);
		t = 1;
		file.write(reinterpret_cast<char*>(&t), 2);
		k = 0x20;
		file.write(reinterpret_cast<char*>(&k), 2);
		k = 3;
		file.write(reinterpret_cast<char*>(&k), 4);
		k = width*height * 4;
		file.write(reinterpret_cast<char*>(&k), 4);
		k = 2835;
		file.write(reinterpret_cast<char*>(&k), 4)
			.write(reinterpret_cast<char*>(&k), 4);
		k = 0;
		file.write(reinterpret_cast<char*>(&k), 4)
			.write(reinterpret_cast<char*>(&k), 4);
		k = 0x00FF0000;
		file.write(reinterpret_cast<char*>(&k), 4);
		k = 0x0000FF00;
		file.write(reinterpret_cast<char*>(&k), 4);
		k = 0x000000FF;
		file.write(reinterpret_cast<char*>(&k), 4);
		k = 0xFF000000;
		file.write(reinterpret_cast<char*>(&k), 4);
		k = 0x57696E20;
		file.write(reinterpret_cast<char*>(&k), 4);
		char cc[48];
		for (int k = 0; k < 48; k++) {
			cc[k] = 0;
		}
		file.write(cc, 48);
		//pixels
		for_each(pixels.begin(), pixels.end(), [&file](PIX&p) {
			file <<(unsigned char)p.pixel[2] <<(unsigned char)p.pixel[1] << (unsigned char)p.pixel[0] <<(unsigned char) p.pixel[3];
		});
		file.close();
	}
	catch (OpeningFileFail& fail) {
		cout << fail;
	}
	
}


