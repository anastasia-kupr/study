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
// режимы работы с буфером (чтение/запись)
enum ModeType {
	mRead,
	mWrite
};
// Размеры битового буфера в битах и байтах
const unsigned long int cnBitStreamByteSize = 100000;
const unsigned long int cnBitStreamBitSize = cnBitStreamByteSize * 8;
//-----------------------------------------------------------------------------------
// Описание класса cBitStream
//-----------------------------------------------------------------------------------
class cBitStream {
public:
	unsigned char *Buffer; // временный буфер
	unsigned long int ptrCurrentBit; // указатель на текущий (свободный) бит
	ofstream ofp; // файл (при записи)
	ifstream ifp; // файл (при чтении)
	ModeType CurMode; // текущий режим работы с буфером (запись/чтение)
	unsigned long int CurBufferBitSize; // текущий размер буфера в битах
	cBitStream(unsigned char *FileName, ModeType mode); // конструктор
	~cBitStream(void); // удаление буфера

	// добавить/прочитать в буфер кодовое слово побитово
	void WriteBitField(unsigned long int CodeWord, unsigned int CodeBitLength);
	void ReadBitField(unsigned long int *CodeWord, unsigned int CodeBitLength);

	// добавить/прочитать в буфер кодовое слово битовым полем
	void WriteField(unsigned long int CodeWord, unsigned int CodeBitLength);
	void ReadField(unsigned long int *CodeWord, unsigned int CodeBitLength);
	// добавить/прочитать в буфер кодовое слово длинным битовым полем
	void WriteLongField(unsigned long int *CodeLongWord, unsigned int CodeBitLength);
	void ReadLongField(unsigned long int *CodeLongWord, unsigned int CodeBitLength);
	void PutBit(unsigned char DataBit); // добавить в буфер бит данных
	void GetBit(unsigned char *DataBit); // взять из буфера бит данных
	void FlushBuffer(void); // сбросить буфер на диск
	void LoadBuffer(void); // загрузить буфер с диска
	void ClearBuffer(void); // очистить буфер
};
//---------------------------------------------------------------------------------
// Создание битового буфера - выделение памяти, очистка
//---------------------------------------------------------------------------------
cBitStream::cBitStream(unsigned char *FileName, ModeType mode)
{
	CurMode = mode;
	// выделить память под буфер с учетом чтения последнего байта как unsigned long int
	Buffer = new unsigned char[cnBitStreamByteSize + 4];
	// в зависимости от типа
	if (CurMode == mWrite)
	{
		ofp.open((char *)FileName, ios::binary);
		assert(!ofp.fail());
		ClearBuffer(); // очистить буфер
	}
	else if (CurMode == mRead)
	{
		ifp.open((char *)FileName, ios::binary);
		assert(!ifp.fail());
		LoadBuffer(); // загрузить буфер
	}
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
// Очистить буфер. Процедура вызывается только если буфер открыт на запись
//---------------------------------------------------------------------------------
void cBitStream::ClearBuffer(void)
{
	memset(Buffer, 0, cnBitStreamByteSize); // упрощает запись битов в буфер
	ptrCurrentBit = 0; // сбросить указатель
	CurBufferBitSize = cnBitStreamBitSize; // размер буфера
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
// Сбросить содержимое битового буфера на диск
//---------------------------------------------------------------------------------
void cBitStream::FlushBuffer(void)
{
	if (CurMode == mWrite) // только буфер в режиме записи
	{
		if (ptrCurrentBit != cnBitStreamBitSize) // заполненная часть буфера
			CurBufferBitSize = (ptrCurrentBit + 0x7) & 0xFFFFFFF8; // округлить вверх
		// сбросить буфер на диск
		ofp.write((char *)Buffer, (CurBufferBitSize >> 3));
		// выполнить очистку буфера
		ClearBuffer();
	}
}
//---------------------------------------------------------------------------------
// Прочитать содержимое битового буфера с диска
//---------------------------------------------------------------------------------
void cBitStream::LoadBuffer(void)
{
	if (CurMode == mRead) // только буфер в режиме чтения
	{
		// загрузить буфер из файла
		assert(!ifp.eof()); // дошли до конца файла, а данные еще требуются
		ifp.read((char *)Buffer, cnBitStreamByteSize);
		CurBufferBitSize = ifp.gcount() << 3; // сколько реально битов было прочитано
		ptrCurrentBit = 0;
	}
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
// Прочитать очередной бит из битового буфера
//---------------------------------------------------------------------------------
inline void cBitStream::GetBit(unsigned char *DataBit)
{
	*DataBit = (*(Buffer + (ptrCurrentBit >> 3))) >> (ptrCurrentBit & 0x7) & 0x01;
	ptrCurrentBit++; // сместить указатель
}

//---------------------------------------------------------------------------------
// Прочитать кодовое слово CodeWord длиной CodeBitLength битов из битового буфера
// последовательно, по одному биту
//---------------------------------------------------------------------------------
void cBitStream::ReadBitField(unsigned long int *CodeWord, unsigned int CodeBitLength)
{
	unsigned char CurDataBit;
	unsigned long int Mask = 1;
	if (CurMode == mRead) // если буфер в режиме чтения
	{
		*CodeWord = 0;
		for (unsigned int i = 0; i<CodeBitLength; i++)
		{
			if (ptrCurrentBit == CurBufferBitSize) // проверить, пройден ли буфер
				LoadBuffer();
			GetBit(&CurDataBit);
			*CodeWord |= ((unsigned long int)(CurDataBit) << i);
		}
	}
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
// Прочитать кодовое слово CodeWord длиной CodeBitLength битов из битового буфера
// как целое битовое поле. Максимальная длина кодового слова - 25 битов
//---------------------------------------------------------------------------------
void cBitStream::ReadField(unsigned long int *CodeWord, unsigned int CodeBitLength)
{
	unsigned long int PartLen;
	if (CurMode == mRead) // если буфер в режиме чтения
	{
		// сначала обнулить кодовое слово
		*CodeWord = 0;
		// если кодовое слово целиком находится в буфере - то просто вытащить его
		if ((ptrCurrentBit + CodeBitLength) <= CurBufferBitSize)
		{// выполнить проверку состояния заполненности буфера
			if (ptrCurrentBit == CurBufferBitSize)
				LoadBuffer();
			*CodeWord = (*(unsigned long int *)((Buffer + (ptrCurrentBit >> 3))))
				>> (ptrCurrentBit & 7);
			*CodeWord = (*CodeWord) & ((1 << CodeBitLength) - 1); // наложить маску
			ptrCurrentBit += CodeBitLength;
		}
		else
		{ // если кодовое слово не помещается целиком в буфер,
			// то надо его делить на 2 части или читать по одному биту
			// (эта ветка выполняется редко, один раз на буфер)
			PartLen = CurBufferBitSize - ptrCurrentBit; // длина помещающейся части
			// читаем первую (младшую) часть кодового слова
			*CodeWord = (*(unsigned long int *)((Buffer + (ptrCurrentBit >> 3))))
				>> (ptrCurrentBit & 7);
			*CodeWord = (*CodeWord) & ((1 << PartLen) - 1);
			ptrCurrentBit += PartLen; // сдвинуть указатель на конец буфера
			LoadBuffer(); // сбросить буфер на диск
			// читаем и накладываем на CodeWord оставшуюся часть кодового слова
			PartLen = CodeBitLength - PartLen;
			*CodeWord |= ((*(unsigned long int *)(Buffer)) & ((1 << PartLen) - 1))
				<< PartLen;
			ptrCurrentBit += PartLen; // сдвинуть указатель
			// заполненность буфера не проверяем - он больше длины одного слова
		}
	}
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
// Прочитать длинное кодовое слово CodeWord длиной CodeBitLength битов из битового буфера.
// Чтение выполняется битовыми полями по 16 битов + остаток.
// Длина кодового слова не ограничена.
// Массив CodeLonbgWord должен быть выровнен по границе слова (16 битов).
//---------------------------------------------------------------------------------
void cBitStream::ReadLongField(unsigned long int *CodeLongWord, unsigned int CodeBitLength)
{
	unsigned long int PartWord; // очередная 16-битовая часть кодового слова
	unsigned int NumWords = CodeBitLength >> 4; // количество полных 16-битовых частей
	unsigned int RemainBits = CodeBitLength & 0x0A; // количество оставшихся битов
	unsigned int nw = 0;
	unsigned int *ptrWord = (unsigned int *)CodeLongWord;
	if (CurMode == mRead) // если буфер для чтения
	{
		for (nw = 0; nw<NumWords; nw++) // сначала целые 2-байтовые слова
		{
			ReadField(&PartWord, 16); // прочитать 16-битное поле
			*ptrWord++ = PartWord; // записать + сдвинуть указатель
		}
		if (RemainBits) // если остались биты
		{
			ReadField(&PartWord, RemainBits); // прочитать оставшиеся биты
			*ptrWord = PartWord; // записать
		}
	}
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
// Записать очередной бит в битовый буфер
// Поскольку буфер очищен, то 0 записывать не надо
//---------------------------------------------------------------------------------
inline void cBitStream::PutBit(unsigned char DataBit)
{
	if (DataBit) // записываем только если бит = 1
		*(Buffer + (ptrCurrentBit >> 3)) |= (1 << (ptrCurrentBit & 0x7)); // наложить бит
	ptrCurrentBit++; // в любом случае сместить указатель
}

//---------------------------------------------------------------------------------
// Записать кодовое слово CodeWord длиной CodeBitLength битов в битовый буфер
// последовательно, по одному биту
//---------------------------------------------------------------------------------
void cBitStream::WriteBitField(unsigned long int CodeWord, unsigned int CodeBitLength)
{
	unsigned char CurDataBit;
	if (CurMode == mWrite) // если буфер в режиме записи
	for (unsigned int i = 0; i<CodeBitLength; i++)
	{
		CurDataBit = (unsigned char)(CodeWord & 1); // взять младший бит
		CodeWord >>= 1; // сдвинуть слово
		PutBit(CurDataBit);
		if (ptrCurrentBit == cnBitStreamBitSize)
			FlushBuffer();
	}
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
// Записать кодовое слово CodeWord длиной CodeBitLength битов в битовый буфер
// как целое битовое поле. Максимальная длина кодового слова - 25 битов
//---------------------------------------------------------------------------------
void cBitStream::WriteField(unsigned long int CodeWord, unsigned int CodeBitLength)
{
	unsigned long int PartLen;
	if (CurMode == mWrite) // если буфер в режиме записи
	{
		// сначала на всякий случай обнулить "ненужные" биты кодового слова
		CodeWord &= ((1 << CodeBitLength) - 1);
		// если кодовое слово помещается целиком - то просто сдвинуть и наложить его
		if ((ptrCurrentBit + CodeBitLength) <= cnBitStreamBitSize)
		{
			*(unsigned long int *)((Buffer + (ptrCurrentBit >> 3))) |=
				(CodeWord << (ptrCurrentBit & 0x7));
			ptrCurrentBit += CodeBitLength;
			// выполнить проверку состояния заполненности буфера
			if (ptrCurrentBit == cnBitStreamBitSize)
				FlushBuffer();
		}
		else
		{ // если кодовое слово не помещается целиком в буфер, то надо его делить
			// на 2 части или записывать по одному биту
			// (эта ветка выполняется редко, один раз на буфер)
			PartLen = cnBitStreamBitSize - ptrCurrentBit; // длина помещающейся части
			// записываем сначала младшую часть кодового слова
			*(unsigned long int *)((Buffer + (ptrCurrentBit >> 3))) |=
				((CodeWord & ((1 << PartLen) - 1)) << (ptrCurrentBit & 0x7));
			CodeWord >>= PartLen; // убрать ту часть, что уже записана
			ptrCurrentBit += PartLen; // сдвинуть указатель на конец буфера
			FlushBuffer(); // сбросить буфер на диск
			// записываем оставшуюся часть кодового слова
			*(unsigned long int *)(Buffer) |= CodeWord;
			ptrCurrentBit += (CodeBitLength - PartLen); // сдвинуть указатель
			// заполненность буфера не проверяем - он больше длины одного слова
		}
	}
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------
// Записать длинное кодовое слово CodeWord длиной CodeBitLength битов в битовый буфер.
// Запись выполняется битовыми полями по 16 битов + остаток.
// Длина кодового слова не ограничена.
// Массив CodeLonbgWord должен быть выровнен по границе слова (16 битов).
//---------------------------------------------------------------------------------
void cBitStream::WriteLongField(unsigned long int *CodeLongWord, unsigned int CodeBitLength)
{
	unsigned long int PartWord; // очередная 16-битовая часть кодового слова
	unsigned int NumWords = CodeBitLength >> 4; // количество полных 16-битовых частей
	unsigned int RemainBits = CodeBitLength & 0xF; // количество оставшихся битов
	unsigned int nw = 0;
	unsigned int *ptrWord = (unsigned int *)CodeLongWord;
	if (CurMode == mWrite) // если буфер в режиме записи
	{
		for (nw = 0; nw<NumWords; nw++) // сначала целые 2-байтовые слова
		{
			PartWord = *ptrWord++; // читать + сдвинуть указатель
			WriteField(PartWord, 16); // записать 16-битное поле
		}
		if (RemainBits) // если остались биты
		{
			PartWord = *ptrWord; // читать
			WriteField(PartWord, RemainBits); // записать оставшиеся биты
		}
	}
}
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
// Завершение работы с буфером - сброс остатков на диск и освобождение памяти
//---------------------------------------------------------------------------------
cBitStream::~cBitStream(void)
{
	// если буфер открыт на запись и там есть данные, то сначала надо сбросить их на диск
	if ((CurMode == mWrite) && (ptrCurrentBit > 0))
		FlushBuffer();
	// закрыть поток и связь с файлом
	ofp.flush();
	ofp.close();
	// после этого освободить память
	delete[cnBitStreamByteSize + 4] Buffer;
}
//


void main()
{
	ifstream DataIn; // Поток входных данных
	ofstream DataOut; // Поток выходных данных
	int N; // Количество элементов входных данных
	cBitStream Buffer1((unsigned char *) "Data.txt", mWrite); // Буфер записи
	unsigned long word = 0; // Очередное считанное из буфера слово
	DataIn.open("Data_input.txt");
	DataOut.open("Data_output.txt");
	DataIn >> N;
	cout << "N=" << N << endl;
	unsigned int *data = new unsigned int[N]; // Массив данных
	unsigned int *lengths = new unsigned int[N]; // Массив длин
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
	cBitStream Buffer2((unsigned char*) "Data.txt", mRead); // Буфер чтения
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

