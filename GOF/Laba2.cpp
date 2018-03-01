// В свойствах проекта в опциях по C/С++ ->Библиотека времени выполнения; установить /MTd
//В свойствах проекта Компоновщик->Ввод->Дополнительные зависимости; поставьте  на первое место uafxcwd.lib
#include "stdafx.h"
#include "afxwin.h"
#include "iostream"
using namespace std;

HWND hwnd = 0;
HDC  hdc = 0;
void InitGraphic(){
	system("mode con cols=168 lines=55");	system("pause >> void");
	hwnd = FindWindow(_T("ConsoleWindowClass"), _T("C:\\Windows\\system32\\cmd.exe")); hdc = GetWindowDC(hwnd);
}
void CloseGraphic(){ ReleaseDC(hwnd, hdc); CloseHandle(hwnd); }

//-------------------------------------------------------------------------- IFigure
class IFigure{ //интерфейсный класс
public:
	int fMove; //0 - фигура двигается; 1 - фигура мигает на месте; 2 - фигура стоит на месте; 
	int fClr; //0 - фигура цвет не меняет; 1 - фигура меняет цвет
public:
	IFigure() : fMove(0), fClr(0){/*cout<<"\n IFigure()";*/ }
	virtual void show() = 0;
	virtual void hide() = 0;
	virtual void move(int x, int y) = 0;
	virtual void setfMove(int fM){
		fMove = fM;
	};
	virtual void setfClre(int fC){
		fClr = fC;
	}
};

//-------------------------------------------------------------------------Square
class Square :virtual public IFigure {
POINT pt[5];
	COLORREF color;
public:
	Square(POINT* p) : color(RGB(255, 0, 0)){ for (int i = 0; i <5; i++) pt[i] = p[i]; }


	Square(POINT* p, int fMove, int fClr) : color(RGB(255, 0, 0)){
		for (int i = 0; i <5; i++) pt[i] = p[i];
		IFigure::fMove = fMove;
		IFigure::fClr = fClr;
	}

	void SetColor(COLORREF cl){ color = cl; }
	void show(){
		CPen pen(PS_SOLID, 2, color);
		SelectObject(hdc, pen);
		Polyline(hdc, pt, 5);
	}
	void hide(){
		CPen pen;
		pen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
		SelectObject(hdc, pen);
		Polyline(hdc, pt, 5);
	}
	void move(int x, int y){ for (int i = 0; i<5; i++){ pt[i].x += x; pt[i].y += y; } }
	virtual ~Square(){/*cout<<"\t ~Square()";*/ }
};
//---------------------------------------------------------------------------ClsEllipse
class ClsEllipse : virtual  public IFigure {
	CPoint pt1, pt2;
	COLORREF color;
public:
	ClsEllipse() :pt1(100, 100), pt2(200, 200) {}
	ClsEllipse(CPoint p1, CPoint p2, COLORREF color) { pt1 = p1; pt2 = p2; fClr = 1; }
	virtual void SetColor(COLORREF cl){ color = cl; } // изменение цвета
	virtual void show() {
		CPen pen(PS_SOLID, 2, RGB(0, 255, 0));
		SelectObject(hdc, pen);
		Arc(hdc, pt1.x, pt1.y, pt2.x, pt2.y, 100, 200, 0, 100);
	}
	virtual void hide() {
		CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
		SelectObject(hdc, pen);
		Arc(hdc, pt1.x, pt1.y, pt2.x, pt2.y, 100, 200, 0, 100);
	}
	virtual void move(int x, int y)  { pt1.x += x, pt1.y += y, pt2.x += x, pt2.y += y; }
	virtual ~ClsEllipse(){/*cout<<"\t ~ClsEllipse()";*/ }
};
//-------------------------------------------------------------------------Rectan
class Rectan : virtual public IFigure {
	Square* pSq;
public:
	virtual void show(){
		pSq->show();
	}//Делегирование
	virtual void move(int x, int y){ pSq->move(x, y); }//Делегирование
	virtual void hide(){ pSq->hide(); }//Делегирование
	void SetColor(COLORREF cl){ pSq->SetColor(cl); }
	Rectan(Square& p){ pSq = new Square(p); }
	virtual ~Rectan(){ }

	Rectan& operator=(Rectan &r) { return r; }; //дописанный operator=

};
//-------------------------------------------------------------------------DrowTxt
class DrowTxt : virtual public IFigure {
CString str;
POINT pText;
COLORREF color;
public:
	DrowTxt() {
		pText.x = 0;
		pText.y = 0;
		color = RGB(255, 0, 0);
	}
	DrowTxt(CString s) :str(s){
		pText.x = 300;
		pText.y = 100;
		color = RGB(255, 0, 0);
	};
	DrowTxt(CString s, int x, int y, int fMove, int fClr) :str(s){
		pText.x = x;
		pText.y = y;
		color = RGB(255, 0, 0);
		IFigure::fMove = fMove;
		IFigure::fClr = fClr;
	};
	DrowTxt(CString s, int x, int y) :str(s){
		pText.x = x;
		pText.y = y;
		color = RGB(255, 0, 0);
	};
	void show(){
		CDC* pCDC = CDC::FromHandle(hdc);
		pCDC->SetTextColor(color);
		pCDC->SetBkColor(RGB(0, 0, 0));
		pCDC->TextOutW(pText.x, pText.y, str); pCDC->TextOutW(0, 0, " ");
	}
	void SetColor(COLORREF cl){ color = cl; }
	virtual void hide() {
		CDC* pCDC = CDC::FromHandle(hdc);
		pCDC->SetTextColor(RGB(0, 0, 0));
		pCDC->SetBkColor(RGB(0, 0, 0));
		pCDC->TextOutW(pText.x, pText.y, str); pCDC->TextOutW(0, 0, " ");
	}
	virtual void move(int x, int y) {
		CDC* pCDC = CDC::FromHandle(hdc);
		pCDC->SetTextColor(color);
		pCDC->SetBkColor(RGB(0, 0, 0));
		pText.x = pText.x + x;
		pText.y = pText.y + y;
		pCDC->TextOutW(pText.x, pText.y, str); pCDC->TextOutW(0, 0, " ");
	}
};

