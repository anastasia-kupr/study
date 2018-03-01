// Laba1.cpp : лаба 8(объекты и указатели), 11(полиморфизм), 12(библиотека stl) -- первые три лабы.
//

#include "stdafx.h"
#include <string.h>
#include <iostream>
#include <MATH.H>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

//                                                         class Matr
//-------------------------------------------------------------------
//struct c {
	//double* Re;
	//double* Im;
//};
template <class T>
class Matr
{
	T ** matr;
	long line;
	long col;

public:
	virtual ~Matr();                  //Деструктор
	Matr(long l, long c);         // Конструктор
	Matr():line(0), col(0), matr(NULL){} //Конструктор по умолчанию
	Matr(Matr<T>& A);                   //Конструктор копии

	T * operator[](long i){ return matr[i]; }
	template <class T>
	friend Matr<T>  operator*(Matr<T>& A, Matr<T>& B);
	const  Matr<T>& operator=(const Matr<T>& A);
	template <class T>
	friend Matr<T>  operator*(double K, const Matr<T>& A);
	Matr<T>         operator+(const Matr<T>& A);
	Matr<T>         operator-(const Matr<T>& A);
	void  display();

	template <class T>
	friend bool operator==(Matr<T>& A, Matr<T>& B);

	template <class T>
	friend bool operator!=(Matr<T>& A, Matr<T>& B);

	template <class T>
	friend bool operator>(Matr<T>& A, Matr<T>& B);

	template <class T>
	friend bool operator<(Matr<T>& A, Matr<T>& B);

	template <class T>
	friend Matr<T>  operator*(const Matr<T>& A, double K);

	template <class T>
	friend ostream& operator<<(ostream& stream, const Matr<T>& A);
};



class ComplexNumber {
	double Re;
	double Im;
public:
	~ComplexNumber(){};                  //Деструктор
	ComplexNumber(double r);         // Конструктор2
	ComplexNumber(double r, double i);         // Конструктор1
	ComplexNumber(){ //Конструктор по умолчанию
		Re = 0;
		Im = 0;
	}
	friend ComplexNumber& operator+(ComplexNumber& A, ComplexNumber& B);
	friend ComplexNumber& operator-(ComplexNumber& A, ComplexNumber& B);
	friend ComplexNumber& operator*(ComplexNumber& A, ComplexNumber& B);
	friend ComplexNumber& operator*(ComplexNumber& A, double K);
	friend ComplexNumber& operator*(double K, ComplexNumber& A);
	friend void operator+=(ComplexNumber& A, ComplexNumber& B);
	friend bool operator==(ComplexNumber& A, ComplexNumber& B);
	friend bool operator!=(ComplexNumber& A, ComplexNumber& B);
	friend ostream& operator<<(ostream& stream, const ComplexNumber& A);
	friend double abs(ComplexNumber A);
};

ComplexNumber::ComplexNumber(double r, double i) {
	//cout << "\nconstructor1 for ComplexNumber";
	Re = r;
	Im = i;
}

ComplexNumber::ComplexNumber(double r){
	//cout << "\nconstructor2 for ComplexNumber";
	Re = r;
	Im = 0;
}

ostream& operator<<(ostream& stream, const ComplexNumber& A) {
	if (A.Im>0)	stream << A.Re << "+" << A.Im<<"i";
	if (A.Im<0)	stream << A.Re << "-" << (A.Im*(-1.0))<<"i";
	if (A.Im==0) stream << A.Re;
	return stream;
}

double abs(ComplexNumber A){
	double result;
	result = sqrt(A.Re*A.Re + A.Im*A.Im);
	return result;
}

bool operator==(ComplexNumber& A, ComplexNumber& B) {
	if ((A.Re == B.Re) && (A.Im == B.Im)) return true;
	else return false;
}
bool operator!=(ComplexNumber& A, ComplexNumber& B) {
	return (!(A==B));
}

ComplexNumber& operator+(ComplexNumber& A, ComplexNumber& B) {
	ComplexNumber result = ComplexNumber(A.Re + B.Re, A.Im + B.Im);
	return result;
}

void operator+=(ComplexNumber& A, ComplexNumber& B) {	
	ComplexNumber result = ComplexNumber(A.Re + B.Re, A.Im + B.Im);
	A = result;
	return;
}
ComplexNumber& operator-(ComplexNumber& A, ComplexNumber& B) {
	ComplexNumber result = ComplexNumber(A.Re - B.Re, A.Im - B.Im);
	return result;
}

