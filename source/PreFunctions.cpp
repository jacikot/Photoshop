#include"PreFunctions.h"
#include<algorithm>
#include <vector>
#include "PIX.h"

void Function::compositeExecute(bool*b, Layer&l) {
	for (int i = 0; i < l.getDim(); i++) {
		if (b[i]) {
			doOperation(l[i]);
		}
	}
}

void Med::compositeExecute(bool*b, Layer&l) {
	Layer temp = l;
	for (int i = 0; i < l.getDim(); i++) {
		if (b[i]) {
			execPixel(temp, l, i);
		}
	}
	l = temp;
}

void Med:: execPixel(Layer&temp, Layer&l, int j) {
	vector<int>median;
	PIX&p = l[j];
	for (int i = 0; i < 3; i++) {
		median.push_back(l[j].pixel[i]);
		//levo -nije u prvoj koloni
		if (j%width != 0) median.push_back(l[j - 1].pixel[i]);
		//gore -nije u prvom redu
		if (j / width != 0) median.push_back(l[j - width].pixel[i]);
		//desno-nije u posl koloni
		if (j%width != width - 1) median.push_back(l[j + 1].pixel[i]);
		//dole - nije u posl redu
		if (j / width != (l.getDim() / width - 1)) median.push_back(l[j + width].pixel[i]);
		sort(median.begin(), median.end(), [](int& a, int& b) {
			return b < a;
		});
		if (median.size() % 2) {
			//neparno
			temp[j].pixel[i] = median[median.size() / 2];
		}
		else {
			temp[j].pixel[i] = (median[median.size() / 2 - 1] + median[median.size() / 2]) / 2;
		}
		median.clear();
	}

}