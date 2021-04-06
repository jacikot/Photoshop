#pragma once
#include"Image.h"
class XML {
public:
	XML(string file, Image* im);
	XML(Image*im) :XML(".xml", im) {}
	void exportXML(); //proveri sta je dostavljeno slici
	static string exportFunction(Operation&o, string path);
	Image* parseXML();
	static void parseFunction(string path); //menjala
private:
	Image*i;
	string path;
};