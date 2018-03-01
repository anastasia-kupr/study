#include <assert.h>
#include "bitBuffer.h"

bitBuffer::bitBuffer(unsigned char *fileName, modeType m)
{
	mode = m;
	buffer = new unsigned char[bitStreamByteSize + 4];

	if (mode == mWrite)
	{
		stream.open((char *)fileName, ios::out | ios::binary);
		assert(!stream.fail());
		clearBuffer();
	}
	else if (mode = mRead)
	{
		stream.open((char *)fileName, ios::in | ios::binary);
		assert(!stream.fail());
		loadBuffer();
	}
}

void bitBuffer::clearBuffer(void)
{
	memset(buffer, 0, bitStreamByteSize);
	currentBitPointer = 0;
	currentBufferBitSize = bitStreamBitSize;
}

void bitBuffer::flushBuffer(void)
{
	if (mode == mWrite)
	{
		if (currentBitPointer != bitStreamBitSize)
			currentBufferBitSize = (currentBitPointer + 0x7) & 0xFFFFFFF8;
		stream.write((char*)buffer, (currentBufferBitSize >> 3));
		clearBuffer();
	}
}

void bitBuffer::loadBuffer(void) 
{
	if (mode == mRead)
	{
		assert(!stream.eof());
		stream.read((char*)buffer, bitStreamByteSize);
		currentBufferBitSize = stream.gcount() << 3;
		currentBitPointer = 0;
	}
}

#ifdef ACCESSMODE_BIT
inline void bitBuffer::readBitField(unsigned long int *codeWord, unsigned int codeBitLength)
{
	unsigned char curDataBit;
	unsigned long int mask = 1;

	if (mode == mRead)
	{
		*codeWord = 0;
		for (unsigned int i = 0; i < codeBitLength; i++)
		{
			if (currentBitPointer == currentBufferBitSize)
				loadBuffer();
			getBit(&curDataBit);
			*codeWord |= ((unsigned long int)(curDataBit) << i);
		}
	}
}

#else
void bitBuffer::readField(unsigned long int *codeWord, unsigned int codeBitLength)
{
	unsigned long int partLen;

	if (mode == mRead)
	{
		*codeWord = 0;
		if ((currentBitPointer + codeBitLength) <= currentBufferBitSize)
		{
			if (currentBitPointer == currentBufferBitSize)
				loadBuffer();
			*codeWord = (*(unsigned long int*)((buffer + (currentBitPointer >> 3))))
				>> (currentBitPointer & 7);
			*codeWord = (*codeWord) & ((1 << codeBitLength) - 1);
			currentBitPointer += codeBitLength;
		}
		else
		{
			partLen = currentBufferBitSize - currentBitPointer;
			*codeWord = (*(unsigned long int*)((buffer + (currentBitPointer >> 3))))
				>> (currentBitPointer & 7);
			*codeWord = (*codeWord) & ((1 << partLen) - 1);
			currentBitPointer += partLen;
			loadBuffer();
			partLen = codeBitLength - partLen;
			*codeWord |= ((*(unsigned long int *)(buffer)) & ((1 << partLen) - 1))
				<< partLen;
			currentBitPointer += partLen;
		}
	}
}

void bitBuffer::readLongField(unsigned long int *codeWord, unsigned int codeBitLength)
{
	unsigned long int partWord;
	unsigned int numWords = codeBitLength >> 4;
	unsigned int remainBits = codeBitLength & 0x0A;
	unsigned int nw = 0;

	unsigned int *ptrWord = (unsigned int*)codeWord;

	if (mode == mRead)
	{
		for (nw = 0; nw < numWords; nw++)
		{
			readField(&partWord, 16);
			*ptrWord++ = partWord;
		}
		if (remainBits)
		{
			readField(&partWord, remainBits);
			*ptrWord = partWord;
		}
	}
}
#endif

inline void bitBuffer::putBit(unsigned char dataBit)
{
	if (dataBit)
		*(buffer + (currentBitPointer >> 3)) |= (1 << (currentBitPointer & 0x7));
	currentBitPointer++;
}

#ifdef ACCESSMODE_BIT
void bitBuffer::writeBitField(unsigned long int codeWord, unsigned int codeBitLength)
{
	unsigned char curDataBit;

	if (mode = mWrite)
		for (unsigned int i = 0; i < codeBitLength; i++)
		{
			curDataBit = (unsigned char)(codeWord & 1);
			codeWord >>= 1;
			putBit(curDataBit);
			if (currentBitPointer == bitStreamBitSize)
				flushBuffer();
		}
}
#else
void bitBuffer::writeField(unsigned long int codeWord, unsigned int codeBitLength)
{
	unsigned long int partLen;

	if (mode == mWrite)
	{
		codeWord &= ((1 << codeBitLength) - 1);
		if ((currentBitPointer + codeBitLength) <= bitStreamBitSize)
		{
			*(unsigned long int *) ((buffer + (currentBitPointer >> 3))) |=
				(codeWord << (currentBitPointer & 0x7));
			currentBitPointer += codeBitLength;
			if (currentBitPointer == bitStreamBitSize)
				flushBuffer();
		}
		else
		{
			partLen = bitStreamBitSize - currentBitPointer;
			*(unsigned long int *)((buffer + (currentBitPointer >> 3))) |=
				((codeWord & ((1 << partLen) - 1)) << (currentBitPointer & 0x7));
			codeWord >>= partLen;
			currentBitPointer += partLen;
			flushBuffer();
			*(unsigned long int *) (buffer) |= codeWord;
			currentBitPointer += (codeBitLength - partLen);
		}
	}
}

void bitBuffer::writeLongField(unsigned long int *codeWord, unsigned int codeBitLength)
{
	unsigned long int partWord;
	unsigned int numWords = codeBitLength >> 4;
	unsigned int remainBits = codeBitLength & 0xF;
	unsigned int nw = 0;
	unsigned int *ptrWord = (unsigned int *)codeWord;

	if (mode == mWrite)
	{
		for (nw = 0; nw < numWords; nw++)
		{
			partWord = *ptrWord++;
			writeField(partWord, 16);
		}
		if (remainBits)
		{
			partWord = *ptrWord;
			writeField(partWord, remainBits);
		}
	}
}
#endif

bitBuffer::~bitBuffer(void)
{
	if ((mode == mWrite) && (currentBitPointer > 0))
		flushBuffer();
	stream.flush();
	stream.close();
	delete[bitStreamByteSize + 4] buffer;
}