#include "task257.h"
#include "constants.h"
#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include <cstring>

using namespace std;

void ProcessSequence(const char* sequence, int& spaceCount, bool& foundYu, bool foundShina[], bool& foundNoOrOn, bool& foundAdjacentIdentical, bool& patternExists) {
	spaceCount = 0;
	foundYu = false;
	memset(foundShina, false, 4 * sizeof(bool));
	foundNoOrOn = false;
	foundAdjacentIdentical = false;
	patternExists = false;

	const char* prevChar = nullptr;
	for (int i = 0; sequence[i] != '\0'; i++) {
		if (sequence[i] == ' ') spaceCount++;
		if (sequence[i] == 'ю') foundYu = true;
		if (sequence[i] == 'ш') foundShina[0] = true;
		if (sequence[i] == 'и') foundShina[1] = true;
		if (sequence[i] == 'н') foundShina[2] = true;
		if (sequence[i] == 'а') foundShina[3] = true;

		if (prevChar) {
			if ((*prevChar == 'н' && sequence[i] == 'о') || (*prevChar == 'о' && sequence[i] == 'н'))
				foundNoOrOn = true;
			if (*prevChar == sequence[i])
				foundAdjacentIdentical = true;
		}

		prevChar = &sequence[i];
	}

	// условие е
	int length = strlen(sequence);
	for (int i = 0; i < length; ++i) {
		for (int j = i + 1; j < length; ++j) {
			if (sequence[i] == sequence[i + 1] && sequence[j + 1] == sequence[j]) {
				patternExists = true;
				break;
			}
		}
		if (patternExists) break;
	}
}

bool readFileContents(const char* filename, char*& buffer) {
	ifstream file(filename, ios::binary | ios::ate);
	if (!file.is_open()) {
		cout << "Ошибка при открытии файла '" << filename << "'" << endl;
		return false;
	}

	streamsize size = file.tellg();
	file.seekg(0, ios::beg);

	buffer = new (nothrow) char[size + 1];
	if (buffer == nullptr) {
		cout << "Не удалось выделить память для чтения файла" << endl;
		file.close();
		return false;
	}

	if (!file.read(buffer, size)) {
		cout << "Ошибка при чтении файла" << endl;
		delete[] buffer;
		buffer = nullptr;
		file.close();
		return false;
	}

	buffer[size] = '\0';
	file.close();
	return true;
}

bool truncateAtExclamation(char*& buffer) {
	if (buffer == nullptr) {
		cout << "Буфер не инициализирован." << endl;
		return false;
	}

	char* pos = strchr(buffer, '!');
	if (pos != nullptr) {
		if (pos == buffer) {
			cout << "Восклицательный знак находится в начале строки." << endl;
			delete[] buffer;
			buffer = nullptr;
			return false;
		}
		*pos = '\0';  // обрзека строки до !
	}
	else {
		cout << "Восклицательный знак не найден." << endl;
		delete[] buffer;
		buffer = nullptr;
		return false;
	}
	return true;
}

bool readFromConsole(char*& buffer) {
	cout << "Введите данные: ";
	string input;
	getline(cin, input);

	buffer = new (nothrow) char[input.length() + 1];
	if (buffer == nullptr) {
		cout << "Не удалось выделить память для ввода данных" << endl;
		return false;
	}

	strcpy(buffer, input.c_str());
	return true;
}

void PrepareTextForOutput(const char* sequence, char*& outputBuffer, int spaceCount, bool foundYu, const bool foundShina[], bool foundNoOrOn, bool foundAdjacentIdentical, bool patternExists) {
	string output;
	output += "Количество пробелов: " + to_string(spaceCount) + "\n";
	output += "Буква 'ю' " + string(foundYu ? "присутствует" : "отсутствует") + "\n";
	output += "Слово 'шина' " + string((foundShina[0] && foundShina[1] && foundShina[2] && foundShina[3]) ? "присутствует" : "отсутствует") + "\n";
	output += "Пары 'но' или 'он' " + string(foundNoOrOn ? "присутствуют" : "отсутствуют") + "\n";
	output += "Пары одинаковых соседних символов " + string(foundAdjacentIdentical ? "присутствуют" : "отсутствуют") + "\n";
	output += "Условие e " + string(patternExists ? "выполнено" : "не выполнено") + "\n";

	size_t outputLength = output.length() + 1;
	outputBuffer = new (nothrow) char[outputLength];
	if (outputBuffer == nullptr) {
		cerr << "Ошибка выделения памяти." << endl;
		return;
	}
	strcpy(outputBuffer, output.c_str());
}

bool SaveTextToFile(const char* filename, const char* text) {
	ofstream outputFile(filename);
	if (!outputFile.is_open()) {
		cerr << "Не удалось открыть файл для записи." << endl;
		return false;
	}
	outputFile << text;
	outputFile.close();
	return true;
}

void toLowerCase(char* str) {
	for (int i = 0; str[i] != '\0'; ++i) {
		str[i] = tolower(str[i]);
	}
}

void init257() {
	regex valid_input("^[12]$");
	string input;
	char in_option, out_option;
	char repeat_option = '1';

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

		char* dataBuffer = nullptr;
		if (in_option == '1') {
			if (!readFromConsole(dataBuffer) || !truncateAtExclamation(dataBuffer)) {
				delete[] dataBuffer;
				continue;
			}
		}
		else {
			if (!readFileContents(MyConstants::task257Input, dataBuffer) || !truncateAtExclamation(dataBuffer)) {
				delete[] dataBuffer;
				continue;
			}
		}

		toLowerCase(dataBuffer);
		char* outputBuffer = nullptr;
		if (dataBuffer != nullptr) {
			int spaceCount{ 0 };
			bool foundYu{ false };
			bool foundShina[4]{};
			bool foundNoOrOn{ false };
			bool foundAdjacentIdentical{ false };
			bool patternExists{ false };

			ProcessSequence(dataBuffer, spaceCount, foundYu, foundShina, foundNoOrOn, foundAdjacentIdentical, patternExists);
			PrepareTextForOutput(dataBuffer, outputBuffer, spaceCount, foundYu, foundShina, foundNoOrOn, foundAdjacentIdentical, patternExists);

			if (out_option == '1') {
				cout << "Исходные Данные: " << dataBuffer << endl;
				cout << outputBuffer << endl;
			}
			else if (out_option == '2') {
				cout << "Исходные Данные: " << dataBuffer << endl;
				if (!SaveTextToFile(MyConstants::task257Output, outputBuffer)) {
					delete[] dataBuffer;
					continue;
				}
			}
			delete[] dataBuffer;
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