#include "loader.h"

Loader::Loader()
{
	_compressed = false;
	_ops = NULL;
	_tagOpen = false;
}

Loader::~Loader()
{
	if (_ops)
	{
		SDL_RWclose(_ops);
		_ops = NULL;
	}	
}

bool Loader::load(const char* name)
{
	_ops = SDL_RWFromFile(name, "rb");
	return true;
}

UInt32 Loader::readU32()
{
	UInt32 u = 0;
	readBytes(&u, 4);
	return u;
}

int	Loader::readSInt(int bitcount)
{
	Sint32	value = (Sint32)readUInt(bitcount);

	// Sign extend...
	if (value & (1 << (bitcount - 1))) {
		value |= -1 << bitcount;
	}

	//		IF_DEBUG(log_msg("stream::read_sint(%d) == %d\n", bitcount, value));

	return value;
}

int Loader::readUInt(int bitcount)
{
	Assert(bitcount <= 32 && bitcount >= 0);

	UInt32	value = 0;

	int	bits_needed = bitcount;
	while (bits_needed > 0)
	{
		if (_unusedBits) {
			if (bits_needed >= _unusedBits) {
				// Consume all the unused bits.
				value |= (_currentByte << (bits_needed - _unusedBits));

				bits_needed -= _unusedBits;

				_currentByte = 0;
				_unusedBits = 0;

			}
			else {
				// Consume some of the unused bits.
				value |= (_currentByte >> (_unusedBits - bits_needed));

				// mask off the bits we consumed.
				_currentByte &= ((1 << (_unusedBits - bits_needed)) - 1);

				_unusedBits -= bits_needed;

				// We're done.
				bits_needed = 0;
			}
		}
		else {
			_currentByte = readByte();
			_unusedBits = 8;
		}
	}

	Assert(bits_needed == 0);

	return value;
}

int Loader::_readBytes(void* bytes, int numBytes)
{
	int	result = SDL_RWread(_ops, bytes, 1, numBytes);
	if (result == -1)
	{
		//
	}
	return result;
}
int Loader::readBytes(void* bytes, int numBytes)
{
	if (_tagOpen)
	{
		numBytes = Min(numBytes, _tagLeftLength);
		_tagLeftLength -= numBytes;
	}
	if (!_compressed)
	{
		return _readBytes(bytes, numBytes);
	}
	else
	{
		_zstream.next_out = (unsigned char*)bytes;
		_zstream.avail_out = numBytes;

		for (;;)
		{
			if (_zstream.avail_in == 0)
			{
				// Get more raw data.
				int	new_bytes = _readBytes(_rawdata, ZBUF_SIZE);
				if (new_bytes == 0)
				{
					// The cupboard is bare!  We have nothing to feed to inflate().
					break;
				}
				else
				{
					_zstream.next_in = _rawdata;
					_zstream.avail_in = new_bytes;
				}
			}

			int	err = inflate(&_zstream, Z_SYNC_FLUSH);
			if (err == Z_STREAM_END)
			{
				_atEof = true;
				break;
			}
			if (err != Z_OK)
			{
				// something's wrong.
				Assert(0);
				break;
			}

			if (_zstream.avail_out == 0)
			{
				break;
			}
		}

		int	bytes_read = numBytes - _zstream.avail_out;

		return bytes_read;
	}
}

void Loader::setCompressed(bool flag)
{
	_compressed = flag;

	if (_compressed)
	{
		_zstream.zalloc = (alloc_func)0;
		_zstream.zfree = (free_func)0;
		_zstream.opaque = (voidpf)0;

		_zstream.next_in = 0;
		_zstream.avail_in = 0;

		_zstream.next_out = 0;
		_zstream.avail_out = 0;

		int	err = inflateInit(&_zstream);
		Assert(err == Z_OK);
	}
}
TagHeader Loader::openTag()
{
	TagHeader th;
	int	tag_header = readU16();
	th.tagType = tag_header >> 6;
	th.tagLength = tag_header & 0x3F;
	if (th.tagLength == 0x3F) 
	{
		th.tagLength = readU32();
	}
	_tagLeftLength = th.tagLength;
	_tagOpen = true;

	return th;
}

void Loader::closeTag()
{
	_tagOpen = false;

	while (_tagLeftLength > 0)
	{
		int num = Min(_tagLeftLength, TEMP_SIZE);
		_tagLeftLength -= num;
		readBytes(_tempData, num);
	}
}