/*
 *  System.cpp
 *  mindstormssimulation
 *
 *  Created by Torsten Kammer on 26.04.10
 *  Copyright 2010 RWTH Aachen University All rights reserved.
 *
 */

#include "System.h"

#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <cstring>

#include "../Robot/NetworkInterface.h"
#include "VMMemory.h"
#include "VFileSystem.h"
#include "../Execution/RXEFile.h"

#include "Syscall.h"
#include "VFile.h"

System::System(const RXEFile* f): networkInterface(nullptr), file(f), ticksSinceStart(0) {
    memory = new VMMemory(f);
    fileSystem = new VFileSystem();
}

const char *System::nameForInputPartID(unsigned ID)
{
	switch (ID)
	{
		case 0x0: return "IO_IN_TYPE";
		case 0x1: return "IO_IN_MODE";
		case 0x2: return "IO_IN_ADRAW";
		case 0x3: return "IO_IN_NORMRAW";
		case 0x4: return "IO_IN_SCALED_VAL";
		case 0x5: return "IO_IN_INVALID_DATA";
		default: return "IO_IN_?";
	}
}

const char *System::nameForOutputPartID(unsigned ID)
{
	switch (ID)
	{
		case 0x0: return "IO_OUT_FLAGS";
		case 0x1: return "IO_OUT_MODE";
		case 0x2: return "IO_OUT_SPEED";
		case 0x3: return "IO_OUT_ACTUAL_SPEED";
		case 0x4: return "IO_OUT_TACH_COUNT";
		case 0x5: return "IO_OUT_TACH_LIMIT";
		case 0x6: return "IO_OUT_RUN_STATE";
		case 0x7: return "IO_OUT_TURN_RATIO";
		case 0x8: return "IO_OUT_REG_MODE";
		case 0x9: return "IO_OUT_OVERLOAD";
		case 0xA: return "IO_OUT_REG_P_VAL";
		case 0xB: return "IO_OUT_REG_I_VAL";
		case 0xC: return "IO_OUT_REG_D_VAL";
		case 0xD: return "IO_OUT_BLOCK_TACH_COUNT";
		case 0xE: return "IO_OUT_ROTATION_COUN";
		default: return "IO_OUT_?";
	}
}

int System::CheckLSStatus(int port, int &howManyBytes)
{
	howManyBytes = bytesReady;
	return 0;
}

int System::LSRead(int port, uint8_t *buffer, int bufferLength)
{
	memcpy(buffer, lowspeedOutputBuffer, bufferLength);
	return 0;
}

int System::LSWrite(int port, const uint8_t *buffer, int bufferLength, int bytesExpectedBack)
{
	if (!networkInterface) return 1;
	
	if (buffer[0] == 0x02 && buffer[1] == 0x42)
	{
		// Command: Read Measurement Byte 0. Just read back whatever is at
		// that port.
		float length = 0.0f;
		
		const Robot *localRobot = networkInterface->getLocalRobot();
		if (localRobot)
			length = std::min(localRobot->getSensorValue(port), 255.0f);
		
		lowspeedOutputBuffer[0] = uint8_t(length);
		bytesReady = 1;
	}
	
	return 0;
}

bool System::sanitizeFilename(unsigned dstocEntry, char *bufferOut)
{
	unsigned length = memory->getArrayLength(dstocEntry);
	
	// Check for size
	if (length > 20) return false;
	
	// Check for Null-Terminated string
	if (memory->getArrayElement(dstocEntry, length-1) != 0) return false;
	
	for (unsigned i = 0; i < (length - 1); i++)
	{
		char character = memory->getArrayElement(dstocEntry, i);
		if (!isalnum(character) && character != ' ' && character != '.' && character != '!' && character != '-' && character != '_')
			bufferOut[i] = '_';
		else
			bufferOut[i] = character;
	}

	bufferOut[length - 1] = 0;
	bufferOut[length - 5] = '.'; // force filename.ext

	return true;
}

