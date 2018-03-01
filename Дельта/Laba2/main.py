import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import math as mt

def read_data(fname):
    df = pd.read_csv(fname, sep=" ", escapechar="#")
    in_val = df.values
    tn = in_val[:, 0]
    xn = in_val[:, 1]
    yn = in_val[:, 2]
    Kn = in_val[:, 3]
    Kn = Kn[~np.isnan(Kn)]
    Kn = np.array(Kn, dtype=np.int16)
    return tn, xn, yn, Kn

def write_data(fname, t, X, Y):
    out_df = pd.DataFrame(
        {"t" : t,
        "x(t)" : X,
        "y(t)" : Y}
    )
    out_df.to_csv(fname, sep="\t", index=False)

#входные данные
#tn = np.array([0., 20., 35., 47., 56., 63., 69., 80., 90., 120.])
#yn = np.array([100., 90., 110., 60., 45., 10., -30., 15., 45., 200.])
#tn = np.array([-3439., -3272., -3234., -2809., -1874., -1300., -676., 320., 1178., 1463.])
#yn = np.array([735., -863., 608., 845., 157., -838., -63., 435., -312., 165.])
#Kn = np.array([3, 7, 8, 5, 3, 5, 7, 5, 9])


def steps(tn): # вычисление длин участков сплайна и величины шага дискретизации для каждого сплайна
    N = yn.shape[0]
    for i in range(N - 1):
        if i == (N - 1):
            T[i] = tn[N] - tn[N - 1]
        else:
            T[i] = tn[i+1] - tn[i]
        dt[i] = (T[i]*1.0)/((Kn[i])*1.0)
    return T, dt

def get_t(dt, Kn): #вычисление аргументов сплайна
    ind = 0
    N = dt.shape[0] + 1
    PointCnt = Kn.sum()
    t = np.zeros(PointCnt + 1)
    t[PointCnt] = tn[N - 1]
    for i in range(N - 1):
        for j in range((Kn[i])):
            if ind >= PointCnt:
                break
            if j == 0:
                t[ind] = tn[i]
                #print("i=", i, ", j=", j,"t[",ind,"]=",t[ind])
                ind = ind + 1
                continue
            else:
                t[ind] = t[ind - 1] + dt[i]
            #print("i=", i, ", j=", j,"t[",ind,"]=",t[ind])
            ind = ind+1
    return t

def calc_dy(yn, tn): #вычисление производных
    N = yn.shape[0]
    dy = np.zeros(N)
    for i in range(1, N-1):
        # print("i=", i, ", yn[i-1]=", yn[i-1], ", yn[i]=", yn[i], ", yn[i+1]=", yn[i+1])
        # if (yn[i-1]< yn[i] > yn[i+1] or yn[i-1]> yn[i] < yn[i+1]):
        #     dy[i] = 0
        # else:
        dy[i] = (yn[i] - yn[i-1]) / (tn[i] - tn[i-1]) + (yn[i+1] - yn[i])/(tn[i+1]-tn[i])
    dy /= 2
    # print(dy)
    return dy

