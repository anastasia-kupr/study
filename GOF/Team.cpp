// Command.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <deque>

using namespace std;

template <class T>
class CountedPtr {
private:
	T* ptr; // Указатель на значение
	long* count; // Количество владельцев (общие данные)
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


// Command
//-----------------------------------------------
class Command {
public:
	virtual void execute() = 0;
	virtual void undo() = 0;
};

class Light {
	string name;
public:
	Light() { ; }
	Light(string n) { name = n; }
	void off() { cout << "\nOff light"; }
	void on() { cout << "\nOn light"; }
};

string LIGHT = "Light";
string MUSIC = "Music";
string VENTILATION = "Ventilation";
string CONDITIONER = "Conditioning";
string TV = "TV";
string MUSICON = "Music on";
string MUSICOFF = "Music off";
string UNDO = "Undo";

// Light
//-----------------------------------------------

class LightOnCommand : public Command {
	Light& light;
public:
	LightOnCommand(Light& light) : light(light) {}
	virtual void execute() { light.on(); }
	virtual void undo() { light.off(); }
};

class LightOffCommand : public Command {
	Light& light;
public:
	LightOffCommand(Light& light) : light(light) {}
	virtual void execute() { light.off(); }
	virtual void undo() { light.on(); }
};

// Music
//------------------------------------------------------
class Music {
	int volume;
	int prevVolume;
	string name;
public:
	Music(string name) :volume(0), prevVolume(0) { this->name = name; }
	void on() { cout << "\n On music " << name; }
	void off() { cout << "\n Off music " << name; }
	void SetVolume(int vol) {
		if (volume + vol >= 0 && volume + vol <= 25) { prevVolume = volume; volume = volume + vol; }
		else if (vol < 0) { prevVolume = volume; volume = 0; }
		else if (vol > 0) { prevVolume = volume; volume = 25; }
		cout << "\n volume = " << volume;
	}
	int GetPrevVolume() { return prevVolume; }
};
class MusicOnCommand : public Command {
	Music& music;
public:
	MusicOnCommand(Music& str) : music(str) {}
	virtual void execute() { music.on(); }
	virtual void undo() { music.off(); }
};

class MusicOffCommand : public Command {
	Music& music;
public:
	MusicOffCommand(Music& str) : music(str) {}
	virtual void execute() { music.off(); }
	virtual void undo() { music.on(); }
};
class MusicUpCommand : public Command {
	Music& music;
public:
	MusicUpCommand(Music& str) : music(str) {}
	virtual void execute() { music.SetVolume(1); };
	virtual void undo() { music.SetVolume(music.GetPrevVolume()); };
};

class MusicDownCommand : public Command {
	Music& music;
public:
	MusicDownCommand(Music& str) : music(str) {}
	virtual void execute() { music.SetVolume(-1); };
	virtual void undo() { music.SetVolume(music.GetPrevVolume()); };
};


typedef CountedPtr<Command> pCommand;

class NoCommand : public Command {
public:
	virtual void execute() { cout << "\n NoCommand "; }
	virtual void undo() { cout << "\n NoCommand "; }
};

class SingleCommand : public Command {
	string name;
	int state;
public:
	SingleCommand(string n, int s) : name(n), state(s) {}
	virtual void execute() {
		if (name == LIGHT) {
			Light lr("living room");
			if (state == 0) lr.on();
			if (state == 1) lr.off();
			return;
		}
		if (name == MUSIC) {
			Music  mlr("Music in living room");
			if (state == 0) mlr.on();
			if (state == 1) mlr.off();
			return;
		}
		if (name == MUSICON) {
			Music m("living room");
			MusicOnCommand mon(m);
			//mon = new MusicOnCommand(m);
			mon.execute();
			return;
		}
		if (name == MUSICOFF) {
			Music m("living room");
			MusicOffCommand moff(m);
			//mon = new MusicOnCommand(m);
			moff.execute();
			return;
		}
		string stateName;
		if (state == 0) stateName = " is off";
		else if (state == 1) stateName == " is on";
		cout << "\n SingleCommand: " << name; if (state != -1) cout << stateName;
	}
	virtual void undo() {
		string stateName;
		if (state == 0) stateName = " is off";
		else if (state == 1) stateName == " is on";
		cout << "\n Undo SingleCommand: " << name;
	}
};

class EmptyCommand : public Command {
public:
	virtual void execute() { cout << "\n EmptyCommand"; }
	virtual void undo() { cout << "\n Undo EmptyCommand"; }
};

class MacroCommand : public Command {
public:
	vector<pCommand> commandVec;
	virtual void execute() {
		cout << "\n MacroCommand:\n";
		for (int i = 0; i < commandVec.size(); i++)
			commandVec[i]->execute();
	}
	virtual void undo() {
		cout << "\n Undo MacroCommand:\n";
		for (int i = commandVec.size() - 1; i > -1; i--)
			commandVec[i]->undo();
	}
};

// RemoteControl
//------------------------------------------------------
class RemoteControl {
	vector<pCommand> onCommand;
	vector<pCommand> offCommand;
	deque<pCommand> journal;
	int lag;
public:
	RemoteControl() : onCommand(7, pCommand(new NoCommand())), offCommand(7, pCommand(new NoCommand())), lag(0) {}
	void setLag(int l) { lag = l; }
	void setCommand(int slot, pCommand onCom, pCommand offCom) { onCommand[slot] = onCom; offCommand[slot] = offCom; }
	void onButtonWasPushed(int slot) {
		if (lag < 0) cout << "\nIncorrect lag!";
		if (journal.size() > lag)
		{
			journal.front()->execute();
			journal.pop_front();
		}
		journal.push_back(onCommand[slot]);
	}
	void offButtonWasPushed(int slot) {
		if (lag < 0) cout << "\nIncorrect lag!";
		if (journal.size() > lag)
		{
			journal.front()->execute();
			journal.pop_front();
		}
		journal.push_back(offCommand[slot]);
	}
	void undoButtonWasPushed() {
		if (journal.size() > 0)
		{
			journal.back()->undo();
			journal.pop_back();
		}
	}
};

int main()
{
	Light lr("Living room");
	RemoteControl RC;
	RC.setLag(1);
	RC.setCommand(0, pCommand(new SingleCommand(LIGHT, 0)), pCommand(new SingleCommand(LIGHT, 1)));
	RC.setCommand(1, pCommand(new SingleCommand(MUSIC, 0)), pCommand(new SingleCommand(MUSIC, 1)));
	RC.setCommand(2, pCommand(new SingleCommand(MUSICON, -1)), pCommand(new SingleCommand(MUSICOFF, -1)));
	RC.setCommand(3, pCommand(new SingleCommand(MUSICON, -1)), pCommand(new SingleCommand(MUSICOFF, -1)));
	RC.setCommand(6, pCommand(new EmptyCommand()), pCommand(new EmptyCommand()));

	cout << "\n1: ";
	RC.onButtonWasPushed(0);
	cout << "\n2: ";
	RC.undoButtonWasPushed();
	RC.offButtonWasPushed(0);
	RC.onButtonWasPushed(1);
	RC.offButtonWasPushed(1);
	RC.onButtonWasPushed(2);
	RC.offButtonWasPushed(2);
	RC.onButtonWasPushed(3);
	RC.offButtonWasPushed(3);
	RC.onButtonWasPushed(3);
	RC.undoButtonWasPushed();
	RC.onButtonWasPushed(3);
	RC.onButtonWasPushed(6);
	RC.undoButtonWasPushed();


	getchar();
	return 0;
}

