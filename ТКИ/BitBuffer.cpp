// BitBuffer.cpp : Helps me to get points.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include "stdlib.h"
#include <cstdio> 
#include <conio.h>
using namespace std;
const unsigned long int BUFFER_SIZE = 6; // ������ ������ (� ������)
const string INPUT_FILE_NAME = "Data_input.txt"; // ��� �������� �����
const string OUTPUT_FILE_NAME = "Data_output.txt"; // ��� ��������� �����
const string TEMP_FILE_NAME = "temp.txt"; // ��� ����� ��� ���������� �������� ������ �������

class Writer { // ����� ������ ��� ������
public:
	unsigned char* data = new unsigned char[BUFFER_SIZE + 4]; // ������ ������
	unsigned long int freePlace; // ��������� �� ������� (���������) ���
	ofstream file; // ������� ����
	unsigned long int bufferSize; // ������� ������ ������ (� �����)
	
	Writer(string fileName) { // �����������
		file.open(fileName);
		memset(data, 0, BUFFER_SIZE); // �������
		freePlace = 0;
		bufferSize = BUFFER_SIZE * 8;
	}

	void Write(unsigned long int word, unsigned int length) { // ������ �����
		unsigned char currentBit;
		for (int i = 0; i < length; i++) 	{
			currentBit = (unsigned char)(word & 1);
			word >>= 1;
			*(data + (freePlace >> 3)) |= (currentBit << (freePlace & 0x7));
			freePlace++;
			if (freePlace == (BUFFER_SIZE * 8))	Flush();
		}
	}

	void Flush() { // ����� �� ����
		if (freePlace != (BUFFER_SIZE * 8))	bufferSize = (freePlace + 0x7) & 0xFFFFFFF8;
		file.write((char*)data, (bufferSize >> 3));
		memset(data, 0, BUFFER_SIZE); // �������
		freePlace = 0;
	}
};

class Reader { // ����� ������ ��� ������
public:
	unsigned char* data; // ������ ������
	unsigned long int freePlace; // ��������� �� ������� (���������) ���
	ifstream file; // ������� ����
	unsigned long int bufferSize; // ������� ������ ������ (� �����)
	
	Reader(string fileName) { // �����������
		data = new unsigned char[BUFFER_SIZE + 4];
		file.open(fileName);
		Load();
	}

	void Read(unsigned long int *word, unsigned int length) { // ������ �����
		unsigned char currentBit;
		*word = 0;
		for (int i = 0; i < length; i++) {
			if (freePlace == bufferSize) Load();
			currentBit = (*(data + (freePlace >> 3))) >> (freePlace & 0x7) & 0x01;
			freePlace++;
			*word |= ((unsigned long int)(currentBit) << i);
		}
	}

	void Load() { // �������� ������ �� �����
		file.read((char*)data, BUFFER_SIZE);
		bufferSize = file.gcount() << 3;
		freePlace = 0;
	}
};

void main()
{
	ifstream inputStream; // ����� ������� ������
	ofstream outputStream; // ����� �������� ������
	int N; // ���������� ��������� ������� ������
	Writer writer(TEMP_FILE_NAME); // ����� ������
	unsigned long word = 0; // ��������� ��������� �� ������ �����
	bool success = true;
	inputStream.open(INPUT_FILE_NAME);
	outputStream.open(OUTPUT_FILE_NAME);
	inputStream >> N;
	unsigned int* data = new unsigned int[N]; // ������ ������
	unsigned int* lengths = new unsigned int[N]; // ������ ����
	for (int i = 0; i < N; i++)	inputStream >> data[i] >> lengths[i];
	for (int i = 0; i < N; i++) writer.Write(data[i], lengths[i]);
	if (writer.freePlace > 0) writer.Flush();
	writer.file.flush();
	writer.file.close();
	Reader reader(TEMP_FILE_NAME); // ����� ������
	for (int i = 0; i < N; i++) {
		reader.Read(&word, lengths[i]);
		outputStream << word << endl;
		if (word != data[i]) {
			success = false;
			cout << "Something went wrong: data is " << data[i] << ", word is " << word << "\n";
		}
	}
	if (success) cout << "Success!\n";
	else cout << "Somethinng went wrong.";
	reader.file.close();
	outputStream.flush();
	outputStream.close();
	inputStream.close();
	system("pause");
}