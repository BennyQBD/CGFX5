#pragma once

#include "core/common.hpp"

class SDLApplication
{
public:
	static SDLApplication* create();
	
	virtual ~SDLApplication();
	virtual void processMessages(double delta);
	virtual bool isRunning();
private:
	bool isAppRunning;
	static uint32 numInstances;

	SDLApplication();
};
