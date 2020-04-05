/*
 *  SingleRobotNetworkInterface.cpp
 *  mindstormssimulation
 *
 *  Created by Torsten Kammer on 25.06.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "SingleRobotNetworkInterface.h"

#include "Robot.h"
#include "../Environment/Simulation.h"

SingleRobotNetworkInterface::SingleRobotNetworkInterface(Simulation *aSimulation)
: simulation(aSimulation)
{
	robot = new Robot(simulation);
	registerNewRobot(robot);
	simulation->addRobot(robot);
	
	iteratorIsThrough = false;
}

SingleRobotNetworkInterface::~SingleRobotNetworkInterface()
{
	simulation->removeRobot(robot);
	removeRobot(robot);
	
	delete robot;
	robot = nullptr;
}

// Network interface
void SingleRobotNetworkInterface::update()
{
	// Ignore
}

void SingleRobotNetworkInterface::playTone(unsigned frequency, unsigned duration, bool loops, float gain)
{
	robot->playTone(frequency, duration, loops, gain);
}

void SingleRobotNetworkInterface::playFile(const char *name, bool loops, float gain)
{
	robot->playFile(name, loops, gain);
}

void SingleRobotNetworkInterface::setIsLifted(bool isRaised) throw()
{
	robot->setIsLifted(isRaised);
}

void SingleRobotNetworkInterface::moveLifted(const union float4 &diff) throw()
{
	robot->moveDirectly(diff);
}

void SingleRobotNetworkInterface::rotateLifted(float radians) throw()
{	
	robot->rotate(radians);
}

void SingleRobotNetworkInterface::setRobotTurnSpeed(float speed) throw()
{
	robot->setLiftedTurnSpeed(speed);
}

void SingleRobotNetworkInterface::updatedCellState(unsigned x, unsigned z)
{
	// Ignore
}

void SingleRobotNetworkInterface::setSensorAngle(unsigned sensor, float angleInDegrees) throw(std::out_of_range)
{
	robot->setSensorAngle(sensor, angleInDegrees);
}

void SingleRobotNetworkInterface::setIsSensorPointedDown(unsigned sensor, float isit) throw(std::out_of_range)
{
	robot->setIsSensorPointedDown(sensor, isit);
}

void SingleRobotNetworkInterface::setSensorType(unsigned sensor, Robot::SensorType type) throw(std::invalid_argument)
{
	robot->setSensorType(sensor, type);
}

Robot *SingleRobotNetworkInterface::getNextRobot() throw()
{
	iteratorIsThrough = !iteratorIsThrough;
	
	if (!iteratorIsThrough) return NULL;
	else return robot;
}

Robot *SingleRobotNetworkInterface::getLocalModifiableRobot() throw()
{
	return robot;
}

const Robot *SingleRobotNetworkInterface::getLocalRobot() const throw()
{
	return robot;
}