ComplexNumber& operator*(ComplexNumber& A, ComplexNumber& B) {	
	ComplexNumber result = ComplexNumber(A.Re*B.Re - A.Im*B.Im, A.Re*B.Im - A.Im*B.Re);
	return result;
}

ComplexNumber& operator*(ComplexNumber& A, double K) {
	ComplexNumber result = ComplexNumber(A.Re*K, A.Im*K);
	return result;
}
ComplexNumber& operator*(double K, ComplexNumber& A) {
	ComplexNumber result = ComplexNumber(K*A.Re, K*A.Im);
	return result;
}

//                                                Matr<T>::Matr()
//-------------------------------------------------------------------
template <class T>
Matr<T>::Matr(long l1, long col1)
{
	//cout << "\nUser constructor";
	line = l1;
	col = col1;
	matr = new T*[line];
	if (matr == NULL) cout << "Out of memory";
	for (int i = 0; i < line; i++){
		matr[i] = new T[col];
		if (matr[i] == NULL) cout << "Out of memory";
	}
}
//                                  Matr<T>::Matr(Matr<T>& A)
//-------------------------------------------------------------------
template <class T>
Matr<T>::Matr(Matr<T>& A) :line(A.line), col(A.col)
{
	//cout << "\ncopy constructor";
	matr = new T*[line];
	if (matr == NULL) cout << "Out of memory";
	for (int i = 0; i < line; i++){
		matr[i] = new T[col];
		if (matr[i] == NULL) cout << "Out of memory";
	}
	for (long j = 0; j<line; j++){
		for (int i = 0; i<col; i++){
			matr[j][i] = A.matr[j][i];
		}
	}
}
//                                               Matr<T>::~Matr()
//-------------------------------------------------------------------
template <class T>
Matr<T>::~Matr()
{
	//cout << "\ndestructor, matr=\n";
	/*for (int i = 0; i < line; i++) {
		for (int j = 0; j < col; j++) {
			cout << matr[i][j] << " ";
		}
		cout << "\n";
	}
	*/
	for (int i = 0; i < line; i++) delete[] matr[i];
	delete matr;
}
//                                 void  display(const Matr<T>& A)
//-------------------------------------------------------------------
template <class T>
void Matr<T>::display(){
	//cout << "\n display function";
	for (int i = 0; i<line; i++){
		cout << "\n";
		for (int j = 0; j < col; j++)
			cout << matr[i][j] << "\t";
	}
}
//                                           Matr<T>::operator*()
//-------------------------------------------------------------------
template <class T>
Matr<T> operator*(Matr<T>& A, Matr<T>& B)
{
	//cout << "\n operator M*M";
	Matr<T> arMatr(A.line, B.col);
	if (!(A.col == B.line)) { 
		cout << "\n A*B A.col != B.line";
		return arMatr;
	}
	long l1 = A.line;
	long col1 = A.col;
	long col2 = B.col;
	for (long i = 0; i < l1; i++){
		for (long j = 0; j < col2; j++){
			arMatr[i][j] = 0;
			for (long k = 0; k < col1; k++) {
				T temp = A[i][k] * B[k][j];
				arMatr[i][j] += temp;
			}
		}
	}
	return arMatr;
}
//                                                  Matr::operator=()
//-------------------------------------------------------------------
template <class T>
const Matr<T>& Matr<T>::operator=(const Matr<T>& A)
{
	//cout << "\n operator =";
	if (this == &A) return *this;
	line = A.line;
	col = A.col;
	for (long i = 0; i<A.line; i++){
		for (long j = 0; j<A.col; j++){
			matr[i][j] = A.matr[i][j];
		}
	}
	return *this;
}
//                                           Matr<T>::operator*()
//-------------------------------------------------------------------
template <class T>
Matr<T> operator*(double K, const Matr<T>& A)
{
	//cout << "\n operator K*M";
	Matr<T> M(A.line, A.col);
	for (long i = 0; i<A.line; i++){
		for (long j = 0; j<A.col; j++){
			M.matr[i][j] = K * A.matr[i][j];
		}
	}
	return M;
}
//                                           Matr<T>::operator+()
//-------------------------------------------------------------------
template <class T>
Matr<T> Matr<T>::operator+(const Matr<T>& A)
{
	//cout << "\n operator M+M";
	Matr<T> M(A.line, A.col);
	if ((line != A.line) || (col != A.col)) {
		cout << "\n problem with dimention:\n A.line!=B.line or A.col!=B.col";
		return M;
	}
	for (long i = 0; i<A.line; i++){
		for (long j = 0; j<A.col; j++){
			M.matr[i][j] = matr[i][j] + A.matr[i][j];
		}
	}
	return M;
}
//                                           Matr<T>::operator-()
//-------------------------------------------------------------------
template <class T>
Matr<T> Matr<T>::operator-(const Matr<T>& A)
{
	//cout << "\n operator M-M";
	Matr<T> M(A.line, A.col);
	if ((line != A.line) || (col != A.col)) {
		cout << "\n problem with dimention:\n A.line!=B.line or A.col!=B.col";
		return M;
	}
	for (long i = 0; i<A.line; i++){
		for (long j = 0; j<A.col; j++){
			M.matr[i][j] = matr[i][j] - A.matr[i][j];
		}
	}
	return M;
}
//                                                       TMatr()
//-------------------------------------------------------------------
template <class T>
Matr<T> TMatr(Matr<T>& M){
	Matr<T> TM(M.col, M.line);
	for (int i = 0; i < M.line; i++)
	for (int j = 0; j < M.col; j++)
		TM[j][i] = M[i][j];
	return TM;
}

