// OneWayHash.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "OneWayHash.h"
#include <fstream>
#include <vector>
#include <bitset>
#include "SHA512.h"

using namespace std;

int main()
{
	string fileName;
	PrintToConsole("This program will generate an SHA-512 hash for a given file.\nType the name of a file (case sensitive, including extension) in this directory OR type the full directory path of a file:");
	cin >> fileName;
	PrintToConsole();

	auto message = GetBitMessageFromFile(fileName);

	PrintToConsole("The SHA-512 hash value is:\n" + GenerateHash(message) + "\n");

	system("pause");
	return EXIT_SUCCESS;
}

string GetBitMessageFromFile(string fileName)
{
	ifstream file(fileName, ifstream::ate | ifstream::binary);

	if (!file.good())
		Error("The file \"" + fileName + "\" was not found.");

	try
	{
		PrintToConsole("Reading file \"" + fileName + "\"...");
		const ifstream::pos_type position = file.tellg();
		vector<char> bytes(position);

		file.seekg(0, ios::beg);
		file.read(&bytes[0], position);

		string message;
		for (auto it = bytes.begin(); it != bytes.end(); ++it)
		{
			string val = to_string(*it);
			message += bitset<EIGHT>(stoi(val)).to_string();
		}

		return message;
	}
	catch (exception& ex)
	{
		const string message = ex.what();
		Error("An error occurred reading the file.\n Error message: " + message);
	}

	// All paths must return a value, so return an empty char array here, even though it should never be hit.
	return "";
}

void Error(string message)
{
	PrintToConsole(message);
	system("pause");
	exit(EXIT_FAILURE);
}

void PrintToConsole(string message)
{
	cout << message << endl;
}
