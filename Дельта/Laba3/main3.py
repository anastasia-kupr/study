import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import math as mt


# i = 0;
# j = 0;
# ix = 0;
# iy = 0;
k = 1 #число точек на один отрезок сплайна
current = 0.0
D = 0.0
dx = 0.0
dy = 0.0
L = 0.0
na = 0.0
P = 0.0
swsign = 0.0
t = 0.0
Tn = 0.0
lx = 0.0 #число шагов по х
ly = 0.0 #число шагов по у
elemX = 0.0
elemY = 0.0

#начальные данные
K = 1
lx = 5
ly = 10
valX = np.zeros(lx)
for i in range(lx):
    valX[i] = i
ly = 10
valY = np.zeros(ly)
for i in range(ly):
    valY[i] = i
print("valX=", valX)
print("valY=", valY)
elemX = int((lx - 1)*K + 1);
elemY = int((ly - 1)*K + 1);
valZ = np.zeros((lx, ly))
print("valZ", valZ)
ind = np.zeros((elemX-1,elemY-1), np.complex)
for i in range(lx):
    for j in range(ly):
        valZ[i][j] = i+j
        for k in range(k):
            ind[i][j] = complex(i+k,j+k)
print("valZ=", valZ)
print("ind=", ind)

#производные
derZ = np.ones((elemX, elemY))
resX = np.ones(elemX)
resY = np.ones(elemY)

resOXZ = np.zeros((elemX, ly), float)
resOYZ = np.zeros((elemX, elemY), float)

for iy in range(ly-1):
    #граничные значения производных
    print("iy=", iy)
    derZ[0][iy] = 0
    derZ[lx - 1][iy] = 0
    for i in range(lx-2):
        if(((valZ[i - 1][iy] <= valZ[i][iy]) and (valZ[i + 1][iy] <= valZ[i][iy])) or ((valZ[i - 1][iy] >= valZ[i][iy])and(valZ[i + 1][iy] >= valZ[i][iy]))):
            derZ[i, iy] = 0
        derZ[i][iy] = 0.5 * ((valZ[i][iy] - valZ[i-1][iy])/(valX[i] - valX[i-1]) + (valZ[i+1][iy] - valZ[i][iy])/(valX[i+1] - valX[i]))

    for i in range(lx-2):
        Tn = valX[i+1] - valX[i]
        L = valZ[i + 1][iy] - valZ[i][iy] - 0.5 * Tn * (derZ[i + 1][iy] + derZ[i][iy])
        P = (-L - np.sign(L) * np.sqrt(L * L + 0.25 * (Tn * Tn) * (pow((derZ[i][iy] - derZ[i + 1][iy]), 2)))) / (0.5 * Tn * Tn)
        if ((L==0)or(P==0)):
            D = 0
        else:
            D = (derZ[i+1][iy] - derZ[i][iy] + Tn*P)/(2*P)
        current = valX[i]
        dx = (valX[i + 1] - valX[i]) / K
        swsign = valX[i + 1] - D
        j = 0
        while(current <= valX[i+1]):
            resX[i * K + j] = current
            t = dx*j
            if (current <= swsign):
                resOXZ[i * K + j][iy] = valZ[i][iy] + derZ[i][iy] * t - t * t * P / 2
            else:
                resOXZ[i*K + j][iy] = valZ[i+1][iy] - derZ[i+1][iy]*(Tn-t) + (Tn-t)*(Tn-t)*P/2
            current = current + dx
            j = j+1


for ix in range(elemX - 1):
    #граничные значения производных
    derZ[ix][0] = 0
    derZ[ix][ly - 1] = 0
    for i in range(ly-2):
        if(((resOXZ[ix][i - 1] <= resOXZ[ix][i])and(resOXZ[ix][i + 1] <= resOXZ[ix][i]))or((resOXZ[ix][i - 1] >= resOXZ[ix][i])and(resOXZ[ix][i + 1] >= resOXZ[ix][i]))):
            derZ[ix, 0] = 0
        derZ[i][iy] = 0.5 * ((resOXZ[ix][i] - resOXZ[ix][i-1])/(valY[i] - valY[i-1]) + (resOXZ[ix][i+1] - resOXZ[ix][i])/(valY[i+1] - valY[i]))

    for i in range(ly-2):
        Tn = valY[i+1] - valY[i]
        L = resOXZ[ix][i+1] - resOXZ[ix][i] - 0.5*Tn*(derZ[ix][i+1]+derZ[ix][i])
        P = (L - np.sign(L) * np.sqrt(L * L + 0.25 * (Tn * Tn) * (pow((derZ[i][iy] - derZ[i + 1][iy]), 2)))) / (0.5 * Tn * Tn)
        if ((L==0)or(P==0)):
            D = 0
        else:
            D = (derZ[ix][i+1] - derZ[ix][i] + Tn*P)/(2*P)
        current = valY[i]
        dy = (valY[i + 1] - valY[i]) / K
        swsign = valY[i + 1] - D
        j = 0
        while(current <= valY[i+1]):
            resX[i * K + j] = current
            t = dy*j
            if (current <= swsign):
                resOYZ[ix][i * K + j] = resOXZ[ix][i] + derZ[ix][i] * t - t * t * P / 2
            else:
                resOYZ[ix][i * K + j] = resOXZ[ix][i + 1] - derZ[ix][i + 1] * (Tn - t) + (Tn - t) * (Tn - t) * P / 2
            current = current + dy
            j = j + 1
print("resOYZ=", resOYZ)
 # for i in elemY-1:
 #     for j in elemX-1:




