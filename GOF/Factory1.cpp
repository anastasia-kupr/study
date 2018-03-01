// Factory1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//template <class T>
//class CountedPtr {
//private:
//	T* ptr; // Указатель на значение
//	long* count; // Количество владельцев (общие данные)
//public:
//	// Инициализация объекта существующим указателем
//	// - указатель p должен быть получен в результате вызова new
//	explicit CountedPtr(T* p = 0)
//		: ptr(p), count(new long(1)) {
//	}
//
//	// Копирующий указатель (увеличивает счётчик владельцев)
//	CountedPtr(const CountedPtr<T>& p) throw()
//		: ptr(p.ptr), count(p.count) {
//		++*count;
//	}
//
//	// Деструктор (уничтожает объект, если владелец был последним)
//	~CountedPtr() throw() {
//		dispose();
//	}
//
//	// Присваивание (перевод указателя на новый объект)
//	CountedPtr<T>& operator= (const CountedPtr<T>& p) throw() {
//		if (this != &p) {
//			dispose();
//			ptr = p.ptr;
//			count = p.count;
//			++*count;
//		}
//		return *this;
//	}
//
//	// Доступ к объекту, на который ссылается указатель
//	T& operator*() const throw() {
//		return *ptr;
//	}
//
//	T* operator->() const throw() {
//		return ptr;
//	}
//
//private:
//	void dispose() {
//		if (--*count == 0) {
//			delete count;
//			delete ptr;
//		}
//	}
//};

class Pizza {
protected:
	string name;
	string dough;
	string sauce;
	string from;
	vector<string> ls;

public:
	virtual ~Pizza() {}
	virtual void prepare() 
	{
		cout << "\n Preparing" + name;
		for (int i = 0; i < ls.size(); i++)
			cout << " " + ls[i];
		cout << endl;
	}
	virtual void bake() { cout << "\n Bake for 25 minutes at 350"; }
	virtual void cut() { cout << "\n Cuting the pizza into diagonal slices"; }
	virtual void box() { cout << "\n Place pizza in official PizzaStore box"; }
	void setFrom(string f) { from = f; }
	void menu()
	{
		cout << "Pizza: " << name << endl;
		cout << "Dough: " << dough << endl;
		cout << "Sauce: " << sauce << endl;
		if (ls.size() > 0)
		{
			cout << "params: ";
			for (int i = 0; i < ls.size(); i++)
				cout << ls[i] << " ";
			cout << endl;
		}
	}
};

//typedef CountedPtr<Pizza> pPizza;

class NYStyleCheesePizza : public Pizza {
public:
	NYStyleCheesePizza() 
	{
		name = "NY Style Sauce and cheese pizza";
		dough = "Thin Crust Dough";
		sauce = "Marinara Sauce";
		ls.push_back(string("NY Grated Reggiano Cheese"));
	}
	void prepare() override { cout << "prepare " << name << endl; }
	void bake() override { cout << "bake " << name << endl; }
	void cut() override { cout << "cut " << name << endl; }
	void box() override { cout << "box " << name << endl; }
};

class NYStylePepperoniPizza : public Pizza {
public:
	NYStylePepperoniPizza() 
	{
		name = "NYStylePepperoniPizza";
		dough = "NYStylePepperoniPizza Dough";
		sauce = "NYStylePepperoniPizza Sauce";
		ls.push_back(string("NY Pepperoni"));
	}
	void prepare() override { cout << "prepare " << name << endl; }
	void bake() override { cout << "bake " << name << endl; }
	void cut() override { cout << "cut " << name << endl; }
	void box() override { cout << "box " << name << endl; }
};

class NYStyleHawaiiPizza : public Pizza {
public:
	NYStyleHawaiiPizza() 
	{
		name = "NYStyleHawaiiPizza";
		dough = "NYStyleHawaiiPizza Dough";
		sauce = "NYStyleHawaiiPizza Sauce";
		ls.push_back(string("NY Ananas"));
	}
	void prepare() override { cout << "prepare " << name << endl; }
	void bake() override { cout << "bake " << name << endl; }
	void cut() override { cout << "cut " << name << endl; }
	void box() override { cout << "box " << name << endl; }
};

class MsStylePepperoniPizza : public Pizza {
public:
	MsStylePepperoniPizza() 
	{		
		name = "MsStylePepperoniPizza";
		dough = "MsStylePepperoniPizza Dough";
		sauce = "MsStylePepperoniPizza Sauce";
		ls.push_back(string("Ms Pepperoni"));
	}
	void prepare() override { cout << "prepare " << name << endl; }
	void bake() override { cout << "bake " << name << endl; }
	void cut() override { cout << "cut " << name << endl; }
	void box() override { cout << "box " << name << endl; }
};

class MsStyleCheesePizza : public Pizza {
public:
	MsStyleCheesePizza() 
	{
		name = "Ms Style Sauce and cheese pizza";
		dough = "Thin Crust Dough";
		sauce = "Marinara Sauce";
		ls.push_back(string("Ms Grated Reggiano Cheese"));
	}
	void prepare() override { cout << "prepare " << name << endl; }
	void bake() override { cout << "bake " << name << endl; }
	void cut() override { cout << "cut " << name << endl; }
	void box() override { cout << "box " << name << endl; }
};

