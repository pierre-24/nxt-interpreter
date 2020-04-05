/*
 *  ExecutionContext.cpp
 *  mindstormssimulation
 *
 *  Created by Torsten Kammer on 25.06.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "ExecutionContext.h"

#include <iostream>

#include "../Robot/NetworkInterface.h"
#include "../Interpreter/Interpreter.h"
#include "RXEFile.h"
#include "../System/System.h"
#include "../utils/Time.h"
#include "../System/VMMemory.h"

ExecutionContext::ExecutionContext(const char *aFilename) noexcept(false)
: filename(aFilename), file(nullptr), memory(nullptr), system(nullptr), interpreter(nullptr), networkInterface(nullptr)
{
	if (!aFilename) throw std::runtime_error("Filename is NULL");
	
	isPaused = false;
	
	load();
}

ExecutionContext::~ExecutionContext()
{
	delete interpreter;
	delete system;
	delete memory;
	delete file;
}

void ExecutionContext::load() noexcept(false)
{
	try {
		file = new RXEFile(filename.c_str());
		memory = new VMMemory(file);
		system = new System(memory);
		interpreter = new Interpreter(file, memory, system);
	}
	catch (std::runtime_error& e)
	{
		delete interpreter;
		delete system;
		delete memory;
		delete file;
		
		throw e;
	}
	
}

void ExecutionContext::reload()
{
	if (!this) return;
	
	delete interpreter;
	delete system;
	delete memory;
	delete file;
	
	load();
	
	if (networkInterface) system->setNetworkInterface(networkInterface);
}

bool ExecutionContext::runForTime(float mintime) noexcept(false)
{
	if (isPaused) return true;
	
	unsigned long start = millisecondsSinceStart();
	
	if (interpreter->waitingUntilTick() > start)
		return true;

	bool executable = true;
	while ((float(millisecondsSinceStart()-start)/1000.0f) < mintime && executable)
	    executable = interpreter->step();

    return executable;
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
