import os
import math as m
import numpy as np
import matplotlib.pyplot as plt

PATH_DATA = "../../data/2015/parse/direction_freq_data_15.csv"

bisDirection = []

def initBins():
    global bisDirection
    for i in range(0, 29):
        bisDirection.append(0.07 * i) #each 12.6 degrees app

with open(PATH_DATA, "r") as f:
    text = f.read()
    text = text.split("\n")
    allData = []
    for i in range(0, len(text)):
        allData.append(float(text[i])/m.pi)
       

#plot
initBins()

weights = [100.0/len(allData) for i in range(0, len(allData))]
plt.hist(allData, 28, align = 'mid',  weights = weights, histtype='bar', rwidth=1)

#plt.axis([0, 15, 0, 50])
plt.legend(bbox_to_anchor=(0.6, 0.8), loc=2, borderaxespad=0)
plt.xlabel("Direccion [rad]")
plt.ylabel("Frecuencia (%)")
plt.title(r"""$Direcci\acute{\o}n\ del\ viento\ 2015\ Valpara\acute{\imath}so$""")
plt.show()