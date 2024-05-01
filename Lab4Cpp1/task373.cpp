#include "task373.h"
#include "constants.h"
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>

using namespace std;

bool inputMatrix(double**& matrix, int& rows, int& cols) {
	char input[256];
	regex valid_number("\\d+");

	cout << "������� ���������� ����� �������: ";
	cin.getline(input, 256);
	if (!regex_match(input, valid_number)) {
		cerr << "������: ������������ ����. ��������� ������ ����� �����." << endl;
		return false;
	}
	rows = stoi(input);

	cout << "������� ���������� �������� �������: ";
	cin.getline(input, 256);
	if (!regex_match(input, valid_number)) {
		cerr << "������: ������������ ����. ��������� ������ ����� �����." << endl;
		return false;
	}
	cols = stoi(input);

	srand(static_cast<unsigned>(time(nullptr)));

	matrix = new (nothrow) double* [rows];
	if (!matrix) {
		cerr << "�� ������� �������� ������ ��� ����� �������." << endl;
		return false;
	}

	cout << "�������" << endl;
	for (int i = 0; i < rows; ++i) {
		matrix[i] = new (nothrow) double[cols];
		if (!matrix[i]) {
			cerr << "�� ������� �������� ������ ��� �������� �������." << endl;
			for (int k = 0; k < i; ++k) {
				delete[] matrix[k];
			}
			delete[] matrix;
			matrix = nullptr;
			return false;
		}

		for (int j = 0; j < cols; ++j) {
			double r = static_cast<double>(rand()) / RAND_MAX * 100.0;
			matrix[i][j] = r;
			cout << r << " ";
		}
		cout << endl;
	}
	return true;
}

bool readMatrixFromFile(const char* filename, double**& matrix, int& rows, int& cols) {
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "�� ������� ������� ���� '" << filename << "'" << endl;
		return false;
	}

	string line;
	getline(file, line);
	istringstream iss(line);
	if (!(iss >> rows >> cols)) {
		cout << "������: ������������ ������ �������� �������." << endl;
		return false;
	}

	matrix = new (nothrow) double* [rows];
	if (!matrix) {
		cout << "������ ��������� ������ ��� ����� �������." << endl;
		return false;
	}

	regex valid_format("^([+-]?\\d*\\.?\\d+\\s*)+$");

	for (int i = 0; i < rows; ++i) {
		if (!getline(file, line) || !regex_match(line, valid_format)) {
			cout << "������: ������������ ������ � ������� ��� ������������ �����." << endl;
			for (int k = 0; k < i; ++k) {
				delete[] matrix[k];
			}
			delete[] matrix;
			matrix = nullptr;
			return false;
		}

		matrix[i] = new (nothrow) double[cols];
		if (!matrix[i]) {
			cout << "������ ��������� ������ ��� �������� �������." << endl;
			for (int k = 0; k <= i; ++k) {
				delete[] matrix[k];
			}
			delete[] matrix;
			matrix = nullptr;
			return false;
		}

		istringstream row_stream(line);
		for (int j = 0; j < cols; ++j) {
			if (!(row_stream >> matrix[i][j])) {
				cout << "������: ������������ ������ � ������ �������." << endl;
				for (int k = 0; k <= i; ++k) {
					delete[] matrix[k];
				}
				delete[] matrix;
				matrix = nullptr;
				return false;
			}
		}
	}

	file.close();
	return true;
}

void calculateColumnAverages(const double* const* matrix, int rows, int cols, double*& averages) {
	averages = new double[cols] {};
	for (int j = 0; j < cols; ++j) {
		double sum = 0.0;
		for (int i = 0; i < rows; ++i) {
			sum += matrix[i][j];
		}
		averages[j] = sum / rows;
	}
}

void calculateEvenColumnAverages(const double* const* matrix, int rows, int cols, double*& averages) {
	if (cols < 2) return; // ���� �������� ������ ����, ��� �� Ip � �� blackbord

	int count = (cols + 1) / 2; // ���-�� ������ �������� (� ��������� 0, 2, 4, ...)
	averages = new double[count] {};
	int index = 0;
	for (int j = 0; j < cols; j += 2) {
		double sum = 0.0;
		for (int i = 0; i < rows; ++i) {
			sum += matrix[i][j];
		}
		averages[index++] = sum / rows;
	}
}

void printAverages(const double* averages, int count, const string& message) {
	cout << message << endl;
	for (int i = 0; i < count; ++i) {
		cout << "������� ��� ������� " << i + 1 << ": " << averages[i] << endl;
	}
}

bool repeatOperation(bool& toFile) {
	if (!toFile) {
		regex valid_input("^[12]$");
		string input;
		cout << "������ ��������� ���� �������� ������? �� � 1, ��� � 2: ";
		getline(cin, input);
		while (!regex_match(input, valid_input)) {
			cout << "������������ ����. ����������, ������� 1 ��� 2: ";
			getline(cin, input);
		}
		return input == "1";
	}
}

bool chooseInputSource() {
	regex valid_input("^[12]$");
	string input;
	cout << "�������� �������� ����� ������: 1 - �������, 2 - ����: ";
	getline(cin, input);
	while (!regex_match(input, valid_input)) {
		cout << "������������ ����. ����������, ������� 1 ��� ������� ��� 2 ��� �����: ";
		getline(cin, input);
	}
	return input == "2";
}

bool chooseOutputDestination() {
	regex valid_input("^[12]$");
	string input;
	cout << "�������� ���� ������ ������: 1 - �������, 2 - ����: ";
	getline(cin, input);
	while (!regex_match(input, valid_input)) {
		cout << "������������ ����. ����������, ������� 1 ��� ������� ��� 2 ��� �����: ";
		getline(cin, input);
	}
	return input == "2";
}

void writeResultsToFile(const char* filename, const double* averages, int count, const string& message) {
	ofstream file(filename);
	if (!file.is_open()) {
		cout << "�� ������� ������� ���� '" << filename << "'" << endl;
		return;
	}

	file << message << endl;
	for (int i = 0; i < count; ++i) {
		file << "������� ��� ������� " << i + 1 << ": " << averages[i] << endl;
	}
	file.close();
}

void init373() {
	double** matrix = nullptr;
	int rows = 0, cols = 0;
	double* allColumnAverages = nullptr;
	double* evenColumnAverages = nullptr;
	bool isFile = { false };
	do {
		bool fromFile = chooseInputSource();
		if (fromFile) {
			if (!readMatrixFromFile(MyConstants::task373Input, matrix, rows, cols)) {
				continue;
			}
		}
		else {
			if (!inputMatrix(matrix, rows, cols)) {
				continue;
			}
		}

		calculateColumnAverages(matrix, rows, cols, allColumnAverages);
		calculateEvenColumnAverages(matrix, rows, cols, evenColumnAverages);

		bool toFile = chooseOutputDestination();
		if (toFile) {
			writeResultsToFile(MyConstants::task373Output, allColumnAverages, cols, "������� �������������� ���� ��������:");
			writeResultsToFile(MyConstants::task373Output, evenColumnAverages, (cols + 1) / 2, "������� �������������� ������ ��������:");
		}
		else {
			printAverages(allColumnAverages, cols, "������� �������������� ���� ��������:");
			printAverages(evenColumnAverages, (cols + 1) / 2, "������� �������������� ������ ��������:");
		}

		for (int i = 0; i < rows; ++i) {
			delete[] matrix[i];
		}
		delete[] matrix;
		delete[] allColumnAverages;
		delete[] evenColumnAverages;

		isFile = fromFile;
	} while (repeatOperation(isFile));
}
