// gof8.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Dough { protected: string name; public: string GetName()const { return name; } }; //Тесто

class ThinDough : public Dough { public: ThinDough() { name = "thin dough"; } };

class FatDough : public Dough { public: FatDough() { name = "fat dough"; } };

class Sauce { protected: string name; public: string GetName()const { return name; } };//Соус

class TomatoSauce : public Sauce { public: TomatoSauce() { name = "tomato sauce"; } };

class KetchupSauce : public Sauce { public: KetchupSauce() { name = "ketchup sauce"; } };

class Cheese { protected: string name; public: string GetName()const { return name; } };//Сыр

class HardCheese : public Cheese { public: HardCheese() { name = "hard cheese"; } };

class SoftCheese : public Cheese { public: SoftCheese() { name = "soft cheese"; } };

class Clams { protected: string name; public: string GetName()const { return name; } };//Мидии

class FreshClams : public Clams { public: FreshClams() { name = "fresh clams"; } };

class FrozenClams : public Clams { public: FrozenClams() { name = "frozen clams"; } };

//---------------------------------------------------------------------------------------------

//фабрика ингредиентов
class PizzaIngredientFactory {
public:
	virtual Dough* createDough() = 0;
	virtual Sauce* createSauce() = 0;
	virtual Cheese* createCheese() = 0;
	virtual Clams* createClams() = 0;
	virtual ~PizzaIngredientFactory() {}
};

//фабрика ингредиентов в Нью-Йорке

class NYPizzaIngredientFactory : public PizzaIngredientFactory {
	Dough* createDough() override { return new ThinDough(); }
	Sauce* createSauce() override { return new TomatoSauce(); }
	Cheese* createCheese() override { return new HardCheese(); }
	Clams* createClams() override { return new FreshClams(); }
};

//фабрика ингредиентов в Чикаго

class CHPizzaIngredientFactory : public PizzaIngredientFactory {
	Dough* createDough() override { return new FatDough(); }
	Sauce* createSauce() override { return new KetchupSauce(); }
	Cheese* createCheese() override { return new SoftCheese(); }
	Clams* createClams() override { return new FrozenClams(); }
};


class PizzaFlavor
{
protected:
	string name;
	int size;
public:
	PizzaFlavor(int sz = 0)
	{
		size = sz >= 1 && sz <= 3 ? sz : 1;
		name = "";
	}
	virtual ~PizzaFlavor() {}
	void SetSize(int sz) { size = sz >= 1 && sz <= 3 ? sz : size; }
	int GetSize()const { return size; }
	string GetName()const { return name; }
};

class FlavorDecorator : public PizzaFlavor
{
protected:
	PizzaFlavor* flavor;
	// set pizza size for flavor reference
	FlavorDecorator(PizzaFlavor* pc) : flavor(pc)
	{
		SetSize(pc->GetSize());
		name = "";
	}
public:
	string GetFlavorName()const { return name; }
};

class CheeseFlavor : public FlavorDecorator
{
public:
	CheeseFlavor(PizzaFlavor* pc) : FlavorDecorator(pc)
	{
		name = "cheese flavour";
	}
};

class SauceFlavor : public FlavorDecorator
{
public:
	SauceFlavor(PizzaFlavor* pc) : FlavorDecorator(pc)
	{
		name = "sause flavour";
	}
};

//-------------------------------------------------------------------------------------

class Pizza {//пицца
protected:
	PizzaIngredientFactory* pingredientFactory; //Интерфейс "Абстрактная фабрика"
	string name;
	int size;
	//Интерфейсы "Абстрактные продукты"
	Dough* pdough; //Тесто
	Sauce* psauce; //Соус
	Cheese* pcheese; //Сыр
	Clams* pclams; //Мидии
	vector<PizzaFlavor*> flavors;
public:
	Pizza(int sz = 1)
	{
		size = sz >= 1 && sz <= 3 ? sz : 1;
		name = "PizzaD";
		flavors.push_back(new PizzaFlavor());
		pingredientFactory = NULL;
		pdough = NULL;
		psauce = NULL;
		pcheese = NULL;
		pclams = NULL;
	}
	virtual ~Pizza()
	{
		delete pdough;
		delete psauce;
		delete pcheese;
		delete pclams;
		delete pingredientFactory;
		for (auto it = flavors.begin(); it != flavors.end(); it++)
			delete *it;
		flavors.clear();
	}
	virtual void prepare() { cout << string("Preparing ") + name << endl; }
	virtual void bake() { cout << "Bake for 25 minutes at 350" << endl; }
	virtual void cut() { cout << "Cutting the pizza into diagonal slices" << endl; }
	virtual void box() { cout << "Place pizza in official PizzaStore box" << endl; }
	void AddFlavor(PizzaFlavor* pc)
	{
		if (this->GetName() != "PizzaD") // forbid flavors for default Pizza class
			flavors.push_back(pc);
	}
	void SetSize(int sz) { size = sz >= 1 && sz <= 3 ? sz : size; }
	void SetName(string nm) { name = nm; }
	PizzaFlavor* GetFlavorBase() { return flavors[0]; }
	vector<PizzaFlavor*> GetFlavors()const { return flavors; }
	int GetSize()const { return size; }
	string GetName()const { return name; }
	Dough* GetDough() { return pdough; }
	Sauce* GetSauce() { return psauce; }
	Cheese* GetCheese() { return pcheese; }
	Clams* GetClams() { return pclams; }
};


