#pragma once

#include <algorithm>
#include <string>

namespace perfectpixel { namespace bedrock {

	inline bool hasOption(const char **argv, int argc, const std::string &flag)
	{
		const char **end = argv + argc;
		return std::find(argv, argv + argc, flag) != end;
	}

	inline const char *getOption(const char **argv, int argc, const std::string &option)
	{
		const char **end = argv + argc;
		const char **it = std::find(argv, argv + argc, option);

		if (it != end && ++it != end)
		{
			return *it;
		}

		return nullptr;
	}





} }
