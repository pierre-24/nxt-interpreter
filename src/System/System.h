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
	void FileOpenRead(unsigned param); // 0x00
    void FileOpenWrite(unsigned param); // 0x01
    void FileOpenAppend(unsigned param); // 0x02
    void FileRead(unsigned param); // 0x03
    void FileWrite(unsigned param); // 0x04
    void FileClose(unsigned param); // 0x05
    void FileResolveHandle(unsigned param); // 0x06
    void FileRename(unsigned param); // 0x07
    void FileDelete(unsigned param); // 0x08
	
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
