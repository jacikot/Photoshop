#pragma once
#include "Operation.h"
#include "rapidxml.hpp"
using namespace rapidxml;

//prebaci sve operacije u cpp fajlove
class CompositeFunction :public Operation {
public:
	CompositeFunction():name("") {}
	CompositeFunction(const CompositeFunction& f);
	~CompositeFunction();
	CompositeFunction*getNewEmptyInstance() {
		return new CompositeFunction(*this);
	}
	virtual void getArguments(istream& in) {}
	static void makeCompositeFunction(istream& in);
	bool isComposite()const override {
		return true;
	};
	void exportOp(xml_node<> *root, xml_document<>* doc)override;
	string getname() const override {
		return name;
	}

	void parseOperation(xml_node<> *root, xml_document<>* doc, string name) override;
private:
	string name; 
	vector<Operation*> members;
	void compositeExecute(bool*b, Layer&l) override;
	void write(ostream&out) const override;
};

