#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

float str_to_float(string help);

const int max_dot = 20;

struct FileData {
	float lat; //широта
	float longitude; //долгота
	string type;	//тип
	string subtype; //подтип
	string name; //название
	string address; // адреса
};

struct  Node
{
	vector<FileData> data;
	Node* one, * two, * three, * four;
	int count_dot;
	float slat, slon, hight, width; // slat, slon - ліва верхня координата по широті, по довготі; hight, wight - розмір клвтинки по довготі, штроті
};

class Rtree {
public:
	Rtree();
	void ReadFromFile();
private:
	float minlat;//40
	float minlon;//21
	float maxlat;//57
	float maxlon;//41
	Node* R_tree;
	vector<FileData> dataV;
	void firstNode();
};

void Rtree::firstNode()
{
	R_tree->slat = minlat;
	R_tree->slon = minlon;
	R_tree->hight = maxlon - minlon;
	R_tree->width = maxlat - minlat;
	R_tree->one = NULL;
	R_tree->two = NULL;
	R_tree->three = NULL;
	R_tree->four = NULL;
}

Rtree::Rtree() {
	minlat = 40;
	minlon = 21;
	maxlat = 57;
	maxlon = 41;
	R_tree = new Node;
	ReadFromFile();
	firstNode();
}

void Rtree::ReadFromFile() {
	ifstream fin("ukraine_poi.csv");
	if (!fin.is_open()) { cout << "Problem with file."; }
	else {
		float a = 0;
		string str, tmplat, tmplong;
		FileData help;
		while (!fin.eof()) {
			getline(fin, str, '\n');
			int i = 0;
			while (str[i] != ';') {
				tmplat += str[i];
				i++;
			}i++;
			help.lat = str_to_float(tmplat);
			while (str[i] != ';') {
				tmplong += str[i];
				i++;
			}i++;
			help.longitude = str_to_float(tmplong);
			while (str[i] != ';') {
				help.type += str[i];
				i++;
			}i++;
			while (str[i] != ';') {
				help.subtype += str[i];
				i++;
			}i++;
			while (str[i] != ';') {
				help.name += str[i];
				i++;
			}i++;
			while (str[i] != ';') {
				help.address += str[i];
				i++;
			}i++;
			dataV.push_back(help);
			tmplat.clear(); tmplong.clear(); help.type.clear(); help.subtype.clear(); help.name.clear(); help.address.clear();
		}
	}

	/*long int size = dataV.size();
	for (int i = 0; i < size; i++) {
		cout << dataV[i].lat << " " << dataV[i].longitude << endl;
	}*/
}


int main()
{
	Rtree tree;
	//tree.ReadFromFile();

	return 0;
}

float str_to_float(string help)
{
	float a = 0, numer = 10, tail = 1, power = 1;
	for (int i = 0; i < help.size(); i++)
	{
		if (help[i] != '.' && help[i] != ',') { a = a * numer + (static_cast<int8_t>(help[i]) - 48) * pow(tail, power); power++; }
		else { numer = 1; tail = 0.1; power = 1; }
	}
	return a;
}
