#ifndef TIMING_HPP
#define TIMING_HPP

#include "core/common.hpp"

struct SDLTiming
{
	static double getTime();
	static void sleep(uint32 milliseconds);
};

#endif
