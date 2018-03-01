#include "stdafx.h"
#include <iostream>
#include <list>
#include <deque>
# include <set>
#include <algorithm>
using namespace std;

/* Класс, обеспечивающий семантику подсчёта ссылок.
*  Объект, на который ссылается указатель, автоматически
*   уничтожается при удалении последнего экземпляра CountedPtr
*   для данного объекта.
*/
template <class T>
class CountedPtr {
public:
	T* ptr; 		// Указатель на значение
	long* count; 	// Количество владельцев (общие данные)
public:
	// Инициализация объекта существующим указателем
	// - указатель p должен быть получен в результате вызова new
	explicit CountedPtr(T* p = 0)
		: ptr(p), count(new long(1)) {
	}

	// Копирующий указатель (увеличивает счётчик владельцев)
	CountedPtr(const CountedPtr<T>& p) throw()
		: ptr(p.ptr), count(p.count) {
		++*count;
	}
	// Деструктор (уничтожает объект, если владелец был последним)
	~CountedPtr() throw() {
		dispose();
	}
	// Присваивание (перевод указателя на новый объект)
	CountedPtr<T>& operator= (const CountedPtr<T>& p) throw() {
		if (this != &p) {
			dispose();
			ptr = p.ptr;
			count = p.count;
			++*count;
		}
		return *this;
	}

	// Доступ к объекту, на который ссылается указатель
	T& operator*() const throw() {
		return *ptr;
	}
	T* operator->() const throw() {
		return ptr;
	}
private:
	void dispose() {
		//cout << "\ndestr, count=" << *(count) << ", ptr=" << ptr;
		if (--*count == 0) {
			cout << "\ndestr, count=" << *(count) << ", ptr=" << ptr;
			delete count;
			delete ptr;
		}
	}
};


struct comp {
public: int* Re, *Im;
	comp() { Re = new int; Im = new int;; *Re = 0; *Im = 0; }
	comp(int r, int i) { Re = new int; Im = new int; *Re = r; *Im = i; }
	~comp() { delete Re; delete Im; }
	const comp& operator=(const comp& T) { // оператор функция= 
		*Re = *T.Re; *Im = *T.Im; return *this;
	}
	const comp& operator-() { // оператор функция- 
		*Re = -*Re; *Im = -*Im; return *this;
	}
	comp(comp& T) { Re = new int;    Im = new int;  *Re = *T.Re; *Im = *T.Im; }
	const comp operator*(const comp& T) { // оператор функция* 
		comp Rez(0, 0);
		*Rez.Re = *Re *  *T.Re - *Im *  *T.Im;
		*Rez.Im = *Re *  *T.Im + *Im *  *T.Re;
		//cout << "\noperator* T.Re=" << *T.Re << ", T.Im=" << *T.Im;
		return Rez;
	}
	double modComp()const { return sqrt(*Re**Re + *Im**Im); }
	bool operator<(const comp T) { // оператор функция<
		if (modComp() < T.modComp()) return 0;//х<х всегда ложно
		return 1;
	}
	/*bool operator<(const comp& T) { // оператор функция<
		if (Re < T.Re) return 0;
		return 1;
	}*/
	void comp::display() const
	{
		cout << "\n Re = " << *Re << "\t Im = " << *Im;
	}
	void printCountedPtr2(comp elem);

};

bool operator< (const CountedPtr<comp> p1, const CountedPtr<comp> p2) {
	if (*p1 < *p2) return 0;
	return 1;
}



void printCountedPtr(CountedPtr<comp> elem)
{
	(*elem).display();
}


void printCountedPtr2(comp elem)
{
	//elem.display();
	cout << "\n" << *(elem.Re) << " " << *(elem.Re);
}

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
		//cout << "\ndefault constructor, this=" << this;
	}
	friend ComplexNumber& operator+(ComplexNumber& A, ComplexNumber& B);
	friend ComplexNumber& operator-(ComplexNumber& A, ComplexNumber& B);
	friend ComplexNumber& operator*(ComplexNumber& A, ComplexNumber& B);
	friend ComplexNumber& operator*(ComplexNumber& A, double K);
	friend ComplexNumber& operator*(double K, ComplexNumber& A);
	friend void operator+=(ComplexNumber& A, ComplexNumber& B);
	friend bool operator==(const ComplexNumber& A, const ComplexNumber& B);
	friend bool operator<(const ComplexNumber& A, const ComplexNumber& B);
	friend bool operator!=(ComplexNumber& A, ComplexNumber& B);
	friend ostream& operator<<(ostream& stream, const ComplexNumber& A);
	friend double abs(const ComplexNumber& A);
	friend void display2(ComplexNumber& A);
};

