#pragma once

#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <cstring>

void ProcessSequence(const char* sequence, int& spaceCount, bool& foundYu, bool foundShina[], bool& foundNoOrOn, bool& foundAdjacentIdentical, bool& patternExists);
bool readFileContents(const char* filename, char*& buffer);
bool truncateAtExclamation(char*& buffer);
bool readFromConsole(char*& buffer);
void PrepareTextForOutput(const char* sequence, char*& outputBuffer, int spaceCount, bool foundYu, const bool foundShina[], bool foundNoOrOn, bool foundAdjacentIdentical, bool patternExists);
bool SaveTextToFile(const char* filename, const char* text);
void toLowerCase(char* str);
void init257();