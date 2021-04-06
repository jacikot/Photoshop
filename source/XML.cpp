#include "rapidxml.hpp"
#include"rapidxml_print.hpp"
#include"rapidxml_iterators.hpp"
#include"XML.h"
#include<regex>
#include<algorithm>
#include<string>
#include<iostream>
#include<fstream>
#include<Windows.h>
#include"Rectangle.h"
#include"Errors.h"
//#include <sstream>
using namespace std;
using namespace rapidxml;

XML::XML(string file, Image* im) :i(im){
	regex r(".*.xml");
	smatch res;
	if (!regex_match(file, res, r)) throw FileNameFormatError();
	path = file;
}

void XML::exportXML() { //regulisati da se ne brisu fajlovi ako postoji neki sa istim imenom
	regex r("(.*\\\\)*([^\\\.]*\.xml)");
	smatch res;
	string dir;
	string name;
	if (regex_match(path, res, r)) {
		dir = res.str(1);
		name = res.str(2);
	}
	else throw FileNameFormatError();
	ofstream file(path);
	if (!file) throw OpeningFileFail();
	xml_document<> doc;
	xml_node<>* decl = doc.allocate_node(node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);
	xml_node<> *root = doc.allocate_node(node_element, "image");

	string c = to_string(i->getW());
	const char* text = doc.allocate_string(c.c_str(), strlen(c.c_str()));
	root->append_attribute(doc.allocate_attribute("width", text));

	c = to_string(i->getH());
	text = doc.allocate_string(c.c_str(), strlen(c.c_str()));
	root->append_attribute(doc.allocate_attribute("height", text));

	xml_node<> *l = doc.allocate_node(node_element, "layers");
	int k = 0;
	try {
		string a = dir + "Layers";
		CreateDirectory(a.c_str(), NULL);
		for (Layer&lay : i->layers) {
			xml_node<> *l1 = doc.allocate_node(node_element, "layer");
			string c = to_string(lay.getActive());
			text = doc.allocate_string(c.c_str(), strlen(c.c_str()));
			l1->append_attribute(doc.allocate_attribute("active", text));
			c = to_string(lay.getChangeable());
			text = doc.allocate_string(c.c_str(), strlen(c.c_str()));
			l1->append_attribute(doc.allocate_attribute("changeable", text));

			c = to_string(lay.getOpacity());
			xml_node<> *opacity = doc.allocate_node(node_element, "opacity");
			text = doc.allocate_string(c.c_str(), strlen(c.c_str()));
			opacity->value(text);
			l1->append_node(opacity);

			i->loadLayer(k);
			k++;
			//string p = a + "\\" + to_string(k++) + ".bmp";
			string p = a + "\\" + lay.getName() + ".bmp";
			cout << p;
			//CheckErrors::saveToExistingFile(p);
			i->saveImage(p);
			xml_node<> *pathl = doc.allocate_node(node_element, "path");
			text = doc.allocate_string(p.c_str(), strlen(p.c_str()));
			pathl->value(text);
			l1->append_node(pathl);
			l->append_node(l1);
		}
		root->append_node(l);
		
		xml_node<> *selections = doc.allocate_node(node_element, "selections");
		for_each(i->selections.begin(), i->selections.end(), [&doc, &selections](pair<string, Selection*> p) {
			xml_node<> *selection = doc.allocate_node(node_element, "selection");
			string c = to_string(p.second->isActive());
			const char * text = doc.allocate_string(c.c_str(), strlen(c.c_str()));
			selection->append_attribute(doc.allocate_attribute("changeable", text));

			xml_node<> *name = doc.allocate_node(node_element, "name");
			text = doc.allocate_string(p.first.c_str(), strlen(p.first.c_str()));
			name->value(text);
			selection->append_node(name);

			xml_node<> *rectangles = doc.allocate_node(node_element, "rectangles");
			for (auto r : p.second->getRectangles()) {
				xml_node<> *rectangle = doc.allocate_node(node_element, "rectangle");
				xml_node<> *x = doc.allocate_node(node_element, "x");
				c = to_string(r->getX());
				text = doc.allocate_string(c.c_str(), strlen(c.c_str()));
				x->value(text);
				rectangle->append_node(x);

				xml_node<> *y = doc.allocate_node(node_element, "y");
				c = to_string(r->getY());
				text = doc.allocate_string(c.c_str(), strlen(c.c_str()));
				y->value(text);
				rectangle->append_node(y);

				xml_node<> *w = doc.allocate_node(node_element, "w");
				c = to_string(r->getW());
				text = doc.allocate_string(c.c_str(), strlen(c.c_str()));
				w->value(text);
				rectangle->append_node(w);

				xml_node<> *h = doc.allocate_node(node_element, "h");
				c = to_string(r->getH());
				text = doc.allocate_string(c.c_str(), strlen(c.c_str()));
				h->value(text);
				rectangle->append_node(h);
				rectangles->append_node(rectangle);
			}
			selection->append_node(rectangles);
			selections->append_node(selection);
		});
		root->append_node(selections);
		a = dir + "Functions";
		CreateDirectory(a.c_str(), NULL);
		xml_node<> *functions = doc.allocate_node(node_element, "functions");
		for (auto& o : i->operations->operations) {
			if (o.second->isComposite()) {
				xml_node<> *path = doc.allocate_node(node_element, "path");
				string ss = exportFunction(*o.second, a);
				auto text = doc.allocate_string(ss.c_str(), strlen(ss.c_str()));
				path->value(text);
				functions->append_node(path);
			}
		}
		root->append_node(functions);
		doc.append_node(root);

		file << doc;
	}
	catch (Errors&e) {
		cout << e;
		file.close();
		doc.clear();
		throw;
	}
	file.close();
	doc.clear();
}

