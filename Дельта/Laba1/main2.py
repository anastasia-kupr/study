import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

def read_data(fname):
    df = pd.read_csv(fname, sep=" ", escapechar="#")
    in_val = df.values
    tn = in_val[:, 0]
    yn = in_val[:, 1]
    Kn = in_val[:, 2]
    Kn = Kn[~np.isnan(Kn)]
    Kn = np.array(Kn, dtype=np.int16)
    return tn, yn, Kn


def write_data(fname, t, Y):
    out_df = pd.DataFrame(
        {"t" : t,
        "y(t)" : Y}
    )
    out_df.to_csv(fname, sep="\t", index=False)
#входные данные
#tn = np.array([0., 20., 35., 47., 56., 63., 69., 80., 90., 120.])
#yn = np.array([100., 90., 110., 60., 45., 10., -30., 15., 45., 200.])
#tn = np.array([-3439., -3272., -3234., -2809., -1874., -1300., -676., 320., 1178., 1463.])
#yn = np.array([735., -863., 608., 845., 157., -838., -63., 435., -312., 165.])
#Kn = np.array([3, 7, 8, 5, 3, 5, 7, 5, 9])
#при одинаковом количестве K участков сплайна
#K = 5
#for i in range(N - 1):
#    Kn[i] = K

tn, yn, Kn = read_data("in.txt")
N = yn.shape[0]
dy = np.zeros(N)

#параметры сплайнов
T = np.zeros(N - 1)
dt = np.zeros(N-1, float)
P = np.zeros(N - 1)
D = np.zeros(N - 1)

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
        if (yn[i-1]< yn[i] > yn[i+1] or yn[i-1]> yn[i] < yn[i+1]):
            dy[i] = 0
        else:
            dy[i] = (yn[i] - yn[i-1]) / (tn[i] - tn[i-1]) + (yn[i+1] - yn[i])/(tn[i+1]-tn[i])
    dy /= 2
    return dy

def calc_spline2D(yn, tn, dy, T, Kn, t, dt): #построение сплайна
    ind = 0
    N = yn.shape[0]
    P = np.zeros(N - 1)
    D = np.zeros(N - 1)
    PointCnt = Kn.sum()
    Y = np.zeros(PointCnt + 1)
    for i in range(N-1):
        L = yn[i+1] - yn[i] - 0.5*T[i]*(dy[i+1]+dy[i])
        P[i] = (-L - np.sign(L)*np.sqrt(L*L + 0.25*T[i]*T[i]*pow(dy[i+1]-dy[i], 2))) / (0.5*T[i]*T[i])
        if P[i] == 0 or L == 0:
            D[i] = 0
        else:
            D[i] = (dy[i+1]-dy[i]+T[i]*P[i]) / (2*P[i])
        #[i] = tn[i + 1] - D[i]
        #print("i=", i, ", tp2[i]=", tp2[i])
        tp = tn[i + 1] - D[i]
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
            tt = 1.0 * (t[ind] - tn[i])
            if t[ind] < tp:
                Y[ind] = yn[i] + dy[i] * tt - (tt * 1.0 * tt * P[i]) / 2.0
                # print("B: j=", j, ", ind=", ind, ", t[ind]=", t[ind], ", Y=", Y[ind], ", first=", dy[i] * t[ind], ", second=", (t[ind]*1.0*t[ind]*P[i]) / 2.0)
            else:
                dt2 = T[i] - tt
                if dt2 < 0:
                    dt2 = -dt2
                Y[ind] = yn[i + 1] - dy[i + 1] * dt2 + pow(dt2, 2) * P[i] / 2.0
                # print("A: j=", j, ", ind=", ind, ", dt2=", dt2, ", Y=", Y[ind], ", first=", (- dy[i+1] * dt2), ", second=", pow(dt2, 2) * P[i] / 2.0)
            ind = ind + 1
    Y[PointCnt] = yn[N - 1]
    return Y, D

def print_spline2D(t, Y, tn, D): #построение графика
    plt.plot(t,Y, marker="", markersize="4", c="C")
    for i, v in enumerate(tn):
        plt.axvline(v,  ls="--", c="R")
        #plt.plot(tn, yn, "bo", markersize="2")
        if i == 0:
            continue
        #plt.plot(tn[i] - D[i-1], Y[i], "bo", markersize="2")
    plt.show()

dy = calc_dy(yn, tn)
T, dt = steps(tn)
t = get_t(dt, Kn)
Y, D = calc_spline2D(yn, tn, dy, T, Kn, t, dt)
write_data("out.txt", t, Y)
print_spline2D(t, Y, tn, D)


