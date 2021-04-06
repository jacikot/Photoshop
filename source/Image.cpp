#include "Image.h"
#include <string>
#include <iostream>
#include "Formater.h"
#include"PAM.h"
#include "BMP.h"
#include <vector>
#include <regex>
#include"Errors.h"
#include"Layer.h"
#include <algorithm>
#include "Selection.h"
#include"OperationMap.h"
#include"XML.h"

using namespace std;

Formater* Image::makeformater(string filename) const {
	string s;
	regex r(".*\.([a-z][a-z][a-z])");
	smatch res;
	if (regex_match(filename, res, r)) {
		s = res.str(1);
	}
	else throw FileNameFormatError();
	Formater* f = nullptr;
	if (s == "pam") {
		f = new PAM();
	}
	else if (s == "bmp") {
		f = new BMP();
	}
	else throw FileNameFormatError();
	return f;
}

Image::Image(string filename, int opacity, bool active, bool changeable):numOfActiveSelection(0) { 
	Formater* f = makeformater(filename);
	f->open(filename);
	height = f->getheight();
	width = f->getwidth();
	Layer l(f->getpixels(), width, height);
	layers.push_back(l);
	Layer l2(width, height);
	image = l2.pixels;
	delete f;
	operations = OperationMap::getInstance();
}

Image::~Image() {
	layers.clear();
	image.clear();
	width = height = 0;
	for (auto s : selections) {
		delete s.second;
	}
	selections.clear();
	operations = nullptr;
}

void Image::resizeLayers(int h, int w) {
	if (h > height) {
		for_each(layers.begin(), layers.end(), [h,w](Layer& l) {
			l.resizeHeight(h);
		});
		Layer pict(image, width, height); //promeni velicinu i slike
		pict.resizeHeight(h);
		image = pict.pixels;
		height = h;
	}
	if (w > width) {
		for (Layer&l : layers) {
			l.resizeWidth(w);
		}
		Layer pict(image, width, height);
		pict.resizeWidth(w);
		image = pict.pixels;
		width = w;
	}

}

void Image::addLayer(string filename,int opacity,bool active,bool changeable) { 
	Formater* f = makeformater(filename);
	f->open(filename);
	int w = f->getwidth();
	int h = f->getheight();
	Layer l(f->getpixels(), w, h, opacity, active, changeable);
	if (h < height) l.resizeHeight(height);
	if (w < width) l.resizeWidth(width);
	if (h > height || w > width) resizeLayers(h, w);
	layers.push_back(l);
	delete f;
	numOfLayers++;
	
}

void Image::saveImage(string filename) {
	Formater* f = makeformater(filename);
	f->setheight(height);
	f->setPix(image);
	f->setwidth(width);
	f->save(filename);
	delete f;
}


Image::Image(int w, int h):width(w),height(h),numOfActiveSelection(0) {
	if (w == 0 || h == 0) throw InputError();
	Layer l(w, h);
	image = l.pixels;
	layers.push_back(l);
	operations = OperationMap::getInstance();
}

void Image::addLayer(int w, int h) {
	int he, wi;
	if (h <= height && w <= width) {
		he = height;
		wi = width;
	}
	else {
		he = h;
		wi = w;
		resizeLayers(h, w);
	}
	Layer l(wi,he);
	layers.push_back(l);
	numOfLayers++;

}

void Image::saveAsLayer() {
	Layer l(image,width,height);
	layers.push_back(l);
	numOfLayers++;
}

void Image::deleteLayer(int id) {
	try {
		if (id >= numOfLayers) throw OutOfRangeError();
		auto x = layers.begin() + id;
		layers.erase(x);
		numOfLayers--;
	}
	catch (OutOfRangeError&e) {
		cout << e;
	}
}

void Image::deleteAll() {
	if (numOfLayers>0) {
		layers.clear();
		numOfLayers = 0;
	}
}
void Image::activateLayer(int ind) {
	try {
		if (ind >= numOfLayers) throw OutOfRangeError();
		layers[ind].setActive();
	}
	catch (OutOfRangeError&e) {
		cout << e;
	}
}
void Image::inactivateLayer(int ind) {
	try {
		if (ind >= numOfLayers) throw OutOfRangeError();
		layers[ind].setInactive();
	}
	catch (OutOfRangeError&e) {
		cout << e;
	}
}

void Image::SelectForChange(int ind) {
	try {
		if (ind >= numOfLayers) throw OutOfRangeError();
		layers[ind].setChangeable();
	}
	catch (OutOfRangeError&e) {
		cout << e;
	}
}
void Image::UnselectForChange(int ind) {
	try {
		if (ind >= numOfLayers) throw OutOfRangeError();
		layers[ind].resetChangeable();
	}
	catch (OutOfRangeError&e) {
		cout << e;
	}
}
void Image::setLayerOpacity(int value, int ind) {
	try {
		if (ind >= numOfLayers ) throw OutOfRangeError();
		if (value > 100 || value < 0) throw ErrorArguments();
		layers[ind].opacity = value;
	}
	catch (OutOfRangeError&e) {
		cout << e;
	}
	catch (ErrorArguments&e) {
		cout << e;
	}
}


