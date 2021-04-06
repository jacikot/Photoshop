#include<regex>
#include"Image.h"
#include"XML.h"
#include"MyMain.h"
#include"Rectangle.h"
#include<vector>
#include"CompositeFunction.h"
#include<sstream>
#include"Errors.h"
#include<string>
using namespace std;

Main* Main::myMain=nullptr;
void Main::startProgram(int argc, char* argv[]) {
	try {
		if (argc < 3) throw ErrorCommandLineArguments();
		string imagePath = argv[1];
		string functionPath = argv[2];
		Image ix(imagePath);
		ix.activateLayer(0);
		ix.SelectForChange(0);
		XML xml(&ix);
		XML::parseFunction(functionPath);
		regex r("(.*\\\\)*([^\\\.]*)\.fun");
		smatch res;
		string dir;
		string name;
		if (regex_match(functionPath, res, r)) {
			dir = res.str(1);
			name = res.str(2);
			cout << name << endl;
		}
		else throw FileNameFormatError();
		stringstream ss;
		ss << name;
		ix.execute(ss); 
		ix.loadImage();
		ix.saveImage(imagePath);
	}
	catch (ErrorCommandLineArguments e) {
		cout << e;
	}
	catch (Errors&e) {
		cout << e;
	}
	catch (...) {

	}

}

