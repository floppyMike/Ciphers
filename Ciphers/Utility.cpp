#include "Utility.h"

void printError()
{
	std::cerr << "Usage: <cipher> <direction> <text> <key>\n"
		<< "<cipher> either \"vigenere\", \"vernam\" or \"ceasar\"\n"
		<< "<direction> \"decrypt\" or \"encrypt\"\n"
		<< "<text> text to operate on\n"
		<< "<key> key for cipher\n";
}

std::string resizeKey(const char* const key, const size_t& size)
{
	std::string genKey;

	while (genKey.size() < size)
		genKey.append(key);
	genKey.resize(size);

	return genKey;
}

std::string hexToString(const char* const ch)
{
	std::string newKey;
	newKey.reserve(::strlen(ch) >> 1);
	for (const auto* iter = ch; *iter != '\0'; iter += 2)
		newKey.push_back(::strtoul(std::string_view(iter, 2).data(), nullptr, 16));

	return newKey;
}
