#include "Test.h"

Test::Test(std::filesystem::directory_entry directory_entry) {
	this->directory_entry = &directory_entry;
	documentMap = new DocumentMap(directory_entry.path().filename().string());

	this->dotProduct = 0ul;
	this->normalizedDotProduct = 0;
	this->lengthRatio = 0;
	this->confidence = 0;

	documentMap->LoadFile(directory_entry.path().string());
}

void Test::Perform(DocumentMap& subjectMap) {
	documentMap->SynchronizeWith(subjectMap);
	subjectMap.SynchronizeWith(*documentMap);

	dotProduct = documentMap->CalculateDotProduct(subjectMap);
	normalizedDotProduct = static_cast<double>(dotProduct) / (documentMap->magnitude * subjectMap.magnitude);

	lengthRatio = documentMap->averageUniqueWordCount / subjectMap.averageUniqueWordCount;
	if (lengthRatio > 1) {
		lengthRatio = 1 / lengthRatio;
	}

	confidence = pow(normalizedDotProduct, 1 / lengthRatio) * 100;
}