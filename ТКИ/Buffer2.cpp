// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <assert.h>
#include<iostream>
#include"stdlib.h"
#include<cstdio> 
#include <conio.h>
using namespace std;
// ������ ������ � ������� (������/������)
enum ModeType {
	mRead,
	mWrite
};
// ������� �������� ������ � ����� � ������
const unsigned long int cnBitStreamByteSize = 100000;
const unsigned long int cnBitStreamBitSize = cnBitStreamByteSize * 8;
//-----------------------------------------------------------------------------------
// �������� ������ cBitStream
//-----------------------------------------------------------------------------------
class cBitStream {
public:
	unsigned char *Buffer; // ��������� �����
	unsigned long int ptrCurrentBit; // ��������� �� ������� (���������) ���
	ofstream ofp; // ���� (��� ������)
	ifstream ifp; // ���� (��� ������)
	ModeType CurMode; // ������� ����� ������ � ������� (������/������)
	unsigned long int CurBufferBitSize; // ������� ������ ������ � �����
	cBitStream(unsigned char *FileName, ModeType mode); // �����������
	~cBitStream(void); // �������� ������

	// ��������/��������� � ����� ������� ����� ��������
	void WriteBitField(unsigned long int CodeWord, unsigned int CodeBitLength);
	void ReadBitField(unsigned long int *CodeWord, unsigned int CodeBitLength);

