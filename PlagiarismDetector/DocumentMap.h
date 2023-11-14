#pragma once

#include <map>
#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>

#include "Thesaurus.h"
using namespace std;

class DocumentMap {

private:
	void Reset();

public:
	string name;
	string path;

	map<wstring, unsigned long> dictionary;
	unsigned int uniqueWordCount;
	unsigned long wordCount;
	double averageUniqueWordCount;
	double magnitude;
	DocumentMap* synchronizedTarget;

	DocumentMap(string name);

	void LoadFile(string path);

	void Increase(wstring &word, unsigned long times = 1UL);

	void Simplify(Thesaurus thesaurus);

	void InsertDefault(wstring word);

	void SynchronizeWith(DocumentMap& other);

	unsigned long CalculateDotProduct(const DocumentMap& other);

	double Bake();

	void PrintMap();

	void PrintInfo();
};