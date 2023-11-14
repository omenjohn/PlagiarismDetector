#include "DocumentMap.h"
#include "Thesaurus.h"

inline void DocumentMap::Reset() {
	uniqueWordCount = 0;
	magnitude = 0;
	dictionary.clear();
}

inline DocumentMap::DocumentMap(string name) {
	this->name = name;
	Reset();
}

inline void DocumentMap::LoadFile(string path) {
	Reset();

	this->path = path;

	wifstream file(path);

	wstring word;
	while (file >> word) {

		wstring cleanedWord;
		for (wchar_t c : word) {
			
			if (c < 0 || c > 255)
				continue;

			if (iswalpha(c)) {
				cleanedWord.push_back(towlower(c));
			}
			
		}
		// HERE

		if (!cleanedWord.empty())
			Increase(cleanedWord, 1);
	}
}

inline void DocumentMap::Increase(wstring &word, unsigned long times) {
	if (dictionary.count(word) == 0) {
		dictionary.insert(make_pair(word, times));
		uniqueWordCount++;
	}
	else {
		dictionary.at(word)++;
	}
}

inline void DocumentMap::Simplify(Thesaurus thesaurus)
{
	for (auto& synonym : thesaurus.synonyms) {
		auto iterator = dictionary.find(synonym.first);

		if (iterator != dictionary.end()) {
			Increase(synonym.second, iterator->second);
			dictionary.erase(iterator);
		}
	}
}

inline void DocumentMap::InsertDefault(wstring word) {
	dictionary.insert(pair<wstring, unsigned long>(word, 0));
}

inline void DocumentMap::SynchronizeWith(DocumentMap& other) {

	synchronizedTarget = &other;

	for (auto& pair : dictionary) {
		other.InsertDefault(pair.first);
	}
}

inline unsigned long DocumentMap::CalculateDotProduct(const DocumentMap& other) {
	unsigned long dotProduct = 0;

	for (const auto& pair : dictionary) {
		if (other.dictionary.count(pair.first) > 0) {
			dotProduct += pair.second * other.dictionary.at(pair.first);
		}
	}
	return dotProduct;
}

inline double DocumentMap::Bake() {
	double cumulativeMagnitude = 0;
	unsigned long cumulativeWords = 0;

	for (const auto& pair : dictionary) {
		if (pair.second > 0) {
			cumulativeMagnitude += pair.second * pair.second;
		}
		cumulativeWords += pair.second;
	}

	this->wordCount = cumulativeWords;
	this->averageUniqueWordCount = static_cast<double>(cumulativeWords / dictionary.size());
	this->magnitude = sqrt(cumulativeMagnitude);
	return this->magnitude;
}

inline void DocumentMap::PrintMap() {
	cout << name << ":" << endl;

	int i = 0;
	for (auto& pair : dictionary) {
		wcout << "  " << ++i << " " << pair.first << ": " << pair.second << endl;
	}
	cout << endl;
}

inline void DocumentMap::PrintInfo() {
	cout << name << ":" << endl;
	cout << "  " << "File Path: " << path << endl;

	if (synchronizedTarget) {
		cout << "  " << "Synchronized: " << synchronizedTarget->name << " (" << synchronizedTarget->path << ")" << endl;
	}
	else {
		cout << "  " << "Synchronized: NO TARGET" << endl;
	}

	cout << "  " << "Unique Words: " << uniqueWordCount << endl;
	cout << "  " << "Average Unique Word Count: " << averageUniqueWordCount << endl;
	cout << "  " << "Magnitude: " << magnitude << endl << endl;
}
