#include <fstream>
#include <iostream>
#include <vector>

#include <cmath>

#include "point.h"

#define NEARZERO 0.0000001

char* outputFileName;

//int numberOfPoints;
int discStepNumber;
//double discStep;
double baseValueDistance;
std::vector< std::vector<CPoint> > baseValues;
std::vector< std::vector<CPoint> > interpolPointsX;
std::vector< std::vector<CPoint> > interpolPointsXY;

int readData(char* fileName);
int printData(char* fileName, std::vector< std::vector<CPoint> > points);
// void showData();
void show (std::vector<CPoint> data);


int countXOZBaseParams(std::vector<CPoint> &basePoints);
int countXOZInterpolParams(std::vector<CPoint> &basePoints);

int countYOZBaseParams(std::vector<CPoint> &basePoints);
int countYOZInterpolParams(std::vector<CPoint> &basePoints);

std::vector<CPoint> transMatrix(std::vector<CPoint> &matrix);

std::vector<CPoint> makeXOZInterpolation(std::vector<CPoint> &basePoints);
std::vector<CPoint> makeYOZInterpolation(std::vector<CPoint> &basePoints);

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

template <typename T> std::vector<std::vector <T> > transMatrix(std::vector< std::vector<T> > &matrix){
	std::vector<std::vector <T> > result (matrix[0].size(), std::vector<T> (matrix.size()));
	//std::cout<<"? sizes "<<result.size()<<" "<<result[0].size()<<std::endl;
	result[0][0] = matrix[0][0];
	//std::cout<<"? sizes "<<matrix.size()<<" "<<matrix[0].size()<<std::endl;
	for(unsigned int i=0; i<matrix.size(); i++){
		for(unsigned int j=0; j<matrix[i].size(); j++){
	//		std::cout<<i<<" "<<j<<std::endl;
			result[j][i]=matrix[i][j];
		}
	}
	return result;
}
void show (std::vector<CPoint> data);


int main(int argc,  char** argv){
	if(argc!=3){
		std::cout<<"Wrong number of operands"<<std::endl;
		return 0;
	}
	if(!readData(argv[1])){
		std::cout<<"Error reading data from file"<<std::endl;
		return 0;
	}
	
	//discStep = baseValueDistance/discStepNumber;
	//showData();
	for(unsigned int i=0; i<baseValues.size(); i++){
		countXOZBaseParams(baseValues[i]);
		countXOZInterpolParams(baseValues[i]);
		interpolPointsX.push_back(makeXOZInterpolation(baseValues[i]));
		std::cout<<i<<std::endl;
	}
	
	//printData(argv[2], interpolPointsX);
	
	std::cout<<"?sizes "<<interpolPointsX.size()<<" "<<interpolPointsX[0].size();
	std::vector< std::vector<CPoint> > buffer = transMatrix(interpolPointsX);
	interpolPointsX = buffer;
	std::cout<<"tr made"<<std::endl;
	
	
	for(unsigned int i=0; i<interpolPointsX.size(); i++){
		countYOZBaseParams(interpolPointsX[i]);
		countYOZInterpolParams(interpolPointsX[i]);
		interpolPointsXY.push_back(makeYOZInterpolation(interpolPointsX[i]));
		std::cout<<i<<std::endl;
	}
	
	printData(argv[2], interpolPointsXY);
	
	
// 	countBaseParams();
// 	countInterpolParams();
// 	
// 	showData();
// 	
// 	makeInterpolation(argv[2]);
	
	return 0;
}

