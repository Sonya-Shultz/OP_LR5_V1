#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

struct FileData{
	string lat; //широта
	string longitude; //долгота
	string type;	//тип
	string subtype; //подтип
	string name; //название
	string address; // адреса
};

class Rtree {
public:
	void ReadFromFile();
private:

	vector<FileData> dataV;
};

void Rtree::ReadFromFile() {
	ifstream fin("ukraine_poi.csv");
	if (!fin.is_open()) { cout << "Problem with file."; }
	else {
		float a = 0; 
		string str;
		FileData help;
		while (!fin.eof()) {
			getline(fin, str, '\n');
			int i = 0;
			while (str[i] != ';') {
				help.lat += str[i];
				i++;
			}i++;
			while (str[i] != ';') {
				help.longitude += str[i];
				i++;
			}i++;
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
			help.lat.clear(); help.longitude.clear(); help.type.clear(); help.subtype.clear(); help.name.clear(); help.address.clear();
		}
	}

	int size = dataV.size();
	for (int i = 0; i < size; i++) {
		cout << dataV[i].lat << " " << dataV[i].longitude<<endl;
	}
}


int main()
{
	Rtree tree;
	tree.ReadFromFile();

	return 0;
}