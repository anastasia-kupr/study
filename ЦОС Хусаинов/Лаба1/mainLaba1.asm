
#include "def21060.h"
#define N 100
#define cs 10.0
#define c1 0.125
#define c5 0.5
#define c2 1.5
#define i 0;
#define Y0 0;
#define deltaY0 0;
#define z0 0;

//Source array
//0x30000
.SECTION/DM dm_data;
.VAR input[N] = "data.txt";



//Result Array
//0x24000
.SECTION/PM    pm_data;
.VAR output[N];
.VAR delta[N];
.VAR F[N];
.VAR deltaZ[N];

.VAR delta2Y[N];
.VAR deltaY[N];
//.VAR c = 10.0;
//.VAR cs = 10.0;


.VAR zi;
.VAR zip;
.VAR Fi;
.VAR deltaz;



//0x20004 - 0x20007
.SECTION/PM	   pm_irq_svc;
		nop;
		jump start;
		nop;
		nop;
		
//0x20100 - 0x20200
.SECTION/PM    pm_code;
start:
		I0 = input;
		I1 = delta;
		I2 = output;
		
		M0 = 1;
		M7 = 1;
		
		F0 = 0;
		F1 = c1*cs; //0.125c
		F2 = cs;
 		F2 = RECIPS F2;
 		F15 = c5;
 		F2 = F15*F2; //0.5/c
 		
 		
 		F3 = z0;
 		F4 = Y0;
 		F9 = deltaY0;
 		F12 = 0.0;		
			
 		LCNTR = N, DO xxx UNTIL LCE;
 			F5 = DM(I0,1);//yi
 			F11 = F4-F5;//zi=Yi-yi
 			F10 = F11-F3; //deltaz=zi-z(i-1)
 			
 			COMP(F10, F0);
 			F15 = -1.0;
 			f14 = 1.0;
 			if LE F13=F15; //F13 = sign(deltaZi)
 			if GT F13=F14;
 			
 			F15 = F10*F10; //deltaz^2
 			F15 = F15*F2; //0.5deltaz^2/c
 			F15 = F15-F1; //0.5deltaz^2/c-0.125c
 			F15 = F15*F13; //(0.5deltaz^2/c-0.125c)*deltazi
 			F14 = c2; //1.5
 			F14 = F14*F10; //1.5*deltazi
 			F15 = F15+F14; //1.5*deltazi+(0.5deltaz^2/c-0.125c)*deltazi
 			F6 = F15+F11; //Fi
 			
 			COMP(F6, F0);
 			F13 = -1.0;
 			F14 = 1.0;
 			if LE F7=F14; //F7 = delta-sign(Fi)
 			if GT F7=F13;
 			DM(I1, 1) = F7; 			
 			
 			F8 = cs;
 			F8 = F8*F7;//delta2Y(i+1)
 			F9 = F9+F8;//deltaY(i+1) = deltaYi+delta2Y(i+1)
 			F4 = F4+F9;//Y(i+1) = Yi+deltaY(i+1) 			
 			
 			//DM(I2, M7) = F4;
 			F3 = F11;
 			F12 = F12+F14;
 			 			
 			nop;
 			nop;
xxx:		nop;

		
		I2 = output;
		I1 = delta;
 		LCNTR = N, DO yyy UNTIL LCE;
 			F7 = DM(I1,1);
 			F8 = cs;
 			F8 = F8*F7;//delta2Y(i+1)
 			F9 = F9+F8;//deltaY(i+1) = deltaYi+delta2Y(i+1)
 			F4 = F4+F9;//Y(i+1) = Yi+deltaY(i+1)  
 			DM(I2, M7) = F4; 			
 			
 			nop;
 			nop;
 			nop;	 			 			

yyy:		nop;
		

wait:	IDLE;
		jump wait;	
		
		
Sign1: F9 = 1.0;
		RTS;
Sign2: F9 = -1.0;
		RTS;