int readData(char* fileName){
	std::ifstream inputDataFile;
	
	inputDataFile.open(fileName, std::ifstream::in);
	
	int numberOfPointsX = 0;
	int numberOfPointsY = 0;
	
	if (inputDataFile.eof()){     
		 inputDataFile.close();// check for EOF
		return 0;
	}
	 else{
		inputDataFile>>numberOfPointsX;
	}
	
	if (inputDataFile.eof()){     
		 inputDataFile.close();// check for EOF
		return 0;
	}
	 else{
		inputDataFile>>numberOfPointsY;
	}
	
// 	if (inputDataFile.eof()){ 
// 		inputDataFile.close(); // check for EOF
// 		return 0;
// 	}
// 	 else{
// 		inputDataFile>>baseValueDistance;
// 	}
	
	if (inputDataFile.eof()){ 
		inputDataFile.close(); // check for EOF
		return 0;
	}
	 else{
		inputDataFile>>discStepNumber;
	}
	
	
	for(int i=0; i<numberOfPointsX; i++){
		std::vector<CPoint> baseRaw;
		for(int j=0; j<numberOfPointsY; j++){
			if (inputDataFile.eof()){ 
				inputDataFile.close(); // check for EOF
				return 0;
			}
			
			CPoint newPoint;
			double readValue;
			
			inputDataFile>>readValue;
			newPoint.SetAbs(readValue);
			
			inputDataFile>>readValue;
			newPoint.SetOrd(readValue);
			
			inputDataFile>>readValue;
			newPoint.SetZord(readValue);
			
			/*
			if(i!=0){
				newPoint.SetAbs(basePoints[i-1].GetAbs()+baseValueDistance);
			}*/
			
			baseRaw.push_back(newPoint);
		}
// 		if (inputDataFile.eof()){ 
// 			inputDataFile.close(); // check for EOF
// 			return 0;
// 		}else{
// 			double leftEdgeDifValueX, rightEdgeDifValueX;
// 			inputDataFile>>leftEdgeDifValue>>rightEdgeDifValue;
// 			basePoints[0].SetFstDif(leftEdgeDifValue);
// 			basePoints[numberOfPoints-1].SetFstDif(rightEdgeDifValue);
// 		}
		baseValues.push_back(baseRaw);
	}
	inputDataFile.close();
	return 1;
}

void show (std::vector<CPoint> data){
	std::cout<<"data!"<<std::endl;
	for(unsigned int i=0; i<data.size(); i++){
		std::cout<<data[i].GetAbs()<<" "<<data[i].GetOrd()<<" "<<data[i].GetZord()<<" P "<<data[i].GetP()<<std::endl;
		std::cout<<" D "<<data[i].GetD()<<" T "<<data[i].GetT()<<std::endl;
		std::cout<<"dif "<<data[i].GetFstDif()<<std::endl;
	}
	std::cout<<"--------------------------"<<std::endl;
}

int printData(char* fileName, std::vector < std::vector<CPoint> > points){
	std::ofstream outputFile(fileName);
	for(unsigned int i=0; i<points.size(); i++){
		for(unsigned int j=0; j<points[i].size(); j++){
			outputFile<<points[i][j].GetAbs()<<" "<<points[i][j].GetOrd()<<" "<<points[i][j].GetZord()<<std::endl;
		}
		outputFile<<std::endl;
	}
	outputFile.close();
	return 1;
}


// void showData(){
// 	std::cout<<"nuber of points: "<<numberOfPoints<<std::endl;
// 	std::cout<<"baseValueDistance: "<<baseValueDistance<<std::endl;
// 	std::cout<<"discStepNumber: "<<discStepNumber<<std::endl;
// 	std::cout<<"Base values: "<<std::endl;
// 	std::cout<<"ort: ";
// 	for(int i=0; i<numberOfPoints; i++){
// 		std::cout<<basePoints[i].GetOrd()<<" ";
// 	}
// 	std::cout<<std::endl;
// 	std::cout<<"D: ";
// 	for(int i=0; i<numberOfPoints; i++){
// 		std::cout<<basePoints[i].GetD()<<" ";
// 	}
// 	
// 	std::cout<<"T: ";
// 	for(int i=0; i<numberOfPoints; i++){
// 		std::cout<<basePoints[i].GetT()<<" ";
// 	}
// 	
// 	
// 	std::cout<<"P: ";
// 	for(int i=0; i<numberOfPoints; i++){
// 		std::cout<<basePoints[i].GetP()<<" ";
// 	}
// 	std::cout<<std::endl;
// 	
// 	std::cout<<"dif: ";
// 	for(int i=0; i<numberOfPoints; i++){
// 		std::cout<<basePoints[i].GetFstDif()<<" ";
// 	}
// 	std::cout<<std::endl;
// }