void System::syscall(unsigned callID, unsigned params)
{
	if (!networkInterface) return;
	
	int returnValue = 0;
	switch(callID)
	{
        case 0x00:
            returnValue = FileOpenRead(params);
            break;
	    case 0x01:
            returnValue = FileOpenWrite(params);
            break;
        case 0x02:
            returnValue = FileOpenAppend(params);
            break;
        case 0x03:
            returnValue = FileRead(params);
            break;
        case 0x04:
            returnValue = FileWrite(params);
            break;
        case 0x05:
            returnValue = FileClose(params);
            break;
        case 0x06:
            returnValue = FileResolveHandle(params);
            break;
        case 0x07:
            returnValue = FileRename(params);
            break;
        case 0x08:
            returnValue = FileDelete(params);
            break;
		case 0x09: // NXTSoundPlayFile
		{
			// Params:
			// 0: Cluster
			// 1: Status code, out, unused
			// 2: Filename, array of unsigned byte
			// 3: -- type of array
			// 4: Loop
			// 5: Volume
			char filename[20];
			if (sanitizeFilename(params + 2, filename))
				networkInterface->playFile(filename, memory->getScalarValue(params+4), float(memory->getScalarValue(params+5))*0.25f);
			break;
		}
		case 0x0A: // NXTSoundPlayTone
			// Params:
			// 0: Cluster
			// 1: Status code, out, unused
			// 2: Frequency
			// 3: Duration
			// 4: Loop
			// 5: Volume
			networkInterface->playTone(memory->getScalarValue(params+2), memory->getScalarValue(params+3), memory->getScalarValue(params+4), float(memory->getScalarValue(params+5))*0.25f);
			break;
		case 0x15: // NXTCommLSWrite
			// Params:
			// 0: Cluster
			// 1: Status code, out
			// 2: Port
			// 3: Buffer
			// 4: Buffer data type
			// 5: BufferLength
			returnValue = LSWrite(memory->getScalarValue(params+2), reinterpret_cast<const uint8_t *> (memory->getArrayData(params + 3)), memory->getArrayLength(params+3), memory->getScalarValue(params+5));
			break;
		case 0x16: // NXTCommLSRead
		{
			memory->setArrayLength(params+3, bytesReady);
			returnValue = LSRead(memory->getScalarValue(params+2), reinterpret_cast<uint8_t *> (memory->getArrayData(params + 3)), memory->getScalarValue(params+5));
		}
			break;
		case 0x17: // NXTCommLSCheckStatus
		{
			int bytesAvailableForRead;
			returnValue = CheckLSStatus(memory->getScalarValue(params+2), bytesAvailableForRead);
			memory->setScalarValue(params+3, bytesAvailableForRead);
		}
			break;
		default:
			std::cout << "Ignoring syscall " << std::hex << callID << std::dec << " (" << nameForSyscall(callID) << ") " << std::endl;
			break;
	}
	memory->setScalarValue(params+1, returnValue);
}

void System::setInputConfiguration(unsigned port, unsigned property, unsigned value)
{
	if (property == 0x5) // IO_IN_INVALID_DATA
	{
		// Setting invalid data. For the real chip, invalid data will return to
		// true once the changes have been processed. Here, it is always false
		// because we can process changes (if any) immediately.
		return;
	}
	std::cout << "Set input configuration for port " << port << " property " << property << " (" << nameForInputPartID(property) << ") to 0x" << std::hex << value << std::dec << std::endl;
}

unsigned System::getInputConfiguration(unsigned port, unsigned property)
{
	if (property == 0x5) // IO_IN_INVALID_DATA
	{
		// Reading INVALID_DATA. Here, it is always false because changes are
		// processed immediately.
		return 0;
	}
	else if (property == 0x4 && networkInterface) // IO_IN_SCALED_VAL
	{
		const Robot *localRobot = networkInterface->getLocalRobot();
		if (!localRobot) return 0;
		
		float value = localRobot->getSensorValue(port);
		value = std::max(std::min(1.0f, value), 0.0f);
		return unsigned(value*100.0f);
	}
	std::cout << "Asked for value for input configuration for port " << port << " property " << property << " (" << nameForInputPartID(property) << ")" << std::endl;
	return rand() % 2;
}

