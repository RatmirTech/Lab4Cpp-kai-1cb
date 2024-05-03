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
	string input;
	regex valid_number("^(?!0\\d)\\d+$");

	do {
		cout << "������� ���������� ����� �������: ";
		getline(cin, input);
		if (!regex_match(input, valid_number)) {
			cerr << "������: ������������ ����. ��������� ������ ����� �����." << endl;
		}
	} while (!regex_match(input, valid_number));
	rows = stoi(input);

	do {
		cout << "������� ���������� �������� �������: ";
		getline(cin, input);
		if (!regex_match(input, valid_number)) {
			cerr << "������: ������������ ����. ��������� ������ ����� �����." << endl;
		}
	} while (!regex_match(input, valid_number));
	cols = stoi(input);

	return true;
}

bool allocateMatrix(double**& matrix, int rows, int cols) {
	matrix = new (nothrow) double* [rows];
	if (!matrix) {
		cout << "������ ��������� ������ ��� ����� �������." << endl;
		return false;
	}

	for (int i = 0; i < rows; ++i) {
		matrix[i] = new (nothrow) double[cols];
		if (!matrix[i]) {
			cout << "������ ��������� ������ ��� �������� �������." << endl;
			while (i--) delete[] matrix[i];
			delete[] matrix;
			matrix = nullptr;
			return false;
		}
	}
	return true;
}

void freeMatrix(double**& matrix, int rows) {
	for (int i = 0; i < rows; ++i) {
		delete[] matrix[i];
	}
	delete[] matrix;
	matrix = nullptr;
}

bool readMatrixFromFile(const char* filename, double**& matrix, int& rows, int& cols) {
	ifstream file(filename);
	if (!file.is_open()) {
		cout << "�� ������� ������� ���� '" << filename << "'" << endl;
		return false;
	}

	string line;
	rows = 0;
	cols = -1;
	bool matrixFound = false; // ���� ��� �������� ������� ������� � �����
	regex valid_format("^\\s*([+-]?\\d*\\.?\\d+\\s*)+$"); // ���������� ��������� ��� �������������� �����
	while (getline(file, line)) {
		if (!regex_match(line, valid_format)) {
			cout << "������: ������������ ������ � ������." << endl;
			file.close();
			return false;
		}

		istringstream iss(line);
		double value;
		int currentColCount = 0;
		while (iss >> value) {
			currentColCount++;
		}
		if (cols == -1) {
			cols = currentColCount;
		}
		else if (currentColCount != cols) {
			cout << "������: ������ ���������� �������� � ������ �������." << endl;
			file.close();
			return false;
		}
		rows++;
		matrixFound = true; // ������������� ����, ���� ���� ������� �����-�� ������ �� �����
	}

	if (!matrixFound) {
		cout << "������: ���� �� �������� �������." << endl;
		file.close();
		return false;
	}

	file.clear(); // ����� ����� ������ �����
	file.seekg(0, ios::beg); // ������� ��������� � ������ �����

	if (!allocateMatrix(matrix, rows, cols)) {
		return false;
	}

	int rowIndex = 0;
	while (getline(file, line)) {
		istringstream iss(line);
		for (int j = 0; j < cols; ++j) {
			if (!(iss >> matrix[rowIndex][j])) {
				cout << "������: ������������ ������ � ������ �������." << endl;
				freeMatrix(matrix, rowIndex + 1); // ������� ������� ����������� ����������� ������
				return false;
			}
		}
		rowIndex++;
	}

	file.close();
	return true;
}

bool generateMatrix(double**& matrix, int& rows, int& cols) {
	srand(static_cast<unsigned>(time(nullptr)));

	matrix = new (nothrow) double* [rows];
	if (!matrix) {
		cerr << "�� ������� �������� ������ ��� ����� �������." << endl;
		return false;
	}

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
		}
	}
	return true;
}

