#pragma once

#include <iostream>
#include <fstream>
#include <string>

bool inputMatrix(double**& matrix, int& rows, int& cols);
bool readMatrixFromFile(const char* filename, double**& matrix, int& rows, int& cols);
void init373();
void calculateColumnAverages(const double* const* matrix, int rows, int cols, double*& averages);
void calculateEvenColumnAverages(const double* const* matrix, int rows, int cols, double*& averages);
void printAverages(const double* averages, int count, const std::string& message);
void writeResultsToFile(const char* filename, const double* averages, int count, const std::string& message);

bool repeatOperation();
bool chooseInputSource();
bool chooseOutputDestination();
