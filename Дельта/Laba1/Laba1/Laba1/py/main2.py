import numpy as np
import matplotlib.pyplot as plt

tn = np.array([0., 20., 35., 47., 56., 63., 69., 80., 90., 120.])
y = np.array([100., 90., 110., 60., 45., 10., -30., 15., 45., 200.])

tn = np.array([-578.,34.,260.,408.,1071.,1874.,2522.,2874.,2991.,3973.,])
y = np.array([-908.,-505.,-599.,736.,-482.,855.,51.,-422.,571.,888.,])

N = y.shape[0]
K = 5

dy = np.zeros(N)

T = np.zeros(N - 1)
dt = np.zeros(N - 1)
P = np.zeros(N - 1)
D = np.zeros(N - 1)

for i in range(N - 1):
    T[i] = tn[i+1] - tn[i]
    dt[i] = T[i] / K

PointCnt = (N-1)* K
Y = np.zeros(PointCnt)
t = np.zeros(PointCnt)

for i in range(N-1):
    for j in range(K):
        if i == 0 and j == 0:
            t[0] = tn[0]
        else:
            t[i*K + j] = t[i*K + j - 1] + dt[i]

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

for i in range(N-1):
    tp = tn[i+1] - D[i]
    for j in range(K):
        ind = i*K + j
        if t[ind] < tp:
            Y[ind] = y[i] + dy[i] * t[ind] - (t[ind]*t[ind]*P[i]) / 2
        else:
            Y[ind] = y[i+1] - dy[i+1] * (T[i] - t[ind]) + pow(T[i] - t[ind], 2) * P[i] / 2

print("T", T)
print("dy", dy)


plt.plot(t,Y, marker="", markersize="4", c="C")
for i, v in enumerate(tn):
    plt.axvline(v, c="M")
    if i == 0:
        continue
    plt.axvline(tn[i] - D[i-1], ls="--", c="R")
plt.show()