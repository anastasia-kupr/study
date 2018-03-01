import numpy as np
import matplotlib.pyplot as plt

tn = np.array([0., 20., 35., 47., 56., 63., 69., 80., 90., 120.])
y = np.array([100., 90., 110., 60., 45., 10., -30., 15., 45., 200.])

#tn = np.array([-3439., -3272., -3234., -2809., -1874., -1300., -676., 320., 1178., 1463.])
#y = np.array([735., -863., 608., 845., 157., -838., -63., 435., -312., 165.])

N = y.shape[0]
K = 30

dy = np.zeros(N)

T = np.zeros(N - 1)
dt = np.zeros(N - 1)
P = np.zeros(N - 1)
D = np.zeros(N - 1)
tp2 = np.zeros(N - 1)

for i in range(N - 1):
    #print("i=", i)
    if i == (N - 1):
        T[i] = tn[N] - tn[N - 1]
    else:
        T[i] = tn[i+1] - tn[i]
    dt[i] = T[i] / K
print("T=", T)
print("dt=", dt)


PointCnt = (N-1)* K
Y = np.zeros(PointCnt + 1)
t = np.zeros(PointCnt + 1)
Y[PointCnt] = y[N-1]
t[PointCnt] = tn[N-1]
for i in range(N - 1):
    for j in range(K):
        ind = i*K + j
        if j == 0:
            t[ind] = tn[i]
        else:
            if j == K and i == (N - 1):
                continue
            else:
                t[ind] = t[ind - 1] + dt[i]
        #print("i=", i, ", j=", j, ", t[ind]=", t[ind])
for i in range(1, N-1):
    if (y[i-1]< y[i] > y[i+1] or y[i-1]> y[i] < y[i+1]):
        dy[i] = 0
    else:
        dy[i] = (y[i] - y[i-1]) / (tn[i] - tn[i-1]) + (y[i+1] - y[i])/(tn[i+1]-tn[i])
dy /= 2

for i in range(N-1):
    L = y[i+1] - y[i] - 0.5*T[i]*(dy[i+1]+dy[i])
    P[i] = (-L - np.sign(L)*np.sqrt(L*L + 0.25*T[i]*T[i]*pow(dy[i+1]-dy[i], 2))) / (0.5*T[i]*T[i])
    if P[i] == 0 or L == 0:
        D[i] = 0
    else:
        D[i] = (dy[i+1]-dy[i]+T[i]*P[i]) / (2*P[i])
    tp2[i] = tn[i + 1] - D[i]
    #print("i=", i, ", tp2[i]=", tp2[i])

print("P=", P)
for i in range(N-1):
    tp = tn[i+1] - D[i]
    print("i=", i, ", tp=", tp, ", y[i]=", y[i], ", dy[i]=", dy[i], ", P=", P[i])
    for j in range(K):
        ind = i*K + j
        if j == 0:
            Y[ind] = y[i]
            print("ind=", ind, ", t[ind]=", t[ind], ", Y=", Y[ind], ", first=", dy[i] * t[ind],
                  ", second=", (t[ind] * 1.0 * t[ind] * P[i]) / 2.0)
            continue
        tk = 1.0*(t[ind] - tn[i])
        if t[ind] < tp:
            Y[ind] = y[i] + dy[i] * tk - (tk*1.0*tk*P[i]) / 2.0
            print("B: j=", j, ", ind=", ind, ", t[ind]=", t[ind], ", Y=", Y[ind], ", first=", dy[i] * t[ind], ", second=", (t[ind]*1.0*t[ind]*P[i]) / 2.0)
        else:
            dt2 = T[i]-tk
            if dt2 < 0:
                dt2 = -dt2
            Y[ind] = y[i+1] - dy[i+1] * dt2 + pow(dt2, 2) * P[i] / 2.0
            print("A: j=", j, ", ind=", ind, ", dt2=", dt2, ", Y=", Y[ind], ", first=", (- dy[i+1] * dt2), ", second=", pow(dt2, 2) * P[i] / 2.0)
print("T", T)
print("dy", dy)


plt.plot(t,Y, marker="", markersize="4", c="C")
for i, v in enumerate(tn):
    plt.axvline(v, c="M")
    if i == 0:
        continue
    plt.axvline(tn[i] - D[i-1], ls="--", c="R")
plt.show()

