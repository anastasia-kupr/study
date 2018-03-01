// Паттерн Стратегия
//#include "stdafx.h"
//#include "iostream"
//using namespace std;

class FlyBehavior {
public:
	virtual void fly() = 0;
};
class FlyWithWings : public FlyBehavior {
public:
	virtual void fly() { cout << "\n I fly "; }
};
class FlyNoWay : public FlyBehavior {
public:
	virtual void fly() { cout << "\n I no fly "; }
};


class QuackBehavior {
public:
	virtual void quack() = 0;
};
class Quack : public QuackBehavior {
public:
	virtual void quack() { cout << "\n I Quack "; };
};
class Squack : public QuackBehavior {
public:
	virtual void quack() { cout << "\n I pishu "; };
};
class MuteQuack : public QuackBehavior {
public:
	virtual void quack() { cout << "\n I no Quack "; };
};



class Duck {
private:
	FlyBehavior * pFly;
	QuackBehavior * pQuack;
public:
	Duck() :pFly(NULL), pQuack(NULL) {}
	Duck(FlyBehavior * pF, QuackBehavior * pQ) : pFly(pF), pQuack(pQ) {}
	virtual ~Duck() { delete pFly; delete pQuack; }

	void swim() { cout << "\n I swim "; }
	virtual void display() = 0;
	void PerformFly() { if(pFly!=NULL)pFly->fly(); }
	void PerformQuack() { if (pQuack != NULL) pQuack->quack(); }
	void SetFlyBehavior(FlyBehavior * p) {
		delete pFly;
		pFly = p;
	}
	void SetQuackBehavior(QuackBehavior * p) {
		delete pQuack;
		pQuack = p;
	}
};

class RubberDuck : public Duck {
public:
	RubberDuck() :Duck() {}
	RubberDuck(FlyBehavior * pF, QuackBehavior * pQ) : Duck(pF, pQ) {}
	void display() { cout << "\n I am RubberDuck"; }
};
class DecoyDuck : public Duck {
public:
	DecoyDuck() :Duck() {}
	DecoyDuck(FlyBehavior * pF, QuackBehavior * pQ) : Duck(pF, pQ) {}
	void display() { cout << "\nI am DecoyDuck"; }
};

void main() {
	RubberDuck Rd;

	Rd.display();
	Rd.PerformFly();
	getchar();
	Rd.SetFlyBehavior(new FlyWithWings());
	Rd.PerformFly();
	getchar();
	Rd.SetFlyBehavior(new FlyNoWay());
	Rd.PerformFly();
	getchar();
	DecoyDuck Dd(new FlyWithWings(), new Squack());
	Dd.display();
	Dd.PerformFly();
	Dd.PerformQuack();
}
