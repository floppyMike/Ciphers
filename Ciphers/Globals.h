#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <CustomLibrary/Error.h>
#include <CustomLibrary/utility.h>

enum Flags
{
	E_VERBOSE, E_FILE, E_HEX, E_ALL
};

extern std::bitset<E_ALL> g_commandFlags;