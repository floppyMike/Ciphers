// Ciphers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Globals.h"
#include "Utility.h"

std::bitset<E_ALL> g_commandFlags;

struct Decrypt {};
struct Encrypt {};

template<typename Type>
std::string vigenere(std::string&&, const char* const);
std::string vernam(std::string&&, const char* const);
template<typename Type>
std::string caesar(std::string&&, const char* const);

int main(int argc, char** argv)
{
	//Check for flags
	for (size_t i = 5; i < argc; ++i)

		if (::strcmp(argv[i], "-f") == 0)
			g_commandFlags[E_FILE] = true;

		else if (::strcmp(argv[i], "-h") == 0)
			g_commandFlags[E_HEX] = true;

	//Error if too much
	if (argc < 5)
	{
		printError();
		return 1;
	}

	//Copy from file or input
	std::string result;
	if (g_commandFlags[E_FILE])
	{
		std::ifstream file(argv[3], std::ios::binary | std::ios::in);
		result = file ? ctl::getInputStreamContent(file) : std::string(argv[3]);
	}
	else
		result = std::string(argv[3]);

	//Handle hex conversion
	if (g_commandFlags[E_HEX])
		result = hexToString(result.c_str());

	//Filter and cypher
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

		if (::strcmp(argv[2], "encrypt") == 0 || ::strcmp(argv[2], "decrypt") == 0)
			result = vernam(std::move(result), argv[4]);
		else
		{
			printError();
			return 1;
		}

	else if (::strcmp(argv[1], "caesar") == 0)

		if (::strcmp(argv[2], "encrypt") == 0)
			result = caesar<Encrypt>(std::move(result), argv[4]);
		else if (::strcmp(argv[2], "decrypt") == 0)
			result = caesar<Decrypt>(std::move(result), argv[4]);
		else
		{
			printError();
			return 1;
		}

	else
	{
		printError();
		return 1;
	}

	//Handle flags conversion
	std::ofstream file("result.txt", std::ios::binary | std::ios::out);
	std::cout << "Result: ";
	for (auto& i : result)
	{
		std::string buf;

		if (g_commandFlags[E_HEX])
		{
			auto&& temp = ctl::toHexadecimal(i);
			buf = temp.size() == 1 ? '0' + temp : std::move(temp);
		}
		else
			buf = i;

		if (g_commandFlags[E_FILE])
			file << buf;

		std::cout << buf;
	}

	return 0;
}

std::string vernam(std::string&& str, const char* const key)
{
	const auto genKey = createKey(key, str.size());

	auto iterKey = genKey.begin();
	for (auto iterCipher = str.begin(); iterCipher != str.end(); ++iterCipher, ++iterKey)
		*iterCipher ^= *iterKey;

	return str;
}

template<typename Type>
std::string vigenere(std::string&& str, const char* const key)
{
	const auto genKey = createKey(key, str.size());

	auto iterKey = genKey.begin();
	for (auto iterCipher = str.begin(); iterCipher != str.end(); ++iterCipher, ++iterKey)

		if constexpr (std::is_same_v<Type, Encrypt>)
			*iterCipher = (*iterCipher + *iterKey) % 26 + 'A';

		else if constexpr (std::is_same_v<Type, Decrypt>)
			*iterCipher = (*iterCipher - *iterKey + 26) % 26 + 'A';

		else
			static_assert(true, "vigenere: Wrong type.");

	return str;
}

template<typename Type>
std::string caesar(std::string&& str, const char* const key)
{
	const auto shift = ::strtoul(g_commandFlags[E_HEX] ? hexToString(key).c_str() : key, nullptr, 10);

	for (auto iterCipher = str.begin(); iterCipher != str.end(); ++iterCipher)

		if constexpr (std::is_same_v<Type, Encrypt>)
			if (::isupper(*iterCipher))
				*iterCipher = (*iterCipher + shift - 'A') % 26 + 'A';
			else
				*iterCipher = (*iterCipher + shift - 'a') % 26 + 'a';

		else if constexpr (std::is_same_v<Type, Decrypt>)
			if (::isupper(*iterCipher))
				*iterCipher = (*iterCipher + (26 - shift) - 'A') % 26 + 'A';
			else
				*iterCipher = (*iterCipher + (26 - shift) - 'a') % 26 + 'a';

		else
			static_assert(true, "caesar: Wrong type.");

	return str;
}
