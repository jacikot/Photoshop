#pragma once
#include "Operation.h"
#include "PIX.h"
#include<cmath>
#include<iostream>
#include "rapidxml.hpp"
#include"rapidxml_print.hpp"
#include<algorithm>
#include"Errors.h"
using namespace rapidxml;


class BasicOperation :public Operation {
public:
	BasicOperation(){}
	virtual void getArguments(istream& in) override{};
	void compositeExecute(bool*b, Layer&l) override;
	virtual bool isComposite()const {
		return false;
	};
	
protected:
	virtual void doOperation(PIX&) = 0;
};

class WithArguments : public BasicOperation {
public:
	virtual void getArguments(istream& in) override {
		operand = CheckErrors::signedIntFromStream(in);
	};
protected:
	virtual void exportArguments(xml_node<> *function, xml_document<>* doc);
	void parseOperation(xml_node<> *root, xml_document<>* doc, string name) override {
		operand = atoi((root->first_node("argument"))->value());
	};
};

class Add :public WithArguments {
public:
	virtual Add*getNewEmptyInstance()override {
		return new Add();
	}
	string getname() const override {
		return "add";
	}
private:
	void doOperation(PIX&p)override {
		for (int i = 0; i < 3; i++) {
			p.pixel[i] += operand;
		}
	}
	virtual void write(ostream&out) const {
		out << "add(int)";
	}
	
};

class Sub :public WithArguments {
public:
	virtual Sub*getNewEmptyInstance()override {
		return new Sub();
	}
	string getname() const override {
		return "sub";
	}
private:
	void doOperation(PIX&p)override {
		for (int i = 0; i < 3; i++) {
			p.pixel[i] -= operand;
		}
	}
	virtual void write(ostream&out) const {
		out << "sub(int)";
	}
};

class InSub :public WithArguments {
public:
	virtual InSub*getNewEmptyInstance()override {
		return new InSub();
	}
	virtual string getname() const{
		return "insub";
	}
private:
	void doOperation(PIX&p)override {
		for (int i = 0; i < 3; i++) {
			p.pixel[i] = operand-p.pixel[i];
		}
	}
	virtual void write(ostream&out) const {
		out << "insub(int)";
	}
};

class Mul :public WithArguments {
public:
	virtual Mul*getNewEmptyInstance()override {
		return new Mul();
	}
	virtual string getname() const {
		return "mul";
	}
private:
	void doOperation(PIX&p)override {
		for (int i = 0; i < 3; i++) {
			p.pixel[i] *= operand;
		}
	}
	virtual void write(ostream&out) const {
		out << "mul(int)";
	}
};

class Div :public WithArguments {
public:
	virtual Div*getNewEmptyInstance()override {
		return new Div();
	}
	virtual void getArguments(istream& in) override {
		in >> operand;
		if (operand == 0) throw ErrorZero();
	};
	virtual string getname() const {
		return "div";
	}
private:
	void doOperation(PIX&p)override {
		for (int i = 0; i < 3; i++) {
			p.pixel[i] /= operand;
		}
	}
	virtual void write(ostream&out) const {
		out << "div(int)";
	}
};

class InDiv :public WithArguments {
public:
	virtual InDiv*getNewEmptyInstance()override {
		return new InDiv();
	}
	virtual string getname() const {
		return "indiv";
	}
private:
	void doOperation(PIX&p)override {
		for (int i = 0; i < 3; i++) {
			if (p.pixel[i] == 0)p.pixel[i] = 1;
			p.pixel[i] = operand/p.pixel[i];
		}
	}
	virtual void write(ostream&out) const {
		out << "indiv(int)";
	}
};

class Pow :public WithArguments {
public:
	virtual Pow*getNewEmptyInstance()override {
		return new Pow();
	}
	virtual string getname() const {
		return "pow";
	}
private:
	void doOperation(PIX&p)override {
		for (int i = 0; i < 3; i++) {
			p.pixel[i] = pow(p.pixel[i], operand);
		}
	}
	virtual void write(ostream&out) const {
		out << "pow(int)";
	}
};
class Min :public WithArguments {
public:
	virtual Min*getNewEmptyInstance()override {
		return new Min();
	}
	virtual string getname() const {
		return "min";
	}
private:
	void doOperation(PIX&p)override {
		for (int i = 0; i < 3; i++) {
			p.pixel[i] = min(p.pixel[i], operand);
		}
	}
	virtual void write(ostream&out) const {
		out << "min(int)";
	}
};

class Max :public WithArguments {
public:
	virtual Max*getNewEmptyInstance()override {
		return new Max();
	}
	virtual string getname() const {
		return "max";
	}
private:
	void doOperation(PIX&p)override {
		for (int i = 0; i < 3; i++) {
			p.pixel[i] = max(p.pixel[i], operand);
		}
	}
	virtual void write(ostream&out) const {
		out << "max(int)";
	}
};

class Log :public BasicOperation {
public:
	virtual Log*getNewEmptyInstance()override {
		return new Log();
	}
	virtual string getname() const {
		return "log";
	}
private:
	void doOperation(PIX&p)override {
		for (int i = 0; i < 3; i++) {
			if (p.pixel[i] == 0)p.pixel[i] = 1;
			p.pixel[i] = (int)log10(p.pixel[i]);
		}
	}
	virtual void write(ostream&out) const {
		out << "log()";
	}
};

class Abs :public BasicOperation {
public:
	virtual Abs*getNewEmptyInstance()override {
		return new Abs();
	}
	virtual string getname() const {
		return "abs";
	}
private:
	void doOperation(PIX&p)override {
		for (int i = 0; i < 3; i++) {
			p.pixel[i] = abs(p.pixel[i]);
		}
	}
	virtual void write(ostream&out) const {
		out << "abs()";
	}
};