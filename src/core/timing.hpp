#pragma once

#include "platform/platformTiming.hpp"
#include "core/common.hpp"

namespace Time
{
	inline double getTime()
	{
		return PlatformTiming::getTime();
	}

	inline void sleep(uint32 milliseconds)
	{
		PlatformTiming::sleep(milliseconds);
	}
};

