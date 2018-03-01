#include "stdafx.h"
#include <iostream>
#include <vector>
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



//										Command
//-----------------------------------------------
class Command {
public:
	virtual void execute() = 0;
	virtual void undo() = 0;

};
class NoCommand: public Command {
public:
	virtual void execute() { cout <<"\n NoCommand "; }
	virtual void undo() { cout << "\n NoCommand "; }
};
//											Light
//-----------------------------------------------
class Light {
	string name;
public:
	Light() { ; }
	Light(string n) {name = n;}
	void off() { cout << "\nOff light in " << name; }
	void on() { cout << "\nOn light in " << name; }

};
class LightOnCommand : public Command {
	Light& light;
public:
	LightOnCommand(Light& light): light(light){}
	virtual void execute() { light.on(); }
	virtual void undo() { light.off(); }
};
class LightOffCommand : public Command {
	Light& light;
public:
	LightOffCommand(Light& light): light(light){}
	virtual void execute() { light.off(); }
	virtual void undo() { light.on(); }
};

//
//------------------------------------------------------
class Music {
	int volume;
	int prevVolume;
	string name;
public:
	Music(string name) :volume(0), prevVolume(0){ this->name = name; }
	void on() { cout << "\n On music "<< name; }
	void off() { cout << "\n Off music "<<name; }
	void SetVolume(int vol) { 
		if (volume + vol >= 0 && volume + vol <= 25) { prevVolume = volume; volume = volume + vol; }
		else if (vol < 0) { prevVolume = volume;  volume = 0; }
		else if (vol > 0) { prevVolume = volume;  volume = 25; }
		cout << "\n volume = "<< volume;
	}
	int GetPrevVolume() {return prevVolume; }
};

class MusicOnCommand: public Command {
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
//										RemoteControl
//------------------------------------------------------
class RemoteControl {
	vector<pCommand> onCommand;
	vector<pCommand> offCommand;
	pCommand undoCommand;
public:
	RemoteControl(): onCommand(7, pCommand(new NoCommand())), offCommand(7, pCommand(new NoCommand())), undoCommand(pCommand(new NoCommand())) {}
	void setCommand(int slot, pCommand onCom, pCommand offCom) {onCommand[slot] = onCom; offCommand[slot] = offCom;}
	void onButtonWasPushed(int slot) { onCommand[slot]->execute(); undoCommand = onCommand[slot];}
	void offButtonWasPushed(int slot){ offCommand[slot]->execute(); undoCommand = onCommand[slot];	}
	void undoButtonWasPushed() { cout << "\nundo"; undoCommand->undo(); }
	};

int main()
{
	RemoteControl RC;
	Light lr("Living room");
	Light br("Bath room");
	RC.setCommand(0, pCommand(new LightOnCommand(lr)), pCommand(new LightOffCommand(lr)));
	RC.setCommand(1, pCommand(new LightOnCommand(br)), pCommand(new LightOffCommand(br)));
	Music  mlr("Music in living room");
	RC.setCommand(2, pCommand(new MusicOnCommand(mlr)), pCommand(new MusicOffCommand(mlr)));
	//Громкость музыки
	RC.setCommand(3, pCommand(new MusicUpCommand(mlr)), pCommand(new MusicDownCommand(mlr))); 
	RC.onButtonWasPushed(0);
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
