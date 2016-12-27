import os
import math as m
import numpy as np
import matplotlib.pyplot as plt

PATH_DATA = "../../data/2015/parse/freq_data_15_jul-sep.csv"

#2015
#k = 2.65
#c = 3.10

#2014
#k = 2.91
#c = 3.37

#2013
#k = 2.78
#c = 3.12

#2015 - 2S
#k = 2.76
#c = 3.39

#All years
#k = 2.78
#c = 3.20

#2015 All.
#k = 1.00
#c = 9.49

#2015 abr-jun
#k = 2.76
#c = 2.65

#2015 ene-mar
#k = 2.85
#c = 3.15

#2015 jul-sep
#k = 2.66
#c = 2.83

#2015 oct-dic
#k = 3.40
#c = 3.75


def weibull(v):
    factor1 = k/c
    factor2 = m.pow(v/c, k-1)
    factor3 = m.exp(-m.pow(v/c, k))
    return factor1 * factor2 * factor3

def cumulativeWeibull(v):
    factor1 = -m.pow((v/c), k)
    return 1 - m.exp(factor1)

with open(PATH_DATA, "r") as f:
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

#E Method
'''
med = sum([d[0]*d[1] for d in dataFreq])
sd = m.sqrt(sum([d[1] * m.pow((d[0] - med), 2) for d in dataFreq]))         
k = m.pow(sd/med, -1.086)
c = med/(m.gamma(1 + 1/k))

print "med: ", med
print "sd: ", sd
print "k: ", k
print "c: ", c
'''

#plot
vel1 = [data[0] for data in dataFreq]
vel2 = [data[0] for data in dataFreq] 
vel2.sort()

weibullY = [weibull(v) for v in vel2] #To test
freqY = [data[1] for data in dataFreq]

plotData = np.arange(0, 15, 0.1) #Plot more smooth
resultData = [weibull(y) * 100 for y in plotData]
plt.plot(plotData, resultData, color="red", label="PSO")

plt.bar(vel1, [y*100 for y in freqY], 0.6, color="blue", label="Histograma")
plt.axis([0, 15, 0, 50])
plt.legend(bbox_to_anchor=(0.6, 0.8), loc=2, borderaxespad=0)
plt.xlabel("Velocidad [m/s]")
plt.ylabel("Frecuencia (%)")
plt.title(r"""$Velocidad\ del\ viento\ 2015\ -\ Jul-Sep\ Valpara\acute{\imath}so$""")
plt.show()

#RMSE - sample standard deviation
suma = 0
dataLen = len(weibullY)
#print weibullY, freqY
for i in range(0, dataLen):
    suma += m.pow(weibullY[i] - freqY[i], 2)
print "RMSE ", m.sqrt(suma / dataLen)

#correlation
X_mean = sum(weibullY) / len(weibullY)
Y_mean = sum(freqY) / len(freqY)
    
factor1 = 0
factor21 = 0
factor22 = 0
for i in range(0, len(freqY)):
    diffX = abs(weibullY[i] - X_mean)
    diffY = abs(freqY[i] - Y_mean)
    factor1 +=  diffX * diffY 
    factor21 += m.pow(diffX, 2)
    factor22 += m.pow(diffY, 2)
    
R = factor1/(m.sqrt(factor21)*m.sqrt(factor22))
print "Correlation: ", R

#Relative bias
RB = abs(X_mean - Y_mean) / Y_mean
print "Relative Bias: ", RB
