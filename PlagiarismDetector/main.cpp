#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <list>
#include <unordered_map>
#include <algorithm>

#include "Test.h"
#include "DocumentMap.h"
#include "Thesaurus.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {

	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " <file_path>" << '\n';
		return 1;
	}

	std::cout << '\n' << "LOADING RESOURCES ==================" << '\n';

	std::cout << '\n' << "Thesaurii:" << '\n';
	// Load thesaurii
	std::list<Thesaurus*> thesaurii;

	for (const auto& file : fs::directory_iterator("./Thesaurus")) {
		if (!file.is_directory()) {
			std::cout << "  Loading " << file.path() << " ..." << std::flush;
			std::string name = file.path().filename().stem().string();
			Thesaurus* thesaurus = new Thesaurus(name);

			std::string pathString = file.path().string();
			thesaurus->LoadFile(pathString);

			thesaurii.push_back(thesaurus);

			std::cout << "done!" << std::endl;
		}
	}

	std::cout << '\n' << "Test Documents:" << '\n';
	// Load documents
	std::list<Test*> tests;
	for (const auto &file : fs::directory_iterator("./CheckDocuments")) {
		if (!file.is_directory()) {
			std::cout << "  Loading " << file.path() << " ..." << std::flush;
			Test* test = new Test(file);

			for (auto& thesaurus : thesaurii) {
				test->documentMap->Simplify(*thesaurus);
			}

			test->documentMap->Bake();
			tests.push_back(test);
			std::cout << "done!" << std::endl;
		}
	}

	// Profile subject document
	std::cout << '\n' << "Profiling subject document..." << std::flush;
	DocumentMap subject_map = DocumentMap("Subject");
	subject_map.LoadFile(argv[1]);
	for (auto thesaurus : thesaurii) {
		subject_map.Simplify(*thesaurus);
	}
	subject_map.Bake();
	std::cout << "done!" << std::endl;

	std::cout << '\n' << "BEGIN TESTS =======================" << '\n' << '\n';

	// Perform tests
	for (auto& test : tests) {
		test->Perform(subject_map);
		
		std::cout << test->documentMap->name << ":" << '\n';
		std::cout << "  Unique Words Regularity: " << test->dotProduct << '\n';
		std::cout << "  Normalized Dot Product: " << test->normalizedDotProduct << '\n';
		std::cout << "  Length Comparison: " << test->lengthRatio << '\n';
		std::cout << "  Confidence: " << test->confidence << '%' << '\n' << std::endl;
	}
	
	std::cout << '\n' << "SUMMARY ===========================" << '\n' << '\n';

	std::cout << "Similar results by confidence (descending):" << '\n';

	tests.sort([](const Test* a, const Test* b) {return a->confidence > b->confidence; });

	int i = 0;
	for (auto& test : tests) {
		std::cout << "  #" << ++i << " " << test->documentMap->name << ": " << test->confidence << "%" << std::endl;
	}

	return 0;
}