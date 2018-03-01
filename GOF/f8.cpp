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


class Dough { protected: string name; double cost; }; //Тесто
class ThinDough : public Dough { public:  ThinDough() { name = "thin dough"; } };
class FatDough:  public Dough { public:  FatDough()   {	name = "fat dough"; }};

class Sauce { protected: string name; double cost; };//Соус
class TomatoSauce:  public Sauce { public:  TomatoSauce() {  name = "tomato sauce"; }};
class KetchupSauce: public Sauce { public:  KetchupSauce() { name = "ketchup sauce";}};

class Cheese { protected: string name; double cost; };//Сыр
class HardCheese:	public Cheese { public:  HardCheese() { name = "hard cheese";}};
class SoftCheese:   public Cheese { public:  SoftCheese() { name = "soft cheese";}};

class Clams { protected: string name; double cost; };//Мидии
class FreshClams:	public Clams { public:  FreshClams() {  name = "fresh clams";}};
class FrozenClams:	public Clams { public:  FrozenClams() { name = "frozen clams";}};

//---------------------------------------------------------------------------------------------
typedef CountedPtr<Dough> pDough;
typedef CountedPtr<Sauce> pSauce;
typedef CountedPtr<Cheese> pCheese;
typedef CountedPtr<Clams> pClams;
//фабрика ингредиентов
class PizzaIngredientFactory {
public:
	virtual pDough createDough() = 0;
	virtual pSauce createSauce() = 0;
	virtual pCheese createCheese() = 0;
	virtual pClams  createClams() = 0;
};
//фабрика ингредиентов в Нью-Йорке
class NYPizzaIngredientFactory : public PizzaIngredientFactory {
	pDough createDough() { return pDough(new ThinDough()); }
	pSauce createSauce() { return pSauce(new TomatoSauce()); }
	pCheese createCheese() { return pCheese(new HardCheese()); }
	pClams  createClams() { return pClams(new FreshClams()); }
};
//фабрика ингредиентов в Чикаго
class CHPizzaIngredientFactory : public PizzaIngredientFactory {
	pDough createDough() { return pDough(new FatDough()); }
	pSauce createSauce() { return pSauce(new KetchupSauce()); }
	pCheese createCheese() { return pCheese(new SoftCheese()); }
	pClams  createClams() { return pClams(new FrozenClams()); }
};

typedef CountedPtr<PizzaIngredientFactory> pPizzaIngredientFactory;

//-------------------------------------------------------------------------------------


class Pizza {//пицца
protected:
	pPizzaIngredientFactory pingredientFactory; //Интерфейс "Абстрактная фабрика"
	string name;
	//Интерфейсы "Абстрактные продукты"
	pDough pdough; //Тесто
	pSauce psauce; //Соус
	pCheese pcheese; //Сыр
	pClams pclams; //Мидии
	
public:
	virtual void prepare(){ //Из каких продуктов готовиться пицца
		cout << string("\n\n Preparing ") + string("default pizza");
	}
	virtual void bake() { cout << "\n Bake for 25 minutes at 350";  }
	virtual void cut() { cout << "\n Cutting the pizza into diagonal slices";  }
	virtual void box() { cout << "\n Place pizza in official PizzaStore box"; }
	void SetName(string nm) { name = nm; }
	string GetName() { return name; }
};
typedef CountedPtr<Pizza> pPizza;

class CheesePizza : public Pizza {//пицца с сыром 
public:
	CheesePizza(pPizzaIngredientFactory pF) { pingredientFactory = pF; name = "CheesePizza"; }
	void prepare() {
		cout << "\n\n Preparing " + name;
		pdough = pingredientFactory->createDough();
		psauce = pingredientFactory->createSauce();
		pcheese = pingredientFactory->createCheese();
	}
};

class ClamsPizza : public Pizza { //пицца с мидиями
public:
	ClamsPizza(pPizzaIngredientFactory pF) { pingredientFactory = pF; name = "ClamsPizza";	}
	void prepare() {
		cout << "\n\n Preparing " + name;
		pdough = pingredientFactory->createDough();
		psauce = pingredientFactory->createSauce();
		pclams = pingredientFactory->createClams();
	}
};
//
//-------------------------------------------------------------------------------------
class PizzaStore { //пиццерия
public:
	pPizza orderPizza(string type) { //заказ пиццы
		pPizza pizza = createPizza(type); //создание пиццы 
		pizza->prepare();
		pizza->bake();
		pizza->cut();
		pizza->box();
		return pizza;

	}
protected:
	virtual  pPizza createPizza(string type) = 0;
};

class NYPizzaStore : public PizzaStore {//Пиццерия в Нью-Йорке
public:
	pPizza createPizza(string type) {//Используем фабрику ингредиентов из Нью-Йорка для приготовления разных видов пиццы 
		if (type == "cheese")return  pPizza(new  CheesePizza( pPizzaIngredientFactory (new NYPizzaIngredientFactory())));
		if (type == "clams") return  pPizza(new  ClamsPizza(  pPizzaIngredientFactory (new NYPizzaIngredientFactory())));
		return pPizza(new Pizza);
	}
};
//Прайс на продукты
//--------------------------------------------------------------------------------------
class Prise;
typedef CountedPtr<Prise> pPrise;
class Prise {
	static bool fCreate;
	static pPrise prise;
	double costDough;
	Prise() { costDough = 0.88; }
public:
	static pPrise& getInstance() {
		if (!fCreate) { fCreate = 1; return prise = pPrise(new Prise()); }
		else return prise;
	}
	double GetCostDout() { return costDout; }
};
bool Prise::fCreate = 0;
pPrise Prise::prise;

//--------------------------------------------------------------------------------------
int main()
{
	
	NYPizzaStore nyp;
	pPrise pr = Prise::getInstance();
pr->GetCostDout();
	pPizza nyCheesePizza = nyp.orderPizza("cheese");
	nyp.orderPizza("clams");
	nyp.orderPizza("");
	getchar();
	return 0;
}
