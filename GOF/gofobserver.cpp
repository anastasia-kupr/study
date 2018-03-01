// gofobserver.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include <list>
#include <algorithm>
#include <vector>

using namespace std;

/* Класс, обеспечивающий семантику подсчёта ссылок.
* Объект, на который ссылается указатель, автоматически
* уничтожается при удалении последнего экземпляра CountedPtr
* для данного объекта.
*/

template <class T>

class CountedPtr 
{
private:
	T* ptr; // Указатель на значение
	long* count; // Количество владельцев (общие данные)

public:
	// Инициализация объекта существующим указателем
	// - указатель p должен быть получен в результате вызова new
	explicit CountedPtr(T* p = 0) : ptr(p), count(new long(1)) {}

	// Копирующий указатель (увеличивает счётчик владельцев)
	CountedPtr(const CountedPtr<T>& p) throw(): ptr(p.ptr), count(p.count) 
	{
		++*count;
	}

	// Деструктор (уничтожает объект, если владелец был последним)
	~CountedPtr() throw() 
	{
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

enum DataType
{
	dt_temperature,
	dt_humidity,
	dt_pressure,
	dt_sun_activity
};

class WeatherData;

class Observer;

class Subject {

public:
	virtual void registerObserver(Observer* ob) = 0;
	virtual void removeObserver(Observer* ob) = 0;
	virtual void notifyObserver(DataType dt) = 0;
};

class Observer {
public:
	virtual void update(DataType dt, float v) = 0;
};

class DisplayElement {
public:
	virtual void display() = 0;
};

class StatisticDisplay : public Observer, DisplayElement {
private:
	float value;
	DataType dataType;
	std::vector<Subject*> weatherData;
public:
	StatisticDisplay(Subject* wD, DataType dt) : dataType(dt)
	{
		weatherData.push_back(wD);
		for (auto s : weatherData)
			s->registerObserver(this);
	}

	void addSubject(Subject* wD)
	{
		weatherData.push_back(wD);
		weatherData.back()->registerObserver(this);
	}

	void update(DataType dt, float v) 
	{ 
		if (dt == dataType)
		{
			value = v;  
			display();
		}
	}
	void display() 
	{ 
		cout << "\n StatisticDisplay: ";
		if (dataType == dt_temperature)
			cout << "Temperature: ";
		else if (dataType == dt_humidity)
			cout << "Humidity: ";
		else if (dataType == dt_pressure)
			cout << "Pressure: ";
		else if (dataType == dt_sun_activity)
			cout << "Sun Activity: ";
		cout << value << "\n";
	}
	void removeObserver() 
	{ 
		for (auto s : weatherData)
			s->removeObserver(this); 
	}
	~StatisticDisplay() 
	{ 
		for (auto s : weatherData)
			s->removeObserver(this);
	}
};

typedef CountedPtr<Observer> ObserverPtr;
class WeatherData : public Subject {
private:
	list<ObserverPtr> list_Obs;
	float temperature;
	float humidity;
	float pressure;
	float sunActivity;
public:
	void registerObserver(Observer* ob) { list_Obs.push_front(ObserverPtr(ob)); }
	void removeObserver(Observer* ob) {
		list<ObserverPtr>::iterator pos;
		for (pos = list_Obs.begin(); pos != list_Obs.end(); ++pos)
			if (&(*(*pos)) == ob) {
				list_Obs.erase(pos);
				break;
			}
	}

	void notifyObserver(DataType dt) {
		list<ObserverPtr>::const_iterator pos;
		for (pos = list_Obs.begin(); pos != list_Obs.end(); ++pos)
		{
			if (dt == dt_temperature)
				(*pos)->update(dt, temperature);
			else if (dt == dt_pressure)
				(*pos)->update(dt, pressure);
			else if (dt == dt_humidity)
				(*pos)->update(dt, humidity);
			else if (dt == dt_sun_activity)
				(*pos)->update(dt, sunActivity);
		}
	}

	void measurementsChange(DataType dt) { notifyObserver(dt); }
	void setMeasurements(float t = NULL, float h = NULL, float p = NULL, float a = NULL)
	{
		if (t)
		{
			temperature = t;
			measurementsChange(dt_temperature);
		}
		if (h)
		{
			humidity = h;
			measurementsChange(dt_humidity);
		}
		if (p)
		{
			pressure = p;
			measurementsChange(dt_pressure);
		}
		if (a)
		{
			sunActivity = a;
			measurementsChange(dt_sun_activity);
		}
	}
	~WeatherData() { list_Obs.clear(); }

};

int main()
{
	WeatherData * pWD = new WeatherData;
	WeatherData * pWD1 = new WeatherData;
	StatisticDisplay *SDT = new StatisticDisplay(pWD, dt_temperature);
	StatisticDisplay *SDH = new StatisticDisplay(pWD, dt_humidity);
	StatisticDisplay *SDP = new StatisticDisplay(pWD, dt_pressure);
	StatisticDisplay *SDSA = new StatisticDisplay(pWD, dt_sun_activity);
	SDT->addSubject(pWD1);
	pWD->setMeasurements(10);
	pWD1->setMeasurements(-20);
	pWD->setMeasurements(40);
	pWD1->setMeasurements(-30);

	getchar();
	delete pWD, pWD1;
	return 0;
}