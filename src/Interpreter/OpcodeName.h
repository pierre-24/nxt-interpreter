//
// Created by pierre on 05/04/2020.
//

#ifndef NXT_INTERPRETER_OPCODENAME_H
#define NXT_INTERPRETER_OPCODENAME_H

/*!
 * @abstract Debug method: Returns the name for an opcode.
 * @discussion The returned name is taken directly from the official
 * documentation. Returns "OP_UNKNOWN" for opcode values that do not exist.
 * This method is mainly meant for debugging purposes.
 * @param opcode The opcode of an instruction.
 * @result The name of the instruction per the programming manual, or
 * OP_UNKNOWN if no such instruction exists.
 */
const char *nameForOpcode(unsigned opcode);

#endif //NXT_INTERPRETER_OPCODENAME_H
