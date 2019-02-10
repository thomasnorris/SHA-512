#include "pch.h"
#include "SHA512.h"
#include "OneWayHash.h"
#include <bitset>
#include <sstream>

using namespace std;

// Algorithms and some variable names for padding and hash generation come from the 
// documentation and pseudo-code found here: http://dx.doi.org/10.6028/NIST.FIPS.180-4

string GenerateHash(string message)
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
	// K = 80 values in CONSTANTS

	// Start hash algorithm
	auto N = messageBlocks.size();
	auto M = messageBlocks;
	auto K = CONSTANTS;
	vector<string> H;
	for (auto i = 0; i < N; i++)
	{
		// Prepare message schedule W
		vector<string> W;
		for (auto t = 0; t <= 15; t++)
			W.push_back(M[i][t]);

		for (auto t = 16; t <= 79; t++)
		{
			auto temp1 = Add(Sigma1(W[t - 2]), W[t - 7]);
			auto temp2 = Add(temp1, Sigma0(W[t - 15]));
			W.push_back(Add(temp2, W[t - 16]));
		}

		// Initialize eight working variables a - h
		if (i == 0)
			for (auto j = 0; j < EIGHT; j++)
				H.push_back(ConvertUnsignedLongLongToBinaryString(INITIAL_HASH[j]));

		auto a = H[0];
		auto b = H[1];
		auto c = H[2];
		auto d = H[3];
		auto e = H[4];
		auto f = H[5];
		auto g = H[6];
		auto h = H[7];

		// Compute T1, T2, rearrange working variables
		for (auto t = 0; t <= 79; t++)
		{
			auto temp1 = Add(h, Summation1(e));
			auto temp2 = Add(temp1, Ch(e, f, g));
			auto temp3 = Add(temp2, ConvertUnsignedLongLongToBinaryString(K[t]));
			auto T1 = Add(temp3, W[t]);
			auto T2 = Add(Summation0(a), Maj(a, b, c));

			h = g;
			g = f;
			f = e;
			e = Add(d, T1);
			d = c;
			c = b;
			b = a;
			a = Add(T1, T2);
		}

		// Compute i'th intermediate hash value H[i]
		H[0] = Add(a, H[0]);
		H[1] = Add(b, H[1]);
		H[2] = Add(c, H[2]);
		H[3] = Add(d, H[3]);
		H[4] = Add(e, H[4]);
		H[5] = Add(f, H[5]);
		H[6] = Add(g, H[6]);
		H[7] = Add(h, H[7]);
	}

	string hash;
	for (auto i = 0; i < EIGHT; i++)
		hash += ConvertBinaryStringToHexString(H[i]);

	return hash;
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
	for (auto i = 0; i < paddedMessage.length() / ONE_THOUSAND_TWENTY_FOUR; i++)
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

string ConvertBinaryStringToHexString(string toConvert)
{
	stringstream stream;
	auto converted = ConvertBinaryStringToUnsignedLongLong(toConvert);
	stream << hex << converted;
	return stream.str();
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

	auto binary = bitset<EIGHT>(l).to_string();

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

string Add(string x1, string x2)
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

