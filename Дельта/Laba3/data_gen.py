import numpy as np
import pandas as pd

df = pd.read_csv("in_pars.txt", sep=" ", escapechar="#")
print("df.values=", df.values)
r = df.values.shape[0]
f = open("in_data.txt", "w")
flag = 1
if flag == 0:
    for i in range(r):
        for j in range(r):
            f.write(str(np.round(200 * np.random.random_sample() - 100, decimals=2)))
            if j != r - 1:
                f.write(" ")
        f.write("\n")
elif flag == 1:
    for i in range(r):
        for j in range(r):
            f.write(str(np.round(np.cos(np.sqrt(i**2/2 + j**2/2))*100, decimals=2)))
            if j != r - 1:
                f.write(" ")
        f.write("\n")
elif flag == 2:
    for i in range(r):
        for j in range(r):
            f.write(str(i + j))
            if j != r - 1:
                f.write(" ")
        f.write("\n")
f.close()