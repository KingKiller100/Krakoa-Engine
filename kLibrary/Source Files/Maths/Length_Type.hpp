#pragma once

#include "../HelperMacros.hpp"

namespace kmaths
{
	using Length_Type = unsigned short;

#ifdef WIN_32
	using Big_Int_Type = int;
#else
	using Big_Int_Type = long long;
#endif
}