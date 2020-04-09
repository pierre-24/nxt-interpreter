//
// Created by pierre on 09/04/2020.
//

#include "Syscall.h"
const char* nameForSyscall(unsigned ID)
{
    switch (ID)
    {
        case 0x00: return "NXTFileOpenRead";
        case 0x01: return "NXTFileOpenWrite";
        case 0x02: return "NXTFileOpenAppend";
        case 0x03: return "NXTFileRead";
        case 0x04: return "NXTFileWrite";
        case 0x05: return "NXTFileClose";
        case 0x06: return "NXTFileResolveHandle";
        case 0x07: return "NXTFileRename";
        case 0x08: return "NXTFileDelete";
        case 0x09: return "NXTSoundPlayFile";
        case 0x0A: return "NXTSoundPlayTone";
        case 0x0B: return "NXTSoundGetState";
        case 0x0C: return "NXTSoundSetState";
        case 0x0D: return "NXTDrawText";
        case 0x0E: return "NXTDrawPoint";
        case 0x0F: return "NXTDrawLine";
        case 0x10: return "NXTDrawCircle";
        case 0x11: return "NXTDrawRect";
        case 0x12: return "NXTDrawPicture";
        case 0x13: return "NXTSetScreenMode";
        case 0x14: return "NXTReadButton";
        case 0x15: return "NXTCommLSWrite";
        case 0x16: return "NXTCommLSRead";
        case 0x17: return "NXTCommLSCheckStatus";
        case 0x18: return "NXTRandomNumber";
        case 0x19: return "NXTGetStartTick";
        case 0x1A: return "NXTMessageWrite";
        case 0x1B: return "NXTMessageRead";
        case 0x1C: return "NXTCommBTCheckStatus";
        case 0x1D: return "NXTCommBTWrite";
        case 0x1F: return "NXTKeepAlive";
        case 0x20: return "NXTIOMapRead";
        case 0x21: return "NXTIOMapWrite";
        case 0x22: return "NXTColorSensorRead";
        case 0x23: return "NXTBTPower";
        case 0x24: return "NXTBTConnection";
        case 0x25: return "NXTCommHSWrite";
        case 0x26: return "NXTCommHSRead";
        case 0x27: return "NXTCommHSCheckStatus";
        case 0x28: return "NXTReadmeSemData";
        case 0x29: return "NXTWriteSemData";
        case 0x2A: return "NXTComputeCalibValue";
        case 0x2B: return "NXTUpdateCalibCacheData";
        case 0x2C: return "NXTDatalogWrite";
        case 0x2D: return "NXTDatalogGetTimes";
        case 0x2E: return "NXTSetSleepTimeout";
        case 0x2F: return "NXTListFiles";
        default: return "Not_A_Valid_Syscall";
    }
}

