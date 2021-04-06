#pragma once
#include <vector>
#include "Layer.h"
#include"Formater.h"
#include <map>
#include "Selection.h"
#include "Operation.h"
#include"OperationMap.h"
#include"Errors.h"
using namespace std;
class Image {
public:
	Image(string file, int opacity = 100, bool active = false, bool changeable = false);
	Image(int width, int height);
	void saveImage(string filename);
	void addLayer(string filename,int opacity=100, bool active = false, bool changeable = false);
	void addLayer(int width, int height);
	void deleteLayer(int id);
	void deleteAll();
	int getW()const {
		return width;
	}
	int getH() const {
		return height;
	}
	void activateLayer(int ind);
	void inactivateLayer(int ind);
	void setLayerOpacity(int value, int ind);
	void SelectForChange(int ind);
	void UnselectForChange(int ind);
	void loadImage();//azurira sliku koju cine aktivni layeri
	void saveAsLayer();//ubacuje tekucu sliku kao layer (ako zelimo na izmenjenu sliku nesto da dodamo)
	void addSelection(Selection*s);
	void deleteSelection(string n);
	void fillSelection(string n,unsigned int r=0,unsigned int g=0,unsigned int b=0);
	bool NoSelections() {
		return selections.empty();
	}
	Selection& operator[](string s) {
		return *selections[s];
	}
	void activateSelection(string name);
	void inactivateSelection(string name);
	void execute(istream&in);
	void exportAll(string name);
	friend class XML;
	void loadLayer(int id);
	static Image*getProgramContext(string path);
	OperationMap* getOperations() {
		return operations;
	}
	~Image();
private:
	Image(){}
	vector<Layer> layers;
	vector<PIX>image;
	int width, height;
	Formater* makeformater(string filename) const;
	void resizeLayers(int h, int w);
	map<string , Selection*> selections;
	int numOfActiveSelection;
	OperationMap*operations;
	int numOfLayers = 1;
};