class CheesePizza : public Pizza {//пицца с сыром
public:
	CheesePizza(PizzaIngredientFactory* pF, int size = 1) : Pizza(size)
	{
		pingredientFactory = pF;
		name = "PizzaCheese";
	}
	void prepare() override
	{
		cout << "Preparing " + name << endl;
		pdough = pingredientFactory->createDough();
		psauce = pingredientFactory->createSauce();
		pcheese = pingredientFactory->createCheese();
	}
};

class ClamsPizza : public Pizza { //пицца с мидиями
public:
	ClamsPizza(PizzaIngredientFactory* pF, int size = 1) : Pizza(size)
	{
		pingredientFactory = pF;
		name = "PizzaClams";
	}
	void prepare() override
	{
		cout << "Preparing " + name << endl;
		pdough = pingredientFactory->createDough();
		psauce = pingredientFactory->createSauce();
		pclams = pingredientFactory->createClams();
	}
};

//-------------------------------------------------------------------------------------

class PizzaStore { //пиццерия
private:
	vector<Pizza*> _pizzas;
public:
	Pizza* orderPizza(string type, int size = 1)
	{
		Pizza* pizza = createPizza(type, size);
		pizza->prepare();
		pizza->bake();
		pizza->cut();
		pizza->box();
		_pizzas.push_back(pizza);
		return pizza;
	}
	~PizzaStore()
	{
		for (auto it = _pizzas.begin(); it != _pizzas.end(); it++)
			delete *it;
		_pizzas.clear();
	}
protected:
	virtual Pizza* createPizza(string type, int size = 1) = 0;
};

class NYPizzaStore : public PizzaStore {//Пиццерия в Нью-Йорке
public:
	Pizza* createPizza(string type, int size = 1) override
	{
		if (type == "cheese")
			return new CheesePizza(new NYPizzaIngredientFactory(), size);
		if (type == "clams")
			return new ClamsPizza(new NYPizzaIngredientFactory(), size);
		return new Pizza(size);
	}
};

class CHPizzaStore : public PizzaStore {
public:
	Pizza* createPizza(string type, int size = 1) override
	{
		if (type == "cheese")
			return new CheesePizza(new CHPizzaIngredientFactory(), size);
		if (type == "clams")
			return new ClamsPizza(new CHPizzaIngredientFactory(), size);
		return new Pizza(size);
	}
};

//Прайс на продукты
//--------------------------------------------------------------------------------------

class Price {
	static bool fCreate;
	static Price* price;
	double doughCost;
	double sauceCost;
	double cheeseCost;
	double clamsCost;
	double pizzaCost;
	double flavorTotal;
	double cheeseFlavorCost;
	double sauceFlavorCost;

