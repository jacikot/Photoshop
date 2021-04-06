#pragma once
#include"PIX.h"
#include<vector>
#include"Layer.h"
#include<iostream>
#include "rapidxml.hpp"
#include"rapidxml_print.hpp"
#include"OperationMap.h"

using namespace rapidxml;
extern class Image;



class Operation {
public:
	void operator()(bool*check,Layer&l,int w) {
		width = w;
		compositeExecute(check,l);
		l.fix();
	}	
	virtual Operation*getNewEmptyInstance()=0;
	virtual void getArguments(istream& in) = 0;
	virtual void compositeExecute(bool*, Layer&l) = 0;
	friend ostream&operator<<(ostream&out, const Operation&oper) {
		oper.write(out);
		return out;
	}
	virtual bool isComposite()const = 0;
	virtual void exportArguments(xml_node<> *root, xml_document<>* doc) {};
	virtual void exportOp(xml_node<> *root, xml_document<>* doc);
	virtual string getname()const =0;
	virtual ~Operation() {}
	void copyOperand(const Operation&o) { operand = o.operand; }
	static Operation* parse(xml_node<> *root, xml_document<>* doc, string name);
	
protected:
	static OperationMap* map;
	virtual void write(ostream&out) const = 0;
	bool argumentsSet = false;
	static int width;
	int operand = 0;
	virtual void parseOperation(xml_node<> *root, xml_document<>* doc, string name) {};
	
};

