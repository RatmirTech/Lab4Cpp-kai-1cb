#pragma once

#include <string>

bool inputMatrix(double**& matrix, int& rows, int& cols);
bool allocateMatrix(double**& matrix, int rows, int cols);
void freeMatrix(double**& matrix, int rows);
bool readMatrixFromFile(const char* filename, double**& matrix, int& rows, int& cols);
bool generateMatrix(double**& matrix, int& rows, int& cols);
void calculateColumnAverages(const double* const* matrix, int rows, int cols, double*& averages);
void calculateEvenColumnAverages(const double* const* matrix, int rows, int cols, double*& averages);
void printAverages(const double* averages, int count, const std::string& message, bool evenColumns = false);
void writeResultsToFile(const char* filename, const double* averages, int count, const std::string& message, bool evenColumns = false);
void clearFile(const char* filename);
void init373();