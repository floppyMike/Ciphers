#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <CustomLibrary/Error.h>
#include <CustomLibrary/utility.h>

enum Flags
{
	VERBOSE, FILE, HEX, ALL
};
std::bitset<ALL> g_commandFlags;