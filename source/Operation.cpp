#include"Operation.h"
#include"CompositeFunction.h"
int Operation::width = 0;


OperationMap* Operation::map = OperationMap::getInstance();

void Operation::exportOp(xml_node<> *root, xml_document<>* doc){
	auto text = doc->allocate_string(getname().c_str(), strlen(getname().c_str()));
	root->append_attribute(doc->allocate_attribute("name", text));
	exportArguments(root, doc);
}

Operation* Operation:: parse(xml_node<> *root, xml_document<>* doc, string name) {
	Operation*o;
	if (map->exists(name)) o = (*map)[name];
	else o = new CompositeFunction();
	o->parseOperation(root, doc, name);
	return o;
}