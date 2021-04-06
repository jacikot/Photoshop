#pragma once
#include <vector>
#include"PIX.h"
using namespace std;
extern struct A;
typedef vector<PIX> pix;

class Layer {
public:
	Layer(const pix& p, int width, int height, int o = 100,bool a=false,bool c=false) :opacity(o),h(height),w(width), pixels(p), active(a),changeable(c) {
	
	}
	Layer(int width, int height, int o = 0);//prazna slika
	void setActive() {
		active = true;
		//ucitaj izmenjenu sliku
	}
	void setChangeable() {
		changeable = true;
	}
	void resetChangeable() {
		changeable = false;
	}
	void setInactive() {
		active = false;
	}
	friend class Image;
	~Layer() {
	}
	void setOpacity(int t) {
		opacity = t;
	}
	PIX& operator [](int i) {
		return pixels[i];
	}
	const PIX& operator [](int i) const {
		return pixels[i];
	}
	int getOpacity() const {
		return opacity;
	}
	int getDim() const {
		return pixels.size();
	}
	bool getActive() const {
		return active;
	}
	bool getChangeable() const {
		return changeable;
	}
	void resizeWidth(int newWidth);//prosiruje sirinu praznim pikselima tog layera
	void resizeHeight(int newHeight); //prosiruje visinu
	void fix();
private:
	pix pixels;
	int opacity;
	bool active; //ucestvuje u slici
	bool changeable; //aktivan u operacijama
	int h, w;
};