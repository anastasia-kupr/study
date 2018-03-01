// gofdecorator.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;

template <class T>
class CountedPtr {
private:
	T* ptr; // Указатель на значение
	long* count; // Количество владельцев (общие данные)

public:
	// Инициализация объекта существующим указателем
	// - указатель p должен быть получен в результате вызова new
	explicit CountedPtr(T* p = 0) : ptr(p), count(new long(1)) 
	{

	}

	// Копирующий указатель (увеличивает счётчик владельцев)
	CountedPtr(const CountedPtr<T>& p) throw() : ptr(p.ptr), count(p.count) 
	{
		++*count;
	}

	// Деструктор (уничтожает объект, если владелец был последним)
	~CountedPtr() throw() 
	{
		dispose();
	}

	// Присваивание (перевод указателя на новый объект)
	CountedPtr<T>& operator= (const CountedPtr<T>& p) throw() 
	{
		if (this != &p) 
		{
			dispose();
			ptr = p.ptr;
			count = p.count;
			++*count;
		}
		return *this;
	}

	// Доступ к объекту, на который ссылается указатель
	T& operator*() const throw() 
	{
		return *ptr;
	}

	T* operator->() const throw() 
	{
		return ptr;
	}

private:

	void dispose() 
	{
		if (--*count == 0) 
		{
			delete count;
			delete ptr;
		}
	}
};

enum CofeSize {
	cs_normal,
	cs_small,
	cs_big
};

class Beverage {
protected:
	string description;
	double size;
public:
	void setDescription(string d) { description = d; }
	virtual string GetDescription() { return description; }
	virtual double cost() = 0;
	double GetSize() { return size; }
	void SetSize(CofeSize s) { 
		if (s == cs_normal)
		{
			description += " Normal";
			size = 0;
		}
		else if (s == cs_small)
		{
			description += " Small";
			size = -0.5;
		}
		else if (s == cs_big)
		{
			description += " Big";
			size = 1.0;
		}
	}

};

typedef CountedPtr<Beverage> pBeverage;

class CondimentDecorator : public Beverage {
protected:
	pBeverage beverage;
	CondimentDecorator(pBeverage pBg) : beverage(pBg) {}
};

class s : public Beverage {
public:
	s() { description = ""; }
	double cost() { return 0; }
};

class Espresso : public Beverage {
public:
	Espresso() { description = "Espresso"; }
	double cost() { return 1.99 + size; }
};

class HouseBlend : public Beverage {
public:
	HouseBlend() { description = "HouseBlend"; }
	double cost() { return 0.89 + size; }
};

class IrishCream : public Beverage {
public:
	IrishCream() { description = "IrishCream"; }
	double cost() { return 3.59 + size; }
};

class Mocco : public Beverage {
public:
	Mocco() { description = "Mocco"; }
	double cost() { return 2.29 + size; }
};

class Mocha : public CondimentDecorator {
public:
	Mocha(pBeverage bg) : CondimentDecorator(bg) {}
public:
	string GetDescription() { return (beverage->GetDescription() + string(", Mocha")); }
	double cost() { return 0.20 + beverage->cost(); }
};

class Soy : public CondimentDecorator {
public:
	Soy(pBeverage bg) : CondimentDecorator(bg) {}
public:
	string GetDescription() { return (beverage->GetDescription() + string(", Soy")); }
	double cost() { return 0.16 + beverage->cost(); }
};

class Canella : public CondimentDecorator {
public:
	Canella(pBeverage bg) : CondimentDecorator(bg) {}
public:
	string GetDescription() { return (beverage->GetDescription() + string(", Canella")); }
	double cost() { return 0.28 + beverage->cost(); }
};

class Milk : public CondimentDecorator {
public:
	Milk(pBeverage bg) : CondimentDecorator(bg) {}
public:
	string GetDescription() { return (beverage->GetDescription() + string(", Milk")); }
	double cost() { return 0.18 + beverage->cost(); }
};

class Menu {
	std::map<string, double> m;
public:
	void add(pBeverage b) { m.insert(std::make_pair<string, double>(b->GetDescription(), b->cost())); }
	void show()
	{
		for (auto it : m)
			cout << it.first << "\t" << it.second << "\n";
	}
};

int main()
{
	pBeverage pB2 = pBeverage(new HouseBlend());
	pB2->SetSize(cs_small);
	pB2 = pBeverage(new Milk(pB2));	
	cout << pB2->GetDescription() + " $ " << pB2->cost();

	//Menu m;
	//pBeverage s0 = pBeverage(new s());
	//m.add(pBeverage(new Canella(s0)));
	//m.add(pBeverage(new Milk(s0)));
	//m.add(pBeverage(new Espresso()));
	//m.add(pBeverage(new HouseBlend()));
	//m.add(pBeverage(new IrishCream()));
	//m.add(pBeverage(new Mocco()));
	//m.show();

	getchar();
	return 0;
}
