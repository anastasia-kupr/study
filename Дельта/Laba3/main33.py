import numpy as np
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
import pandas as pd


# i = 0;
# j = 0;
# ix = 0;
# iy = 0;
# k = 1 #число точек на один отрезок сплайна
# current = 0.0
# D = 0.0
# dx = 0.0
# dy = 0.0
# L = 0.0
# na = 0.0
# P = 0.0
# swsign = 0.0
# t = 0.0
# Tn = 0.0
# lx = 0.0 #число шагов по х
# ly = 0.0 #число шагов по у
# elemX = 0.0
# elemY = 0.0

#начальные данные

# reading data
pars_df = pd.read_csv("in_pars2.txt", sep=" ")
valX = pars_df["x"].values
valY = pars_df["y"].values
print("valX=", valX)
print("valY=", valY)
data_df = pd.read_csv("in_data.txt", sep=" ", header=None)
valZ = data_df.values
print("valZ=", valZ)
print("first=", valZ[0][0])
# from the above
K = int(input("Enter amount of points per spline: "))

lx = len(valX)
ly = len(valY)
print("lx=", lx)
print("ly=", ly)
elemX = int((lx - 1)*K + 1)
elemY = int((ly - 1)*K + 1)
print("elemX=", elemX)
print("elemY=", elemY)
# print("lx=",lx)
#производные
derZ = np.zeros((lx, ly))
resX = np.zeros(elemX)
resY = np.zeros(elemY)

resOXZ = np.zeros((elemX, ly), dtype=np.float32)
resOYZ = np.zeros((elemX, elemY), dtype=np.float32)

derZ[0] = pars_df["bot"].values
derZ[-1] = pars_df["top"].values
print("derZ1=", derZ)
# print("valZ", valZ)
# Построим одномерные кривые в плоскостях OXZ
for iy in range(ly):
    # print("iy=", iy)
    for i in range(lx):
        if i == 0:
            derZ[i][iy] = (valZ[i + 1][iy] - valZ[i][iy]) / (valX[i + 1] - valX[i])
            continue
        # if(((valZ[i - 1][iy] <= valZ[i][iy]) and (valZ[i + 1][iy] <= valZ[i][iy])) or ((valZ[i - 1][iy] >= valZ[i][iy])and(valZ[i + 1][iy] >= valZ[i][iy]))):
        #     derZ[i, iy] = 0
        # print("valZ[i][iy]=", valZ[i][iy], ", valZ[i-1][iy]=", valZ[i-1][iy], ", valZ[i+1][iy]=", valZ[i+1][iy], ", valX[i]=", valX[i], ", valX[i-1]=", valX[i-1], ", valX[i+1]=", valX[i+1])
        if i == (lx-1):
            # print("!i=", i)
            derZ[i][iy] = (valZ[i][iy] - valZ[i-1][iy])/(valX[i] - valX[i-1])
            continue
        else:
            derZ[i][iy] = 0.5 * ((valZ[i][iy] - valZ[i-1][iy])/(valX[i] - valX[i-1]) + (valZ[i+1][iy] - valZ[i][iy])/(valX[i+1] - valX[i]))
    # print("iy=", iy, ", derZ=", derZ)
    for i in range(lx-1):
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
                # print("delta=", (derZ[i][iy] * t - t * t * P / 2))
            else:
                resOXZ[i*K + j][iy] = valZ[i+1][iy] - derZ[i+1][iy]*(Tn-t) + (Tn-t)*(Tn-t)*P/2
                # print("delta=", (- derZ[i+1][iy]*(Tn-t) + (Tn-t)*(Tn-t)*P/2))
            current = current + dx
            j = j+1

derZ[:, 0] = pars_df["left"].values
derZ[:, -1] = pars_df["right"].values
print("resOXZ=", resOXZ)
derZ2 = np.zeros((elemX, elemY))
# for i in range (elemX-1):
print("derZ2=", derZ)

# Построим одномерные кривые в плоскостях OYZ
# Данные для построения берутся из OXZ - результатов вычислений на предыдущем шаге
for ix in range(elemX):

    derZ2[ix, 0] = derZ[round(ix/K), 0]
    derZ2[ix, ly-1] = derZ[round(ix/K), ly-1]
    print("ix=", ix)
    for i in range(ly):
        print("i=", i)
        if i==0:
            derZ2[ix][i] = (resOXZ[ix][i+1] - resOXZ[ix][i])/(valY[i+1] - valY[i])
            continue
        elif i==(ly-1):
            derZ2[ix][i] = (resOXZ[ix][i] - resOXZ[ix][i-1])/(valY[i] - valY[i-1])
            continue
        # if(((resOXZ[ix][i - 1] <= resOXZ[ix][i])and(resOXZ[ix][i + 1] <= resOXZ[ix][i]))or((resOXZ[ix][i - 1] >= resOXZ[ix][i])and(resOXZ[ix][i + 1] >= resOXZ[ix][i]))):
        #     derZ2[ix, i] = 0
        else:
            derZ2[ix][i] = 0.5 * ((resOXZ[ix][i] - resOXZ[ix][i-1])/(valY[i] - valY[i-1]) + (resOXZ[ix][i+1] - resOXZ[ix][i])/(valY[i+1] - valY[i]))
        # print("derZ2[", ix, "][", i, "]=", derZ2[ix][i])
    for i in range(ly-1):
        Tn = valY[i+1] - valY[i]
        L = resOXZ[ix][i+1] - resOXZ[ix][i] - 0.5*Tn*(derZ2[ix][i+1]+derZ2[ix][i])
        P = (L - np.sign(L) * np.sqrt(L * L + 0.25 * (Tn * Tn) * (pow((derZ2[i][iy] - derZ2[i + 1][iy]), 2)))) / (0.5 * Tn * Tn)
        if ((L==0)or(P==0)):
            D = 0
        else:
            D = (derZ2[ix][i+1] - derZ2[ix][i] + Tn*P)/(2*P)
        current = valY[i]
        dy = (valY[i + 1] - valY[i]) / K
        swsign = valY[i + 1] - D
        j = 0
        while(current <= valY[i+1]):
            resY[i * K + j] = current
            t = dy*j
            if (current <= swsign):
                resOYZ[ix][i * K + j] = resOXZ[ix][i] + derZ2[ix][i] * t - t * t * P / 2
            else:
                resOYZ[ix][i * K + j] = resOXZ[ix][i + 1] - derZ2[ix][i + 1] * (Tn - t) + (Tn - t) * (Tn - t) * P / 2
            current = current + dy
            j = j + 1

# Массив resX содержит координаты Х плоскостей OXZ
# Массив resY содержит координаты Y плоскостей OYZ
# Массив resOYZ содержит значения узлов интерполированной поверхности
print("resOYZ=", resOYZ)
print("resX=", resX)
print("resY=", resY)

X, Y = np.meshgrid(resX, resY)

fig = plt.figure()
ax = fig.gca(projection='3d')
ax.set_xlabel('X axis')
ax.set_ylabel('Y axis')
ax.set_zlabel('Z axis')
surf = ax.plot_surface(X, Y, resOYZ, cmap=cm.bone, linewidth=0, antialiased=True)
Xp, Yp = np.meshgrid(valX, valY)
ax.scatter(Xp, Yp, valZ, c="r", s=20)
plt.show()