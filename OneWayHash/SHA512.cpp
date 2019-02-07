#include "pch.h"
#include "SHA512.h"
#include "OneWayHash.h"
#include <bitset>

using namespace std;

void GenerateHash(string message)
{
	// Pad the message
	int l = message.length();
	int k = 896 - (l + 1);

	message += "1";
	for (auto i = 0; i < k; ++i)
	{
		message += "0";
	}

	auto lBinary = bitset<8>(l).to_string();

	for (auto i = 0; i < 128 - lBinary.length(); ++i)
	{
		message += "0";
	}

	message += lBinary;

	PrintToConsole(std::to_string(message.length()));
}
