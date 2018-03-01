// В свойствах проекта в опциях по C/С++ ->Библиотека времени выполнения; установить /MTd
//В свойствах проекта Компоновщик->Ввод->Дополнительные зависимости; поставьте  на первое место uafxcwd.lib
#include "stdafx.h"
#include "afxwin.h"
#include "iostream"
using namespace std ;

HWND hwnd = 0;
HDC  hdc = 0;
void InitGraphic(){
system("mode con cols=168 lines=55");	system("pause >> void");
hwnd=FindWindow(_T("ConsoleWindowClass"),_T("C:\\Windows\\system32\\cmd.exe"));hdc=GetWindowDC(hwnd);
}
void CloseGraphic(){ReleaseDC(hwnd, hdc); CloseHandle(hwnd);}

//-------------------------------------------------------------------------- IFigure
class IFigure{ //интерфейсный класс
protected:
	int fMove; //0 - фигура двигается; 1 - фигура мигает на месте; 2 - фигура стоит на месте; 
	int fClr; //0 - фигура цвет не меняет; 1 - фигура меняет цвет
	COLORREF color;
	
public:
	IFigure(): fMove(0), fClr(0){/*cout<<"\n IFigure()";*/}
	void SetColor(COLORREF cl) { color = cl; }
	virtual void show()=0;
	virtual void hide()=0;
	virtual void move(int x, int y)=0;
};

//-------------------------------------------------------------------------Square
class Square:virtual public IFigure {
	POINT pt[5];
	
public:
	Square(POINT* p) { color = RGB(255, 0, 0); for (int i = 0; i < 5; i++) pt[i] = p[i]; }
	
	void show(){
		CPen pen(PS_SOLID,2,color);
		SelectObject(hdc,pen);
		Polyline(hdc,pt,5 );
	}
	void hide(){
		CPen pen;
		pen.CreatePen(PS_SOLID,2,RGB(0,0,0));
		SelectObject(hdc,pen);
		Polyline(hdc,pt,5 );
	}
	void move(int x, int y){for(int i = 0; i<5;i++){ pt[i].x+=x;pt[i].y+=y;} }
	virtual ~Square(){/*cout<<"\t ~Square()";*/}
};
//---------------------------------------------------------------------------ClsEllipse
class ClsEllipse: virtual  public IFigure {
	CPoint pt1,pt2;
	//COLORREF color;
public:
	ClsEllipse() :pt1(100, 100), pt2(200, 200) { color = RGB(0, 255, 0); }
	ClsEllipse(CPoint p1, CPoint p2, COLORREF c = (RGB(0, 255, 0))) { pt1 = p1; pt2 = p2; color = c; }
	//void SetColor(COLORREF cl) { color = cl; };
	virtual void show() {
		CPen pen(PS_SOLID,2,color);
		SelectObject(hdc,pen);
		Arc(hdc,pt1.x,pt1.y,pt2.x,pt2.y,100,200,0,100);
	}
	virtual void hide() {
		CPen pen(PS_SOLID,2,RGB(0,0,0));
		SelectObject(hdc,pen);
		Arc(hdc,pt1.x,pt1.y,pt2.x,pt2.y,100,200,0,100);
	}
	virtual void move(int x, int y)  { pt1.x+=x,pt1.y+=y,pt2.x+=x,pt2.y+=y; }
	virtual ~ClsEllipse(){/*cout<<"\t ~ClsEllipse()";*/}
};
//-------------------------------------------------------------------------Rectan
class Rectan: public IFigure {
	Square* pSq;
public:
	virtual void show(){pSq->show();}//Делегирование
	virtual void move(int x, int y){pSq->move(x,y);}//Делегирование
	virtual void hide(){pSq->hide();}//Делегирование
	void SetColor(COLORREF cl){pSq->SetColor(cl);}
	Rectan (Square& p){pSq = new Square(p);}
	Rectan& operator=(Rectan &r) { return r; };
	virtual ~Rectan(){delete pSq;}
};
//-------------------------------------------------------------------------DrowTxt
class DrowTxt: virtual public IFigure {
	CString str;
	int ix, iy;
public:
	DrowTxt(CString s):str(s), ix(300), iy(100){}
	void setCoord(int x, int y) { ix = x; iy = y; }
	void show(){
		CDC* pCDC = CDC::FromHandle(hdc);
		pCDC->SetTextColor(RGB(255,0,0));
		pCDC->SetBkColor(RGB(0,0,0));
		pCDC->TextOutW(ix,iy,str); pCDC->TextOutW(0,0," ");
	}
	void hide() {
		CDC* pCDC = CDC::FromHandle(hdc);
		pCDC->SetTextColor(RGB(0, 0, 0));
		pCDC->SetBkColor(RGB(0, 0, 0));		
	}
	void move(int x, int y) {
		CDC* pCDC = CDC::FromHandle(hdc);
		pCDC->SetTextColor(RGB(255,0,0));
		pCDC->SetBkColor(RGB(0, 0, 0));
		ix += x; iy += y;
		pCDC->TextOutW(ix, iy, str); pCDC->TextOutW(0, 0, " ");
	}
};

class TextInSquare : public Square, public DrowTxt {
public:
	TextInSquare(CString s, POINT *p) : Square(p), DrowTxt(s) { DrowTxt::setCoord(p[0].x, p[0].y); }
	void show() { Square::show(); DrowTxt::show(); }
	void hide() { Square::hide(); DrowTxt::hide(); }
	void move(int x, int y) { Square::move(x, y); DrowTxt::move(x, y); }
};

