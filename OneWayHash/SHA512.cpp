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

	// TODO: delete/change these after completion
	// Parameter descriptions
	// m = number of bits in a message block = 1024
	// w = number of bits in a word = 64 bits
	// M = padded message
	// N = number of message blocks in padded message
	// M[i] = message block i with a size of m bits
	// M[i][j] = j'th word of the i'th message block. M[i][0] is left-most word of the message block i
	// W[t] = t'th w-bit word of the message schedule, generated from the padded message
	// H[i] = i'th hash value, H[0] is the INITIAL_HASH value, H[N] is the final hash value
	// H[i][j] = j'th word of the 'ith hash value. H[i][0] is left-most word of hash value i

	// Start hash algorithm

	int N = messageBlocks.size();
	auto shiftR = Shr("1001101010011010100110101001101010011010100110101001101010011010", 2);
	auto shiftL = Shl("1001101010011010100110101001101010011010100110101001101010011010", 2);
	auto rotr = Rotr("1001101010011010100110101001101010011010100110101001101010011010", 2);
	auto rotl = Rotl("1001101010011010100110101001101010011010100110101001101010011010", 2);
	for (auto i = 1; i <= N; i++)
	{
		// Prepare message schedule W
	}
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

string ConvertUnsignedLongLongToBinaryString(unsigned long long toConvert)
{
	return bitset<SIXTY_FOUR>(toConvert).to_string();
}

unsigned long long ConvertBinaryStringToUnsignedLongLong(string toConvert)
{
	return bitset<SIXTY_FOUR>(toConvert).to_ullong();
}

// Algorithm functions
string PadMessageBlock(string block)
{
	// TODO: delete/change these after completion
	// Parameter descriptions
	// l = message length
	// k = number of zero bits to pad the message with

	int l = block.length();
	int k = EIGHT_HUNDRED_NINETY_SIX - (l + 1);

	block += "1";
	for (auto i = 0; i < k; ++i)
		block += "0";

	auto binary = bitset<EIGHT>(l).to_string();

	for (auto i = 0; i < ONE_HUNDRED_TWENTY_EIGHT - binary.length(); ++i)
		block += "0";

	block += binary;
	return block;
}

string Ch(string x, string y, string z)
{
	return "";
}

string Maj(string x, string y, string z)
{
	return "";
}

string Shr(string x, int n)
{
	auto convertedX = ConvertBinaryStringToUnsignedLongLong(x);
	auto shiftedX = convertedX >> n;
	return ConvertUnsignedLongLongToBinaryString(shiftedX);
}

string Shl(string x, int n)
{
	auto convertedX = ConvertBinaryStringToUnsignedLongLong(x);
	auto shiftedX = convertedX << n;
	return ConvertUnsignedLongLongToBinaryString(shiftedX);
}

string Rotr(string x, int n)
{
	auto lhs = ConvertBinaryStringToUnsignedLongLong(Shr(x, n));
	auto rhs = ConvertBinaryStringToUnsignedLongLong(Shl(x, SIXTY_FOUR - n));
	return ConvertUnsignedLongLongToBinaryString(lhs | rhs);
}

string Rotl(string x, int n)
{
	auto lhs = ConvertBinaryStringToUnsignedLongLong(Shl(x, n));
	auto rhs = ConvertBinaryStringToUnsignedLongLong(Shr(x, SIXTY_FOUR - n));
	return ConvertUnsignedLongLongToBinaryString(lhs | rhs);
}