//Пункт 3 лабораторной №8.
template <class T>
bool operator==(Matr<T>& A, Matr<T>& B) {
	if (A.col != B.col)  {
		return false;
	}
	if (A.line != B.line)  {
		return false;
	}
	for (int i = 0; i < A.line; i++) {
		for (int j = 0; j < A.col; j++) {
			if (A[i][j] != B[i][j])  {
				return false;
			}
		}
	}
	return true;
}

template <class T>
bool operator!=(Matr<T>& A, Matr<T>& B) {
	return !(A==B);
}

template <class T>
bool operator>(Matr<T>& A, Matr<T>& B) {
	double maxB = abs(B[0][0]);
	bool result = false;
	for (int i = 0; i < B.line; i++) {
		for (int j = 0; j < B.col; j++) {
			if (abs(B[i][j])>maxB) maxB = abs(B[i][j]);
		}
	}
	for (int i = 0; i < A.line; i++) {
		for (int j = 0; j < A.col; j++) {
			if (abs(A[i][j]) <= maxB)  {
				//cout << "\nA[" << i << "][" << j << "]=" << A[i][j] << ", maxB=" << maxB;
				return false;
			}
		}
	}
	result = true;
	return result;
}

template <class T>
bool operator<(Matr<T>& A, Matr<T>& B) {
	if (A == B) return false;
	if (A>B) return false;
	return true;
}

template <class T>
Matr<T> operator*(const Matr<T>& A, double K) {
	//cout << "\n operator M*K";
	Matr<T> M(A.line, A.col);
	for (long i = 0; i<A.line; i++){
		for (long j = 0; j<A.col; j++){
			M.matr[i][j] = A.matr[i][j]*K;
		}
	}
	return M;
}

template <class T>
ostream& operator<<(ostream& stream, const Matr<T>& A) {
	stream << "\n";
	for (long i = 0; i < A.line; i++){
		for (long j = 0; j < A.col; j++){
			stream << A.matr[i][j] << " ";
		}
		stream << "\n";
	}
	return stream;
}
struct CRCL {
public:
	double R;
	double x;
	double y;
};

class crcl {
public:
	CRCL C;
	double* R;
	double* x;
	double* y;
	crcl(){ //Конструктор по умолчанию
		cout << "\nconst1";
		//C = new CRCL;
		R = new double;
		x = new double;
		y = new double;
		//R = 0;
		//x = 0;
		//y = 0;
	}
	/*~crcl(){
		cout << "\ndest";
		delete R;
		delete x;
		delete y;
	};*/
	crcl(double R2, double x2, double y2) {
		C.R = R2;
		C.x = x2;
		C.y = y2;
		cout << "\nconst2";
		cout << "\nC.R=" << C.R << ", C.x=" << C.x << ", C.y=" << C.y;
		cout << "\nR2=" << R2 << ", x2=" << x2 << ", y2=" << y2;
		cout << "\n&R2=" << &R2 << ", &x2=" << &x2 << ", &y2=" << &y2<<"\n";
		//double* r3 = new double;
		//double* x3 = new double;
		//double* x3 = new double;
		R = &R2;
		x = &x2;
		y = &y2;
		cout << "\nR=" << *(R) << ", x=" << *(x) << ", y=" << *(y) << "\n";
	}
	/*crcl(crcl& A){
		cout << "\ncrcl(crcl& T)";
		R = A.R;
		x = A.x;
		y = A.y;
	};*/
	friend ostream& operator<<(ostream& stream, const crcl& A) {
		stream << "\nR="<<A.R << ", x=" << A.x << ", y="<<A.y;
		return stream;
	};
	/*friend void crcl::display()	{
		cout << "\n R = " << this.C.R << "\t x = " << this.C.x<<", y="<<C.y;
	}*/
};

