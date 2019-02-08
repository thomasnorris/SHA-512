#include "pch.h"
#include "SHA512.h"
#include "OneWayHash.h"
#include <bitset>

using namespace std;

void GenerateHash(string message)
{
	auto messageLength = message.length();
	auto excessStartIndex = messageLength - (messageLength % ONE_THOUSAND_TWENTY_FOUR);
	auto paddedMessage = message.substr(0, excessStartIndex);

	auto excessBlock = message.substr(excessStartIndex, messageLength);
	paddedMessage += PadMessageBlock(excessBlock);

	// The message is chunked into 1024 bit blocks which are split into 64 bit groups
	vector<vector<string>> blocks;
	for (auto i = 0; i < paddedMessage.length() / ONE_THOUSAND_TWENTY_FOUR; ++i)
	{
		int startIndex;

		i == 0 ? startIndex = 0 : startIndex = i * ONE_THOUSAND_TWENTY_FOUR;
		string block = paddedMessage.substr(startIndex, ONE_THOUSAND_TWENTY_FOUR);
		vector<string> groups;

		for (auto j = 0; j < SIXTEEN; j++)
		{
			j == 0 ? startIndex = 0 : startIndex = j * SIXTY_FOUR;
			string group = block.substr(startIndex, SIXTY_FOUR);
			groups.push_back(group);
		}

		blocks.push_back(groups);
	}
}

string PadMessageBlock(string block)
{
	// Variable names come from the SHA-512 documentation and pseudo-code
	int l = block.length();
	int k = 896 - (l + 1);

	block += "1";
	for (auto i = 0; i < k; ++i)
		block += "0";

	auto lBinary = bitset<EIGHT>(l).to_string();

	for (auto i = 0; i < ONE_HUNDRED_TWENTY_EIGHT - lBinary.length(); ++i)
		block += "0";

	block += lBinary;
	return block;
}