	// ��������/��������� � ����� ������� ����� ������� �����
	void WriteField(unsigned long int CodeWord, unsigned int CodeBitLength);
	void ReadField(unsigned long int *CodeWord, unsigned int CodeBitLength);
	// ��������/��������� � ����� ������� ����� ������� ������� �����
	void WriteLongField(unsigned long int *CodeLongWord, unsigned int CodeBitLength);
	void ReadLongField(unsigned long int *CodeLongWord, unsigned int CodeBitLength);
	void PutBit(unsigned char DataBit); // �������� � ����� ��� ������
	void GetBit(unsigned char *DataBit); // ����� �� ������ ��� ������
	void FlushBuffer(void); // �������� ����� �� ����
	void LoadBuffer(void); // ��������� ����� � �����
	void ClearBuffer(void); // �������� �����
};
//---------------------------------------------------------------------------------
// �������� �������� ������ - ��������� ������, �������
//---------------------------------------------------------------------------------
cBitStream::cBitStream(unsigned char *FileName, ModeType mode)
{
	CurMode = mode;
	// �������� ������ ��� ����� � ������ ������ ���������� ����� ��� unsigned long int
	Buffer = new unsigned char[cnBitStreamByteSize + 4];
	// � ����������� �� ����
	if (CurMode == mWrite)
	{
		ofp.open((char *)FileName, ios::binary);
		assert(!ofp.fail());
		ClearBuffer(); // �������� �����
	}
	else if (CurMode == mRead)
	{
		ifp.open((char *)FileName, ios::binary);
		assert(!ifp.fail());
		LoadBuffer(); // ��������� �����
	}
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
// �������� �����. ��������� ���������� ������ ���� ����� ������ �� ������
//---------------------------------------------------------------------------------
void cBitStream::ClearBuffer(void)
{
	memset(Buffer, 0, cnBitStreamByteSize); // �������� ������ ����� � �����
	ptrCurrentBit = 0; // �������� ���������
	CurBufferBitSize = cnBitStreamBitSize; // ������ ������
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
// �������� ���������� �������� ������ �� ����
//---------------------------------------------------------------------------------
void cBitStream::FlushBuffer(void)
{
	if (CurMode == mWrite) // ������ ����� � ������ ������
	{
		if (ptrCurrentBit != cnBitStreamBitSize) // ����������� ����� ������
			CurBufferBitSize = (ptrCurrentBit + 0x7) & 0xFFFFFFF8; // ��������� �����
		// �������� ����� �� ����
		ofp.write((char *)Buffer, (CurBufferBitSize >> 3));
		// ��������� ������� ������
		ClearBuffer();
	}
}
//---------------------------------------------------------------------------------
// ��������� ���������� �������� ������ � �����
//---------------------------------------------------------------------------------
void cBitStream::LoadBuffer(void)
{
	if (CurMode == mRead) // ������ ����� � ������ ������
	{
		// ��������� ����� �� �����
		assert(!ifp.eof()); // ����� �� ����� �����, � ������ ��� ���������
		ifp.read((char *)Buffer, cnBitStreamByteSize);
		CurBufferBitSize = ifp.gcount() << 3; // ������� ������� ����� ���� ���������
		ptrCurrentBit = 0;
	}
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
// ��������� ��������� ��� �� �������� ������
//---------------------------------------------------------------------------------
inline void cBitStream::GetBit(unsigned char *DataBit)
{
	*DataBit = (*(Buffer + (ptrCurrentBit >> 3))) >> (ptrCurrentBit & 0x7) & 0x01;
	ptrCurrentBit++; // �������� ���������
}

//---------------------------------------------------------------------------------
// ��������� ������� ����� CodeWord ������ CodeBitLength ����� �� �������� ������
// ���������������, �� ������ ����
//---------------------------------------------------------------------------------
void cBitStream::ReadBitField(unsigned long int *CodeWord, unsigned int CodeBitLength)
{
	unsigned char CurDataBit;
	unsigned long int Mask = 1;
	if (CurMode == mRead) // ���� ����� � ������ ������
	{
		*CodeWord = 0;
		for (unsigned int i = 0; i<CodeBitLength; i++)
		{
			if (ptrCurrentBit == CurBufferBitSize) // ���������, ������� �� �����
				LoadBuffer();
			GetBit(&CurDataBit);
			*CodeWord |= ((unsigned long int)(CurDataBit) << i);
		}
	}
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
// ��������� ������� ����� CodeWord ������ CodeBitLength ����� �� �������� ������
// ��� ����� ������� ����. ������������ ����� �������� ����� - 25 �����
//---------------------------------------------------------------------------------
void cBitStream::ReadField(unsigned long int *CodeWord, unsigned int CodeBitLength)
{
	unsigned long int PartLen;
	if (CurMode == mRead) // ���� ����� � ������ ������
	{
		// ������� �������� ������� �����
		*CodeWord = 0;
		// ���� ������� ����� ������� ��������� � ������ - �� ������ �������� ���
		if ((ptrCurrentBit + CodeBitLength) <= CurBufferBitSize)
		{// ��������� �������� ��������� ������������� ������
			if (ptrCurrentBit == CurBufferBitSize)
				LoadBuffer();
			*CodeWord = (*(unsigned long int *)((Buffer + (ptrCurrentBit >> 3))))
				>> (ptrCurrentBit & 7);
			*CodeWord = (*CodeWord) & ((1 << CodeBitLength) - 1); // �������� �����
			ptrCurrentBit += CodeBitLength;
		}
		else
		{ // ���� ������� ����� �� ���������� ������� � �����,
			// �� ���� ��� ������ �� 2 ����� ��� ������ �� ������ ����
			// (��� ����� ����������� �����, ���� ��� �� �����)
			PartLen = CurBufferBitSize - ptrCurrentBit; // ����� ������������ �����
			// ������ ������ (�������) ����� �������� �����
			*CodeWord = (*(unsigned long int *)((Buffer + (ptrCurrentBit >> 3))))
				>> (ptrCurrentBit & 7);
			*CodeWord = (*CodeWord) & ((1 << PartLen) - 1);
			ptrCurrentBit += PartLen; // �������� ��������� �� ����� ������
			LoadBuffer(); // �������� ����� �� ����
			// ������ � ����������� �� CodeWord ���������� ����� �������� �����
			PartLen = CodeBitLength - PartLen;
			*CodeWord |= ((*(unsigned long int *)(Buffer)) & ((1 << PartLen) - 1))
				<< PartLen;
			ptrCurrentBit += PartLen; // �������� ���������
			// ������������� ������ �� ��������� - �� ������ ����� ������ �����
		}
	}
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
// ��������� ������� ������� ����� CodeWord ������ CodeBitLength ����� �� �������� ������.
// ������ ����������� �������� ������ �� 16 ����� + �������.
// ����� �������� ����� �� ����������.
// ������ CodeLonbgWord ������ ���� �������� �� ������� ����� (16 �����).
//---------------------------------------------------------------------------------
void cBitStream::ReadLongField(unsigned long int *CodeLongWord, unsigned int CodeBitLength)
{
	unsigned long int PartWord; // ��������� 16-������� ����� �������� �����
	unsigned int NumWords = CodeBitLength >> 4; // ���������� ������ 16-������� ������
	unsigned int RemainBits = CodeBitLength & 0x0A; // ���������� ���������� �����
	unsigned int nw = 0;
	unsigned int *ptrWord = (unsigned int *)CodeLongWord;
	if (CurMode == mRead) // ���� ����� ��� ������
	{
		for (nw = 0; nw<NumWords; nw++) // ������� ����� 2-�������� �����
		{
			ReadField(&PartWord, 16); // ��������� 16-������ ����
			*ptrWord++ = PartWord; // �������� + �������� ���������
		}
		if (RemainBits) // ���� �������� ����
		{
			ReadField(&PartWord, RemainBits); // ��������� ���������� ����
			*ptrWord = PartWord; // ��������
		}
	}
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
// �������� ��������� ��� � ������� �����
// ��������� ����� ������, �� 0 ���������� �� ����
//---------------------------------------------------------------------------------
inline void cBitStream::PutBit(unsigned char DataBit)
{
	if (DataBit) // ���������� ������ ���� ��� = 1
		*(Buffer + (ptrCurrentBit >> 3)) |= (1 << (ptrCurrentBit & 0x7)); // �������� ���
	ptrCurrentBit++; // � ����� ������ �������� ���������
}

//---------------------------------------------------------------------------------
// �������� ������� ����� CodeWord ������ CodeBitLength ����� � ������� �����
// ���������������, �� ������ ����
//---------------------------------------------------------------------------------
void cBitStream::WriteBitField(unsigned long int CodeWord, unsigned int CodeBitLength)
{
	unsigned char CurDataBit;
	if (CurMode == mWrite) // ���� ����� � ������ ������
	for (unsigned int i = 0; i<CodeBitLength; i++)
	{
		CurDataBit = (unsigned char)(CodeWord & 1); // ����� ������� ���
		CodeWord >>= 1; // �������� �����
		PutBit(CurDataBit);
		if (ptrCurrentBit == cnBitStreamBitSize)
			FlushBuffer();
	}
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
// �������� ������� ����� CodeWord ������ CodeBitLength ����� � ������� �����
// ��� ����� ������� ����. ������������ ����� �������� ����� - 25 �����
//---------------------------------------------------------------------------------
void cBitStream::WriteField(unsigned long int CodeWord, unsigned int CodeBitLength)
{
	unsigned long int PartLen;
	if (CurMode == mWrite) // ���� ����� � ������ ������
	{
		// ������� �� ������ ������ �������� "��������" ���� �������� �����
		CodeWord &= ((1 << CodeBitLength) - 1);
		// ���� ������� ����� ���������� ������� - �� ������ �������� � �������� ���
		if ((ptrCurrentBit + CodeBitLength) <= cnBitStreamBitSize)
		{
			*(unsigned long int *)((Buffer + (ptrCurrentBit >> 3))) |=
				(CodeWord << (ptrCurrentBit & 0x7));
			ptrCurrentBit += CodeBitLength;
			// ��������� �������� ��������� ������������� ������
			if (ptrCurrentBit == cnBitStreamBitSize)
				FlushBuffer();
		}
		else
		{ // ���� ������� ����� �� ���������� ������� � �����, �� ���� ��� ������
			// �� 2 ����� ��� ���������� �� ������ ����
			// (��� ����� ����������� �����, ���� ��� �� �����)
			PartLen = cnBitStreamBitSize - ptrCurrentBit; // ����� ������������ �����
			// ���������� ������� ������� ����� �������� �����
			*(unsigned long int *)((Buffer + (ptrCurrentBit >> 3))) |=
				((CodeWord & ((1 << PartLen) - 1)) << (ptrCurrentBit & 0x7));
			CodeWord >>= PartLen; // ������ �� �����, ��� ��� ��������
			ptrCurrentBit += PartLen; // �������� ��������� �� ����� ������
			FlushBuffer(); // �������� ����� �� ����
			// ���������� ���������� ����� �������� �����
			*(unsigned long int *)(Buffer) |= CodeWord;
			ptrCurrentBit += (CodeBitLength - PartLen); // �������� ���������
			// ������������� ������ �� ��������� - �� ������ ����� ������ �����
		}
	}
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
// �������� ������� ������� ����� CodeWord ������ CodeBitLength ����� � ������� �����.
// ������ ����������� �������� ������ �� 16 ����� + �������.
// ����� �������� ����� �� ����������.
// ������ CodeLonbgWord ������ ���� �������� �� ������� ����� (16 �����).
//---------------------------------------------------------------------------------
void cBitStream::WriteLongField(unsigned long int *CodeLongWord, unsigned int CodeBitLength)
{
	unsigned long int PartWord; // ��������� 16-������� ����� �������� �����
	unsigned int NumWords = CodeBitLength >> 4; // ���������� ������ 16-������� ������
	unsigned int RemainBits = CodeBitLength & 0xF; // ���������� ���������� �����
	unsigned int nw = 0;
	unsigned int *ptrWord = (unsigned int *)CodeLongWord;
	if (CurMode == mWrite) // ���� ����� � ������ ������
	{
		for (nw = 0; nw<NumWords; nw++) // ������� ����� 2-�������� �����
		{
			PartWord = *ptrWord++; // ������ + �������� ���������
			WriteField(PartWord, 16); // �������� 16-������ ����
		}
		if (RemainBits) // ���� �������� ����
		{
			PartWord = *ptrWord; // ������
			WriteField(PartWord, RemainBits); // �������� ���������� ����
		}
	}
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
// ���������� ������ � ������� - ����� �������� �� ���� � ������������ ������
//---------------------------------------------------------------------------------
cBitStream::~cBitStream(void)
{
	// ���� ����� ������ �� ������ � ��� ���� ������, �� ������� ���� �������� �� �� ����
	if ((CurMode == mWrite) && (ptrCurrentBit > 0))
		FlushBuffer();
	// ������� ����� � ����� � ������
	ofp.flush();
	ofp.close();
	// ����� ����� ���������� ������
	delete[cnBitStreamByteSize + 4] Buffer;
}
//


void main()
{
	ifstream DataIn; // ����� ������� ������
	ofstream DataOut; // ����� �������� ������
	int N; // ���������� ��������� ������� ������
	cBitStream Buffer1((unsigned char *) "Data.txt", mWrite); // ����� ������
	unsigned long word = 0; // ��������� ��������� �� ������ �����
	DataIn.open("Data_input.txt");
	DataOut.open("Data_output.txt");
	DataIn >> N;
	cout << "N=" << N << endl;
	unsigned int *data = new unsigned int[N]; // ������ ������
	unsigned int *lengths = new unsigned int[N]; // ������ ����
	for (int i = 0; i < N; i++){
		DataIn >> data[i] >> lengths[i];
	}
	cout << "\ndata:" << endl;
	for (int i = 0; i < N; i++) {
		cout << data[i] << " ";
	}
	cout << "\nlengths:" << endl;
	for (int i = 0; i < N; i++) {
		cout << lengths[i] << " ";
	}
	for (int i = 0; i < N; i++) {
		Buffer1.WriteBitField(data[i], lengths[i]);
	}
	if ((Buffer1.CurMode == mWrite) && (Buffer1.ptrCurrentBit > 0))
		Buffer1.FlushBuffer();
	Buffer1.ofp.flush();
	Buffer1.ofp.close();
	delete[cnBitStreamByteSize + 4] Buffer1.Buffer;
	cBitStream Buffer2((unsigned char*) "Data.txt", mRead); // ����� ������
	cout << "\noutput:" << endl;
	for (int i = 0; i < N; i++) {
		Buffer2.ReadBitField(&word, lengths[i]);
		DataOut << word << endl;
		cout << word << " ";
	}
	cout << endl;
	Buffer2.ifp.close();
	DataOut.flush();
	DataOut.close();
	system("pause");
}