Main::Main():isSaved(false),isEnd(false),isLoaded(false) {
	//ucitavanje slike zadatog formata
	mainMap[10] = [this]() {
		if (i != nullptr) {
			delete i; i = nullptr;
		}
		cout << "Unesite putanju slike u pam ili bmp formatu" << endl;
		string path;
		cin >> path;
		this->setImage(path);
		this->setIsSaved(false);
		this->setIsLoaded(true);
	};
	//stvaranje prazne slike
	mainMap[11] = [this]() {
		if (i != nullptr) {
			delete i; i = nullptr;
		}
		try {
			cout << "Unesite velicinu slike" << endl;
			cout << "sirina(u pikselima):";
			int width, height;
			width = CheckErrors::intFromStream(cin);
			cout << "visina(u pikselima):";
			height = CheckErrors::intFromStream(cin);
			this->setImage(width, height);
			this->setIsSaved(false);
			this->setIsLoaded(true);
		}
		catch (InputError& e) {
			cout << e;
		}
	};
	//ucitavanje konteksta programa (xml)
	mainMap[12] = [this]() {
		if (i != nullptr) {
			delete i; i = nullptr;
		}
		cout << "Unesite putanju sacuvanog konteksta u xml formatu" << endl;
		string path;
		cin >> path;
		this->setImage(Image::getProgramContext(path));
		this->setIsSaved(false);
		this->setIsLoaded(true);
	};
	//dodavanje layera
	mainMap[20] = [this]() {
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite putanju do slike u bmp ili pam formatu" << endl;
			string path;
			cin >> path;
			this->getImage()->addLayer(path);
			this->setIsSaved(false);
		}
		catch (Errors&e) {
			cout << e;
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//dodavanje praznog layera
	mainMap[21] = [this]() {
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite velicinu sloja" << endl;
			cout << "sirina(u pikselima):";
			int width, height;
			width = CheckErrors::intFromStream(cin);
			cout << "visina(u pikselima):";
			height = CheckErrors::intFromStream(cin);
			this->getImage()->addLayer(width, height);
			this->setIsSaved(false);
		}
		catch (InputError&e) {
			cout << e;
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//ucitavanje tekuce slike kao layera (dodatna funkcionalnost, stedi prostor ukoliko se radi sa vise layera)
	//nije potrebno drzati sve layere
	//one sa kojima smo zavrsili obradu mozemo da spojimo u sliku i obrisemo, a zatim sliku sacuvamo kao poseban layer
	//operacije sa layerima su skupe (trose mnogo procesorskog vremena) i zato je dobro imati ustedu u tom smislu
	mainMap[22] = [this]() {
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			this->getImage()->loadImage();
			this->getImage()->saveAsLayer();
			this->setIsSaved(false);
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//brisanje layera
	mainMap[23] = [this]() {
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite redni broj sloja koji brisete" << endl <<
				"Unesite -1 ako zelite da obrisete sve postojece slojeve" << endl;
			int i;
			i = CheckErrors::signedIntFromStream(cin);
			if (i < 0) this->getImage()->deleteAll();
			else this->getImage()->deleteLayer(i);
			this->setIsSaved(false);
		}
		catch (InputError&e) {
			cout << e;
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//postavljanje vidljivosti sloja na krajnjoj slici
	mainMap[24] = [this]() {
		
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite redni bloj layera koji zelite da ucinite vidljivim na slici" << endl;
			int i = CheckErrors::intFromStream(cin);
			this->getImage()->activateLayer(i);
			this->setIsSaved(false);
		}
		catch (InputError&e) {
			cout << e;
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//postavljanje nevidljivosti sloja
	mainMap[25] = [this]() {
		
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite redni bloj layera koji ne zelite da ucinite vidljivim na slici" << endl;
			int i = CheckErrors::intFromStream(cin);
			this->getImage()->inactivateLayer(i);
			this->setIsSaved(false);
		}
		catch (InputError&e) {
			cout << e;
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//aktivacija sloja za promene na njemu
	mainMap[26] = [this]() {
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite redni bloj layera koji zelite da menjate" << endl;
			int i = CheckErrors::intFromStream(cin);
			this->getImage()->SelectForChange(i);
			this->setIsSaved(false);
		}
		catch (InputError&e) {
			cout << e;
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//deaktivacija za promene
	mainMap[27] = [this]() {
		
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite redni bloj layera koji ne zelite da menjate" << endl;
			int i = CheckErrors::intFromStream(cin);
			this->getImage()->UnselectForChange(i);
			this->setIsSaved(false);
		}
		catch (InputError&e) {
			cout << e;
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//postavljanje providnosti sloja
	mainMap[28] = [this]() {
		
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite redni bloj layera i providnost koju zelite(0->providno:100->neprovidno)" << endl
				<< "Ako ste zadovoljni podrazumevanom providnoscu (100) unesite 'dalje'" << endl;
			int i, o;
			string odg;
			i = CheckErrors::intFromStream(cin);
			cin >> odg;
			if (odg == "dalje") o = 100;
			else {
				stringstream s;
				s << odg;
				o = CheckErrors::intFromStream(s);
			}
			this->getImage()->setLayerOpacity(o, i);
			this->setIsSaved(false);
		}
		catch (InputError&e) {
			cout << e;
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//dodavanje selekcije
	mainMap[30] = [this]() {
		try{
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite ime selekcije" << endl;
			string name;
			cin >> name;
			getchar();
			vector<rectangle*> rects;
			cout << "Unesite pravougaonike koji cine selekciju po obrascu(x,y,w,h)" << endl;
			cout << "x i y->koordinate gornjeg levog ugla, w - sirina, h-visina" << endl;
			cout << "Za kraj unesite sve 0" << endl;
			int count = 0;
			while (true) {
				int x, y, w, h;
				string s;
				getline(cin,s);
				stringstream ss;
				ss << s;
				x = CheckErrors::intFromStream(ss);
				y = CheckErrors::intFromStream(ss);
				w = CheckErrors::intFromStream(ss);
				h = CheckErrors::intFromStream(ss);
				if (!x && !y && !w && !h) break;
				count++;
				rects.push_back(new rectangle(x, y, w, h));

			}
			if (count) this->getImage()->addSelection(new Selection(name, rects));
			else cout << "Selekcija nije dodata jer je prazna" << endl;
		}
		catch (InputError&e) {
			cout << e;
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//aktiviranje selekcije
	mainMap[31] = [this]() {
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite ime selekcije koju zelite da aktivirate" << endl;
			string s;
			cin >> s;
			this->getImage()->activateSelection(s);
			this->setIsSaved(false);
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//deaktiviranje selekcije
	mainMap[32] = [this]() {
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite ime selekcije koju zelite da deaktivirate" << endl;
			string s;
			cin >> s;
			this->getImage()->inactivateSelection(s);
			this->setIsSaved(false);
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//popunjavanje selekcije bojom
	mainMap[33] = [this]() {
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite ime selekcije koju zelite da obojite" << endl;
			string s;
			cin >> s;
			getchar();
			cout << "Unseite RGB boje kojom zelite da obojite selekciju (u formatu R G B)" << endl;
			cout << "Ako zelite podrazumevano crnu unesite 'dalje'" << endl;
			string rgb;
			getline(cin, rgb);
			stringstream ss;
			ss << rgb;
			if (rgb == "dalje") {
				this->getImage()->fillSelection(s);
			}
			else {
				try {
					int r, g, b;
					r = CheckErrors::intFromStream(ss);
					g = CheckErrors::intFromStream(ss);
					b = CheckErrors::intFromStream(ss);
					this->getImage()->fillSelection(s, r, g, b);
				}
				catch (InputError&e) {
					cout << e;
				}
			}
			this->setIsSaved(false);
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//brisanje selekcije
	mainMap[34] = [this]() {
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite ime selekcije koju zelite da obrisete" << endl;
			string s;
			cin >> s;
			this->getImage()->deleteSelection(s);
			this->setIsSaved(false);
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//izvrsavanje operacija
	mainMap[40] = [this]() {
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite ime operacije i potrebne argumente" << endl;
			this->getImage()->execute(cin);
			this->setIsSaved(false);
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
		catch(...){}
	};
	//formiranje kompozitne operacije
	mainMap[41] = [this]() {
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite ime kompozitne operacije koju zelite da formirate" << endl;
			CompositeFunction::makeCompositeFunction(cin);
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//ucitavanje kompozitne funkcije iz xml
	mainMap[42] = [this]() {
		
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite putanju funkcije koju zelite da ucitate (ekstenzija .fun)" << endl;
			string path;
			cin >> path;
			XML::parseFunction(path);
		}
		catch (Errors&e) {
			cout << e;
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//cuvanje kompozitne funkcije u xml formatu
	mainMap[43] = [this]() {
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite putanju direktorijuma u koji zelite da sacuvate svoju operaciju";
			string path;
			cin >> path;
			cout << "Unesite ime operacije koju zelite da sacuvate" << endl;
			string op;
			cin >> op;
			try {
				if (!this->getImage()->getOperations()->exists(op)) throw OperationNotExists();
				Operation* o = (*this->getImage()->getOperations())[op];
				if (o->isComposite()) XML::exportFunction(*(*this->getImage()->getOperations())[op], path);
				else throw SaveBasicOperationError();
			}
			catch (NotValidOperationError& e) {
				cout << e;
			}
			catch (Errors&e) {
				cout << e;
			}
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	mainMap[44] = [this]() {
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << (*this->getImage()->getOperations());
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//eksportovanje slike u bmp ili pam formatu
	mainMap[50]=[this](){
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			this->getImage()->loadImage();
			cout << "Unesite putanju do fajla u kome zelite da sacuvate sliku (ekstenzija bmp ili pam)" << endl;
			string path;
			cin >> path;
			CheckErrors::saveToExistingFile(path);
			this->getImage()->saveImage(path);
			this->setIsSaved(true);
		}
		catch (Errors&e) {
			cout << e;
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//eksportovanje celog konteksta u xml formatu
	mainMap[51] = [this]() {
		try {
			if (!this->getIsLoaded()) throw NotLoadedImageError();
			cout << "Unesite putanju do fajla u kome zelite da sacuvate kontekst programa (xml)" << endl;
			string path;
			cin >> path;
			CheckErrors::saveToExistingFile(path);
			this->getImage()->exportAll(path);
			this->setIsSaved(true);
		}
		catch (Errors&e) {
			cout << e;
		}
		catch (NotLoadedImageError&e) {
			cout << e;
		}
	};
	//kraj
	mainMap[6] = [this]() {
		if (!this->getIsSaved()) {
			cout << "Da li zelite da sacuvate sliku?(unesite da ili ne)" << endl;
			string odg;
			cin >> odg;
			if (odg == "da") mainMap[50]();
			delete i;
		}
		this->setEnd();
	};
	mainMap[7] = [this]() {
	};
	mainMap[1] = [this]() {
		cout << "Unesite redni broj podstavke:" << endl;
		cout << "........................................................." << endl;
		cout << "	10. Ucitavanje slike zadatog formata" << endl;
		cout << "	11. Stvaranje prazne slike" << endl;
		cout << "	12. Ucitavanje konteksta programa" << endl;
		cout << "........................................................." << endl;
		cout << "	7. Nazad na glavni meni" << endl;
		try {
			int ind = CheckErrors::intFromStream(cin);
			if ((ind < 10 || ind>12) && ind != 7) throw InputError();
			mainMap[ind]();
		}
		catch (InputError&e) {
			cout << e;
		};
	};
	mainMap[2] = [this]() {
		cout << "Unesite redni broj podstavke:" << endl;
		cout << "........................................................." << endl;
		cout << "	20. Dodavanje sloja sa zadatom slikom" << endl;
		cout << "	21. Dodavanje praznog sloja" << endl;
		cout << "	22. Postavljanje sloja tekuce slike" << endl;
		cout << "	23. Brisanje sloja" << endl;
		cout << "	24. Postavljanje vidljivosti sloja u konacnoj slici" << endl;
		cout << "	25. Uklanjanje vidljivosti sloja u konacnoj slici" << endl;
		cout << "	26. Aktivacija sloja" << endl;
		cout << "	27. Deaktivacija sloja" << endl;
		cout << "	28. Postavljanje providnosti sloja" << endl;
		cout << "........................................................." << endl;
		cout << "	7. Nazad na glavni meni" << endl;
		try {
			int ind = CheckErrors::intFromStream(cin);
			if ((ind < 20 || ind>28) && ind != 7) throw InputError();
			mainMap[ind]();
		}
		catch (InputError&e) {
			cout << e;
		};
	};
	mainMap[3] = [this]() {
		cout << "Unesite redni broj podstavke:" << endl;
		cout << "........................................................." << endl;
		cout << "	30. Dodavanje selekcije" << endl;
		cout << "	31. Aktiviranje selekcije" << endl;
		cout << "	32. Deaktiviranje selekcije" << endl;
		cout << "	33. Popunjavanje selekcije bojom" << endl;
		cout << "	34. Brisanje selekcije" << endl;
		cout << "........................................................." << endl;
		cout << "	7. Nazad na glavni meni" << endl;
		try {
			int ind = CheckErrors::intFromStream(cin);
			if ((ind < 30 || ind>34) && ind != 7) throw InputError();
			mainMap[ind]();
		}
		catch (InputError&e) {
			cout << e;
		};
	};
	mainMap[4] = [this]() {
		cout << "Unesite redni broj podstavke:" << endl;
		cout << "........................................................." << endl;
		cout << "	40. Izvrsavanje operacije" << endl;
		cout << "	41. Formiranje kompozitne operacije" << endl;
		cout << "	42. Ucitavanje kompozitne operacije" << endl;
		cout << "	43. Cuvanje kompozitne operacije" << endl;
		cout << "	44. Prikaz svih operacija" << endl;
		cout << "........................................................." << endl;
		cout << "	7. Nazad na glavni meni" << endl;
		try {
			int ind = CheckErrors::intFromStream(cin);
			if ((ind < 40 || ind>44) && ind != 7) throw InputError();
			mainMap[ind]();
		}
		catch (InputError&e) {
			cout << e;
		};
	};
	mainMap[5] = [this]() {
		cout << "Unesite redni broj podstavke:" << endl;
		cout << "........................................................." << endl;
		cout << "	50. Eksportovanje slike" << endl;
		cout << "	51. Snimanje projekta" << endl;
		cout << "........................................................." << endl;
		cout << "	7. Nazad na glavni meni" << endl;
		try {
			int ind = CheckErrors::intFromStream(cin);
			if ((ind < 50 || ind>51) && ind != 7) throw InputError();
			mainMap[ind]();
		}
		catch (InputError&e) {
			cout << e;
		};
	};
	mainMap[0] = [this]() {
		cout << "Unesite redni broj stavke:" << endl;
		cout << "........................................................." << endl;
		cout << "1. Ucitavanje" << endl;
		cout << "	10. Ucitavanje slike zadatog formata" << endl;
		cout << "	11. Stvaranje prazne slike" << endl;
		cout << "	12. Ucitavanje konteksta programa" << endl;
		cout << "........................................................." << endl;
		cout << "2. Rad sa slojevima" << endl;
		cout << "	20. Dodavanje sloja sa zadatom slikom" << endl;
		cout << "	21. Dodavanje praznog sloja" << endl;
		cout << "	22. Postavljanje sloja tekuce slike" << endl;
		cout << "	23. Brisanje sloja" << endl;
		cout << "	24. Postavljanje vidljivosti sloja u konacnoj slici" << endl;
		cout << "	25. Uklanjanje vidljivosti sloja u konacnoj slici" << endl;
		cout << "	26. Aktivacija sloja" << endl;
		cout << "	27. Deaktivacija sloja" << endl;
		cout << "	28. Postavljanje providnosti sloja" << endl;
		cout << "........................................................." << endl;
		cout << "3. Rad sa selekcijama" << endl;
		cout << "	30. Dodavanje selekcije" << endl;
		cout << "	31. Aktiviranje selekcije" << endl;
		cout << "	32. Deaktiviranje selekcije" << endl;
		cout << "	33. Popunjavanje selekcije bojom" << endl;
		cout << "	34. Brisanje selekcije" << endl;
		cout << "........................................................." << endl;
		cout << "4. Rad sa operacijama nad slikom" << endl;
		cout << "	40. Izvrsavanje operacije" << endl;
		cout << "	41. Formiranje kompozitne operacije" << endl;
		cout << "	42. Ucitavanje kompozitne operacije" << endl;
		cout << "	43. Cuvanje kompozitne operacije" << endl;
		cout << "	44. Prikaz svih operacija" << endl;
		cout << "........................................................." << endl;
		cout << "5. Eksportovanje i cuvanje" << endl;
		cout << "	50. Eksportovanje slike" << endl;
		cout << "	51. Snimanje projekta" << endl;
		cout << "........................................................." << endl;
		cout << "6. Kraj" << endl;
		try {
			int ind = CheckErrors::intFromStream(cin);
			mainMap[ind]();
		}
		catch (InputError&e) {
			cout << e;
		};
	};

}

void Main::startProgram() {
	while (!isEnd) {
		cout << "Unesite redni broj stavke:" << endl;
		cout << "........................................................." << endl;
		cout << "0. Prikaz celog menija" << endl;
		cout << "........................................................." << endl;
		cout << "1. Ucitavanje" << endl;
		cout << "........................................................." << endl;
		cout << "2. Rad sa slojevima" << endl;
		cout << "........................................................." << endl;
		cout << "3. Rad sa selekcijama" << endl;
		cout << "........................................................." << endl;
		cout << "4. Rad sa operacijama nad slikom" << endl;
		cout << "........................................................." << endl;
		cout << "5. Eksportovanje i cuvanje" << endl;
		cout << "........................................................." << endl;
		cout << "6. Kraj" << endl;
		try {
			int ind = CheckErrors::intFromStream(cin);
			if (ind < 0 || ind>6) throw InputError();
			mainMap[ind]();
		}
		catch (InputError&e) {
			cout << e;
		};
	}
}