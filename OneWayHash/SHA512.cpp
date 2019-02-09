#include "pch.h"
#include "SHA512.h"
#include "OneWayHash.h"
#include <bitset>
#include <sstream>

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
		for (auto j = 0; j < 16; j++)
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

string ConvertBinaryStringToHexString(string toConvert)
{
	string hexString = "0x";
	for (auto i = 0; i < toConvert.length(); i += 4)
	{
		stringstream stream;
		int decimalEquivalent = ConvertBinaryStringToUnsignedLongLong(toConvert.substr(i, 4));
		stream << hex << decimalEquivalent;
		hexString += stream.str();
	}
	return hexString;
}

// Algorithm functions
string PadMessageBlock(string block)
{
	// TODO: delete/change these after completion
	// Parameter descriptions
	// l = message length
	// k = number of zero bits to pad the message with

	int l = block.length();
	int k = 896 - (l + 1);

	block += "1";
	for (auto i = 0; i < k; ++i)
		block += "0";

	auto binary = bitset<8>(l).to_string();

	for (auto i = 0; i < 128 - binary.length(); ++i)
		block += "0";

	block += binary;
	return block;
}

string Ch(string x, string y, string z)
{
	auto xAndY = And(x, y);
	auto notXAndZ = And(Comp(x), z);
	return Xor(xAndY, notXAndZ);
}

string Maj(string x, string y, string z)
{
	auto xAndY = And(x, y);
	auto xAndZ = And(x, z);
	auto yAndZ = And(y, z);
	auto xor1 = Xor(xAndY, xAndZ);
	return Xor(xor1, yAndZ);
}

string Sigma0(string x)
{
	auto xor1 = Xor(Rotr(x, 1), Rotr(x, 8));
	return Xor(xor1, Shr(x, 7));
}

string Sigma1(string x)
{
	auto xor1 = Xor(Rotr(x, 19), Rotr(x, 61));
	return Xor(xor1, Shr(x, 6));
}

string Summation0(string x)
{
	auto xor1 = Xor(Rotr(x, 28), Rotr(x, 34));
	return Xor(xor1, Rotr(x, 39));
}

string Summation1(string x)
{
	auto xor1 = Xor(Rotr(x, 14), Rotr(x, 18));
	return Xor(xor1, Rotr(x, 41));
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

string Xor(string x1, string x2)
{
	auto lhs = ConvertBinaryStringToUnsignedLongLong(x1);
	auto rhs = ConvertBinaryStringToUnsignedLongLong(x2);
	return ConvertUnsignedLongLongToBinaryString(lhs ^ rhs);
}

string Comp(string x)
{
	auto longX = ConvertBinaryStringToUnsignedLongLong(x);
	return ConvertUnsignedLongLongToBinaryString(~longX);
}

string AddModulo2(string x1, string x2)
{
	auto longX1 = ConvertBinaryStringToUnsignedLongLong(x1);
	auto longX2 = ConvertBinaryStringToUnsignedLongLong(x2);
	return ConvertUnsignedLongLongToBinaryString(longX1 + longX2);
}

string And(string x1, string x2)
{
	auto lhs = ConvertBinaryStringToUnsignedLongLong(x1);
	auto rhs = ConvertBinaryStringToUnsignedLongLong(x2);
	return ConvertUnsignedLongLongToBinaryString(lhs & rhs);
}

