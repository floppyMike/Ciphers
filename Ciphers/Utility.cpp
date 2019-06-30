#include "Utility.h"

void printError()
{
	std::cerr << "Usage: <cipher> <direction> <text> <key> [flags]\n"
		<< "<cipher> either \"vigenere\", \"vernam\" or \"ceasar\"\n"
		<< "<direction> \"decrypt\" or \"encrypt\"\n"
		<< "<text> text to operate on\n"
		<< "<key> key for cipher\n"
		<< "[flags] -f text may be a file location and output is also locatated in \"result.txt\"\n"
		<< "	-h text, key and output are inputted and outputed as hex";
}

std::string createKey(const char* const key, const size_t& size)
{
	std::string genKey = g_commandFlags[E_HEX] ? hexToString(key) : key;
	std::string_view beg(genKey);

	while (genKey.size() < size)
		genKey.append(beg);
	genKey.resize(size);

	return genKey;
}

std::string hexToString(const char* const ch)
{
	std::string newKey;
	newKey.reserve(::strlen(ch) >> 1);
	for (const auto* iter = ch; *iter != '\0'; iter += 2)
	{
		char str[] = { *iter, *(iter + 1), '\0' };
		newKey.push_back(::strtol(str, nullptr, 16));
	}

	return newKey;
}