void printComplexNumberDeque(ComplexNumber elem)
{
	cout << "\n" << elem;
}

ComplexNumber::ComplexNumber(double r, double i) {
	Re = r;
	Im = i;
	//cout << "\nconstructor1 for ComplexNumber, this="<<this;
}

ComplexNumber::ComplexNumber(double r){
	Re = r;
	Im = 0;
	//cout << "\nconstructor2 for ComplexNumber, r=" << r<<" , this="<<this;
}

ostream& operator<<(ostream& stream, const ComplexNumber& A) {
	if (A.Im>0)	stream << A.Re << "+" << A.Im << "i";
	if (A.Im<0)	stream << A.Re << "-" << (A.Im*(-1.0)) << "i";
	if (A.Im == 0) stream << A.Re;
	return stream;
}

double abs(const ComplexNumber& A){
	//cout << "\n double abs" << A;
	//double result;
	//cout << "\nA.Re=" << A.Re << ", A.Im=" << A.Im;
	//result = sqrt(A.Re*A.Re + A.Im*A.Im);
	//cout << "\nresult abs=" << result;
	return sqrt(A.Re*A.Re + A.Im*A.Im);
}

bool operator==(const ComplexNumber& A, const ComplexNumber& B) {
	if ((A.Re == B.Re) && (A.Im == B.Im)) return true;
	else return false;
}
bool operator<(const ComplexNumber& A, const ComplexNumber& B) {
	//if ((A.Re < B.Re) && (A.Im < B.Im)) return true;
	//cout << "\n operator<";
	if (abs(A) < abs(B)) return true;
	else return false;
}
bool operator!=(ComplexNumber& A, ComplexNumber& B) {
	return (!(A == B));
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
	ComplexNumber result = ComplexNumber(A.Re*B.Re - A.Im*B.Im, A.Re*B.Im + A.Im*B.Re);
	//cout << "\noperator*(ComplexNumber& A, ComplexNumber& B)\nA.Re=" << A.Re << ", A.Im=" << A.Im << ", B.Re=" << B.Re << ", B.Im=" << B.Im;
	//cout << "\nresult=" << result;
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

void display2(ComplexNumber& A){
	if ((A.Im)<0) {
		printf("%5.2f%5.2fi\n", A.Re, A.Im);
		return;
	}
	if ((A.Im)==0) {
		printf("%5.2fi\n", A.Re, A.Im);
		return;
	}
	printf("%5.2f+%5.2fi", A.Re, A.Im);
}
typedef CountedPtr<comp> IntPtr;

CountedPtr<comp> func(CountedPtr<comp> Y) {
	//cout << "Y.ptr=" << Y.ptr;
	return Y;
};


void printIntSet(int elem)
{
	//elem.display();
	cout << elem << " ";
}

int main()
{


	// Три разные коллекции
	//CountedPtr<comp> X, Z;
	//cout << "X.ptr=" << X.ptr;
	//Z = func(X);
	deque<IntPtr> coll1;
	list<IntPtr> coll2;
	set<IntPtr> coll3;
	deque<comp> coll11;
	list<comp> coll22;
	//set<comp> coll33;

	set<ComplexNumber> COLL;
	set<ComplexNumber> COLL2;
	for (int i = 0; i < 5; ++i) {
		COLL.insert(i);
		COLL2.insert(i);
	}
	cout << "\nset, COLL\n";
	for_each(COLL.begin(), COLL.end(), printComplexNumberDeque);
	cout << "\nset, COLL2\n";
	for_each(COLL2.begin(), COLL2.end(), printComplexNumberDeque);
		
	/* Вставка общих объектов в коллекции*/
	/*for (int i = 0; i<5; ++i) {
		IntPtr ptr(new comp(i+1, i+1));
		coll1.push_back(ptr);
		coll2.push_front(ptr);
		coll3.insert(ptr);
		coll11.push_back(comp(i+1, i+1));
		coll22.push_front(comp(i+1, i+1));
	}
	// Вывод содержимого коллекций
	cout << "\n deque, coll1\n";
	for_each(coll1.begin(), coll1.end(), printCountedPtr);
	cout << "\n list, coll2\n";
	for_each(coll2.begin(), coll2.end(), printCountedPtr);
	cout << "\n set, coll3\n";
	for_each(coll3.begin(), coll3.end(), printCountedPtr);
	cout << endl << endl;
	cout << "\n deque, coll11:\n";
	for_each(coll11.begin(), coll11.end(), printCountedPtr2);
	cout << "\n list, coll22\n";
	for_each(coll22.begin(), coll22.end(), printCountedPtr2);
	//cout << "\n set, coll33\n";
	//for_each(coll33.begin(), coll33.end(), printCountedPtr2);
	cout << endl << endl;


	*coll2.front() = -(*coll2.front());

	cout << "\n1: \n";
	*coll1[1] = *coll1[1] * *coll1[1];
	cout << "\n2: \n";
	coll11[1] = coll11[1] * coll11[1];
	coll22.front() = -(coll22.front());
	coll22.back() = -(coll22.back());


	// Повторный вывод содержимого коллекций
	cout << "\n deque, coll1\n";
	for_each(coll1.begin(), coll1.end(), printCountedPtr);
	cout << "\n list, coll2\n";
	for_each(coll2.begin(), coll2.end(), printCountedPtr);
	cout << "\n set, coll3\n";
	for_each(coll3.begin(), coll3.end(), printCountedPtr);
	cout << endl << endl;
	cout << "\n deque, coll11:\n";
	for_each(coll11.begin(), coll11.end(), printCountedPtr2);
	cout << "\n list, coll22\n";
	for_each(coll22.begin(), coll22.end(), printCountedPtr2);
	//cout << "\n set, coll33\n";
	//for_each(coll33.begin(), coll33.end(), printCountedPtr2);
	cout << endl << endl;
	
	deque<ComplexNumber> coll4;
	list<ComplexNumber> coll5;
	set<ComplexNumber> coll6;

	for (int i = 0; i<5; ++i) {
		ComplexNumber C = ComplexNumber(i, i+1);
		coll4.push_back(C); //coll1.push_back(comp(i,i));
		coll5.push_front(C);
		coll6.insert(C);
		COLL.insert(i);
	}
	
	cout << "\n set, COLL\n";
	for_each(COLL.begin(), COLL.end(), printComplexNumberDeque);
	// Вывод содержимого коллекций
	cout << "\n deque, coll4";
	for_each(coll4.begin(), coll4.end(), printComplexNumberDeque);
	cout << "\n list, coll5";
	for_each(coll5.begin(), coll5.end(), printComplexNumberDeque);
	cout << "\n set, coll6";
	for_each(coll6.begin(), coll6.end(), printComplexNumberDeque);
	cout << endl << endl;
	

	coll4[0] = *(new ComplexNumber(coll4[0] * coll4[0]));
	coll4[1] = *(new ComplexNumber(ComplexNumber(coll4[0]) * ComplexNumber(0, 1)));
	coll4[2] = *(new ComplexNumber(ComplexNumber(0, 100) + ComplexNumber(coll4[2])));
	coll4[3] = *(new ComplexNumber(ComplexNumber(abs(coll4[3])) + ComplexNumber(1, 0)));
	ComplexNumber c;
	//cout << "\n 1: coll4[4]=" << coll4[4] << ", &coll4[4]=" << &coll4[4] << ", c=" << c;
	c = ComplexNumber(10, 10) * coll4[4];
	//cout << "\n 1: coll4[4]=" << coll4[4] << ", &coll4[4]=" << &coll4[4] << ", c=" << c;
	coll4[4] = c;
	//cout << "\n 2: coll4[4]=" << coll4[4] << ", &coll4[4]=" << &coll4[4] << ", c=" << c;
	c = c * 2;
	//cout << "\n 3: coll4[4]=" << coll4[4] << ", &coll4[4]=" << &coll4[4] << ", c=" << c;
	//printf("\ncoll4[1]1=%8d\n", coll4[1]);
	coll5.front() = *(new ComplexNumber(coll5.front()*(-1.0)));
	

	// Вывод содержимого коллекций
	cout << "\n deque, coll4:";
	for_each(coll4.begin(), coll4.end(), printComplexNumberDeque);
	cout << "\n list, coll5";
	for_each(coll5.begin(), coll5.end(), printComplexNumberDeque);
	cout << "\n set, coll6";
	for_each(coll6.begin(), coll6.end(), printComplexNumberDeque);
	cout << endl << endl;
	*/

	system("pause");
}
