#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

template <class T>
class CountedPtr {
private:
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
		if (--*count == 0) {
			delete count;
			delete ptr;
		}
	}
};


class Pizza {
protected:
	string name;
	string dough;
	string sauce;
	vector<string> ls;
public:
	virtual void prepare() { 
		cout << "\n Preparing" + name;	
		for (int i = 0; i < ls.size(); i++) cout << " " + ls[i];
	}
	virtual void bake() { cout << "\n Bake for 25 minutes at 350"; }
	virtual void cut() { cout << "\n Cuting the pizza into diagonal slices"; }
	virtual void box() { cout << "\n Place pizza in official PizzaStore box"; }
};

typedef CountedPtr<Pizza> pPizza;

class NYStyleCheesePizza : public Pizza {
public: 
	NYStyleCheesePizza() {
		name = "NY Style Sauce and cheese pizza"; 
		dough = "Thin Crust Dough"; 
		sauce = "Marinara Sauce"; 
		ls.push_back(string("Grated Reggiano Chese"));
	}
	virtual void prepare() { cout << "\n prepare NYStyleCheesePizza"; }
	virtual void bake() { cout << "\n bake cheesePizza"; }
	virtual void cut() { cout << "\n cut cheesePizza"; }
	virtual void box() { cout << "\n box cheesePizza"; }
};

class NYStylePepperoniPizza : public Pizza {
	virtual void prepare() { cout << "\n prepare  NYStylePepperoniPizza"; }
	virtual void bake() { cout << "\n bake  PepperoniPizza"; }
	virtual void cut() { cout << "\n cut  PepperoniPizza"; }
	virtual void box() { cout << "\n box  PepperoniPizza"; }
};


class MsStyleCheesePizza : public Pizza {
	virtual void prepare() { cout << "\n prepare MsStyleCheesePizza"; }
	virtual void bake() { cout << "\n bake cheesePizza"; }
	virtual void cut() { cout << "\n cut cheesePizza"; }
	virtual void box() { cout << "\n box cheesePizza"; }
};


class PizzaStore { //пиццерия
	
public:
	pPizza orderPizza(string type) { //заказ пиццы
		pPizza pizza = createPizza(type);
		pizza->prepare();
		pizza->bake();
		pizza->cut();
		pizza->box();
		return pizza;

	}
protected:
	virtual  pPizza createPizza(string type)=0;
};

class NYPizzaStore: public PizzaStore {
	public:
	pPizza createPizza(string type) {
		pPizza ppizza;
		if (type == "cheese") ppizza = pPizza(new  NYStyleCheesePizza());
		else if (type == "pepperoni")  ppizza = pPizza(new NYStylePepperoniPizza());
		else ppizza = pPizza(new Pizza());
		return ppizza;
	}
};

int main()
{
	NYPizzaStore NYps;
	NYps.orderPizza("");
	NYps.orderPizza("cheese");
	NYps.orderPizza("pepperoni");
	getchar();
	return 0;
}
