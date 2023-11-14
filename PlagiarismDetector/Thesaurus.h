#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <unordered_map>

namespace fs = std::filesystem;
using namespace std;

class Thesaurus {

public:
	string name;
	string path;

	unordered_map<wstring, wstring> synonyms;

	Thesaurus(string name);
	void LoadFile(string path);
};