//-------------------------------------------------------------------------Heir
class Heir : public  Square, public ClsEllipse { //Виртуальный базовый класс
public:                                        //(Множественное наследование)
	Heir(POINT *p) :Square(p), ClsEllipse() {/*cout<<"\t Heir()";*/ }
	void show() { Square::show(); ClsEllipse::show(); }
	void move(int x, int y) { Square::move(x, y); ClsEllipse::move(x, y); }
	void hide() { Square::hide(); ClsEllipse::hide(); }	
	virtual ~Heir(){/*cout<<"\n ~Heir()";*/}
};
 //------------------------------------------------------------------------RecordPlayer
class RecordPlayer{ //Чтобы воспользоваться классом, объекты должны поддерживать интерфейс IFigure
	IFigure**pFig;//Массив указателей IFigure*
	int n; //Текущее количество указателей в массиве
	int N; //Размерность массива
public:	
	void Insert(IFigure* pF){if (n<N) pFig[n++] =pF; }
	RecordPlayer(int Nfig): N(Nfig), n(0) {	pFig = new IFigure*[N]; }
	virtual void show(){  for(int i = 0; i < n; i++) pFig[i]->show();  }//Полиморфизм
	virtual void hide(){  for(int i = 0; i < n; i++) pFig[i]->hide();  }//Полиморфизм
	virtual void move(int x, int y){  for(int i = 0; i < n; i++) pFig[i]->move(x,y);  }//Полиморфизм
	void PlayMyObject(int x, int y){  for(int i = 0; i <150 ; i++){show();Sleep(24);hide(); move(x,y);} show();}
	void BlinkMyObject() { for (int i = 0; i < 150; i++) { show(); Sleep(24); hide(); Sleep(24); } }
	void SetColorMyObject() { for (int i = 0; i < n; i++) pFig[i]->hide(); }
	virtual ~RecordPlayer(){delete []pFig;}
};

void main(){
	
	POINT pt1[5];
	pt1[0].x = 40;pt1[0].y=80; 	
	pt1[1].x = 440;pt1[1].y=80; 	
	pt1[2].x = 440;pt1[2].y=240; 	
	pt1[3].x = 40;pt1[3].y=240;
	pt1[4].x = 40;pt1[4].y=80;
	POINT pt2[5];
	pt2[0].x = 360; pt2[0].y = 50;
	pt2[1].x = 420; pt2[1].y = 50;
	pt2[2].x = 420; pt2[2].y = 80;
	pt2[3].x = 360; pt2[3].y = 80;
	pt2[4].x = 360; pt2[4].y = 50;

	InitGraphic();

	//Heir hr(pt1);
	//TextInSquare txtsqr("ola", pt1);
	//txtsqr.show();
	//for (int i = 0; i < 100; i++) {
	//	hr.show(); txtsqr.show(); Sleep(24);  hr.hide(); txtsqr.hide(); hr.move(0, 3); txtsqr.move(0, 3);}
	//getchar();
	//ClsEllipse elp;
	//elp.SetColor(RGB(128, 43, 230));
	//Square sq1(pt1), sq2(pt1), sq3(pt1);
	//sq1.SetColor(RGB(255,255,0));  sq2.SetColor(RGB(0,255,0)); 
	//sq3.SetColor(RGB(0, 0, 255));     hr.Square::SetColor(RGB(0, 255, 255));
	//sq2.move(20,20); 	sq3.move(40,30); 	hr.move(0,-150);    
	//Rectan rec(sq3);
	//RecordPlayer RPlayer(5);
	//RPlayer.Insert(&elp);
	//RPlayer.Insert(&sq1);
	//RPlayer.Insert(&sq2);
	//RPlayer.Insert(&rec);
	//RPlayer.Insert(&hr);
	////RPlayer.PlayMyObject(3,0);
	//RPlayer.BlinkMyObject();

	Square body(pt1); body.SetColor(RGB(189, 183, 107));
	Square head(pt2); head.SetColor(RGB(189, 183, 107));
	ClsEllipse head1( CPoint(345, 80), CPoint(325, 60), RGB(0, 255, 0));
	ClsEllipse head2(CPoint(315, 80), CPoint(295, 60), RGB(0, 255, 0));
	ClsEllipse head3(CPoint(285, 80), CPoint(265, 60), RGB(0, 255, 0));
	ClsEllipse head4(CPoint(255, 80), CPoint(235, 60), RGB(0, 255, 0));
	ClsEllipse head5(CPoint(225, 80), CPoint(205, 60), RGB(0, 255, 0));
	ClsEllipse head6(CPoint(195, 80), CPoint(175, 60), RGB(0, 255, 0));
	ClsEllipse head7(CPoint(165, 80), CPoint(145, 60), RGB(0, 255, 0));
	ClsEllipse head8(CPoint(135, 80), CPoint(115, 60), RGB(0, 255, 0));
	ClsEllipse head9(CPoint(105, 80), CPoint(85, 60), RGB(0, 255, 0));
	ClsEllipse head10(CPoint(75, 80), CPoint(55, 60), RGB(0, 255, 0));
	ClsEllipse wheel1(CPoint(60, 195), CPoint(150, 285), RGB(255, 128, 0));
	ClsEllipse wheel2(CPoint(330, 195), CPoint(420, 285), RGB(255, 128, 0));

	RecordPlayer rp(14);
	rp.Insert(&body);
	rp.Insert(&head);
	rp.Insert(&head1);
	rp.Insert(&head2);
	rp.Insert(&head3);
	rp.Insert(&head4);
	rp.Insert(&head5);
	rp.Insert(&head6);
	rp.Insert(&head7);
	rp.Insert(&head8);
	rp.Insert(&head9);
	rp.Insert(&head10);
	rp.Insert(&wheel2);
	rp.Insert(&wheel1);

	rp.PlayMyObject(3, 3);

	getchar();
	CloseGraphic();
}