int countXOZBaseParams(std::vector<CPoint> &basePoints){
	std::cout<<"counting base params.."<<std::endl;
	for(unsigned int i=1; i<basePoints.size()-1; i++){
		double newDif = 0.0;
// 		if((basePoints[i-1].GetZord()<=basePoints[i].GetZord()&&basePoints[i].GetZord()>=basePoints[i+1].GetZord())||(basePoints[i-1].GetZord()>=basePoints[i].GetZord()&&basePoints[i].GetZord()<=basePoints[i+1].GetZord())){
// 			basePoints[i].SetFstDif(0.0);
// 		}
// 		else{
// 			newDif = 0.5*(((basePoints[i].GetZord()-basePoints[i-1].GetZord())/(basePoints[i].GetAbs()-basePoints[i-1].GetAbs()))+((basePoints[i+1].GetZord()-basePoints[i].GetZord())/(basePoints[i+1].GetAbs()-basePoints[i].GetAbs())));
// 			basePoints[i].SetFstDif(newDif);
// 		}
		
		newDif = 0.5*(((basePoints[i].GetZord()-basePoints[i-1].GetZord())/(basePoints[i].GetAbs()-basePoints[i-1].GetAbs()))+((basePoints[i+1].GetZord()-basePoints[i].GetZord())/(basePoints[i+1].GetAbs()-basePoints[i].GetAbs())));
		
		basePoints[i].SetFstDif(newDif);
		std::cout<<basePoints[i].GetFstDif()<<" "<<newDif<<std::endl;
	}
// 	basePoints[1].SetFstDif(3);
// 	basePoints[2].SetFstDif(3);
	return 1;
}



int countYOZBaseParams(std::vector<CPoint> &basePoints){
	std::cout<<"counting base params.."<<std::endl;
	for(unsigned int i=1; i<basePoints.size()-1; i++){
		double newDif = 0.0;
// 		if((basePoints[i-1].GetZord()<=basePoints[i].GetZord()&&basePoints[i].GetZord()>=basePoints[i+1].GetZord())||(basePoints[i-1].GetZord()>=basePoints[i].GetZord()&&basePoints[i].GetZord()<=basePoints[i+1].GetZord())){
// 			basePoints[i].SetFstDif(0.0);
// 		}
// 		else{
// 			newDif = 0.5*(((basePoints[i].GetZord()-basePoints[i-1].GetZord())/(basePoints[i].GetOrd()-basePoints[i-1].GetOrd()))+((basePoints[i+1].GetZord()-basePoints[i].GetZord())/(basePoints[i+1].GetOrd()-basePoints[i].GetOrd())));
// 			basePoints[i].SetFstDif(newDif);
// 		}
		
		newDif = 0.5*(((basePoints[i].GetZord()-basePoints[i-1].GetZord())/(basePoints[i].GetOrd()-basePoints[i-1].GetOrd()))+((basePoints[i+1].GetZord()-basePoints[i].GetZord())/(basePoints[i+1].GetOrd()-basePoints[i].GetOrd())));
		
		basePoints[i].SetFstDif(newDif);
		
	}
// 	basePoints[1].SetFstDif(3);
// 	basePoints[2].SetFstDif(3);
	return 1;
}

int countXOZInterpolParams(std::vector<CPoint> &basePoints){
	std::cout<<"counting.."<<std::endl;
	for(unsigned int i=0; i<basePoints.size()-1; i++){
		
		double T = basePoints[i+1].GetAbs()-basePoints[i].GetAbs();
		double L = basePoints[i+1].GetZord()-basePoints[i].GetZord()-0.5*T*(basePoints[i+1].GetFstDif()+basePoints[i].GetFstDif());
		double P = (-1*L-sgn(L)*sqrt(L*L+0.25*T*T*(basePoints[i+1].GetFstDif()-basePoints[i].GetFstDif())*(basePoints[i+1].GetFstDif()-basePoints[i].GetFstDif())))/(0.5*T*T);
		double D;
		//std::cout<<"L "<<L<<"  "<<std::abs(0.25)<<std::endl;
		if(std::abs(P)<NEARZERO||std::abs(L)<NEARZERO){
			D = 0.0;
		}
		else{
			D = (basePoints[i+1].GetFstDif()-basePoints[i].GetFstDif()+T*P)/(2*P);
		}
		basePoints[i].SetD(D);
		basePoints[i].SetP(P);
		basePoints[i].SetT(T);
		std::cout<<"L "<<L<<" sign "<<sgn(L)<<" D "<<D<<" P "<<T<<" P "<<P<<std::endl;
		show(basePoints);
	}
	return 1;
}

