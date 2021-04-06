#include"CompositeFunction.h"
#include<algorithm>
#include<iostream>
#include"rapidxml_print.hpp"
#include"OperationMap.h"
#include"Errors.h"
using namespace rapidxml;

CompositeFunction::CompositeFunction(const CompositeFunction& f) :Operation(f), name(f.name) {
	for (Operation* o : f.members) {
		Operation* newop = o->getNewEmptyInstance();
		newop->copyOperand(*o);
		members.push_back(newop);

	}
}

CompositeFunction::~CompositeFunction() {
	for_each(members.begin(), members.end(), [](Operation*o) {
		delete o;
	});
	members.clear();
	map = nullptr;
}

void CompositeFunction:: makeCompositeFunction(istream& in) {
	CompositeFunction* cf = new CompositeFunction();
	string name;
	in >> name;
	try {
		if (map->exists(name)) throw OperationExists();
		cf->name = name;
		cout << "Unesite funkcije sa argumentima(za kraj napisite kraj):" << endl;
		string inn;
		in >> inn;
		while (inn != "kraj") {
			if (!map->exists(inn)) throw OperationNotExists();
			Operation* op = (*cf->map)[inn];
			op->getArguments(in);
			cf->members.push_back(op);
			in >> inn;
		}
		cf->map->setNewOperation(name, cf);
	}
	catch (NotValidOperationError&e) {
		cout << e;
	}
	catch (InputError&e) {
		cout << e;
	}
	catch (ErrorZero&e) {
		cout << e;
	}

}

void CompositeFunction:: exportOp(xml_node<> *root, xml_document<>* doc){
	auto text = doc->allocate_string(getname().c_str(), strlen(getname().c_str()));
	root->append_attribute(doc->allocate_attribute("name", text));
	xml_node<> *members = doc->allocate_node(node_element, "members");
	for (Operation* o : this->members) {
		xml_node<> *function = doc->allocate_node(node_element, "function");
		o->exportOp(function, doc);
		members->append_node(function);
	}
	root->append_node(members);
}

void CompositeFunction::parseOperation(xml_node<> *root, xml_document<>* doc, string name) {
	if (!map->exists(name)) {
		this->name = name;
		xml_node<>* members = root->first_node("members");
		for (xml_node<>*function = members->first_node("function"); function; function = function->next_sibling()) {
			string name = function->first_attribute("name")->value();
			this->members.push_back(parse(function, doc, name));
		}
		this->map->setNewOperation(name, this);
	}
	else cout << "Funkcija sa imenom: " << name << " vec postoji i ne ucitava se ponovo" << endl;
}


void CompositeFunction::compositeExecute(bool*b, Layer&l) {
	for_each(members.begin(), members.end(), [b, &l](Operation*o) {
		o->compositeExecute(b, l);
	});

}

void CompositeFunction::write(ostream&out) const {
	out << name << "[ ";
	for (Operation*o : members) {
		out << (*o) << " ";
	}
	out << "]";
}