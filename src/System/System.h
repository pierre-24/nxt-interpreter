/*
 *  System.h
 *  mindstormssimulation
 *
 *  Created by Torsten Kammer on 26.04.10
 *  Copyright 2010 RWTH Aachen University All rights reserved.
 *
 */

#include <cstdint>
#include "../utils/Time.h"

class NetworkInterface;
class VMMemory;
class VFileSystem;
class RXEFile;


class System
{
	NetworkInterface *networkInterface;
	VMMemory *memory;
	VFileSystem* fileSystem;
	const RXEFile* file;
	
	uint8_t lowspeedOutputBuffer[16];
	int bytesReady;

    unsigned ticksSinceStart;
    const unsigned ticksPerMillisecond = 100;
	
	// Debug only
	static const char *nameForInputPartID(unsigned ID);
	static const char *nameForOutputPartID(unsigned ID);
	
	// Communication (mainly with ultrasound sensor)
	int CheckLSStatus(int port, int &bytesReady);
	int LSRead(int port, uint8_t *buffer, int bufferLength);
	int LSWrite(int port, const uint8_t *buffer, int bufferLength, int bytesExpectedBack);
	
	// Sound support: Makes sure no invalid filenames get used.
	bool sanitizeFilename(unsigned dstocEntry, char *bufferOut);

	// file support
    unsigned int FileOpenRead(unsigned param); // 0x00
    unsigned int FileOpenWrite(unsigned param); // 0x01
    unsigned int FileOpenAppend(unsigned param); // 0x02
    unsigned int FileRead(unsigned param); // 0x03
    unsigned int FileWrite(unsigned param); // 0x04
    unsigned int FileClose(unsigned param); // 0x05
    unsigned int FileResolveHandle(unsigned param); // 0x06
    unsigned int FileRename(unsigned param); // 0x07
    unsigned int FileDelete(unsigned param); // 0x08
	
public:
	System(const RXEFile* file);
	
	void setNetworkInterface(NetworkInterface *anInterface) { networkInterface = anInterface; };
	
	// Called by interpreted code
	void syscall(unsigned callID, unsigned paramClusterDSTOCIndex);
	
	void setInputConfiguration(unsigned port, unsigned property, unsigned value);
	unsigned getInputConfiguration(unsigned port, unsigned property);
	
	void setOutputConfiguration(unsigned port, unsigned property, unsigned value);
	int getOutputConfiguration(unsigned port, unsigned property);
	
	unsigned getTimeSinceStart() const { return ticksSinceStart / ticksPerMillisecond; }

	unsigned ticksPerMilis() const { return ticksPerMillisecond; }

	void tickIt() { ticksSinceStart += 1; }

	void tickIt(unsigned value) { ticksSinceStart += value; }

    unsigned getTicks() const { return ticksSinceStart; }

	VMMemory* getMemory() { return memory; }
};
