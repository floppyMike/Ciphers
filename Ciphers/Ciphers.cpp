// Ciphers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Globals.h"
#include "Utility.h"

struct Decrypt {};
struct Encrypt {};

template<typename Type>
auto vigenere(std::string&&, const char* const);
auto vernam(std::string&&, const char* const);

int main(int argc, char** argv)
{
	for (size_t i = 0; i < argc; ++i)

		if (::strcmp(argv[i], "-v"))
			g_commandFlags[VERBOSE] = true;

		else if (::strcmp(argv[i], "-f"))
			g_commandFlags[FILE] = true;

		else if (::strcmp(argv[i], "-h"))
			g_commandFlags[HEX] = true;

	if (argc < 5)
	{
		printError();
		return 1;
	}


	std::ifstream file(argv[3], std::ios::binary | std::ios::in);
	std::string result = g_commandFlags[FILE] && file ? ctl::getInputStreamContent(file) : std::string(argv[3]);

	if (::strcmp(argv[1], "vigenere") == 0)

		if (::strcmp(argv[2], "encrypt") == 0)
			result = vigenere<Encrypt>(std::move(result), argv[4]);
		else if (::strcmp(argv[2], "decrypt") == 0)
			result = vigenere<Decrypt>(std::move(result), argv[4]);
		else
		{
			printError();
			return 1;
		}

	else if (::strcmp(argv[1], "vernam") == 0)
	{
		if (::strcmp(argv[2], "encrypt") == 0)
			result = vernam(file ? ctl::getInputStreamContent(file) : std::string(argv[3]), argv[4]);
		else if (::strcmp(argv[2], "decrypt") == 0)
			result = vernam(file ? ctl::getInputStreamContent(file) : std::string(argv[3]), argv[4]);
		else
		{
			printError();
			return 1;
		}
	}

	else
	{
		printError();
		return 1;
	}

	std::cout << "Result: ";
	for (auto& i : result)
	{
		std::cout << std::hex << +i;
	}
	std::cout << '\n';
	return 0;
}

auto vernam(std::string&& str, const char* const key)
{
	str = hexToString(str.c_str());
	const auto genKey = resizeKey(hexToString(key).c_str(), str.size());

	auto iterKey = genKey.begin();
	for (auto iterCipher = str.begin(); iterCipher != str.end(); ++iterCipher, ++iterKey)
		* iterCipher ^= *iterKey;

	return str;
}

template<typename Type>
auto vigenere(std::string&& str, const char* const key)
{
	const auto genKey = resizeKey(key, str.size());

	auto iterKey = genKey.begin();
	for (auto iterCipher = str.begin(); iterCipher != str.end(); ++iterCipher, ++iterKey)
		if constexpr (std::is_same_v<Type, Encrypt>)
			* iterCipher = (*iterCipher + *iterKey) % 26 + 'A';
		else
			*iterCipher = (*iterCipher - *iterKey + 26) % 26 + 'A';

	return str;
}
