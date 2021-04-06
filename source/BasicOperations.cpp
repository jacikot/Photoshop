#include"BasicOperations.h"


void BasicOperation::compositeExecute(bool*b, Layer&l) {
	for (int i = 0; i < l.getDim(); i++) {
		if (b[i]) {
			doOperation(l[i]);
		}
	}
}

void WithArguments::exportArguments(xml_node<> *function, xml_document<>* doc) {
	xml_node<> *argument = doc->allocate_node(node_element, "argument");
	auto text = doc->allocate_string(to_string(operand).c_str(), strlen(to_string(operand).c_str()));
	argument->value(text);
	function->append_node(argument);
}