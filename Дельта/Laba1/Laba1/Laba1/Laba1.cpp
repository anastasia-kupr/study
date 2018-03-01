// Laba1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <string>
#include <iostream>
#include "stdlib.h"
#include <cstdio> 
#include <conio.h>
#include <string>
using namespace std;
const string INPUT_FILE_NAME = "Data_input.txt"; // ��� �������� ����� � �������
const string OUTPUT_FILE_NAME = "Data_output.txt"; // ��� �������� ����� � �������

int sign(double x) {
	if (x > 0)
		return 1;
	else if (x < 0)
		return -1;
	return 0;
}

void main()
{
	fstream inputStream; // ����� ������� ������
	fstream outputStream; // ����� �������� ������
	long int N = 0; // ���������� ������� ��������
	long int K = 30; // ���������� ����� ������������� ��� ���� �������� �������
	inputStream.open(INPUT_FILE_NAME);
	inputStream >> K;
	inputStream >> N;
	string data;
	double *tn = new double[N]; // ���� ������������
	double *y = new double[N]; // �������� ������� � ����� ������������
	double *dy = new double[N]; // �������� �����������
	// ��������� �������:
	double *T = new double[N - 1]; // ����� �������
	double *dt = new double[N - 1]; // ��� �������������
	double *P = new double[N - 1]; // ����� ��������������
	double *D = new double[N - 1]; // ����� ������������ ����� ������ ���������������
	double L = 0; // �����-�� ������������� ��������, �� ��� ��� �������
	for (int n = 0; n < N; n++) {
		inputStream >> tn[n];
	}
	for (int n = 0; n < N; n++) {
		inputStream >> y[n];
	}
	inputStream >> dy[0];
	inputStream >> dy[N - 1];
	inputStream.close();
	T[N - 1] = tn[N] - tn[N - 1];
	dt[N - 1] = T[N - 1] / K;
	for (int n = 0; n < N - 1; n++) { // ���������� ���� �������� ������� � ���� ������������� ��� ������� �������
		T[n] = tn[n + 1] - tn[n];
		dt[n] = T[n] / K;
		//cout << "T[" << n << "]=" << T[n] << ", ";
		//cout << "dt[" << n << "]=" << dt[n] << ", ";
		/*if (dt[n-1] <= 0) {
		cout << "!!! \ntn[" << n + 1 << "]=" << tn[n + 1] << ", tn[" << n << "]=" << tn[n] << "\n";
		system("pause");
		return;
		}*/
	}

	long int I = (N - 1)*K; // ���������� �����, �� ������� ����� ��������� ������
	double *Y = new double[I]; // ���������� �������: �������� ������� y(t)
	double *t = new double[I]; // ���������� �������: �������� ������� y(t)

	t[0] = tn[0];
	int i = 0;
	for (int n = 0; n < N - 1; n++) {
		for (int k = 0; k < K; k++) {
			i = n*K + k;
			if (i == I)
				break;
			t[i + 1] = t[i] + dt[n];
			//cout << "n=" << n << ", dt= " << dt[n] << " t[" << i << "]=" << t[i] << "\n";
		}
	}
	for (int n = 1; n < N - 1; n++) { // ���������� �����������
		if (((y[n - 1] <= y[n]) && (y[n + 1] <= y[n])) || ((y[n - 1] >= y[n]) && (y[n + 1] >= y[n]))) {
			dy[n] = 0;
		}
		else {
			dy[n] = (((y[n] - y[n - 1]) / (tn[n] - tn[n - 1])) + ((y[n + 1] - y[n]) / (tn[n + 1] - tn[n]))) / 2;
		}
	}
	for (int n = 0; n < N - 1; n++) { // ���������� ���������� �������
		//cout << "n=" << n << ", tn[n+1]=" << tn[n + 1] << " ,tn[n]=" << tn[n] << ", T=" << T[n] << "\n";
		L = y[n + 1] - y[n] - 0.5*T[n] * (dy[n + 1] + dy[n]);
		P[n] = (-L - sign(L)*sqrt(L*L + 0.25*T[n] * T[n] * (dy[n + 1] - dy[n])*(dy[n + 1] - dy[n]))) / (0.5*T[n] * T[n]);
		printf("n=%1d, L=%6.3f, P=%6.3f, ", n, L, P[n]);
		if ((P[n] == 0) || (L == 0))
			D[n] = 0;
		else D[n] = (dy[n + 1] - dy[n] + T[n] * P[n]) / (2 * P[n]);
		printf("D=%6.3f\n", D[n]);
		if ((tn[n + 1] - D[n]) < tn[n]) { // �������� �� ������ ���������� ������
			cout << "\n(tn[n + 1] - D[n]) < tn[n] - FUCKED UP\n";
			return;
		}
	}
	double tp; // ����� ������������ ������ ��������������
	double ta;
	double tb;
	double deltaA; // �������� ����������
	double deltaB; // �������� ����������
	//int i = 0;
	for (int n = 0; n < N - 1; n++) {
		//cout << "n=" << n << ", tp=" << tp << "\n";
		tp = tn[n + 1] - D[n];
		ta = tn[n + 1];
		tb = tn[n];
		printf("n=%1d, tp=%5.2f, dt=%5.2f, P=%5.2f\n", n, tp, dt[n], P[n]);
		for (int k = 0; k < K; k++) {
			int i = n*K + k;
			if (t[i] < tp) {
				cout << "B: ";
				//deltaB = -dy[n + 1] * (T[n] - t[i]) + ((T[n] - t[i])*(T[n] - t[i])*P[n]) / 2;
				deltaB = dy[n] * t[i] - (t[i] * t[i] * P[n]) / 2;
				Y[i] = y[n] + deltaB;
				cout << "i=" << i << ", t[" << i << "]=" << t[i] << ", deltaB=" << deltaB << ", Y=" << Y[i] << "\n";
			}
			else {
				cout << "A: ";
				deltaA = -dy[n + 1] * (T[n] - t[i]) + ((T[n] - t[i])*(T[n] - t[i])*P[n]) / 2;
				Y[i] = y[n + 1] + deltaA;
				cout << "i=" << i << ", t[" << i << "]=" << t[i] << ", deltaA=" << deltaA << ", Y=" << Y[i] << "\n";
			}
		}

		//system("pause");
	}

	outputStream.open(OUTPUT_FILE_NAME);
	for (int j = 0; j < I; j++) {
		outputStream << t[j] << " ";
	}
	outputStream << "\n";
	for (int j = 0; j < I; j++) {
		outputStream << Y[j] << " ";
	}
	/*outputStream << "\n";
	for (int j = 0; j < N; j++) {
		outputStream << tn[j] << " ";
	}
	outputStream << "\n";
	for (int j = 0; j < N; j++) {-
		outputStream << y[j] << " ";
	}*/
	//outputStream << "\n";
	outputStream.clear();
	outputStream.close();
	cout << "\nt:";
	for (int n = 0; n < N; n++) {
		cout << tn[n] << " ";
	}
	cout << "\ndt:";
	for (int n = 0; n < N; n++) {
		cout << dt[n] << " ";
	}
	cout << "\ny:";
	for (int n = 0; n < N; n++) {
		cout << y[n] << " ";
	}
	cout << "\ndy:";
	for (int n = 0; n < N; n++) {
		cout << dy[n] << " ";
	}
	system("pause");
}

