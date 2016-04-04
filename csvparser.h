#pragma once
#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
class csvparser
{
public:
	csvparser();
	~csvparser();
	vector<vector<string> > parseCsvFile(const string &fileName,const string &separator = ",");
	void printData();
	vector<string> splitString(const string &str,const string &separator);
	string loadCsvFile(const string &filename);
private:
	vector<vector<string> > m_Data;
};

