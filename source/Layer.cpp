#include "Layer.h"
#include<algorithm>
#include <iterator>
using namespace std;
Layer::Layer(int width, int height, int o):h(height),w(width),opacity(o),active(0) {
	for (int i = 0; i < width*height; i++) {
		PIX p; //pravi providan piksel
		pixels.push_back(p);
	}
}

void Layer::resizeHeight(int newHeight) {
	for (int i = 0; i < (newHeight-h)*w; i++) {
		PIX p;
		pixels.push_back(p);
	}
	h = newHeight;
}

void Layer::resizeWidth(int newWidth) {
	pix p = this->pixels;
	this->pixels.clear();
	for (auto i = p.begin(); i != p.end();i+=w) {
		copy(i, i + w, back_inserter(this->pixels));
		for (int j = w; j < newWidth; j++) {
			PIX pr;
			this->pixels.push_back(pr);
		}
	}
	w = newWidth;
}

void Layer::fix() {
	for_each(pixels.begin(), pixels.end(), [](PIX& p) {
		p = p % 256;
	});
}