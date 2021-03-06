/*
 *  ExecutionContext.h
 *  mindstormssimulation
 *
 *  Created by Torsten Kammer on 25.06.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdexcept>
#include <string>

class Interpreter;
class NetworkInterface;
class RXEFile;
class System;
class VMMemory;

class ExecutionContext
{
private:
	std::string filename;

	RXEFile *file;
	System *system;
	Interpreter *interpreter;
	NetworkInterface *networkInterface;

	bool isPaused;
	
	void load() noexcept(false);
	
public:
	/*!
	 * @abstract Creates the system objects for the robot.
	 * @discussion This creates all the files necessary for running the robot.
	 * In particular, it initializes the RXEFile, VMMemory, System and Interpreter
	 * @param filename The name of the RXE file. Must not be NULL.
	 * @throws std::runtime_error For the reasons given in the classes RXEFile,
	 * VMMemory, System and Interpreter, or if a NULL filename is passed.
	 */
	ExecutionContext(const char *filename) noexcept(false);
	
	/*! Destructor. */
	virtual ~ExecutionContext();	
	
	/*!
	 * @abstract Executes code for the specified number of seconds.
	 * Return false if there is nothing more to execute.
	 * @discussion Executes Lego bytecode until at least the time specified here
	 * has run out. If no interpreter exists, it exits immediately, while for
	 * lengthy operations, it can return after significantly more than the
	 * minimal time. On the other hand, if the context is paused, this method
	 * returns immediately.
	 * @param nticks The time to execute for.
	 */
    bool runForTicks(unsigned int nticks);

    bool runForTime(float secs);
	
	/*!
	 * @abstract Sets the interface necessary for communicating with the outside
	 * world.
	 * @discussion All communication, such as setting motors and reading sensor
	 * values, goes through a network interface, which either executes the
	 * request direclty or passes it through the network.
	 */
	void setNetworkInterface(NetworkInterface *anInterface);
	
	/*!
	 * @abstract Sets the context to paused.
	 * @discussion If the context is paused, the runForTicks method does nothing.
	 */
	void setIsPaused(bool pause) throw();
	
	/*!
	 * @abstract Returns whether the execution context is paused.
	 */
	bool getIsPaused() const throw() { return isPaused; }	
	
	/*!
	 * @abstract Reloads the file contents and starts execution from the
	 * beginning.
	 */
	void reload();
	
	/*!
	 * @abstract Returns the current interface.
	 */
	NetworkInterface *getNetworkInterface() { return networkInterface; }

    /*!
     * @abstract Returns the ticks.
     */
    unsigned int getTicks();
};
