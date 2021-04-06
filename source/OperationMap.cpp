#include"OperationMap.h"
#include "BasicOperations.h"
#include"PreFunctions.h"
#include"Operation.h"
typedef pair<string, Operation*> elem;

OperationMap::OperationMap() {
	elem p("add", new Add());
	operations.insert(p);
	p=make_pair("sub", new Sub());
	operations.insert(p);
	p=make_pair("insub", new InSub());
	operations.insert(p);
	p=make_pair("mul", new Mul());
	operations.insert(p);
	p=make_pair("div", new Div());
	operations.insert(p);
	p=make_pair("indiv", new InDiv());
	operations.insert(p);
	p=make_pair("pow", new Pow());
	operations.insert(p);
	p=make_pair("min", new Min());
	operations.insert(p);
	p=make_pair("max", new Max());
	operations.insert(p);
	p = make_pair("log", new Log());
	operations.insert(p);
	p = make_pair("abs", new Abs());
	operations.insert(p);
	p = make_pair("inv", new Inv());
	operations.insert(p);
	p = make_pair("gray", new Gray());
	operations.insert(p);
	p = make_pair("bw", new BW());
	operations.insert(p);
	p = make_pair("med", new Med());
	operations.insert(p);
	//inicijalizuj mapu operacija
}

OperationMap* OperationMap::instance = nullptr;
OperationMap::~OperationMap() {
	for (auto elem : operations) {
		delete elem.second;
	}
}
Operation* OperationMap::operator[](string name) {//proveri da li postoji
	Operation* result = operations[name];
	if (result == nullptr) throw ErrorOperation();
	operations[name] = result->getNewEmptyInstance();
	return result;
}

ostream&operator<<(ostream&out, const OperationMap&om) {
	for (auto ops : om.operations) {
		cout << (*ops.second) << endl;
	}
	return out;
}