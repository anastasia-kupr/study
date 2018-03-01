#pragma once

#include <fstream>
using namespace std;

//#define ACCESSMODE_BIT

enum modeType {
	mRead,
	mWrite
};

const unsigned long int bitStreamByteSize = 65536;
const unsigned long int bitStreamBitSize = bitStreamByteSize * 8;

class bitBuffer {
	unsigned char *buffer;
	modeType mode;
	//ofstream out;
	//ifstream in;
	fstream stream;
	unsigned long int currentBufferBitSize;
	unsigned long int currentBitPointer;

public:
	bitBuffer(unsigned char *fileName, modeType m);
	~bitBuffer(void);

#ifdef ACCESSMODE_BIT
	void writeBitField(unsigned long int codeWord, unsigned int codeBitLength);
	void readBitField(unsigned long int *codeWord, unsigned int codeBitLength);
#else
	void writeField(unsigned long int codeWord, unsigned int codeBitLength);
	void readField(unsigned long int *codeWord, unsigned int codeBitLength);

	void writeLongField(unsigned long int *codeWord, unsigned int codeBitLength);
	void readLongField(unsigned long int *codeWord, unsigned int codeBitLength);
#endif

private:
	void putBit(unsigned char dataBit);
	void getBit(unsigned char *dataBit);
	void flushBuffer(void);
	void loadBuffer(void);
	void clearBuffer(void);
};