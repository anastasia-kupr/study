#include "stdafx.h"
#include <iostream>
#include <string>
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
public:
	virtual void prepare() { cout << "\n prepare pizza"; }
	virtual void bake() { cout << "\n bake pizza"; }
	virtual void cut() { cout << "\n cut pizza"; }
	virtual void box() { cout << "\n box pizza"; }
};

class CheesePizza : public Pizza {
	virtual void prepare() { cout << "\n prepare cheesePizza"; }
	virtual void bake() { cout << "\n bake cheesePizza"; }
	virtual void cut() { cout << "\n cut cheesePizza"; }
	virtual void box() { cout << "\n box cheesePizza"; }
};
class PepperoniPizza : public Pizza {
	virtual void prepare() { cout << "\n prepare  PepperoniPizza"; }
	virtual void bake() { cout << "\n bake  PepperoniPizza"; }
	virtual void cut() { cout << "\n cut  PepperoniPizza"; }
	virtual void box() { cout << "\n box  PepperoniPizza"; }
};
class NewPizza : public Pizza {
	virtual void prepare() { cout << "\n prepare  NewPizza"; }
	virtual void bake() { cout << "\n bake  NewPizza"; }
	virtual void cut() { cout << "\n cut  NewPizza"; }
	virtual void box() { cout << "\n box  NewPizza"; }
};
class NewPizza2 : public Pizza {
	virtual void prepare() { cout << "\n prepare  NewPizza2"; }
	virtual void bake() { cout << "\n bake  NewPizza2"; }
	virtual void cut() { cout << "\n cut  NewPizza2"; }
	virtual void box() { cout << "\n box  NewPizza2"; }
};

typedef CountedPtr<Pizza> pPizza;
class SimplePizzaFactory {
	pPizza ppizza;
public:
	/*pPizza createPizza(string type) {
		if (type == "cheese") ppizza = pPizza(new CheesePizza());
		else if (type == "pepperoni")  ppizza = pPizza(new PepperoniPizza());
		else if (type == "newpizza")  ppizza = pPizza(new NewPizza());
		else if (type == "newpizza2")  ppizza = pPizza(new NewPizza2());
		else ppizza = pPizza(new Pizza());
		return ppizza;
	}*/
	virtual pPizza createPizza(string type) = 0;
};

class WestPizzaFactory : public SimplePizzaFactory {
	pPizza ppizza;
public:
	WestPizzaFactory() { }
	pPizza createPizza(string type) {
		cout << "\nWest Pizzeria\n";
		if (type == "cheese") {
			cout << "\nwest cheese\n";
			ppizza = pPizza(new CheesePizza());
		}
		else if (type == "pepperoni") {
			cout << "\nwest pepperoni\n";
			ppizza = pPizza(new PepperoniPizza());
		}
		else if (type == "newpizza")  {
			cout << "\nwest newpizza\n";
			ppizza = pPizza(new NewPizza());
		}
		else if (type == "newpizza2") {
			cout << "\nwest newpizza2\n";
			ppizza = pPizza(new NewPizza2());
		}
		else ppizza = pPizza(new Pizza());
		return ppizza;
	}
};


class PizzaStore { //пиццерия
	SimplePizzaFactory &factory;
public:
	PizzaStore(SimplePizzaFactory *fct) :factory(*fct) {
		
	}
	pPizza orderPizza(string type) { //заказ пиццы
		pPizza ppizza = factory.createPizza(type);
		ppizza->prepare();
		ppizza->bake();
		ppizza->cut();
		ppizza->box();
		return ppizza;

	}


};


int main()
{
	WestPizzaFactory spf;
	PizzaStore ps(&spf);
	ps.orderPizza("");
	ps.orderPizza("cheese");
	ps.orderPizza("pepperoni");
	ps.orderPizza("newpizza");
	ps.orderPizza("newpizza2");

	getchar();

	return 0;
}
