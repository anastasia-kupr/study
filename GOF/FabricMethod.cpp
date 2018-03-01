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
	string place;
	vector<string> ls;

public:
	virtual void prepare() {
		cout << "\n Preparing" + name;
		for (int i = 0; i < ls.size(); i++)	cout << " " + ls[i];
		cout << endl;
	}
	virtual void bake() { cout << "\n Bake for 25 minutes at 350"; }
	virtual void cut() { cout << "\n Cuting the pizza into diagonal slices"; }
	virtual void box() { cout << "\n Place pizza in official PizzaStore box"; }
	void setPlase(string pl) { place = pl; }
	void showMenu()	{
		//cout << "\n"<< place << " menu:\n";
		cout << "pizza: " << name << endl;
		cout << "dough: " << dough << endl;
		cout << "sauce: " << sauce << endl;
		if (ls.size() > 0) {
			for (int i = 0; i < ls.size(); i++)
				cout << ls[i] << " ";
			cout << endl;
		}
	}
	virtual ~Pizza() {}
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

class NYStyleMeetPizza : public Pizza {
public:
	NYStyleMeetPizza()
	{
		name = "NYStyleMeetPizza";
		dough = "NYStyleMeetPizza Dough";
		sauce = "NYStyleMeetPizza Sauce";
		ls.push_back(string("NY Petrushka"));
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

class MsStyleMeetPizza : public Pizza {
public:
	MsStyleMeetPizza()
	{
		name = "MsStyleMeetPizza";
		dough = "MsStyleMeetPizza Dough";
		sauce = "MsStyleMeetPizza Sauce";
		ls.push_back(string("Ms Petrushka"));
	}
	void prepare() override { cout << "prepare " << name << endl; }
	void bake() override { cout << "bake " << name << endl; }
	void cut() override { cout << "cut " << name << endl; }
	void box() override { cout << "box " << name << endl; }
};

class TgStylePepperoniPizza : public Pizza {
public:
	TgStylePepperoniPizza()	{
		name = "TgStylePepperoniPizza";
		dough = "TgStylePepperoniPizza Dough";
		sauce = "TgStylePepperoniPizza Sauce";
		ls.push_back(string("Taganrog Pepperoni"));
	}
	void prepare() override { cout << "prepare " << name << endl; }
	void bake() override { cout << "bake " << name << endl; }
	void cut() override { cout << "cut " << name << endl; }
	void box() override { cout << "box " << name << endl; }
};

class TgStyleCheesePizza : public Pizza {
public:
	TgStyleCheesePizza() {
		name = "Taganrog Style Sauce and cheese pizza";
		dough = "Thin Crust Dough";
		sauce = "Marinara Sauce";
		ls.push_back(string("Taganrog Grated Reggiano Cheese"));
	}
	void prepare() override { cout << "prepare " << name << endl; }
	void bake() override { cout << "bake " << name << endl; }
	void cut() override { cout << "cut " << name << endl; }
	void box() override { cout << "box " << name << endl; }
};

class TgStyleMeetPizza : public Pizza {
public:
	TgStyleMeetPizza()	{
		name = "TgStyleMeetPizza";
		dough = "TgStyleMeetPizza Dough";
		sauce = "TgStyleMeetPizza Sauce";
		ls.push_back(string("Tg Petrushka"));
	}
	void prepare() override { cout << "prepare " << name << endl; }
	void bake() override { cout << "bake " << name << endl; }
	void cut() override { cout << "cut " << name << endl; }
	void box() override { cout << "box " << name << endl; }
};

class PizzaStore { //пиццерия
protected:
	string place;
	string name;
	vector<string> types;
public:
	PizzaStore(string n, string pl)	{
		name = n; place = pl;
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
	void showMenu()	{
		cout << name <<"`s menu:\n";
		int i = 1;
		for (string pizzaType : types) {
			Pizza* pizza = createPizza(pizzaType);
			cout << "\n";
			pizza->showMenu();
			delete pizza;
		}
	}

protected:
	virtual Pizza* createPizza(string type) = 0;
};

class NYPizzaStore : public PizzaStore {
public:
	NYPizzaStore(string n, string pl) : PizzaStore(n, pl)
	{
		types.push_back("cheese");
		types.push_back("pepperoni");
		types.push_back("meet");
	}
	Pizza* createPizza(string type) override
	{
		Pizza* pPizza;
		if (type == types[0])
			pPizza = new NYStyleCheesePizza();
		else if (type == types[1])
			pPizza = new NYStylePepperoniPizza();
		else if (type == types[2])
			pPizza = new NYStyleMeetPizza();
		else
			pPizza = new Pizza();
		pPizza->setPlase(place);
		return pPizza;
	}
};

class MsPizzaStore : public PizzaStore {
public:
	MsPizzaStore(string n, string pl) : PizzaStore(n, pl)
	{
		types.push_back("cheese");
		types.push_back("pepperoni");
		types.push_back("meet");
	}
	Pizza* createPizza(string type) override
	{
		Pizza* pPizza;
		if (type == types[0])
			pPizza = new MsStyleCheesePizza();
		else if (type == types[1])
			pPizza = new MsStylePepperoniPizza();
		else if (type == types[2])
			pPizza = new MsStyleMeetPizza();
		else
			pPizza = new Pizza();
		pPizza->setPlase(place);
		return pPizza;
	}
};

class TgPizzaStore : public PizzaStore {
public:
	TgPizzaStore(string n, string pl) : PizzaStore(n, pl)
	{
		types.push_back("cheese");
		types.push_back("pepperoni");
		types.push_back("meet");
	}
	Pizza* createPizza(string type) override
	{
		Pizza* pPizza;
		if (type == types[0])
			pPizza = new TgStyleCheesePizza();
		else if (type == types[1])
			pPizza = new TgStylePepperoniPizza();
		else if (type == types[2])
			pPizza = new TgStyleMeetPizza();
		else
			pPizza = new Pizza();
		pPizza->setPlase(place);
		return pPizza;
	}
};

int main()
{
	Pizza *p0, *p1, *p2;
	NYPizzaStore *nps = new NYPizzaStore("NY Store", "New York");
	MsPizzaStore *mps = new MsPizzaStore("Ms Store", "Moskow");
	TgPizzaStore *aps = new TgPizzaStore("Taganrog Store", "Taganrog");
	p0 = nps->orderPizza("cheese");
	p1 = mps->orderPizza("pepperoni");
	p2 = aps->orderPizza("meet");

	nps->showMenu();
	cout << "\n";
	mps->showMenu();
	cout << "\n";
	aps->showMenu();

	delete p1, p2, p0, nps, mps, aps;

	getchar();
	return 0;
}