void Image::loadImage() {
	for (int i = 0; i < height*width; i++) {//kroz sve piksele
		int sum = 0;
		auto last = layers.begin();
		for (auto j = layers.begin(); j != layers.end(); j++) {
			Layer& cur = *j;
			if (cur.active) {
				last = j;
				sum += cur.getOpacity()*cur[i]['a']/100;
				if (sum >= 255) {
					sum = 255;
					j++;
					break;
				}
			}
		}
		if (sum == 0) {
			PIX p;
			image[i] = p;
		}
		else {
			image[i] = (*last).pixels[i];
			if (last != layers.begin()) {
				for (last--; last != layers.begin(); last--) {
					Layer& cur = *last;
					if (cur.active) {
						image[i](cur.pixels[i], cur.opacity);
					}
				}
				Layer& cur = *last;
				if (cur.active) {
					image[i](cur.pixels[i], cur.opacity);
				}
			}
			image[i].pixel[3] = sum;
		}
	}
}

void Image::fillSelection(string name, unsigned int r, unsigned int g, unsigned int b) {
	try {
		if (r > 255 || g > 255 || b > 255) throw ErrorArguments();
		if (selections.find(name) == selections.end()) throw NoSelectionNameError();
		Selection& s = *(selections[name]);
		for (Layer& l : layers) {
			if (l.changeable) {
				for (int i = 0; i < height*width; i++)
					if (s.isInActiveSelection(i%width, i / width)) {
						l[i] = PIX(r, g, b, 255);
					}
			}
		}
	}
	catch (NoSelectionNameError&e) {
		cout << e;
	}
	catch (ErrorArguments&e) {
		cout << e;
	}
}

void Image:: addSelection(Selection*s) {
	try {
		if (selections.find(s->getName())!=selections.end()) throw NotValidSelectionNameError();
		if (!s->isValid(this)) throw NotValidSelectionError();
		pair<string, Selection*> p(s->getName(), s);
		selections.insert(p);
	}
	catch (NotValidSelectionError&e) {
		cout << e;
	}
}

void Image::activateSelection(string name) {
	try {
		if (selections.find(name) == selections.end()) throw NoSelectionNameError();
		selections[name]->activate();
		numOfActiveSelection++;
	}
	catch (NotValidSelectionError&e) {
		cout << e;
	}
}

void Image::inactivateSelection(string name) {
	try {
		if (selections.find(name) == selections.end()) throw NoSelectionNameError();
		selections[name]->inactivate();
		numOfActiveSelection--;
	}
	catch (NotValidSelectionError&e) {
		cout << e;
	}
}

void Image::deleteSelection(string n) {
	try {
		if (selections.find(n) == selections.end()) throw NoSelectionNameError();
		if (selections[n]->isActive()) numOfActiveSelection--;
		selections.erase(n);
	}
	catch (NotValidSelectionError&e) {
		cout << e;
	}
}

void Image::execute(istream& in) {
	string name;
	in >> name;
	OperationMap& op = *operations;
	try {
		if (!op.exists(name)) throw OperationNotExists();
		cout << "executing..." << endl;
		bool* inSelection = new bool[width*height];
		for (int i = 0; i < width*height; i++) { //proverava za svaki piksel da li ga treba menjati 
			//ovo je optimalnije jer je ova provera skupa (prolazi kroz sve selekcije), a nije potrebno ponavljati to za svaki layer
			if (numOfActiveSelection == 0 ||
				(any_of(selections.begin(), selections.end(), [i, this](pair<string, Selection*> p) {
				return p.second->isInActiveSelection(i%getW(), i / getW());

			}))) inSelection[i] = true;
			else inSelection[i] = false;
		}
		//proveri da li je operacija validna??? pre svega
		Operation&o = *op[name];
		o.getArguments(in);
		for (Layer& l : layers) {
			if (l.changeable) {
				o(inSelection, l, width);
			}
		}
	}
	catch (NotValidOperationError&e) {
		cout << e;
		throw;
	}
	catch (InputError&e) {
		cout << e;
		throw;
	}
	catch (ErrorZero&e) {
		cout << e;
		throw;
	}
}

void Image::exportAll(string directoryName) {
	XML file(directoryName, this);
	file.exportXML();
}

void Image::loadLayer(int id) {
	if (id >= layers.size()) {
		cout << "layer sa tim id ne postoji" << endl;
		return;
	}
	for (int i = 0; i < width*height; i++) {
		image[i] = layers[id][i];
	}
}

Image*Image::getProgramContext(string path) {
	Image* i = new Image(1, 1);
	try {
		XML xml(path, i);
		xml.parseXML();
		i->deleteLayer(0);
	}
	catch (Errors&f) {
		cout << f;
	}
	return i;
}