int countYOZInterpolParams(std::vector<CPoint> &basePoints){
	//std::cout<<"counting.."<<std::endl;
	for(unsigned int i=0; i<basePoints.size()-1; i++){
		double T = basePoints[i+1].GetOrd()-basePoints[i].GetOrd();
		double L = basePoints[i+1].GetZord()-basePoints[i].GetZord()-0.5*T*(basePoints[i+1].GetFstDif()+basePoints[i].GetFstDif());
		double P = (-1*L-sgn(L)*sqrt(L*L+0.25*T*T*(basePoints[i+1].GetFstDif()-basePoints[i].GetFstDif())*(basePoints[i+1].GetFstDif()-basePoints[i].GetFstDif())))/(0.5*T*T);
		double D;
		//std::cout<<"L "<<L<<"  "<<std::abs(0.25)<<std::endl;
		if(std::abs(P)<NEARZERO||std::abs(L)<NEARZERO){
			D = 0.0;
		}
		else{
			D = (basePoints[i+1].GetFstDif()-basePoints[i].GetFstDif()+T*P)/(2*P);
		}
		basePoints[i].SetD(D);
		basePoints[i].SetP(P);
		basePoints[i].SetT(T);
		//std::cout<<"L "<<L<<" sign "<<sgn(L)<<" D "<<D<<" P "<<T<<" P "<<P<<std::endl;
	}
	return 1;
}

std::vector<CPoint> makeXOZInterpolation(std::vector<CPoint> &basePoints){
	//std::ofstream outputFile;
	std::vector<CPoint> result;
	//outputFile.open(fileName, std::ifstream::out);
	show(basePoints);
	
	for(unsigned int i=0; i<basePoints.size()-1; i++){
		double discStep = (basePoints[i+1].GetAbs()-basePoints[i].GetAbs())/discStepNumber;
		std::cout<<"Step "<<discStep<<std::endl;
		double t = basePoints[i].GetAbs()+discStep;
		double x = discStep;
		result.push_back(basePoints[i]);
		
		//std::cout<<"t "<<t<<" "<<basePoints[i+1].GetAbs();
		
		while(t<basePoints[i+1].GetAbs()){
			CPoint newPoint;
			newPoint.SetAbs(t);
			newPoint.SetOrd(basePoints[i].GetOrd());
			
			double z;
			if(t<(basePoints[i+1].GetAbs()-basePoints[i].GetD())){
				z = basePoints[i].GetZord()+basePoints[i].GetFstDif()*x-0.5*x*x*basePoints[i].GetP();
			}
			else{
				z = basePoints[i+1].GetZord()-basePoints[i+1].GetFstDif()*(basePoints[i].GetT()-x)+0.5*(basePoints[i].GetT()-x)*(basePoints[i].GetT()-x)*basePoints[i].GetP();
			}
			newPoint.SetZord(z);
			//std::cout<<z<<" "<<basePoints[i].GetP()<<std::endl;
			t+=discStep;
			x+=discStep;
			result.push_back(newPoint);
		}
	}
	result.push_back(basePoints[basePoints.size()-1]);
	return result;
}

std::vector<CPoint> makeYOZInterpolation(std::vector<CPoint> &basePoints){
	//std::ofstream outputFile;
	std::vector<CPoint> result;
	
	for(unsigned int i=0; i<basePoints.size()-1; i++){
		double discStep = (basePoints[i+1].GetOrd()-basePoints[i].GetOrd())/discStepNumber;
		double t = basePoints[i].GetOrd()+discStep;
		double x = discStep;
		result.push_back(basePoints[i]);
		
		while(t<basePoints[i+1].GetOrd()){
			CPoint newPoint;
			newPoint.SetOrd(t);
			newPoint.SetAbs(basePoints[i].GetAbs());
			
			double z;
			if(t<(basePoints[i+1].GetOrd()-basePoints[i].GetD())){
				z = basePoints[i].GetZord()+basePoints[i].GetFstDif()*x-0.5*x*x*basePoints[i].GetP();
			}
			else{
				z = basePoints[i+1].GetZord()-basePoints[i+1].GetFstDif()*(basePoints[i].GetT()-x)+0.5*(basePoints[i].GetT()-x)*(basePoints[i].GetT()-x)*basePoints[i].GetP();
			}
			newPoint.SetZord(z);
// 			//std::cout<<z<<std::endl;
			t+=discStep;
			x+=discStep;
			result.push_back(newPoint);
		}
	}
	result.push_back(basePoints[basePoints.size()-1]);
	return result;
}