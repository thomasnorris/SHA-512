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
	Print("This program will generate an SHA 512 hash for a given file.\nType the name of a file (including extension) in this directory OR type the full directory path of a file.");
	cin >> _fileName;
	Print();

	vector<char> fileBytes = ReadFileBytes();

	system("pause");
	return EXIT_SUCCESS;
}

vector<char> ReadFileBytes()
{
	ifstream file(_fileName, ifstream::ate | ifstream::binary);

	if (!file.good())
		Error("The file \"" + _fileName + "\" was not found.");

	try
	{
		Print("Reading bytes from file \"" + _fileName + "\"");
		const ifstream::pos_type position = file.tellg();
		vector<char> bytes(position);

		file.seekg(0, ios::beg);
		file.read(&bytes[0], position);

		return bytes;
	}
	catch (exception& ex)
	{
		const string message = ex.what();
		Error("An error was thrown reading bytes from the file.\n Error message: " + message);
	}

	// All paths must return a value, so return an empty char array here, even though it should never be hit.
	return vector<char>(0);
}

void Error(string message)
{
	Print(message);
	system("pause");
	exit(EXIT_FAILURE);
}

void Print(string message)
{
	cout << message << endl;
}