#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <unordered_map>

namespace fs = std::filesystem;

class Thesaurus {

public:
	std::string name;
	std::string path;

	std::unordered_map<std::wstring, std::wstring> synonyms;

	Thesaurus(std::string& name);
	void LoadFile(std::string& path);
};