//
// Created by pierre on 05/04/2020.
//

const char* nameForOpcode(unsigned opcode)
{
    switch (opcode)
    {
        case 0x00: return "OP_ADD";
        case 0x01: return "OP_SUB";
        case 0x02: return "OP_NEG";
        case 0x03: return "OP_MUL";
        case 0x04: return "OP_DIV";
        case 0x05: return "OP_MOD";
        case 0x36: return "OP_SQRT";
        case 0x37: return "OP_ABS";
        case 0x06: return "OP_AND";
        case 0x07: return "OP_OR";
        case 0x08: return "OP_XOR";
        case 0x09: return "OP_NOT";
        case 0x11: return "OP_CMP";
        case 0x12: return "OP_TST";
        case 0x15: return "OP_INDEX";
        case 0x16: return "OP_REPLACE";
        case 0x17: return "OP_ARRSIZE";
        case 0x18: return "OP_ARRBUILD";
        case 0x19: return "OP_ARRSUBSET";
        case 0x1A: return "OP_ARRINIT";
        case 0x1B: return "OP_MOV";
        case 0x1C: return "OP_SET";
        case 0x1D: return "OP_FLATTEN";
        case 0x1E: return "OP_UNFLATTEN";
        case 0x1F: return "OP_NUMTOSTRING";
        case 0x20: return "OP_STRINGTONUM";
        case 0x21: return "OP_STRCAT";
        case 0x22: return "OP_STRSUBSET";
        case 0x23: return "OP_STRTOBYTEARR";
        case 0x24: return "OP_BYTEARRTOSTR";
        case 0x25: return "OP_JMP";
        case 0x26: return "OP_BRCMP";
        case 0x27: return "OP_BRTST";
        case 0x29: return "OP_STOP";
        case 0x2A: return "OP_FINCLUMP";
        case 0x2B: return "OP_FINCLUMPIMMED";
        case 0x2C: return "OP_ACQUIRE";
        case 0x2D: return "OP_RELEASE";
        case 0x2E: return "OP_SUBCALL";
        case 0x2F: return "OP_SUBRET";
        case 0x28: return "OP_SYSCALL";
        case 0x30: return "OP_SETIN";
        case 0x31: return "OP_SETOUT";
        case 0x32: return "OP_GETIN";
        case 0x33: return "OP_GETOUT";
        case 0x34: return "OP_WAIT";
        case 0x35: return "OP_GETTICK";
        default: return "OP_UNKNOWN";
    }
}