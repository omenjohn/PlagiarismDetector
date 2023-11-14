#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <list>
#include <unordered_map>
#include <algorithm>

#include "Test.cpp"
#include "DocumentMap.cpp"
#include "Thesaurus.cpp"

namespace fs = std::filesystem;
using namespace std;

bool CompareTestsBySimilarity(const Test* a, const Test* b);

int main(int argc, char* argv[]) {

	if (argc < 2) {
		cerr << "Usage: " << argv[0] << " <file_path>" << endl;
		return 1;
	}

	cout << endl << "LOADING RESOURCES ==================" << endl;

	cout << endl << "Thesaurii:" << endl;
	// Load thesaurii
	list<Thesaurus*> thesaurii;

	for (const auto& file : fs::directory_iterator("./Thesaurus")) {
		if (!file.is_directory()) {
			cout << "  Loading " << file.path() << " ...";
			string name = file.path().filename().stem().string();
			Thesaurus* thesaurus = new Thesaurus(name);
			thesaurus->LoadFile(file.path().string());

			thesaurii.push_back(thesaurus);

			cout << "done!" << endl;
		}
	}

	cout << endl << "Test Documents:" << endl;
	// Load documents
	list<Test*> tests;
	for (const auto &file : fs::directory_iterator("./CheckDocuments")) {
		if (!file.is_directory()) {
			cout << "  Loading " << file.path() << " ...";
			Test* test = new Test(file);

			for (auto& thesaurus : thesaurii) {
				test->documentMap->Simplify(*thesaurus);
			}

			test->documentMap->Bake();
			tests.push_back(test);
			cout << "done!" << endl;
		}
	}

	// Profile subject document
	cout << endl << "Profiling subject document...";
	DocumentMap subject_map = DocumentMap("Subject");
	subject_map.LoadFile(argv[1]);
	for (auto thesaurus : thesaurii) {
		subject_map.Simplify(*thesaurus);
	}
	subject_map.Bake();
	cout << "done!" << endl;

	cout << endl << "BEGIN TESTS =======================" << endl << endl;

	// Perform tests
	for (auto& test : tests) {
		test->Perform(subject_map);
		
		cout << test->documentMap->name << ":" << endl;
		cout << "  Unique Words Regularity: " << test->dotProduct << endl;
		cout << "  Normalized Dot Product: " << test->normalizedDotProduct << endl;
		cout << "  Length Comparison: " << test->lengthRatio << endl;
		cout << "  Confidence: " << test->confidence << '%' << endl << endl;
	}
	
	cout << endl << "SUMMARY ===========================" << endl << endl;

	cout << "Similar results by confidence (descending):" << endl;

	tests.sort(CompareTestsBySimilarity);

	int i = 0;
	for (auto& test : tests) {
		cout << "  #" << ++i << " " << test->documentMap->name << ": " << test->confidence << "%" << endl;
	}

	return 0;
}

bool CompareTestsBySimilarity(const Test* a, const Test* b) {

	return a->confidence > b->confidence;
}