	Price()
	{
		doughCost = 0;
		sauceCost = 0; 
		cheeseCost = 0;
		clamsCost = 0;
		pizzaCost = 0;
		flavorTotal = 0;
		cheeseFlavorCost = 0;
		sauceFlavorCost = 0;
	}
	void ResetPrices()
	{
		doughCost = 0;
		sauceCost = 0;
		cheeseCost = 0;
		clamsCost = 0;
		pizzaCost = 0;
		flavorTotal = 0;
		cheeseFlavorCost = 0;
		sauceFlavorCost = 0;
	}
public:
	static Price* getInstance()
	{
		if (!fCreate)
		{
			fCreate = true;
			price = new Price();
		}
		return price;
	}
	static void destroyInstance()
	{
		if (fCreate)
		{
			fCreate = false;
			delete price;
		}
	}
	void PrintPizzaPrices(Pizza* pizza)
	{
		cout << endl;
		ResetPrices();
		Dough* dough = pizza->GetDough();
		Sauce* sauce = pizza->GetSauce();
		Cheese* cheese = pizza->GetCheese();
		Clams* clams = pizza->GetClams();
		auto flavors = pizza->GetFlavors();
		int size = pizza->GetSize();
		string sizeStr = size == 3 ? "XL" : size == 2 ? "N" : "XS";
		cout << sizeStr << " pizza " << pizza->GetName() << ": $" << GetPizzaCost(pizza) << endl;
		if (dough != NULL || sauce != NULL || cheese != NULL || clams != NULL)
		{
			if (dough != NULL)
				cout << dough->GetName() << " cost of dough: $" << GetDoughCost(dough) << "\t";
			if (sauce != NULL)
				cout << sauce->GetName() << " cost of sauce: $" << GetSauceCost(sauce) << "\t";
			if (cheese != NULL)
				cout << cheese->GetName() << " cost of cheese: $" << GetCheeseCost(cheese) << "\t";
			if (clams != NULL)
				cout << clams->GetName() << " cost of clams: $" << GetClamsCost(clams) << "\t";
			cout << endl;
		}

		// Print flavor prices
		if (flavors.size() > 1)
		{
			GetCheeseFlavorCost(pizza);
			GetSauceFlavorCost(pizza);
			cout << endl;
		}

		cout << "Total cost: $" << GetPizzaTotal(pizza) << endl;
	}
	double GetPizzaTotal(Pizza* pizza)
	{
		int size = pizza->GetSize();
		return GetPizzaCost(pizza) + doughCost + sauceCost + cheeseCost 
			+ clamsCost + cheeseFlavorCost * size + sauceFlavorCost * size;
	}
	double GetPizzaCost(Pizza* pizza)
	{
		if (pizza->GetName() == "PizzaCheese")
			pizzaCost = 1.25;
		else if (pizza->GetName() == "PizzaClams")
			pizzaCost = 1.75;
		return pizzaCost;
	}
	double GetDoughCost(Dough* dough)
	{
		if (dough == NULL)
			return doughCost;
		if (dough->GetName() == "thin")
			doughCost = 0.5;
		if (dough->GetName() == "fat")
			doughCost = 0.65;
		return doughCost;
	}
	double GetSauceCost(Sauce* sauce)
	{
		if (sauce == NULL)
			return sauceCost;
		else if (sauce->GetName() == "tomato")
			sauceCost = 0.4;
		else if (sauce->GetName() == "ketchup")
			sauceCost = 0.6;
		return sauceCost;
	}
	double GetCheeseCost(Cheese* cheese)
	{
		if (cheese == NULL)
			return cheeseCost;
		else if (cheese->GetName() == "hard cheese")
			cheeseCost = 0.6;
		if (cheese->GetName() == "soft cheese")
			cheeseCost = 1.2;
		return cheeseCost;
	}
	double GetClamsCost(Clams* clams)
	{
		if (clams == NULL)
			return clamsCost;
		else if (clams->GetName() == "fresh clams")
			clamsCost = 1.5;
		else if (clams->GetName() == "frozen clams")
			clamsCost = 2.25;
		return clamsCost;
	}
	double GetCheeseFlavorCost(Pizza* pizza)
	{
		auto flavors = pizza->GetFlavors();
		for (auto it = flavors.begin(); it != flavors.end(); it++)
			if ((*it)->GetName() == "cheese flavor")
			{
				cheeseFlavorCost = 0.51;
				cout << (*it)->GetName() << ": $" << cheeseFlavorCost << " * " << pizza->GetSize() << " = " << cheeseFlavorCost * pizza->GetSize() << "\t";
			}
		return cheeseFlavorCost;
	}
	double GetSauceFlavorCost(Pizza* pizza)
	{
		auto flavors = pizza->GetFlavors();
		for (auto it = flavors.begin(); it != flavors.end(); it++)
			if ((*it)->GetName() == "sauce flavor")
			{
				sauceFlavorCost = 0.28;
				cout << (*it)->GetName() << ": $" << sauceFlavorCost << " * " << pizza->GetSize() << " = " << sauceFlavorCost * pizza->GetSize() << "\t";
			}
		return sauceFlavorCost;
	}
};

bool Price::fCreate = false;
Price* Price::price;

//--------------------------------------------------------------------------------------
int main()
{
	NYPizzaStore nyp;
	CHPizzaStore chp;
	Price* pr = Price::getInstance();

	Pizza* nyCheesePizza = nyp.orderPizza("cheese", 2);
	cout << "\n\n";
	nyp.orderPizza("clams");
	cout << "\n\n";
	nyp.orderPizza("");
	cout << "\n\n";

	Pizza* chClamsPizza = chp.orderPizza("clams");
	cout << "\n\n";
	Pizza* chBasePizza = chp.orderPizza("");
	cout << "\n\n";

	nyCheesePizza->AddFlavor(new CheeseFlavor(nyCheesePizza->GetFlavorBase()));
	nyCheesePizza->AddFlavor(new SauceFlavor(nyCheesePizza->GetFlavorBase()));

	chClamsPizza->AddFlavor(new SauceFlavor(chClamsPizza->GetFlavorBase()));

	pr->PrintPizzaPrices(nyCheesePizza);
	pr->PrintPizzaPrices(chBasePizza);
	pr->PrintPizzaPrices(chClamsPizza);

	Price::destroyInstance();

	system("pause");
	return 0;
}
