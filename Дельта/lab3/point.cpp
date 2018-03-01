#include "point.h"

CPoint::CPoint(){
	abs = 0.0;
	ord = 0.0;
	zord = 0.0;
	fstDif = 0.0;
	fstDifXZ = 0.0;
	fstDifYZ = 0.0;
	P = 0.0;
	D = 0.0;
	T = 0.0;
}
CPoint::~CPoint(){
}
	
void CPoint::SetAbs(double value){
	abs = value;
}

void CPoint::SetOrd(double value){
	ord = value;
}

void CPoint::SetZord(double value){
	zord = value;
}

void CPoint::SetFstDifXZ(double value){
	fstDifXZ = value;
}

void CPoint::SetFstDif(double value){
	fstDif = value;
}

void CPoint::SetFstDifYZ(double value){
	fstDifYZ = value;
}

void CPoint::SetT(double value){
	T = value;
}
	
void CPoint::SetP(double value){
	P = value;
}
	
void CPoint::SetD(double value){
	D = value;
}

double CPoint::GetAbs(){
	return abs;
}
double CPoint::GetOrd(){
	return ord;
}
double CPoint::GetZord(){
	return zord;
}
double CPoint::GetFstDifXZ(){
	return fstDifXZ;
}
double CPoint::GetFstDifYZ(){
	return fstDifYZ;
}
double CPoint::GetFstDif(){
	return fstDif;
}
double CPoint::GetP(){
	return P;
}
double CPoint::GetD(){
	return D;
}
double CPoint::GetT(){
	return T;
}