void System::setOutputConfiguration(unsigned port, unsigned property, unsigned value)
{
	if (!networkInterface) return;
		
	// Read-only and hence ignored:
	// 0x3 IO_OUT_ACTUAL_SPEED
	// 0x4 IO_OUT_TACH_COUNT
	// 0x9 IO_OUT_OVERLOAD
	// 0xD IO_OUT_BLOCK_TACH_COUNT
	// 0xE IO_OUT_ROTATION_COUNT

	// Ignored because they do not make a difference in our physics model:
	// 0x1 IO_OUT_MODE
	// 0x6 IO_OUT_RUN_STATE
	// 0xA IO_OUT_REG_P_VAL
	// 0xB IO_OUT_REG_I_VAL
	// 0xC IO_OUT_REG_D_VAL
	
	if (property == 0x0)
	{
		// IO_OUT_FLAGS
		// Used to update something
		if (value & 0x03)
			networkInterface->commitMotorChanges(port);
		if (value & 0x04)
			networkInterface->commitTurnTargetChanges(port);
		if (value & 0x08)
			networkInterface->resetAllCounters(port);
		if (value & 0x20)
			networkInterface->resetBlockCounter(port);
		if (value & 0x40)
			networkInterface->resetRotationCounter(port);
	}
	else if (property == 0x2)
	{
		// IO_OUT_SPEED
		int8_t valueByte = int8_t(value);
		networkInterface->setMotorPower(port, float(valueByte));
	}
	else if (property == 0x5)
	{
		// IO_OUT_TACH_LIMIT
		networkInterface->setMotorTurnTarget(port, float(value));
	}
	else if (property == 0x7)
	{
		// IO_OUT_TURN_RATIO
		int8_t valueByte = int8_t(value);
		networkInterface->setMotorTurnRatio(port, float(valueByte) / 100.0f);
	}
	else if (property == 0x8)
	{
		// IO_OUT_REG_MODE
		networkInterface->setMotorIsSynchronized(port, value & 0x02);
	}
}

int System::getOutputConfiguration(unsigned port, unsigned property)
{
	const Robot *localRobot = networkInterface->getLocalRobot();
	if (!localRobot) return 0;
		
	// Ignored because they do not make a difference in our physics model:
	// 0xA IO_OUT_REG_P_VAL
	// 0xB IO_OUT_REG_I_VAL
	// 0xC IO_OUT_REG_D_VAL	
	
	if (property == 0x0)
	{
		// IO_OUT_FLAGS
		return 0; // Pretend they get cleared right away.
	}
	else if (property == 0x1)
	{
		// IO_OUT_MODE
		return 0x01; // MOTORON always
	}
	else if (property == 0x2)
	{
		// IO_OUT_SPEED
		return int(localRobot->getMotor(port)->getPower());
	}
	else if (property == 0x3)
	{
		// IO_OUT_ACTUAL_SPEED
		return int(localRobot->getMotor(port)->getPower());
	}
	else if (property == 0x4)
	{
		// IO_OUT_TACH_COUNT
		return int(localRobot->getMotor(port)->getTargetCounterValue());
	}
	else if (property == 0x5)
	{
		// IO_OUT_TACH_LIMIT
		return int(localRobot->getMotor(port)->getTurnTarget());
	}
	else if (property == 0x6)
	{
		// IO_OUT_RUN_STATE
		if (localRobot->getMotor(port)->isRunning()) return 0x20;
		else return 0x0;
	}
	else if (property == 0x7)
	{
		// IO_OUT_TURN_RATIO
		return int(localRobot->getMotor(port)->getTurnFactor() * 100.0f);
	}
	else if (property == 0x8)
	{
		// IO_OUT_REG_MODE
		if (localRobot->getMotorIsSynchronized(port))
			return 0x3;
		else
			return 0x1; // We always regulate the speed.
	}
	else if (property == 0x9)
	{
		// IO_OUT_OVERLOAD
		return 0; // Never overloaded
	}
	else if (property == 0xD)
	{
		// IO_OUT_BLOCK_TACH_COUNT
		return int(localRobot->getMotor(port)->getBlockCounterValue());
	}
	else if (property == 0xE)
	{
		// IO_OUT_ROTATION_COUNT
		return int(localRobot->getMotor(port)->getRotationCounterValue());
	}
	else
		// Ignored
		return 0;
}

// file
unsigned int System::FileOpenWrite(unsigned param) {
    // 1: Return value, uword (out)
    // 2: File handle, ubyte (out)
    // 3: File name, array (in)
    // 4: array type, ubyte
    // 5: Length, ulong (in)

    char fileName[20];
    if (!sanitizeFilename(param+3, fileName))
        return VFileError::IllegalFileName;
    else {
        unsigned handle;
        unsigned status;

        fileSystem->FileOpenWrite(status, handle, fileName, memory->getScalarValue(param + 5));
        memory->setScalarValue(param+2, handle);
        return status;
    }
}