class MsStyleHawaiiPizza : public Pizza {
public:
	MsStyleHawaiiPizza() 
	{
		name = "MsStyleHawaiiPizza";
		dough = "MsStyleHawaiiPizza Dough";
		sauce = "MsStyleHawaiiPizza Sauce";
		ls.push_back(string("Ms Ananas"));
	}
	void prepare() override { cout << "prepare " << name << endl; }
	void bake() override { cout << "bake " << name << endl; }
	void cut() override { cout << "cut " << name << endl; }
	void box() override { cout << "box " << name << endl; }
};

class AStylePepperoniPizza : public Pizza {
public:
	AStylePepperoniPizza() 
	{		
		name = "AStylePepperoniPizza";
		dough = "AStylePepperoniPizza Dough";
		sauce = "AStylePepperoniPizza Sauce";
		ls.push_back(string("A Pepperoni"));
	}
	void prepare() override { cout << "prepare " << name << endl; }
	void bake() override { cout << "bake " << name << endl; }
	void cut() override { cout << "cut " << name << endl; }
	void box() override { cout << "box " << name << endl; }
};

class AStyleCheesePizza : public Pizza {
public:
	AStyleCheesePizza() 
	{
		name = "A Style Sauce and cheese pizza";
		dough = "Thin Crust Dough";
		sauce = "Marinara Sauce";
		ls.push_back(string("A Grated Reggiano Cheese"));
	}
	void prepare() override { cout << "prepare " << name << endl; }
	void bake() override { cout << "bake " << name << endl; }
	void cut() override { cout << "cut " << name << endl; }
	void box() override { cout << "box " << name << endl; }
};

class AStyleHawaiiPizza : public Pizza {
public:
	AStyleHawaiiPizza() 
	{
		name = "AStyleHawaiiPizza";
		dough = "AStyleHawaiiPizza Dough";
		sauce = "AStyleHawaiiPizza Sauce";
		ls.push_back(string("A Ananas"));
	}
	void prepare() override { cout << "prepare " << name << endl; }
	void bake() override { cout << "bake " << name << endl; }
	void cut() override { cout << "cut " << name << endl; }
	void box() override { cout << "box " << name << endl; }
};

class PizzaStore { //пиццерия
protected:
	string from;
	string name;
	vector<string> types;
public:
	PizzaStore(string n, string f)
	{
		name = n; from = f;
	}
	virtual ~PizzaStore() {}
	Pizza* orderPizza(string type)
	{
		Pizza* pizza = createPizza(type);
		pizza->prepare();
		pizza->bake();
		pizza->cut();
		pizza->box();
		return pizza;
	}
	void menu()
	{
		cout << "Pizza store: " << name << ", from " << from << endl;
		int i = 1;
		for (string pizzaType : types)
		{
			Pizza* pizza = createPizza(pizzaType);
			cout << "Pizza " << i++ << endl;
			pizza->menu();
			delete pizza;
		}
	}

protected:
	virtual Pizza* createPizza(string type) = 0;
};

class NYPizzaStore : public PizzaStore {
public:
	NYPizzaStore(string n, string f) : PizzaStore(n, f)
	{
		types.push_back("cheese");
		types.push_back("pepperoni");
		types.push_back("hawaii");
	}
	Pizza* createPizza(string type) override
	{
		Pizza* pPizza;
		if (type == types[0])
			pPizza = new NYStyleCheesePizza();
		else if (type == types[1])
			pPizza = new NYStylePepperoniPizza();
		else if (type == types[2])
			pPizza = new NYStyleHawaiiPizza();
		else
			pPizza = new Pizza();
		pPizza->setFrom(from);
		return pPizza;
	}
};

class MsPizzaStore : public PizzaStore {
public:
	MsPizzaStore(string n, string f) : PizzaStore(n, f)
	{
		types.push_back("cheese");
		types.push_back("pepperoni");
		types.push_back("hawaii");
	}
	Pizza* createPizza(string type) override
	{
		Pizza* pPizza;
		if (type == types[0])
			pPizza = new MsStyleCheesePizza();
		else if (type == types[1])
			pPizza = new MsStylePepperoniPizza();
		else if (type == types[2])
			pPizza = new MsStyleHawaiiPizza();
		else
			pPizza = new Pizza();
		pPizza->setFrom(from);
		return pPizza;
	}
};

class APizzaStore : public PizzaStore {
public:
	APizzaStore(string n, string f) : PizzaStore(n, f)
	{
		types.push_back("cheese");
		types.push_back("pepperoni");
		types.push_back("hawaii");
	}
	Pizza* createPizza(string type) override
	{
		Pizza* pPizza;
		if (type == types[0])
			pPizza = new AStyleCheesePizza();
		else if (type == types[1])
			pPizza = new AStylePepperoniPizza();
		else if (type == types[2])
			pPizza = new AStyleHawaiiPizza();
		else
			pPizza = new Pizza();
		pPizza->setFrom(from);
		return pPizza;
	}
};

int main()
{
	Pizza *p0, *p1, *p2;
	NYPizzaStore *nps = new NYPizzaStore("NY Store", "New York");
	MsPizzaStore *mps = new MsPizzaStore("Ms Store", "Moskow");
	APizzaStore *aps = new APizzaStore("A Store", "Akapulko");	
	p0 = nps->orderPizza("cheese");
	p1 = mps->orderPizza("pepperoni");
	p2 = aps->orderPizza("hawaii");

	cout << "\n\n\nMenus: ";

	nps->menu();
	cout << "\n";
	mps->menu();
	cout << "\n";
	aps->menu();

	delete p1, p2, p0, nps, mps, aps;

	getchar();
	return 0;
}

