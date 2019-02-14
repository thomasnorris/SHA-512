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
	// Pad and convert message into 1024-bit blocks
	auto M = PadAndChunkMessage(message);
	// Create vector to hold intermediate hash values for each iteration
	vector<string> H;
	for (auto i = 0; i < M.size(); i++)
	{
		// Prepare message schedule W
		vector<string> W;
		for (auto t = 0; t <= 15; t++)
			W.push_back(M[i][t]);

		for (auto t = 16; t <= 79; t++)
		{
			auto temp1 = BitOp(ADD, Sigma1(W[t - 2]), W[t - 7]);
			auto temp2 = BitOp(ADD, temp1, Sigma0(W[t - 15]));
			W.push_back(BitOp(ADD, temp2, W[t - 16]));
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
			auto temp1 = BitOp(ADD,h, Summation1(e));
			auto temp2 = BitOp(ADD, temp1, Ch(e, f, g));
			auto temp3 = BitOp(ADD, temp2, ConvertUnsignedLongLongToBinaryString(CONSTANTS[t]));
			auto T1 = BitOp(ADD, temp3, W[t]);
			auto T2 = BitOp(ADD, Summation0(a), Maj(a, b, c));

			h = g;
			g = f;
			f = e;
			e = BitOp(ADD, d, T1);
			d = c;
			c = b;
			b = a;
			a = BitOp(ADD, T1, T2);
		}

		// Compute i'th intermediate hash value H[i]
		H[0] = BitOp(ADD, a, H[0]);
		H[1] = BitOp(ADD, b, H[1]);
		H[2] = BitOp(ADD, c, H[2]);
		H[3] = BitOp(ADD, d, H[3]);
		H[4] = BitOp(ADD, e, H[4]);
		H[5] = BitOp(ADD, f, H[5]);
		H[6] = BitOp(ADD, g, H[6]);
		H[7] = BitOp(ADD, h, H[7]);
	}

	string hash;
	for (auto i = 0; i < EIGHT; i++)
		hash += ConvertBinaryStringToHexString(H[i]);

	return hash;
}

vector<vector<string>> PadAndChunkMessage(string message)
{
	// l = message length
	// k = number of zeroes to pad the message with
	int l = message.length();
	int k = ONE_THOUSAND_TWENTY_FOUR - (l % ONE_THOUSAND_TWENTY_FOUR);

	string binary;
	while (l > 0)
	{
		binary.insert(0, to_string(l % 2));
		l = l / 2;
	}

	k -= binary.length();

	// Add 1 to the message and then add k - 1 zeroes
	message += "1";
	for (auto i = 1; i < k; ++i)
		message += "0";

	message += binary;

	// Chunk the message into m-bit blocks that is chunked into w-bit binary words
	vector<vector<string>> blocks;
	for (auto i = 0; i < message.length() / ONE_THOUSAND_TWENTY_FOUR; i++)
	{
		int startIndex;

		i == 0 ? startIndex = 0 : startIndex = i * ONE_THOUSAND_TWENTY_FOUR;
		string block = message.substr(startIndex, ONE_THOUSAND_TWENTY_FOUR);

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
	for (auto i = 0; i < toConvert.length(); i+= 4)
	{
		auto converted = ConvertBinaryStringToUnsignedLongLong(toConvert.substr(i, 4));
		stream << hex << converted;
	}

	return stream.str();
}

string BitOp(BitOpEnum bitOp, string x1, string x2, int n)
{
	auto longX1 = ConvertBinaryStringToUnsignedLongLong(x1);
	auto longX2 = ConvertBinaryStringToUnsignedLongLong(x2);
	unsigned long long result = 0;
	switch (bitOp)
	{
	case SHR:
	{
		result = longX1 >> n;
		break;
	}
	case SHL:
	{
		result = longX1 << n;
		break;
	}
	case NOT:
	{
		result = ~longX1;
		break;
	}
	case ADD:
	{
		result = longX1 + longX2;
		break;
	}
	case AND:
	{
		result = longX1 & longX2;
		break;
	}
	case XOR:
	{
		result = longX1 ^ longX2;
		break;
	}
	}

	return ConvertUnsignedLongLongToBinaryString(result);
}

// Algorithm functions
string Ch(string x, string y, string z)
{
	auto xAndY = BitOp(AND, x, y);
	auto notXAndZ = BitOp(AND, BitOp(NOT, x), z);
	return BitOp(XOR, xAndY, notXAndZ);
}

string Maj(string x, string y, string z)
{
	auto xAndY = BitOp(AND, x, y);
	auto xAndZ = BitOp(AND, x, z);
	auto yAndZ = BitOp(AND, y, z);
	auto xor1 = BitOp(XOR, xAndY, xAndZ);
	return BitOp(XOR, xor1, yAndZ);
}

string Sigma0(string x)
{
	auto xor1 = BitOp(XOR, Rotr(x, 1), Rotr(x, 8));
	return BitOp(XOR, xor1, BitOp(SHR, x, "", 7));
}

string Sigma1(string x)
{
	auto xor1 = BitOp(XOR, Rotr(x, 19), Rotr(x, 61));
	return BitOp(XOR, xor1, BitOp(SHR, x, "", 6));
}

string Summation0(string x)
{
	auto xor1 = BitOp(XOR, Rotr(x, 28), Rotr(x, 34));
	return BitOp(XOR, xor1, Rotr(x, 39));
}

string Summation1(string x)
{
	auto xor1 = BitOp(XOR,Rotr(x, 14), Rotr(x, 18));
	return BitOp(XOR, xor1, Rotr(x, 41));
}

string Rotr(string x, int n)
{
	auto lhs = ConvertBinaryStringToUnsignedLongLong(BitOp(SHR, x, "", n));
	auto rhs = ConvertBinaryStringToUnsignedLongLong(BitOp(SHL, x, "", SIXTY_FOUR - n));
	return ConvertUnsignedLongLongToBinaryString(lhs | rhs);
}