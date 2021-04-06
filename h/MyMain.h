#pragma once
#include <map>
#include<functional>
#include<iostream>
#include"Image.h"
#include"Errors.h"


using namespace std;


class Main {
public:
	static Main*getInstance() {
		if (myMain == nullptr) myMain = new Main();
		myMain->isEnd = false;
		myMain->isSaved = false;
		myMain->isLoaded = false;
		myMain->i = nullptr;
		return myMain;
	}
	static void startProgram(int argc, char* argv[]);
	Image*getImage() {
		return i;
	}
	void startProgram();
	void setImage(string path) {
		try {
			if (i != nullptr) delete i;
			i = new Image(path);
		}
		catch (Errors&f) {
			cout << f;
		}
	}
	void setImage(int w, int h) {
		if (i != nullptr) delete i;
		i = new Image(w, h);
	}
	void setImage(Image*ii) {
		if (i != nullptr) delete i;
		i = ii;
	}
	bool getIsSaved() {
		return isSaved;
	}
	void setIsSaved(bool yes) {
		isSaved = yes;
	}
	void setEnd() {
		isEnd = true;
	}
	~Main() {
		delete i;
	}
	void setIsLoaded(bool flag) {
		isLoaded = flag;
	}
	bool getIsLoaded() {
		return isLoaded;
	}
	
private:
	Image*i=nullptr;
	static Main* myMain;
	map<int, function<void()>> mainMap;
	bool isSaved;
	bool isEnd;
	bool isLoaded = false;
	Main();
};
