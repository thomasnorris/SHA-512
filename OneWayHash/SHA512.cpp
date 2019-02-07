#include "pch.h"
#include "SHA512.h"
#include "OneWayHash.h"
#include <bitset>

using namespace std;

void GenerateHash(string message)
{
	message = PadMessage(message);
}

string PadMessage(string message)
{
	auto messageLength = message.length();
	string paddedMessage;
	if (messageLength > 1024)
	{
		// End of the message must be padded to be a multiple of 1024
		auto excessStartIndex = messageLength - (messageLength % 1024);
		paddedMessage += message.substr(0, excessStartIndex);

		auto excessBlock = message.substr(excessStartIndex + 1, messageLength);
		paddedMessage += PadMessageBlock(excessBlock);
	}
	else
	{
		paddedMessage += PadMessageBlock(message);
	}

	return paddedMessage;
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
