import os
import math as m
import numpy as np
import matplotlib.pyplot as plt

#Normal
#k = 1.81
#c = 4.66

#k = 1.75
#c = 4.99

k = 2.59
c = 6.05

def weibull(v):
    factor1 = k/c
    factor2 = m.pow(v/c, k-1)
    factor3 = m.exp(-m.pow(v/c, k))
    return factor1 * factor2 * factor3


with open("../../data/2015/parse/freq_data_15.csv", "r") as f:
    text = f.read()
    text = text.split("\n")
    parValue = []
    dataFreq = []
    dataWeibull = []
    for i in range(0, len(text)):
        parValue = text[i].split(",")
        if len(parValue) != 2:
            continue
        
        parValue[0] = float(parValue[0]) 
        parValue[1] = float(parValue[1])
        dataFreq.append(parValue)
        vel = parValue[0]
        print vel, weibull(vel), parValue[1]

        parValue = [vel, weibull(vel)]
        
        dataWeibull.append(parValue)
        parValue = []

#plot
x = []
for data in dataFreq:
    x = x + [data[0] for i in range(0, int(data[1] * 100))]

num_bins = 50
#n, bins, patches = plt.hist(x, num_bins, normed = 1, facecolor='green', alpha=0.5)
axisX1 = [data[0] for data in dataWeibull]
axisX2 = [data[0] for data in dataFreq]
axisY1 = [data[1] for data in dataWeibull]
axisY2 = [data[1] for data in dataFreq]
plt.plot(axisX1, axisY1, 'ro', axisX2, axisY2, 'g^')
plt.axis([0, 20, 0, 1])
plt.show()

#error
suma = 0
for i in range(0, len(axisY1)):
    suma = m.pow(axisY1[i] - axisY2[i], 2)
print m.sqrt(suma / i) 