//-------------------------------------------------------------------------Heir
class Heir : public  Square, public ClsEllipse{ //Виртуальный базовый класс
public:                                        //(Множественное наследование)
	Heir(POINT *p) :Square(p), ClsEllipse(){/*cout<<"\t Heir()";*/ }
	void show(){ Square::show(); ClsEllipse::show(); }
	void move(int x, int y){ Square::move(x, y); ClsEllipse::move(x, y); }
	void hide(){ Square::hide(); ClsEllipse::hide(); }
	virtual ~Heir(){/*cout<<"\n ~Heir()";*/ }
	virtual void SetColor(COLORREF cl){ ClsEllipse::SetColor(cl); }
};
class TextInTheSquare : public Square, public DrowTxt {
public:
	TextInTheSquare(POINT *p, CString s) : Square(p), DrowTxt(s, p[0].x +10, p[0].y+10) {  }
	TextInTheSquare(POINT *p, CString s, int fMove, int fClr) : Square(p, fMove, fClr), DrowTxt(s, p[0].x + 10, p[0].y + 10) {  }
	void show() {
		Square::show();
		DrowTxt::show();
	}
	void hide() {
		Square::hide();
		DrowTxt::hide();
	}
	void move(int x, int y) {
		Square::move(x, y);
		DrowTxt::move(x, y);
	}
};
//------------------------------------------------------------------------RecordPlayer
class RecordPlayer{ //Чтобы воспользоваться классом, объекты должны поддерживать интерфейс IFigure
	IFigure**pFig;//Массив указателей IFigure*
	int n; //Текущее количество указателей в массиве
	int N; //Размерность массива
public:
	void Insert(IFigure* pF){
		if (n<N) pFig[n++] = pF;
		int fm = (*(pFig[n - 1])).fMove;
		int fc = (*(pFig[n - 1])).fClr;
		cout << "\nfMove=" << fm << ", fClr=" << fc;
	}
	RecordPlayer(int Nfig) : N(Nfig), n(0) {
		pFig = new IFigure*[N];
	}
	virtual void show(){ for (int i = 0; i < n; i++) pFig[i]->show(); }//Полиморфизм
	virtual void hide(){ for (int i = 0; i < n; i++) pFig[i]->hide(); }//Полиморфизм
	virtual void move(int x, int y){
		for (int i = 0; i < n; i++) {
			int fM = (*(pFig[i])).fMove;
			cout << "fM=" << fM << "\n";
			if (fM == 1) {
				cout << "fM=1, fm" << fM << "\n";
				show();
				Sleep(500);
				hide();
				Sleep(500);
				continue;
			}
			if (fM == 0) {
				pFig[i]->move(x, y);
				cout << "fM=2, fm" << fM << "\n";
			}
		}
	}//Полиморфизм
	void PlayMyObject(int x, int y){
		for (int i = 0; i <150; i++){
			show();
			Sleep(24);
			hide();
			move(x, y);
		}
		show();
	}
	virtual ~RecordPlayer(){ delete[]pFig; }
};

void main(){

	POINT pt1[5];
	pt1[0].x = 40; pt1[0].y = 40;
	pt1[1].x = 40; pt1[1].y = 140;
	pt1[2].x = 140; pt1[2].y = 140;
	pt1[3].x = 140; pt1[3].y = 40;
	pt1[4].x = 40; pt1[4].y = 40;

	InitGraphic();

	DrowTxt dtxt("Привет");
	//dtxt.show();
	//getchar();

	Heir hr(pt1);
	//for (int i = 0; i <100; i++){ hr.show();  Sleep(24);  hr.hide();  hr.move(0, 3); }
	//getchar();

	ClsEllipse elp;
	Square sq1(pt1), sq2(pt1), sq3(pt1);
	sq1.SetColor(RGB(255, 255, 0));  sq2.SetColor(RGB(0, 255, 0));
	sq3.SetColor(RGB(0, 0, 255));     hr.SetColor(RGB(0, 255, 255));
	sq2.move(20, 20); 	sq3.move(40, 30); 	hr.move(0, -150);
	Rectan rec(sq3);
	Rectan rec2 = rec;
	RecordPlayer RPlayer(5);
	elp.setfMove(2);
	RPlayer.Insert(&elp);
	//RPlayer.Insert(&sq1);
	//RPlayer.Insert(&sq2);
	//RPlayer.Insert(&rec);
	//RPlayer.Insert(&dtxt);
	//RPlayer.Insert(&rec2);
	//RPlayer.Insert(&hr);
	TextInTheSquare tits(pt1, "Text!");
	tits.setfMove(0);
	//tits.show();
	//RPlayer.Insert(&tits);
	RPlayer.Insert(&tits);
	RPlayer.PlayMyObject(3, 0);

	getchar();
	CloseGraphic();
}
