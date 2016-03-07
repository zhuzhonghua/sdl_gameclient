#ifndef __LOADER_H__
#define __LOADER_H__

#include "SDL.h"
#include "types.h"
#include "zlib.h"

const int	ZBUF_SIZE = 4096;
class Loader
{
public:
	Loader();
	~Loader();
	bool load(const char* name);
	UInt32 readU32();
	int		readBytes(void* bytes, int numBytes);
	void	setCompressed(bool flag);
	void	align() { _unusedBits = 0; }
	int		readUInt(int bitcount);
	int		readSInt(int bitcount);
	UInt16	readU16(){ UInt16 u; readBytes(&u, 2); return u; }
	UInt8 	readByte() { return read8(); }
	UInt8	read8() { UInt8 u; readBytes(&u, 1); return u; }
	UInt8	readU8() { align(); return readByte(); }
	SInt8	readS8() { align(); return readByte(); }
private:
	int		_readBytes(void* bytes, int numBytes);
private:	
	SDL_RWops*	_ops;
private:
	UInt8		_unusedBits;
	UInt8		_currentByte;
private:
	bool		_compressed;
	z_stream	_zstream;
	bool		_atEof;
	Byte		_rawdata[ZBUF_SIZE];
};
#endif