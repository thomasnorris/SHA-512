#pragma once
#include <string>
#include <vector>

using namespace std;

vector<char> ReadFileBytes();
void Error(string message);
void Print(string message = "");