void calculateColumnAverages(const double* const* matrix, int rows, int cols, double*& averages) {
	if (averages != nullptr) delete[] averages; // ����������� ���������� ������, ���� ��� ����
	averages = new double[cols] {}; // �������������� ������ � ������ ��� ���� ��������

	for (int j = 0; j < cols; j++) {
		double sum = 0.0;
		for (int i = 0; i < rows; i++) {
			sum += matrix[i][j];
		}
		averages[j] = sum / rows; // ��������� ������� ������� �������
	}
}
void calculateEvenColumnAverages(const double* const* matrix, int rows, int cols, double*& averages) {
	if (averages != nullptr) delete[] averages;
	int count = cols / 2;
	if (cols % 2 != 0)
		count++;

	averages = new double[count] {};

	int index = 0;
	for (int j = 1; j < cols; j += 2) {
		if (j >= cols) break;
		double sum = 0.0;
		for (int i = 0; i < rows; i++) {
			sum += matrix[i][j];
		}
		averages[index++] = sum / rows;
	}
}

void printAverages(const double* averages, int count, const string& message, bool evenColumns = false) {
	cout << message << endl;
	for (int i = 0; i < count; ++i) {
		int columnNumber = evenColumns ? 2 * (i + 1) : (i + 1);
		cout << "������� ��� ������� " << columnNumber << ": " << averages[i] << endl;
	}
}

void writeResultsToFile(const char* filename, const double* averages, int count, const string& message, bool evenColumns = false) {
	ofstream file(filename);
	if (!file.is_open()) {
		cerr << "������ �������� ����� " << filename << endl;
		return;
	}

	file << message << endl;
	for (int i = 0; i < count; ++i) {
		int columnNumber = evenColumns ? 2 * (i + 1) : (i + 1);
		file << "������� ��� ������� " << columnNumber << ": " << averages[i] << endl;
	}

	file.close();
}

void init373() {
	regex valid_input("^[12]$");
	string input;
	char in_option, out_option, repeat_option = '1';

	do {
		do {
			cout << "������� '1' ��� ����� � �������, '2' ��� ����� �� �����: ";
			getline(cin, input);
		} while (!regex_match(input, valid_input));
		in_option = input[0];

		do {
			cout << "������� '1' ��� ������ �� �������, '2' ��� ������ � ����: ";
			getline(cin, input);
		} while (!regex_match(input, valid_input));
		out_option = input[0];

		double** matrix = nullptr;
		int rows{ 0 };
		int cols{ 0 };
		if (in_option == '1') {
			if (!inputMatrix(matrix, rows, cols) || !generateMatrix(matrix, rows, cols)) {
				continue;
			}
		}
		else {
			if (!readMatrixFromFile(MyConstants::task373Input, matrix, rows, cols)) {
				continue;
			}
		}

		if (matrix != nullptr) {
			for (int i{ 0 }; i < rows; ++i) {
				for (int j{ 0 }; j < cols; ++j) {
					cout << matrix[i][j] << " ";
				}
				cout << endl;
			}
			double* allColumnAverages = nullptr;
			double* evenColumnAverages = nullptr;

			calculateColumnAverages(matrix, rows, cols, allColumnAverages);
			calculateEvenColumnAverages(matrix, rows, cols, evenColumnAverages);

			if (out_option == '1') {
				printAverages(allColumnAverages, cols, "������� �������������� ���� ��������:", false);
				int evenCount = (cols + 1) / 2;
				printAverages(evenColumnAverages, evenCount, "������� �������������� ������ ��������:", true);
			}
			else if (out_option == '2') {
				writeResultsToFile(MyConstants::task373Output, allColumnAverages, cols, "������� �������������� ���� ��������:", false);
				int evenCount = (cols + 1) / 2;
				writeResultsToFile(MyConstants::task373Output, evenColumnAverages, evenCount, "������� �������������� ������ ��������:", true);
			}
			freeMatrix(matrix, rows);
			delete[] allColumnAverages;
			delete[] evenColumnAverages;
		}

		if (in_option == '1') {
			do {
				cout << "������ ������ �������? 1 - ��, 2 - ���: ";
				getline(cin, input);
			} while (!regex_match(input, valid_input));
			repeat_option = input[0];
		}
		else {
			repeat_option = '2';
		}
	} while (repeat_option == '1');
}

/*
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
*/

/*void init373() {
	double** matrix = nullptr;
	int rows{ 0 }, cols{ 0 };
	double* allColumnAverages = nullptr;
	double* evenColumnAverages = nullptr;
	bool isFile{ false };
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
}*/