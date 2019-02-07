#include "pch.h"
#include "SHA512.h"
#include "OneWayHash.h"
#include <bitset>

using namespace std;

string _message;

void GenerateHash(string message)
{
	// Chunk and pad the message
	auto messageLength = message.length();
	if (messageLength > 1024)
	{
		// End of the message must be padded to be a multiple of 1024
		auto excessStartIndex = messageLength - (messageLength % 1024);
		_message += message.substr(0, excessStartIndex);

		auto excessBlock = message.substr(excessStartIndex + 1, messageLength);
		_message += PadMessageBlock(excessBlock);
	}
	else
	{
		_message += PadMessageBlock(message);
	}
}

string PadMessageBlock(string block)
{
	int l = block.length();
	int k = 896 - (l + 1);

	block += "1";
	for (auto i = 0; i < k; ++i)
		block += "0";

	auto lBinary = bitset<8>(l).to_string();

	for (auto i = 0; i < 128 - lBinary.length(); ++i)
		block += "0";

	block += lBinary;
	return block;
}
