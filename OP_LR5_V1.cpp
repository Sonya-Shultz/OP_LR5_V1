﻿#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

float str_to_float(string help);

const int max_dot = 7;

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
	Node* one, * two, * three, * four, *up;
	bool is_see;
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
	int help;
	Node* R_tree;
	vector<FileData> dataV;
	void firstNode();
	void push_new_dot(Node** nod, Node** parent, FileData dot);
	void print(Node* tree, int u);
	void fill_tree();
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
	R_tree->up = NULL;
}

Rtree::Rtree() {
	minlat = 4000;
	minlon = 2100;
	maxlat = 5700;
	maxlon = 4100;
	help = 0;
	R_tree =NULL;
	ReadFromFile();
	fill_tree();
	print(R_tree, 0);
	//firstNode();
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
			help.lat = str_to_float(tmplat)*100;
			while (str[i] != ';') {
				tmplong += str[i];
				i++;
			}i++;
			help.longitude = str_to_float(tmplong)*100;
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

void Rtree::fill_tree()
{
	for (int i = 0; i < dataV.size(); i++)
	{
		push_new_dot(&R_tree, NULL, dataV[i]);
	}
}

void Rtree::push_new_dot(Node **nod, Node **parent, FileData dot)
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
			(*nod)->data.push_back( dot);
			(*nod)->count_dot =1;
			(*nod)->is_see = false;
			(*nod)->up = NULL;
			return;
		}
		else {
			if ((*parent)->slon + ((*parent)->width / 2)  > dot.longitude) { (*nod)->slon = (*parent)->slon; }
			else { (*nod)->slon = (*parent)->slon+ ((*parent)->width / 2) ; }
			if ((*parent)->slat - ((*parent)->hight / 2)  > dot.lat) { (*nod)->slat = (*parent)->slat; }
			else { (*nod)->slat = (*parent)->slat - ((*parent)->hight / 2) ; }
			(*nod)->hight = ((*parent)->hight / 2) ;
			(*nod)->width = ((*parent)->width / 2) ;
			(*nod)->data.push_back(dot);
			(*nod)->count_dot = 1;
			(*nod)->is_see = false;
			(*nod)->up = *parent;
			return;
		}
	}
	else {
		if ((*nod)->count_dot >= max_dot && (*nod)->one==NULL && (*nod)->two == NULL && (*nod)->three == NULL && (*nod)->four == NULL)
		{
			(*nod)->data.push_back(dot);
			for (int it = 0; it < (*nod)->data.size(); it++) {
				if ((*nod)->slon + ((*nod)->width / 2) > (*nod)->data[it].longitude && (*nod)->slat - ((*nod)->hight / 2)> (*nod)->data[it].lat)
				{
					push_new_dot(&((*nod)->one), &(*nod), (*nod)->data[it]);
				}
				if ((*nod)->slon + ((*nod)->width / 2) > (*nod)->data[it].longitude && (*nod)->slat - ((*nod)->hight / 2) <= (*nod)->data[it].lat)
				{
					push_new_dot(&((*nod)->three), &(*nod), (*nod)->data[it]);
				}
				if ((*nod)->slon + ((*nod)->width / 2)  <= (*nod)->data[it].longitude && (*nod)->slat - ((*nod)->hight / 2)  > (*nod)->data[it].lat)
				{
					push_new_dot(&((*nod)->two), &(*nod), (*nod)->data[it]);
				}
				if ((*nod)->slon + ((*nod)->width / 2)  <= (*nod)->data[it].longitude && (*nod)->slat - ((*nod)->hight / 2)  <= (*nod)->data[it].lat)
				{
					push_new_dot(&((*nod)->four), &(*nod), (*nod)->data[it]);
				}
			}
			(*nod)->data.clear();
			cout << (*nod)->data.size() << endl;
			return;
		}
		if ((*nod)->count_dot >= max_dot )//&& ((*nod)->one != NULL || (*nod)->two != NULL || (*nod)->three != NULL || (*nod)->four != NULL))
		{
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

void Rtree::print(Node* tree, int u)
{
	if (tree == NULL) {  return; }
	else
	{
		print(tree->one, u++);
		print(tree->three, u++);
		print(tree->two, u++);
		print(tree->four, u++);
		for (int i = 0; i < u; ++i) {
			for (int j = 0; j < tree->data.size(); j++)
			{
				cout << tree->data[j].type << " ";
			}
			u--;
		}
	}
}

int main()
{
	Rtree tree;
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
