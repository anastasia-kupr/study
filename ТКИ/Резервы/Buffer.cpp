// Buffer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#pragma once

#include <fstream>
using namespace std;

#include <assert.h>
#include<iostream>
#include"stdlib.h"
#include<cstdio> 
#include <conio.h>
using namespace std;

enum modeType {
	mRead,
	mWrite
};

const unsigned long int bitStreamByteSize = 3;
const unsigned long int bitStreamBitSize = bitStreamByteSize * 8;
class bitBuffer {
public:
	unsigned char *buffer;
	modeType mode;
	ifstream istream;
	ofstream ostream;
	unsigned long int currentBufferBitSize;
	unsigned long int currentBitPointer;

public:
	bitBuffer(unsigned char *fileName, modeType m);
	~bitBuffer(void);

	void writeBitField(unsigned long int codeWord, unsigned int codeBitLength);
	void readBitField(unsigned long int *codeWord, unsigned int codeBitLength);

	void writeField(unsigned long int codeWord, unsigned int codeBitLength);
	void readField(unsigned long int *codeWord, unsigned int codeBitLength);

	void show_buffer(void);


	void putBit(unsigned char dataBit);
	void getBit(unsigned char *dataBit);
	void flushBuffer(void);
	void loadBuffer(void);
	void clearBuffer(void);
};

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

bitBuffer::bitBuffer(unsigned char *fileName, modeType m)
{
	mode = m;
	buffer = new unsigned char[bitStreamByteSize + 4];

	if (mode == mWrite)
	{
		ostream.open((char *)fileName, ios::binary);
		assert(!ostream.fail());
		clearBuffer();
	}
	else if (mode = mRead)
	{
		istream.open((char *)fileName, ios::binary);
		assert(!istream.fail());
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
		ostream.write((char*)buffer, (currentBufferBitSize >> 3));
		clearBuffer();
	}
}

void bitBuffer::loadBuffer(void)
{
	if (mode == mRead)
	{
		assert(!istream.eof());
		istream.read((char*)buffer, bitStreamByteSize);
		currentBufferBitSize = istream.gcount() << 3;
		currentBitPointer = 0;
	}
}

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

inline void bitBuffer::putBit(unsigned char dataBit)
{
	if (dataBit)
		*(buffer + (currentBitPointer >> 3)) |= (1 << (currentBitPointer & 0x7));
	currentBitPointer++;
}

inline void bitBuffer::getBit(unsigned char *dataBit)
{
	*dataBit = (*(buffer + (currentBitPointer >> 3))) >> (currentBitPointer & 0x7) & 0x01;
	currentBitPointer++;
}

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

void bitBuffer::show_buffer(void)
{
	cout << "Current mode:\t" << mode << "\nCurrent buffer bit size:\t" << currentBufferBitSize
		<< "\nCurrent bit pointer:\t" << currentBitPointer;
	for (int i = 0; i < currentBufferBitSize / 8; i++)
		cout << (int)buffer[i] << endl;
}

bitBuffer::~bitBuffer(void)
{
	if ((mode == mWrite) && (currentBitPointer > 0))
		flushBuffer();
	ostream.flush();
	ostream.close();
	delete[bitStreamByteSize + 4] buffer;
}


using namespace std;
int _tmain(int argc, _TCHAR* argv[])
{
	ifstream DataIn;
	ifstream inData;
	ofstream outData;
	int N;
	modeType read = mRead;
	modeType write = mWrite;
	bitBuffer Buffer1((unsigned char *) "Data.txt", write);
	unsigned long word = 0;
	DataIn.open("Data_all.txt");
	DataIn >> N;
	cout << "N=" << N << endl;
	unsigned int *data = new unsigned int[N];
	unsigned int *lengths = new unsigned int[N];
	for (int i = 0; i < N; i++){
		DataIn >> data[i] >> lengths[i];
	}
	cout << endl;
	Buffer1.istream.open((char *)(unsigned char *)"Data.txt", ios::binary);
	Buffer1.ostream.open((char *)(unsigned char *)"Data.txt", ios::binary);
	for (int i = 0; i < N; i++) {
		Buffer1.mode = write;
		Buffer1.writeField(data[i], lengths[i]);
		cout << "in=" << data[i] << endl;
	}
	cout << "buffer1=" << Buffer1.buffer << endl;
	Buffer1.istream.close();
	Buffer1.ostream.close();
	bitBuffer Buffer2((unsigned char*) "Data2.txt", read);
	Buffer2.buffer = Buffer1.buffer;
	cout << "buffer2=" << Buffer2.buffer << endl;
	unsigned long codeWord = 0;
	outData.open("out.txt");
	cout << "output\n";
	Buffer1.mode = read;
	for (int i = 0; i < N; i++)
	{
		Buffer2.readBitField(&word, lengths[i]);
		outData << word << endl;
		cout << word << endl;
	}
	outData.close();
	system("pause");
	return 0;
}

