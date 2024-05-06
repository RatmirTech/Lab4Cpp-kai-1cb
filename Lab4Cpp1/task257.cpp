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
		if (sequence[i] == '�') foundYu = true;
		if (sequence[i] == '�') foundShina[0] = true;
		if (sequence[i] == '�') foundShina[1] = true;
		if (sequence[i] == '�') foundShina[2] = true;
		if (sequence[i] == '�') foundShina[3] = true;

		if (prevChar) {
			if ((*prevChar == '�' && sequence[i] == '�') || (*prevChar == '�' && sequence[i] == '�'))
				foundNoOrOn = true;
			if (*prevChar == sequence[i])
				foundAdjacentIdentical = true;
		}

		prevChar = &sequence[i];
	}

	// ������� �
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
		cout << "������ ��� �������� ����� '" << filename << "'" << endl;
		return false;
	}

	streamsize size = file.tellg();
	file.seekg(0, ios::beg);

	buffer = new (nothrow) char[size + 1];
	if (buffer == nullptr) {
		cout << "�� ������� �������� ������ ��� ������ �����" << endl;
		file.close();
		return false;
	}

	if (!file.read(buffer, size)) {
		cout << "������ ��� ������ �����" << endl;
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
		cout << "����� �� ���������������." << endl;
		return false;
	}

	char* pos = strchr(buffer, '!');
	if (pos != nullptr) {
		if (pos == buffer) {
			cout << "��������������� ���� ��������� � ������ ������." << endl;
			delete[] buffer;
			buffer = nullptr;
			return false;
		}
		*pos = '\0';  // ������� ������ �� !
	}
	else {
		cout << "��������������� ���� �� ������." << endl;
		delete[] buffer;
		buffer = nullptr;
		return false;
	}
	return true;
}

bool readFromConsole(char*& buffer) {
	cout << "������� ������: ";
	string input;
	getline(cin, input);

	buffer = new (nothrow) char[input.length() + 1];
	if (buffer == nullptr) {
		cout << "�� ������� �������� ������ ��� ����� ������" << endl;
		return false;
	}

	strcpy(buffer, input.c_str());
	return true;
}

void PrepareTextForOutput(const char* sequence, char*& outputBuffer, int spaceCount, bool foundYu, const bool foundShina[], bool foundNoOrOn, bool foundAdjacentIdentical, bool patternExists) {
	string output;
	output += "���������� ��������: " + to_string(spaceCount) + "\n";
	output += "����� '�' " + string(foundYu ? "������������" : "�����������") + "\n";
	output += "����� '����' " + string((foundShina[0] && foundShina[1] && foundShina[2] && foundShina[3]) ? "������������" : "�����������") + "\n";
	output += "���� '��' ��� '��' " + string(foundNoOrOn ? "������������" : "�����������") + "\n";
	output += "���� ���������� �������� �������� " + string(foundAdjacentIdentical ? "������������" : "�����������") + "\n";
	output += "������� e " + string(patternExists ? "���������" : "�� ���������") + "\n";

	size_t outputLength = output.length() + 1;
	outputBuffer = new (nothrow) char[outputLength];
	if (outputBuffer == nullptr) {
		cerr << "������ ��������� ������." << endl;
		return;
	}
	strcpy(outputBuffer, output.c_str());
}

bool SaveTextToFile(const char* filename, const char* text) {
	ofstream outputFile(filename);
	if (!outputFile.is_open()) {
		cerr << "�� ������� ������� ���� ��� ������." << endl;
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
			cout << "������� '1' ��� ����� � �������, '2' ��� ����� �� �����: ";
			getline(cin, input);
		} while (!regex_match(input, valid_input));
		in_option = input[0];

		do {
			cout << "������� '1' ��� ������ �� �������, '2' ��� ������ � ����: ";
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
				cout << "�������� ������: " << dataBuffer << endl;
				cout << outputBuffer << endl;
			}
			else if (out_option == '2') {
				cout << "�������� ������: " << dataBuffer << endl;
				if (!SaveTextToFile(MyConstants::task257Output, outputBuffer)) {
					delete[] dataBuffer;
					continue;
				}
			}
			delete[] dataBuffer;
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