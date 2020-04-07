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
class VFile;
class VFileSystem;
class RXEFile;

namespace SystemConstant {
    const unsigned maxFile = 16; // there is a limit of 16 opened file in the firmware
}

class System
{
	NetworkInterface *networkInterface;
	VMMemory *memory;
	VFileSystem* fileSystem;
	
	uint8_t lowspeedOutputBuffer[16];
	int bytesReady;

	VFile* fileHandlers[SystemConstant::maxFile];
	
	// Debug only
	static const char *nameForInputPartID(unsigned ID);
	static const char *nameForOutputPartID(unsigned ID);
	static const char *nameForSyscall(unsigned ID);
	
	// Communication (mainly with ultrasound sensor)
	int CheckLSStatus(int port, int &bytesReady);
	int LSRead(int port, uint8_t *buffer, int bufferLength);
	int LSWrite(int port, const uint8_t *buffer, int bufferLength, int bytesExpectedBack);
	
	// Sound support: Makes sure no invalid filenames get used.
	bool sanitizeFilename(unsigned dstocEntry, char *bufferOut);
	
public:
	System(const RXEFile* file);
	
	void setNetworkInterface(NetworkInterface *anInterface) { networkInterface = anInterface; };
	
	// Called by interpreted code
	void syscall(unsigned callID, unsigned paramClusterDSTOCIndex);
	
	void setInputConfiguration(unsigned port, unsigned property, unsigned value);
	unsigned getInputConfiguration(unsigned port, unsigned property);
	
	void setOutputConfiguration(unsigned port, unsigned property, unsigned value);
	int getOutputConfiguration(unsigned port, unsigned property);
	
	unsigned getTick();

	VMMemory* getMemory() { return memory; }
};
