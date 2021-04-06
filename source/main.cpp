#include"Image.h"
#include<iostream>
#include"PIX.h"
#include"Rectangle.h"
#include"OperationMap.h"
#include "BasicOperations.h"
#include"CompositeFunction.h"
#include"XML.h"
#include<regex>
#include "MyMain.h"



void main(int argc, char* argv[]) {
	bool flag = true;
	Main*m = nullptr;
	while (flag) {
		cout << "Unesite nacin rada:" << endl;
		cout << "1. Izvrsavanje sa zadatim argumentima" << endl;
		cout << "2. Rad pomocu menija" << endl;
		cout << "0. Kraj rada" << endl;
		char i;
		cin >> i;
		switch (i) {
		case '0': flag = false;  break;
		case '1': Main::startProgram(argc, argv); break;
		case '2': m = Main::getInstance(); m->startProgram(); break;
		default: cout << "Birate nepostojecu stavku" << endl; break;
		}
	}
	system("pause");
}

