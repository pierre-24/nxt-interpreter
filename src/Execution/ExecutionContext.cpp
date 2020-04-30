/*
 *  ExecutionContext.cpp
 *  mindstormssimulation
 *
 *  Created by Torsten Kammer on 25.06.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ExecutionContext.h"

#include "../Robot/NetworkInterface.h"
#include "../Interpreter/Interpreter.h"
#include "../System/System.h"
#include "RXEFile.h"

ExecutionContext::ExecutionContext(const char *aFilename) noexcept(false)
: filename(aFilename), file(nullptr), system(nullptr), interpreter(nullptr), networkInterface(nullptr)
{
	if (!aFilename) throw std::runtime_error("Filename is NULL");
	
	isPaused = false;
	
	load();
}

ExecutionContext::~ExecutionContext()
{
	delete interpreter;
	delete system;
	delete file;
}

void ExecutionContext::load() noexcept(false)
{
	try {
		file = new RXEFile(filename.c_str());
		system = new System(file);
		interpreter = new Interpreter(file, system);
	}
	catch (std::runtime_error& e)
	{
		delete interpreter;
		delete system;
		delete file;

		throw e;
	}
	
}

void ExecutionContext::reload()
{
	delete interpreter;
	delete system;
	
	load();
	
	if (networkInterface) system->setNetworkInterface(networkInterface);
}

bool ExecutionContext::runForTicks(unsigned int nticks) noexcept(false)
{
	if (isPaused) return true;
    return interpreter->runForTicks(nticks);
}

bool ExecutionContext::runForTime(float secs) noexcept(false)
{
    if (isPaused) return true;
    return interpreter->runForTicks(unsigned(secs * 1000.f * float(system->ticksPerMilis())));
}

void ExecutionContext::setIsPaused(bool pause) throw()
{
	isPaused = pause;
	networkInterface->setIsPaused(isPaused);
}

void ExecutionContext::setNetworkInterface(NetworkInterface *anInterface)
{
	networkInterface = anInterface;
	system->setNetworkInterface(networkInterface);
}

unsigned int ExecutionContext::getTicks() {
    return this->system->getTicks();
}