unsigned int System::FileClose(unsigned param) {
    // 1: Return value, uword (out)
    // 2: File handle, ubyte (in)

    unsigned status;
    fileSystem->FileClose(status, memory->getScalarValue(param + 2));
    return status;
}

unsigned int System::FileOpenRead(unsigned param) {
    // 1: Return value, uword (out)
    // 2: File handle, ubyte (out)
    // 3: File name, array (in)
    // 4: array type, ubyte
    // 5: Length, ulong (in)


    char fileName[20];
    if (!sanitizeFilename(param+3, fileName))
        return VFileError::IllegalFileName;
    else {
        unsigned handle;
        unsigned status;

        fileSystem->FileOpenRead(status, handle, fileName, memory->getScalarValue(param + 5));
        memory->setScalarValue(param+2, handle);
        return status;
    }
}

unsigned int System::FileOpenAppend(unsigned param) {
    // 1: Return value, uword (out)
    // 2: File handle, ubyte (out)
    // 3: File name, array (in)
    // 4: array type, ubyte
    // 5: Length, ulong (in)

    char fileName[20];
    if (!sanitizeFilename(param+3, fileName))
        return VFileError::IllegalFileName;
    else {
        unsigned handle;
        unsigned status;

        fileSystem->FileOpenAppend(status, handle, fileName, memory->getScalarValue(param + 5));
        memory->setScalarValue(param+2, handle);
        return status;
    }

}

unsigned int System::FileRead(unsigned param) {
    // 1: Return value, uword (out)
    // 2: File handle, ubyte (in)
    // 3: Buffer, array (out)
    // 4: array type, ubyte
    // 5: Length, ulong (inout)

    unsigned status;
    unsigned length = memory->getScalarValue(param + 5);
    auto* buffer = new char[length];
    fileSystem->FileRead(status, memory->getScalarValue(param + 2), buffer, length);

    memory->setScalarValue(param + 5, length);

    memory->setArrayLength(param + 3, length + 1);
    for (int i = 0; i < length; ++i)
        memory->setArrayElement(param + 3, i, buffer[i]);

    memory->setArrayElement(param+ 3, length, '\0'); // set '\0'
    delete[] buffer;

    return status;
}

unsigned int System::FileWrite(unsigned param) {
    // 1: Return value, uword (out)
    // 2: File handle, ubyte (in)
    // 3: Buffer, array (in)
    // 4: array type, ubyte
    // 5: Length, ulong (inout)

    unsigned status = VFileError::Success;
    unsigned length = memory->getScalarValue(param + 5);
    unsigned handle = memory->getScalarValue(param + 2);

    if(handle > 0) {
        if(length > 0)
            fileSystem->FileWrite(status, memory->getScalarValue(param + 2), reinterpret_cast<char*>(memory->getArrayData(param + 3)), length);
    } else { // directly into standard output
        char* buffer = new char[length + 1];
        memcpy(buffer, reinterpret_cast<char*>(memory->getArrayData(param + 3)), length);
        buffer[length] = '\0';
        std::cout << buffer;
        delete[] buffer;
    }

    memory->setScalarValue(param + 5, length);
    return status;
}

unsigned int System::FileResolveHandle(unsigned param) {
    // 1: Return value, uword (out)
    // 2: File handle, ubyte (out)
    // 3: write ?, ubyte (out)
    // 4, File name, array (in)

    unsigned status, handle;
    bool write;
    fileSystem->FileResolveHandle(status, handle, write, reinterpret_cast<char*>(memory->getArrayData(param + 4)));

    memory->setScalarValue(param + 2, handle);
    memory->setScalarValue(param + 3, write);
    return status;
}

unsigned int System::FileRename(unsigned param) {
    // 1: Return value, uword (out)
    // 2: Old filename, array (in)
    // 3: array type, ubyte
    // 4: New filename, array (in)

    unsigned status;
    fileSystem->FileRename(status, reinterpret_cast<char*>(memory->getArrayData(param + 2)), reinterpret_cast<char*>(memory->getArrayData(param + 4)));
    return status;
}

unsigned int System::FileDelete(unsigned param) {
    // 1: Return value, uword (out)
    // 2: File name, array (in)

    unsigned status;
    fileSystem->FileDelete(status, reinterpret_cast<char*>(memory->getArrayData(param + 2)));
    return status;
}
