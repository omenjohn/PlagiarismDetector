#include "Thesaurus.h"

Thesaurus::Thesaurus(string& name) {
	this->name = name;
}

void Thesaurus::LoadFile(string& path) {
	this->path = path;

	wifstream file(path);

	wstring line;
	while (std::getline(file, line)) {

		int linepos = line.find('=');
		wstring key = line.substr(0, linepos);
		wstring values = line.substr(linepos + 1);

		vector<wstring> tokens;
		wstringstream ss(values);
		wstring token;

		while (ss >> token) {
			synonyms.insert(make_pair(token, key));
		}
	}
}