from __future__ import division
import numpy as np

f = open('text.txt', 'r')
string =f.read()
string = string
dict_simple = {}
dict_mark = {}

length = len(string)

for sym in string:
    if dict_simple.get(sym) is None:
        dict_simple[sym] = 1
    else:
        dict_simple[sym] = dict_simple[sym] + 1

#for k, v in dict_simple.items():
#    print(k, v)

entropy1 = 0
for k, v in dict_simple.items():
    entropy1 -= (v / length) * (np.log(v / length) / np.log(2))

#hi = 1 - entropy1 / (np.log(9) / np.log(2))

print("entropy1")
print(entropy1)
#print("hi")
#print(hi)

for i, sym in enumerate(string):
    if i == 0:
        prev = string[-1]
    else:
        prev = string[i - 1]
    if dict_mark.get((prev, sym)) is None:
        dict_mark[(prev, sym)] = 1
    else:
        dict_mark[(prev, sym)] = dict_mark[(prev, sym)] + 1

#for k, v in dict_mark.items():
#    print(k, v)

dict3 = {}
for k, v in dict_mark.items():
    if dict3.get(k[0]) is None:
        dict3[k[0]] = 1
    else:
        dict3[k[0]] = dict3[k[0]] + 1

temp = 0
entropy2 = 0

for k1, v1 in dict3.items():
    for k, v in dict_mark.items():
        if k[0] == k1:
            temp -= (v / v1) * (np.log(v / v1) / np.log(2))
        entropy2 += dict_simple[k1] * temp / length
        temp = 0


print("entropy2")
print(entropy2)

f1 = open("result.txt", 'w')

f1.write("entropy1\n")
f1.write(str(entropy1))
f1.write("\n")
f1.write(str(entropy2))