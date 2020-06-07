#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

float str_to_float(string help);

const int max_dot = 50;

const float R_e = 6371.008;

struct FileData {
	float lat = 0; //широта
	float longitude = 0; //долгота
	string type;	//тип
	string subtype; //подтип
	string name; //название
	string address; // адреса
};

struct  Node
{
	vector<FileData> data;
	Node* one, * two, * three, * four, * up;
	bool is_see;
	long int count_dot=0;
	float slat=0, slon=0, hight=0, width=0; // slat, slon - ліва верхня координата по широті, по довготі; hight, wight - розмір клвтинки по довготі, штроті
};

class Rtree {
public:
	Rtree(string s);
	void ReadFromFile();
	void find_around(float lat1, float lon1, float size, string type1, string subtype1);
private:
	float minlat;//40
	float minlon;//21
	float maxlat;//57
	float maxlon;//41
	string file_name;
	Node* R_tree;
	vector<FileData> dataV;
	void push_new_dot(Node** nod, Node** parent, FileData dot);
	void fill_tree();
	float distance(FileData dot1, FileData dot2);
	FileData dot_for_look(FileData start, float alpha, float size_step);
	vector<FileData> find_node(FileData dot, Node* nod);
	void show_data(FileData dot);
	string deterStr(string str, int& i);
	void ifFunc(Node** node, FileData dot);
};

void Rtree::show_data(FileData dot)
{
	cout << dot.lat << "; " << dot.longitude << "; " 
		<< dot.type << "; " << dot.subtype << "; " 
		<< dot.name << "; " << dot.address << ";" << endl;
}

Rtree::Rtree(string s) {
	minlat = 40;
	minlon = 21;
	maxlat = 57;
	maxlon = 41;
	file_name = s;
	R_tree = NULL;
	ReadFromFile();
	fill_tree();
}

string Rtree::deterStr(string str, int& i) {
	string tmplat;
	while (str[i] != ';') {
		tmplat += str[i];
		i++;
	}i++;
	return tmplat;
}

void Rtree::ReadFromFile() {
	ifstream fin(file_name);
	if (!fin.is_open()) { cout << "Problem with file."; }
	else {
		float a = 0;
		string str, tmplat, tmplong;
		FileData help;
		while (!fin.eof()) {
			getline(fin, str, '\n');
			int i = 0;
			tmplat = deterStr(str, i);
			help.lat = str_to_float(tmplat);
			
			tmplong = deterStr(str, i);
			help.longitude = str_to_float(tmplong);

			help.type = deterStr(str, i);
			help.subtype = deterStr(str, i);
			help.name = deterStr(str, i);
			help.address = deterStr(str, i);
			dataV.push_back(help);
			tmplat.clear(); tmplong.clear(); help.type.clear(); help.subtype.clear(); help.name.clear(); help.address.clear();
		}
	}
}

void Rtree::fill_tree()
{
	for (int i = 0; i < dataV.size(); i++)
	{
		push_new_dot(&R_tree, NULL, dataV[i]);
	}
}

void Rtree::ifFunc( Node **nod, FileData dot) {
	if ((*nod)->slon + ((*nod)->width / 2) > dot.longitude && (*nod)->slat - ((*nod)->hight / 2) > dot.lat)
	{
		push_new_dot(&((*nod)->one), &(*nod), dot);
	}
	if ((*nod)->slon + ((*nod)->width / 2) > dot.longitude && (*nod)->slat - ((*nod)->hight / 2) <= dot.lat)
	{
		push_new_dot(&((*nod)->three), &(*nod), dot);
	}
	if ((*nod)->slon + ((*nod)->width / 2) <= dot.longitude && (*nod)->slat - ((*nod)->hight / 2) > dot.lat)
	{
		push_new_dot(&((*nod)->two), &(*nod), dot);
	}
	if ((*nod)->slon + ((*nod)->width / 2) <= dot.longitude && (*nod)->slat - ((*nod)->hight / 2) <= dot.lat)
	{
		push_new_dot(&((*nod)->four), &(*nod), dot);
	}

}

