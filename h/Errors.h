#pragma once
#include <iostream>
#include<algorithm>
#include<cctype>
#include<string>
#include<fstream>
using namespace std;
class Errors {
public:
	friend std::ostream & operator<<(std::ostream& out, const Errors& f) {
		f.write(out);
		return out;
	}
	//virtual ~Errors() {};
protected:
	virtual void write(ostream&o) const{};
};

class OpeningFileFail:public Errors {
private:
	virtual void write(ostream&out)const override {
		out << " Greska pri otvaranju fajla/direktorijuma \n";
		out << "Vas fajl/direktorijum ne postoji, proverite ime fajla/direktorijuma" << endl;
	}
};
class ReadingFail:public Errors {
private:
	virtual void write(ostream&out) const override {
		out << " Greska pri citanju fajla\n";
		out << "Problem u sadrzaju fajla" << endl;
	}
};

class FormatError:public Errors {
private:
	virtual void write(ostream&out)const override {
		out << " Greska u formatu fajla\n";
		out << "Problem u zaglavlju slike" << endl;
	}
};

class FileNameFormatError:public Errors {
private:
	virtual void write(ostream&out)const override {
		out << " Greska u formatu imena fajla\n";
		out << "Proverite da li ste uneli ispravno ime fajla(ekstenzija .pam,.bmp,.xml,.fun)" << endl;
	}
};
class ExistingFileError :public Errors {
private:
	virtual void write(ostream&out)const override {}
};
class InputError:public Errors {
public:
	friend std::ostream & operator<<(std::ostream& out, const InputError& f) {
		out << "Greska u unosu podatka" << endl;
		return out;
	}
};

class CheckErrors {
public:
	static int intFromStream(istream&in) {
		int i;
		string s;
		in >> s;
		if (!CheckErrors::IsNumber(s)) throw InputError();
		return atoi(s.c_str());
	}
	static int signedIntFromStream(istream&in) {
		int i;
		string s;
		in >> s;
		if (!IsNumber(s)&&!IsNegativeNumber(s)) throw InputError();
		return atoi(s.c_str());
	}
	static void saveToExistingFile(string name) {
		if (testFileExists(name)) {
			cout << "Fajl u koji zelite da sacuvate vas sadrzaj vec postoji("<<name<<")" << endl;
			cout << "Sadrzaj koji cuvate ce izbrisati sadrzaj postojeceg fajla" << endl;
			cout << "Da li zelite da nastavite? (unesite da ili ne)" << endl;
			string odg;
			cin >> odg;
			if (odg == "ne") throw ExistingFileError();
		}
	}
	
private:
	static bool testFileExists(string name) {
		ifstream f(name.c_str());
		bool b = f.good();
		f.close();
		return b;
	}
	static bool IsNegativeNumber(const string& s) {
		return !s.empty() && std::find_if(s.begin() + 1,
			s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end() && s[0] == '-';
	}
	static bool IsNumber(const string& s) {
		return !s.empty() && std::find_if(s.begin(),
			s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
	}
};

class ErrorCommandLineArguments {
	friend ostream& operator<<(ostream&o, const ErrorCommandLineArguments e) {
		o << "Pogresno ste uneli argumente komandne linije" << endl;
		return o;
	}
};
class ErrorArguments {
	friend ostream& operator<<(ostream&o, const ErrorArguments& e) {
		o << "Pogresno ste uneli argumente" << endl;
		return o;

	}
};

class OutOfRangeError {
	friend ostream& operator<<(ostream&o, const OutOfRangeError& e) {
		o << "Id je van opsega" << endl;
		return o;
	}
};

class NotValidSelectionError {
	friend ostream& operator<<(ostream&o,const  NotValidSelectionError& e) {
		e.write(o);
		return o;
	}
protected:
	virtual void write(ostream&o)const {
		o << "Dimenzije nekog od pravougaonika iz selekcije su nevalidne" << endl;
	}
};
class NotValidSelectionNameError :public NotValidSelectionError {
private:
	void write(ostream&o)const override {
		o << "Postoji selekcija sa ovim imenom" << endl;
	}
};

class NoSelectionNameError :public NotValidSelectionError {
private:
	void write(ostream&o)const override {
		o << "Ne postoji selekcija se ovim imenom" << endl;
	}
};

class NotValidOperationError {
	friend ostream& operator<<(ostream&o, const  NotValidOperationError& e) {
		e.write(o);
		return o;
	}
protected:
	virtual void write(ostream&o) const = 0;
};

class OperationNotExists :public NotValidOperationError {
	virtual void write(ostream&o) const {
		o << "Ne postoji operacija sa tim imenom" << endl;
	}
};
class OperationExists :public NotValidOperationError {
	virtual void write(ostream&o) const {
		o << "Postoji operacija sa tim imenom" << endl;
	}
};
class SaveBasicOperationError :public NotValidOperationError {
	virtual void write(ostream&o) const {
		o << "Nije moguce sacuvati neku od osnovnih operacija" << endl;
	}
};
class ErrorZero {
public:
	friend ostream& operator<< (ostream& out, const ErrorZero&e) {
		out << "Nije dozvoljeno deljenje s nulom" << endl;
		return out;
	}
};

class NotLoadedImageError {
	friend ostream& operator<< (ostream& out, const NotLoadedImageError&e) {
		out << "Niste ucitali sliku" << endl;
		out << "Za ucitavanje slike izaberite stavku iz podmenija 1" << endl;
		return out;
	}
};