#pragma once
#include "Operation.h"
#include<cmath>

class Function : public Operation{
public:
	virtual void getArguments(istream& in) override {};
	void compositeExecute(bool*b, Layer&l) override;
	virtual bool isComposite()const override {
		return false;
	};
protected:
	virtual void doOperation(PIX&) = 0;
};

class Inv :public Function {
public:
	virtual Inv*getNewEmptyInstance()override {
		return new Inv();
	}
	virtual string getname() const {
		return "inv";
	}
private:
	void doOperation(PIX&p)override {
		for (int i = 0; i < 3; i++) {
			p.pixel[i] = 255-p.pixel[i];
		}
	}
	virtual void write(ostream&out) const{
		out << "inv()";
	}
};

class Gray :public Function {
public:
	virtual Gray*getNewEmptyInstance()override {
		return new Gray();
	}
private:
	void doOperation(PIX&p)override {
		int k = (p['r'] + p['g'] + p['b']) / 3;
		for (int i = 0; i < 3; i++) {
			p.pixel[i] = k;
		}
	}
	virtual string getname() const {
		return "gray";
	}
	virtual void write(ostream&out) const {
		out << "gray()";
	}
};

class BW :public Function {
public:
	virtual BW*getNewEmptyInstance()override {
		return new BW();
	}
	virtual string getname() const {
		return "bw";
	}
private:
	void doOperation(PIX&p)override {
		int k = (p['r'] + p['g'] + p['b']) / 3;
		for (int i = 0; i < 3; i++) {
			p.pixel[i] = (k<127)?0:255;
		}
	}
	virtual void write(ostream&out) const {
		out << "bw()";
	}
};

class Med :public Operation {
public:
	virtual Med*getNewEmptyInstance()override {
		return new Med();
	}
	virtual bool isComposite()const override {
		return false;
	};
	virtual void getArguments(istream& in) override{}
	void compositeExecute(bool*b, Layer&l) override;
	virtual string getname() const {
		return "med";
	}
	
private:
	void execPixel(Layer&temp, Layer&l, int j);
	virtual void write(ostream&out) const {
		out << "med()";
	}
};