void Rtree::push_new_dot(Node** nod, Node** parent, FileData dot)
{
	if (*nod == NULL)
	{
		*nod = new Node;
		(*nod)->one = NULL;
		(*nod)->two = NULL;
		(*nod)->three = NULL;
		(*nod)->four = NULL;
		if (parent == NULL) {
			(*nod)->slat = maxlat;
			(*nod)->slon = minlon;
			(*nod)->hight = maxlat - minlat;
			(*nod)->width = maxlon - minlon;
			(*nod)->data.push_back(dot);
			(*nod)->count_dot = 1;
			(*nod)->is_see = false;
			(*nod)->up = NULL;
			return;
		}
		else {
			if ((*parent)->slon + ((*parent)->width / 2) > dot.longitude) { (*nod)->slon = (*parent)->slon; }
			else { (*nod)->slon = (*parent)->slon + ((*parent)->width / 2); }
			if ((*parent)->slat - ((*parent)->hight / 2) > dot.lat) { (*nod)->slat = (*parent)->slat; }
			else { (*nod)->slat = (*parent)->slat - ((*parent)->hight / 2); }
			(*nod)->hight = ((*parent)->hight / 2);
			(*nod)->width = ((*parent)->width / 2);
			(*nod)->data.push_back(dot);
			(*nod)->count_dot = 1;
			(*nod)->is_see = false;
			(*nod)->up = *parent;
			return;
		}
	}
	else {
		if ((*nod)->count_dot >= max_dot && (*nod)->one == NULL && (*nod)->two == NULL && (*nod)->three == NULL && (*nod)->four == NULL)
		{
			(*nod)->data.push_back(dot);
			for (int it = 0; it < (*nod)->data.size(); it++) {
				ifFunc(&*nod, (*nod)->data[it]);
			}
			(*nod)->data.clear();
			return;
		}
		if ((*nod)->count_dot >= max_dot)
		{
			ifFunc(&*nod, dot);
			return;
		}
		if ((*nod)->count_dot < max_dot) {
			(*nod)->data.push_back(dot);
			(*nod)->count_dot = (*nod)->count_dot + 1;
			(*nod)->is_see = false;
			return;
		}
	}
}

float Rtree::distance(FileData dot1, FileData dot2)
{
	float dX = (cos(dot2.lat / 57.4)) * (cos(dot2.longitude / 57.4)) - (cos(dot1.lat / 57.4)) * (cos(dot1.longitude / 57.4));
	float dY = (cos(dot2.lat / 57.4)) * (sin(dot2.longitude / 57.4)) - (cos(dot1.lat / 57.4)) * (sin(dot1.longitude / 57.4));
	float dZ = sin(dot2.lat / 57.4) - sin(dot1.lat / 57.4);
	float Cn = sqrt(dX * dX + dY * dY + dZ * dZ);
	float alfa = acos(1 - ((R_e * R_e * Cn * Cn) / (2 * (R_e * R_e))));
	float l = R_e * alfa;
	return l;
}

vector<FileData> Rtree::find_node(FileData dot, Node* nod)
{
	vector <FileData> help;
	if ((nod)->one == NULL && (nod)->two == NULL && (nod)->three == NULL && (nod)->four == NULL)
	{
		if (!(nod)->is_see) {
			(nod)->is_see = true;
			help = nod->data;
		}
		return help;
	}
	if ((nod)->slon + ((nod)->width / 2) > dot.longitude && (nod)->slat - ((nod)->hight / 2) > dot.lat)
	{
		help = find_node(dot, ((nod)->one));
	}
	if ((nod)->slon + ((nod)->width / 2) > dot.longitude && (nod)->slat - ((nod)->hight / 2) <= dot.lat)
	{
		help = find_node(dot, ((nod)->three));
	}
	if ((nod)->slon + ((nod)->width / 2) <= dot.longitude && (nod)->slat - ((nod)->hight / 2) > dot.lat)
	{
		help = find_node(dot, ((nod)->two));
	}
	if ((nod)->slon + ((nod)->width / 2) <= dot.longitude && (nod)->slat - ((nod)->hight / 2) <= dot.lat)
	{
		help = find_node(dot, ((nod)->four));
	}
	return help;
}

FileData Rtree::dot_for_look(FileData start, float alpha, float size_step)
{
	FileData help;
	help.longitude = start.longitude + size_step / sin(alpha / 57.5);
	help.lat = start.lat + size_step / cos(alpha / 57.5);
	return help;
}

void Rtree::find_around(float lat1, float lon1, float size, string type1, string subtype1)
{
	FileData start, now_dot; start.longitude = lon1; start.lat = lat1;
	float alpha;
	vector <FileData> help;
	for (alpha = 0; alpha < 360; alpha++)
	{
		now_dot.longitude = lon1; now_dot.lat = lat1;
		float step_size = 0.001;
		while (distance(start, now_dot) < size)
		{
			help = find_node(now_dot, R_tree);
			for (int i = 0; i < help.size(); i++) {
				if (help[i].type == type1 && subtype1 == "")
				{
					show_data(help[i]);
				}
				if (help[i].type == type1 && subtype1 != "" && help[i].subtype == subtype1)
				{
					show_data(help[i]);
				}
			}
			step_size += 0.001;
			now_dot = dot_for_look(start, alpha, step_size);
		}
	}

}

int main(int arg, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	string a, b, c, d, e, f = "";
	float lat, lon, size;
	if (arg < 6) { cout << "Not all data \n"; }
	else {
		a = argv[1];
		b = argv[2]; lat = str_to_float(b);
		c = argv[3]; lon = str_to_float(c);
		d = argv[4]; size = str_to_float(d);
		e = argv[5];
		a = "data.csv"; lat = 30.212; lon = 35.872; size = 20; e = "shop";
		if (arg == 7) { f = argv[6]; }
		Rtree tree(a);
		tree.find_around(lat, lon, size, e, f);
	}
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
