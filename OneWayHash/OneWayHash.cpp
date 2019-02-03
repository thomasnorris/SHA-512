// OneWayHash.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "OneWayHash.h"
#include <fstream>
#include <vector>

using namespace std;

int main()
{
	vector<char> fileBytes = ReadFile("InputFile.txt");

	return EXIT_SUCCESS;
}

vector<char> ReadFile(string fileName)
{
	ifstream file(fileName, ifstream::ate | ifstream::binary);

	if (!file.good())
		Error("The file was not found.");

	const ifstream::pos_type position = file.tellg();
	vector<char> bytes(position);

	file.seekg(0, ios::beg);
	file.read(&bytes[0], position);

	return bytes;
}

void Error(string message)
{
	cout << message;
	exit(0);
}
