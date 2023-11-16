#pragma once

#include "DocumentMap.h"
#include <filesystem>

class Test {
public:
	std::filesystem::directory_entry *directory_entry;
	DocumentMap *documentMap;

	unsigned long dotProduct;
	double normalizedDotProduct;
	double lengthRatio;
	double confidence;

	Test(std::filesystem::directory_entry directory_entry);

	void Perform(DocumentMap& subjectMap);
};