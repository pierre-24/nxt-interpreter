
dseg segment

// useful structures declaration
FileOpenT struct
	Result word
	FileHandle byte
	Filename byte[]
	Length dword
FileOpenT ends

FileReadWriteT struct
	Result word
	FileHandle byte
	Buffer byte[]
	Length dword
FileReadWriteT ends

FileCloseT struct
	Result word
	FileHandle byte
FileCloseT ends

FileResolveHandleT struct
	Result word
	FileHandle byte
	WriteHandle byte
	Filename byte[]
FileResolveHandleT ends

FileRenameT struct
	Result word
	OldFilename byte[]
	NewFilename byte[]
FileRenameT ends

FileDeleteT struct
	Result word
	Filename byte[]
FileDeleteT ends

// useful variables for macro
__frw FileReadWriteT
__intval dword

dseg ends

/* dumps some stuffs in standard output */
#define dumpStr(_strval) \
    set __frw.FileHandle, 0 \
    mov __frw.Buffer, _strval \
    strlen __frw.Length, __frw.Buffer \
    syscall FileWrite, __frw

#define dumpInt(_intval) \
    set __frw.FileHandle, 0 \
    mov __intval, _intval \
    numtostr __frw.Buffer, __intval \
    strlen __frw.Length, __frw.Buffer \
    syscall FileWrite, __frw


// finish by at least a blank line