def calc_spline2D(yn, tn, dy, T, Kn, t, dt): #построение сплайна
    print()
    ind = 0
    N = yn.shape[0]
    P = np.zeros(N - 1)
    D = np.zeros(N - 1)
    PointCnt = Kn.sum()
    Y = np.zeros(PointCnt + 1)
    for i in range(N-1):
        L = 1.0*yn[i+1] - 1.0*yn[i] - 0.5*T[i]*(dy[i+1]+dy[i])
        # print("i=", i, ", T[i]=", T[i], "yn[i]=", yn[i], ", yn[i+1]=", yn[i + 1])
        P[i] = (-L - np.sign(L)*np.sqrt(L*L + 0.25*T[i]*T[i]*pow(dy[i+1]-dy[i], 2))) / (0.5*T[i]*T[i])
        # if L == 0:
        #     P[i] = 0
        if P[i] == 0 or L == 0:
            D[i] = 0
        else:
            D[i] = (dy[i+1]-dy[i]+T[i]*P[i]) / (2*P[i])
        # if D[i] < dt[i]:
        #     D[i] = 0
        #[i] = tn[i + 1] - D[i]
        #print("i=", i, ", tp2[i]=", tp2[i])
        tp = tn[i + 1] - D[i]
        # print("tp=", tp, ", P[", i, "]=", P[i], ", dt=", dt[i], ", L=", L, ", D[i]=", D[i], ", dy[i]=", dy[i], ", dy[i+1]=", dy[i + 1])
        # print("i=", i, ", tp=", tp, ", yn[i]=", yn[i], ", dy[i]=", dy[i], ", P=", P[i])
        for j in range((Kn[i])):
            # ind = i*(Kn[i-1]) + j
            if ind >= PointCnt:
                break
            if j == 0:
                Y[ind] = yn[i]
                # print("ind=", ind, ", t[ind]=", t[ind], ", Y=", Y[ind], ", first=", dy[i] * t[ind],", second=", (t[ind] * 1.0 * t[ind] * P[i]) / 2.0)
                ind = ind + 1
                continue
            # if j == (Kn[i]-1):
            #     Y[ind] = yn[i+1]
            #     # print("ind=", ind, ", t[ind]=", t[ind], ", Y=", Y[ind], ", first=", dy[i] * t[ind],", second=", (t[ind] * 1.0 * t[ind] * P[i]) / 2.0)
            #     ind = ind + 1
            #     continue
            #T[i] = tn[i + 1] - tn[i]
            tL = 1.0 * (t[ind] - tn[i])
            tt = 1.0*(t[ind] - tn[i])
            if t[ind] < tp:
                Y[ind] = yn[i] + dy[i] * tL - (tL * 1.0 * tL * P[i]) / 2.0
                # print("B: j=", j, ", ind=", ind, ", t[ind]=", t[ind], ", tt=", tt, ", Y=", Y[ind], ", first=", dy[i] * tL, ", second=", - (tL * 1.0 * tL * P[i]) / 2.0)
            else:
                tR = T[i] - tt
                if tR < 0:
                   tR = -tR
                Y[ind] = yn[i + 1] - dy[i + 1] * tR + pow(tR, 2) * P[i] / 2.0
                # print("A: j=", j, ", ind=", ind, ", t[ind]=", t[ind], ", tR=", tR, ", Y=", Y[ind], ", first=", - dy[i + 1] * tR, ", second=", pow(tR, 2) * P[i] / 2.0)
            ind = ind + 1
    Y[PointCnt] = yn[N - 1]
    # print("D=", D)
    return Y, D

def print_parametric_spline2D(X, Y, xn, yn, id="111"): #построение графика
    plt.subplot(id)
    plt.plot(X,Y, marker="", markersize="4", c="C")
    plt.plot(xn, yn, "bo", markersize="2")
    #plt.show()

def print_spline2D(t, Y, tn, D, id="111"): #построение графика
    plt.subplot(id)
    plt.plot(t,Y, marker="", markersize="4", c="C")
    for i, v in enumerate(tn):
        plt.axvline(v,  ls="-", c="R")
        # plt.plot(xn, yn, "bo", markersize="2")
        if i == 0:
            continue
        plt.axvline(tn[i] - D[i-1], ls="--", c="B")
    #plt.show()

tn, xn, yn, Kn = read_data("in.txt")
print("xn=", xn)
print("yn=", yn)
N = yn.shape[0]

#привязка К к длине участка сплайна (теорема Пифагора)
for i in range(N - 1):
    k = 5  # количество точек на единицу длины участка
    Kn[i] = round(mt.sqrt(pow((xn[i+1]-xn[i]), 2)+pow((yn[i+1]-yn[i]), 2))*k)
    if Kn[i] == 0:
        Kn[i]=1
print("Kn=", Kn)
dx = np.zeros(N)
dy = np.zeros(N)

dx = calc_dy(xn, tn)
dy = calc_dy(yn, tn)
# dx[0] = 1
# dx[N-1] = 1

dy[0]=input("Ender y'(x) in first point:\n")
dy[N-1]=input("Ender y'(x) in last point:\n")
dx[0] = (xn[1]-xn[0])/(tn[1]-tn[0])
dx[N-1] = (xn[N-1]-xn[N-2])/(tn[N-1]-tn[N-2])
dy[0] = dy[0]*dx[0]
dy[N-1] = dy[N-1]*dx[N-1]
print("dx=", dx)
print("dy=", dy)

#параметры сплайнов
T = np.zeros(N - 1)
dt = np.zeros(N-1, float)
P = np.zeros(N - 1)
D = np.zeros(N - 1)

T, dt = steps(tn)
t = get_t(dt, Kn)
# print("X")
X, D = calc_spline2D(xn, tn, dx, T, Kn, t, dt)
# print("Y")
Y, D2 = calc_spline2D(yn, tn, dy, T, Kn, t, dt)
write_data("out.txt", t, X, Y)
plt.figure(1)
print_spline2D(t, X, tn, D, id="311")
print_spline2D(t, Y, tn, D2, id="312")
print_parametric_spline2D(X, Y, xn, yn, id="313")
plt.show()


