// Entropy.cpp : Calculate enpropies.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include "stdlib.h"
#include <cstdio> 
#include <conio.h>
using namespace std;
const string INPUT_FILE_NAME = "test2.txt";
const string OUTPUT_FILE_NAME = "result.txt";
const int N = 255; // максимальное количество символов в алфавите
typedef struct {
	char symbol;
	double count;
	double p;
	double h;
} statistic;

typedef struct {
	char first;
	char second;
	double count;
	double p;
} couple;

void main()
{
	ifstream input(INPUT_FILE_NAME);
	ofstream output(OUTPUT_FILE_NAME);
	char next, previous = input.get();
	statistic* array1 = new statistic[N];
	couple* array2 = new couple[N * N];
	unsigned long int count = 1, symbols = 1, couples = 0;
	double entropy1 = 0, entropy2 = 0;
	bool found = false;
	array1[0].symbol = previous;
	array1[0].count = 1;
	while ((next = input.get()) != EOF) {
		found = false;
		for (int i = 0; i < symbols; i++) {
			if (next == array1[i].symbol) {
				found = true;
				array1[i].count++;
			}
		}
		if (!found) {
			array1[symbols].symbol = next;
			array1[symbols].count = 1;
			symbols++;
		}
		found = false;
		for (int i = 0; i < couples; i++) {
			if ((previous == array2[i].first)&&(next == array2[i].second)) {
				found = true;
				array2[i].count++;
			}
		}
		if (!found) {
			array2[couples].first = previous;
			array2[couples].second = next;
			array2[couples].count = 1;
			couples++;
		}
		count++;
		previous = next;
	}
	for (int i = 0; i < symbols; i++) {
		array1[i].p = array1[i].count / count;
		entropy1 = entropy1 - array1[i].p*log(array1[i].p)/log(2);
		if (array1[i].symbol == previous) {
			array1[i].count--;
		}
	}
	for (int i = 0; i < couples; i++) {
		for (int j = 0; j < symbols; j++) {
			if ((array2[i].first == array1[j].symbol) && (array1[j].count!=0)) {
				array2[i].p = array2[i].count / array1[j].count;
				break;
			}
		}
	}
	for (int i = 0; i < symbols; i++) {
		array1[i].h = 0;
		for (int j = 0; j < couples; j++) {
			if (array2[j].first == array1[i].symbol) {
				array1[i].h = array1[i].h - array2[j].p*log(array2[j].p) / log(2);
			}
		}
		entropy2 = entropy2 + array1[i].h*array1[i].p;
	}
	output << "entropy1=" << entropy1 << ", entropy2=" << entropy2;
	cout << "entropy1 = " << entropy1 << endl;
	cout << "entropy2 = " << entropy2 << endl;
	input.close();
	output.close();
	system("pause");
}