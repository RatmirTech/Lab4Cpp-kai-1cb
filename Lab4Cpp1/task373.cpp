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
		cout << "Введите количество строк матрицы: ";
		getline(cin, input);
		if (!regex_match(input, valid_number)) {
			cerr << "Ошибка: Некорректный ввод. Допустимы только целые числа." << endl;
		}
	} while (!regex_match(input, valid_number));
	rows = stoi(input);

	do {
		cout << "Введите количество столбцов матрицы: ";
		getline(cin, input);
		if (!regex_match(input, valid_number)) {
			cerr << "Ошибка: Некорректный ввод. Допустимы только целые числа." << endl;
		}
	} while (!regex_match(input, valid_number));
	cols = stoi(input);

	return true;
}

bool allocateMatrix(double**& matrix, int rows, int cols) {
	matrix = new (nothrow) double* [rows];
	if (!matrix) {
		cout << "Ошибка выделения памяти для строк матрицы." << endl;
		return false;
	}

	for (int i = 0; i < rows; ++i) {
		matrix[i] = new (nothrow) double[cols];
		if (!matrix[i]) {
			cout << "Ошибка выделения памяти для столбцов матрицы." << endl;
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
		cout << "Не удалось открыть файл '" << filename << "'" << endl;
		return false;
	}

	string line;
	rows = 0;
	cols = -1;
	bool matrixFound = false; // Флаг для проверки наличия матрицы в файле
	regex valid_format("^\\s*([+-]?\\d*\\.?\\d+\\s*)+$"); // Регулярное выражение для действительных чисел
	while (getline(file, line)) {
		if (!regex_match(line, valid_format)) {
			cout << "Ошибка: некорректные данные в строке." << endl;
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
			cout << "Ошибка: разное количество столбцов в разных строках." << endl;
			file.close();
			return false;
		}
		rows++;
		matrixFound = true; // Устанавливаем флаг, если были считаны какие-то данные из файла
	}

	if (!matrixFound) {
		cout << "Ошибка: файл не содержит матрицы." << endl;
		file.close();
		return false;
	}

	file.clear(); // Сброс флага ошибки файла
	file.seekg(0, ios::beg); // Вернуть указатель в начало файла

	if (!allocateMatrix(matrix, rows, cols)) {
		return false;
	}

	int rowIndex = 0;
	while (getline(file, line)) {
		istringstream iss(line);
		for (int j = 0; j < cols; ++j) {
			if (!(iss >> matrix[rowIndex][j])) {
				cout << "Ошибка: некорректные данные в строке матрицы." << endl;
				freeMatrix(matrix, rowIndex + 1); // включая текущую неполностью заполненную строку
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
		cerr << "Не удалось выделить память для строк матрицы." << endl;
		return false;
	}

	for (int i = 0; i < rows; ++i) {
		matrix[i] = new (nothrow) double[cols];
		if (!matrix[i]) {
			cerr << "Не удалось выделить память для столбцов матрицы." << endl;
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
	if (averages != nullptr) delete[] averages; // Освобождаем предыдущие данные, если они есть
	averages = new double[cols] {}; // Инициализируем массив с нулями для всех столбцов

	for (int j = 0; j < cols; j++) {
		double sum = 0.0;
		for (int i = 0; i < rows; i++) {
			sum += matrix[i][j];
		}
		averages[j] = sum / rows; // Сохраняем среднее каждого столбца
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
		cout << "Среднее для столбца " << columnNumber << ": " << averages[i] << endl;
	}
}

void writeResultsToFile(const char* filename, const double* averages, int count, const string& message, bool evenColumns = false) {
	ofstream file(filename);
	if (!file.is_open()) {
		cerr << "Ошибка открытия файла " << filename << endl;
		return;
	}

	file << message << endl;
	for (int i = 0; i < count; ++i) {
		int columnNumber = evenColumns ? 2 * (i + 1) : (i + 1);
		file << "Среднее для столбца " << columnNumber << ": " << averages[i] << endl;
	}

	file.close();
}

void init373() {
	regex valid_input("^[12]$");
	string input;
	char in_option, out_option, repeat_option = '1';

	do {
		do {
			cout << "Введите '1' для ввода с консоли, '2' для ввода из файла: ";
			getline(cin, input);
		} while (!regex_match(input, valid_input));
		in_option = input[0];

		do {
			cout << "Введите '1' для вывода на консоль, '2' для вывода в файл: ";
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
				printAverages(allColumnAverages, cols, "Среднее арифметическое всех столбцов:", false);
				int evenCount = (cols + 1) / 2;
				printAverages(evenColumnAverages, evenCount, "Среднее арифметическое четных столбцов:", true);
			}
			else if (out_option == '2') {
				writeResultsToFile(MyConstants::task373Output, allColumnAverages, cols, "Среднее арифметическое всех столбцов:", false);
				int evenCount = (cols + 1) / 2;
				writeResultsToFile(MyConstants::task373Output, evenColumnAverages, evenCount, "Среднее арифметическое четных столбцов:", true);
			}
			freeMatrix(matrix, rows);
			delete[] allColumnAverages;
			delete[] evenColumnAverages;
		}

		if (in_option == '1') {
			do {
				cout << "Хотите начать сначала? 1 - Да, 2 - Нет: ";
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
		cout << "Хотите повторить ввод исходных данных? Да — 1, Нет — 2: ";
		getline(cin, input);
		while (!regex_match(input, valid_input)) {
			cout << "Некорректный ввод. Пожалуйста, введите 1 или 2: ";
			getline(cin, input);
		}
		return input == "1";
	}
}

bool chooseInputSource() {
	regex valid_input("^[12]$");
	string input;
	cout << "Выберите источник ввода данных: 1 - Консоль, 2 - Файл: ";
	getline(cin, input);
	while (!regex_match(input, valid_input)) {
		cout << "Некорректный ввод. Пожалуйста, введите 1 для консоли или 2 для файла: ";
		getline(cin, input);
	}
	return input == "2";
}

bool chooseOutputDestination() {
	regex valid_input("^[12]$");
	string input;
	cout << "Выберите цель вывода данных: 1 - Консоль, 2 - Файл: ";
	getline(cin, input);
	while (!regex_match(input, valid_input)) {
		cout << "Некорректный ввод. Пожалуйста, введите 1 для консоли или 2 для файла: ";
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
			writeResultsToFile(MyConstants::task373Output, allColumnAverages, cols, "Среднее арифметическое всех столбцов:");
			writeResultsToFile(MyConstants::task373Output, evenColumnAverages, (cols + 1) / 2, "Среднее арифметическое четных столбцов:");
		}
		else {
			printAverages(allColumnAverages, cols, "Среднее арифметическое всех столбцов:");
			printAverages(evenColumnAverages, (cols + 1) / 2, "Среднее арифметическое четных столбцов:");
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