class ComplexNumber2{
public:
	double* Re, *Im;
	ComplexNumber2() {
		cout << "\nComplexNumber2()";
		Re = new double;
		Im = new double;
		*Re = 0;
		*Im = 0;
	}
	ComplexNumber2(double r, double i)	{
		cout << "\nComplexNumber2(double r, double i)";
		Re = new double;
		Im = new double;
		*Re = r;
		*Im = i;
		cout << "\nRe=" << Re << ", Im=" << Im;
	}
	~ComplexNumber2(){
		cout << "\ndestr";
		cout << "\nRe=" << Re << ", Im=" << Im;
		delete Re;
		delete Im;
	}
	/*ComplexNumber2(const ComplexNumber2& T) {
		Re = T.Re;
		Im = T.Im;
	}*/
	ComplexNumber2(ComplexNumber2& T) {
	//const ComplexNumber2& operator=(const ComplexNumber2& T) {
		cout << "\noperator=";
		//*Re = *T.Re;
		//*Im = *T.Im;
		//cout << "\nRe=" << Re << ", Im=" << Im;
		//return *this;

		Re = new double;
		Im = new double;
		*Re = *T.Re;
		*Im = *T.Im;

	}



	friend ostream& operator<<(ostream& stream, const ComplexNumber2 A) {
		stream << "\nRe=" << *(A.Re) << ", Im=" << *(A.Im);
		return stream;
	};
};

struct comp{
	int* Re, *Im;
	comp() { Re = new int; Im = new int; *Re = 0; *Im = 0; }
	comp(int r, int i) { Re = new int; Im = new int; *Re = r; *Im = i; }
	friend ostream& operator<<(ostream& stream, const comp A) {
		stream << "\nRe=" << *(A.Re) << ", Im=" << *(A.Im);
		return stream;
	};
};

ComplexNumber2 func(ComplexNumber2 A) {
	return A;
}


void main(){
	//ComplexNumber2 C6;
	//comp C6 = comp(1,0);
	//cout << "\nC6.Re=" << *(C6.Re);
	//cout << "\nC6=" << C6<<endl;
	ComplexNumber2 C7 = ComplexNumber2(1.0, 1.0);
	cout << "\nC7=" << C7 << endl;
	func(C7);
	ComplexNumber2 C8 = C7;
	cout << "\nC8=" << C8;
	//crcl* X = new crcl;
	//crcl X2 = crcl(1, 2, 3);
	//X = &X2;
	/**(X2.R) = 1;
	*(X2.x) = 3;
	*(X2.y) = 4;*/
	//printf("\nX: R=%3.2f, x=%3.2f, y=%3.2f", (*X).R, (*X).x, (*X).y);
	//printf("\nX2: R=%3.2f, x=%3.2f, y=%3.2f", *(X2).R, *(X2).x, *(X2).y);
	//ComplexNumber* C = new ComplexNumber;
	//ComplexNumber C2 = *(C);
	//cout << "C=" << C2;
	/*Matr<ComplexNumber> A(2, 2), B(2, 2);
	A[0][0] = A[0][1] = A[1][0] = A[1][1] = ComplexNumber(1.0, 0.0);
	B[0][0] = B[0][1] = B[1][0] = B[1][1] = ComplexNumber(2.0, 0.0);
	//A.display();
	//B.display();
	cout << "\nA=" << A;
	cout << "\nB=" << B;
	A = (2.5*A - A + B)*B;
	cout << "\nA=" << A;
	Matr<double> A2(2, 3), B2(2, 2);
	A2[0][0] = A2[0][1] = A2[1][0] = A2[1][1] = 1.0;
	B2[0][0] = B2[0][1] = B2[1][0] = B2[1][1] = 2.0;
	cout << "\nA2=" << A2;
	cout << "\nB2=" << B2;
	A2 = (2.5*A2 - A2 + B2)*B2;
	cout << "\nA2=" << A2;
	ComplexNumber C1 = ComplexNumber(1);
	ComplexNumber C2 = ComplexNumber(2, -4);
	ComplexNumber C3 = C1 - C2;
	ComplexNumber C4 = C1 * C2;
	//cout << "\nC1=" << C1;
	//cout << "\nC2=" << C2;
	C1 = C1 + C2;
	double D = abs(C1);
	cout << "\nD="<<D;*/
	system("pause");
	//getchar();
}