#pragma once
#include <string>
//#include "Operation.h"
#include<map>
#include<utility>
#include <iostream>
using namespace std;
class Operation;
class ErrorOperation {
public:
	friend ostream&operator<<(ostream&out, const ErrorOperation& fail) {
		out << "Operation name is not valid" << endl;
	}
};

class OperationMap { //wrapper fja, unikat
public:
	static OperationMap*getInstance() {
		if (!instance) instance=new OperationMap();
		return instance;
	}
	
	Operation* operator[](string name);
	bool exists(string name) {
		if (operations.find(name) == operations.end()) {
			cout << "ovde usao" << endl;
			return false;

		}
		return true;
	}
	~OperationMap();
	void setNewOperation(string name, Operation*composite) {
		pair<string, Operation*> p(name, composite);
		operations[name] = composite;
		if (operations[name] == nullptr) cout << "ne radi" << endl;
	}
	bool operationExists(string name) {
		return operations.find(name)!=operations.end();
	}
	void deleteOperation(string name) {
		operations.erase(name);
	}
	friend ostream&operator<<(ostream&o, const OperationMap&);
	friend class XML;
private:
	OperationMap();
	map<string, Operation*> operations;
	static OperationMap* instance;
};
