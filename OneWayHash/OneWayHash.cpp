// OneWayHash.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "OneWayHash.h"
#include <fstream>
#include <vector>

using namespace std;

string _fileName;

int main()
{
	cout << "This program will generate an SHA 512 hash for a given file.\nType the name of a file in this directory OR type the full directory path of a file." << endl;
	cin >> _fileName;
	cout << endl;

	vector<char> fileBytes = ReadFileBytes();

	return EXIT_SUCCESS;
}

vector<char> ReadFileBytes()
{
	ifstream file(_fileName, ifstream::ate | ifstream::binary);

	if (!file.good())
		Error("The file \"" + _fileName + "\" was not found.");
	cout << "Reading bytes from file \"" << _fileName << "\"" << endl;

	const ifstream::pos_type position = file.tellg();
	vector<char> bytes(position);

	file.seekg(0, ios::beg);
	file.read(&bytes[0], position);

	return bytes;
}

void Error(string message)
{
	cout << message << endl;
	system("pause");
	exit(EXIT_FAILURE);
}
