/*
 *  InterpreterThread.cpp
 *  mindstormssimulation
 *
 *  Created by Torsten Kammer on 25.04.10
 *  Copyright 2010 RWTH Aachen University All rights reserved.
 *
 */

#include "InterpreterThread.h"

#include "../Execution/RXEFile.h"
#include "../System/System.h"
#include "../System/VMMemory.h"
#include "Interpreter.h"

InterpreterThread::InterpreterThread(unsigned clump, Interpreter *interpreter, System *system) :
        memory(system->getMemory()), system(system), interpreter(interpreter)
{
    currentClump = clump;
	instruction = memory->getFile()->getCodeStartForClump(currentClump);
	waitUntil = 0;
    isTerminated = false;
}

bool InterpreterThread::step()
{
	if (done()) return false;
	
	if (system->getTimeSinceStart() < waitUntil)
		return true;
	
	const uint16_t *code = memory->getFile()->getCode();
	
	if (code[instruction] & (1 << 11))
	{
		// Short format	
		unsigned opcode = (code[instruction] & 0x0700) >> 8;
		int argDiff = (int8_t) (code[instruction] & 0xFF);
		uint16_t arguments[2];
		switch(opcode)
		{
			case 0: // SHORT_OP_MOV
				arguments[0] = argDiff + code[instruction + 1];
				arguments[1] = code[instruction + 1];
				instruction += 2;
				op_mov(0, arguments);
				break;
			case 1: // SHORT_OP_ACQUIRE
				arguments[0] = argDiff;
				instruction += 1;
				op_acquire(0, arguments);
				break;
			case 2: // SHORT_OP_RELEASE
				arguments[0] = argDiff;
				instruction += 1;
				op_release(0, arguments);
				break;
			case 3: // SHORT_OP_SUBCALL
				arguments[0] = argDiff + code[instruction + 1]; // Subroutine
				arguments[1] = code[instruction + 1]; // Caller ID
				instruction += 2; // Instruction has to be on the next before
				// branching to subroutine so it can be used as return inst.
				op_subcall(0, arguments);
				break;
			default:
				throw std::runtime_error("Invalid short opcode.");
		}		
	}
	else
	{
		// Normal format
		unsigned opcode = code[instruction] & 0x00FF;
		// You’d think it’d be the other way around, but Lego’s documentation
		// tries to account for endianess and gets it all wrong.
		unsigned size = (code[instruction] & 0xF000) >> 12;	
		if (size == 0xE) size = code[instruction+1];
		unsigned flags = (code[instruction] & 0x0F00) >> 8;
		
		const uint16_t *params = &code[instruction + 1];
		
		instruction += size/2;
		
		switch(opcode)
		{	
			case 0x00: op_add(flags, params); break;
			case 0x01: op_sub(flags, params); break;
			case 0x02: op_neg(flags, params); break;
			case 0x03: op_mul(flags, params); break;
			case 0x04: op_div(flags, params); break;
			case 0x05: op_mod(flags, params); break;
			case 0x36: op_sqrt(flags, params); break;
			case 0x37: op_abs(flags, params); break;
			case 0x06: op_and(flags, params); break;
			case 0x07: op_or(flags, params); break;
			case 0x08: op_xor(flags, params); break;
			case 0x09: op_not(flags, params); break;
			case 0x11: op_cmp(flags, params); break;
			case 0x12: op_tst(flags, params); break;
			case 0x15: op_index(flags, params); break;
			case 0x16: op_replace(flags, params); break;
			case 0x17: op_arrsize(flags, params); break;
			case 0x18: op_arrbuild(flags, params); break;
			case 0x19: op_arrsubset(flags, params); break;
			case 0x1A: op_arrinit(flags, params); break;
			case 0x1B: op_mov(flags, params); break;
			case 0x1C: op_set(flags, params); break;
			case 0x1D: op_flatten(flags, params); break;
			case 0x1E: op_unflatten(flags, params); break;
			case 0x1F: op_numtostring(flags, params); break;
			case 0x20: op_stringtonum(flags, params); break;
			case 0x21: op_strcat(flags, params); break;
			case 0x22: op_strsubset(flags, params); break;
			case 0x23: op_strtobytearr(flags, params); break;
			case 0x24: op_bytearrtostr(flags, params); break;
			case 0x25: op_jmp(flags, params); break;
			case 0x26: op_brcmp(flags, params); break;
			case 0x27: op_brtst(flags, params); break;
			case 0x29: op_stop(flags, params); break;
			case 0x2A: op_finclump(flags, params); break;
			case 0x2B: op_finclumpimmed(flags, params); break;
			case 0x2C: op_acquire(flags, params); break;
			case 0x2D: op_release(flags, params); break;
			case 0x2E: op_subcall(flags, params); break;
			case 0x2F: op_subret(flags, params); break;
			case 0x28: op_syscall(flags, params); break;
			case 0x30: op_setin(flags, params); break;
			case 0x31: op_setout(flags, params); break;
			case 0x32: op_getin(flags, params); break;
			case 0x33: op_getout(flags, params); break;
			case 0x34: op_wait(flags, params); break;
			case 0x35: op_gettick(flags, params); break;

			default:
				{
					char opcodeString[255];
					sprintf(opcodeString, "Invalid opcode 0x%x size %d", opcode, size);
					throw std::invalid_argument(opcodeString);	
				}
		}
	}

    return true;
}