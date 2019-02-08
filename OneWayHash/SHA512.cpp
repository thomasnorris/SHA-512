#include "pch.h"
#include "SHA512.h"
#include "OneWayHash.h"
#include <bitset>

using namespace std;

// Algorithms and some variable names for padding and hash generation come from the 
// documentation and pseudo-code found here: http://dx.doi.org/10.6028/NIST.FIPS.180-4

void GenerateHash(string message)
{
	// Pad the message and turn in into blocks
	message = PadMessage(message);
	auto messageBlocks = ChunkPaddedMessageIntoBlocks(message);

	// Parameter descriptions
	// m = number of bits in a message block = 1024
	// w = number of bits in a word = 64 bits
	// M = padded message
	// N = number of message blocks in padded message
	// M[i] = message block i with a size of m bits
	// M[i][j] = j'th word of the i'th message block. M[i][0] is left-most word of the message block i
	// W[t] = t'th w-bit word of the MESSAGE_SCHEDULE
	// H[i] = i'th hash value, H[0] is the INITIAL_HASH value, H[N] is the final hash value
	// H[i][j] = j'th word of the 'ith hash value. H[i][0] is left-most word of hash value i

	// Start hash algorithm
}

string PadMessage(string message)
{
	auto messageLength = message.length();
	auto excessStartIndex = messageLength - (messageLength % ONE_THOUSAND_TWENTY_FOUR);
	auto paddedMessage = message.substr(0, excessStartIndex);

	auto excessBlock = message.substr(excessStartIndex, messageLength);
	paddedMessage += PadMessageBlock(excessBlock);

	return paddedMessage;
}

vector<vector<string>> ChunkPaddedMessageIntoBlocks(string paddedMessage)
{
	vector<vector<string>> blocks;
	for (auto i = 0; i < paddedMessage.length() / ONE_THOUSAND_TWENTY_FOUR; ++i)
	{
		int startIndex;

		i == 0 ? startIndex = 0 : startIndex = i * ONE_THOUSAND_TWENTY_FOUR;
		string block = paddedMessage.substr(startIndex, ONE_THOUSAND_TWENTY_FOUR);

		vector<string> words;
		for (auto j = 0; j < SIXTEEN; j++)
		{
			j == 0 ? startIndex = 0 : startIndex = j * SIXTY_FOUR;
			string word = block.substr(startIndex, SIXTY_FOUR);
			words.push_back(word);
		}

		blocks.push_back(words);
	}

	return blocks;
}

string PadMessageBlock(string block)
{
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