string XML::exportFunction(Operation&o, string directory="") {
	xml_document<> doc;
	xml_node<>* decl = doc.allocate_node(node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);
	xml_node<> *function = doc.allocate_node(node_element, "function");
	o.exportOp(function, &doc);
	doc.append_node(function);
	string s="";
	if (directory != s) s = directory + "\\";
	s+=o.getname()+".fun";
	//CheckErrors::saveToExistingFile(s);
	ofstream file(s);
	if (!file) throw OpeningFileFail();
	file << doc;
	file.close();
	doc.clear();
	return s;
}

Image* XML::parseXML() {//izbaci pamcenje velicine layera
	xml_document<> doc;
	xml_node<> * root;
	ifstream file (path);
	
	if (!file) throw OpeningFileFail();
	vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);
	root = doc.first_node("image");
	//layers
	
	xml_node<> * layers = root->first_node("layers");
	for (xml_node<>* layer = layers->first_node("layer"); layer; layer = layer->next_sibling()) {
		int changeable = atoi(layer->first_attribute("changeable")->value());
		int active = atoi(layer->first_attribute("active")->value());
		int opacity = atoi(layer->first_node("opacity")->value());
		string p= layer->first_node("path")->value();
		try {
			if (i == nullptr) {
				i = new Image(p, opacity, active, changeable);
			}
			else i->addLayer(p, opacity, active, changeable);
		}
		catch (Errors&e) {
			cout << "Greska u .xml fajlu, fajl:" << p<< endl;
			file.close();
			doc.clear();
			throw;
		}
	}
	xml_node<>*selections = root->first_node("selections");
	for (xml_node<>* selection = selections->first_node("selection"); selection; selection = selection->next_sibling()) {
		int changeable = atoi(selection->first_attribute("changeable")->value());
		string name = selection->first_node("name")->value();
		vector<rectangle*>rects;
		xml_node<>*rectangles = selection->first_node("rectangles");
		for (xml_node<>* rectan = rectangles->first_node("rectangle"); rectan; rectan = rectan->next_sibling()) {
			int x = atoi(rectan->first_node("x")->value());
			int y= atoi(rectan->first_node("y")->value());
			int w= atoi(rectan->first_node("w")->value());
			int h= atoi(rectan->first_node("h")->value());
			rects.push_back(new rectangle(x, y, w, h));
		}
		i->addSelection(new Selection(name, rects));
		if(changeable) i->activateSelection(name);
		rects.clear();
	}
	xml_node<>*functions = root->first_node("functions");
	string p;
	try {
		for (xml_node<>*function = functions->first_node("path"); function; function = function->next_sibling()) {
			p = function->value();
			parseFunction(p);
		}
	}
	catch (Errors&e) {
		cout << "Greska u .xml fajlu, fajl:" << p << endl;
		file.close();
		doc.clear();
		throw;
	}
	file.close();
	doc.clear();
	return i;

}

void XML::parseFunction(string path) {
	xml_document<> doc;
	xml_node<> * root;
	regex r(".*\.fun");
	smatch s;
	if (!regex_match(path, s, r)) throw FileNameFormatError();
	ifstream file(path);
	if (!file) throw OpeningFileFail();
	vector<char> buffer((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);
	root = doc.first_node("function");
	string name = root->first_attribute("name")->value();
	Operation::parse(root, &doc, name);
	file.close();
	doc.clear();
	
}

