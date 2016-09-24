import os
import math as m
import numpy as np
import matplotlib.pyplot as plt
from scipy import special

PATH_DATA = "../../data/2015/parse/direction_freq_data_15.csv"

bisDirection = []

RAW_DATA = []
LEN_DATA = 0
MIXTURE_AMOUNT = 18
LEN_SOL = MIXTURE_AMOUNT * 3
NUMBER_OF_CLASSES = 36

def loadData():
  with open(PATH_DATA, "r") as f:
    text = f.read()
    text = text.split("\n")
    LEN_DATA = len(text)
    for i in range(0, LEN_DATA):
      RAW_DATA.append(float(text[i]) / m.pi)

def vonMises(angle, u, k):
  u = u * 2.0 * m.pi
  k = k * 700.0
  return m.exp(k * m.cos(angle - u)) / (2 * m.pi * special.iv(0, k))

def mixtureVonMises(angle, solution):
  suma = 0.0
  for i in range(0, MIXTURE_AMOUNT): 
    u = solution[i]
    k = solution[i + MIXTURE_AMOUNT]
    w = solution[i + MIXTURE_AMOUNT * 2]
    suma += w * vonMises(angle, u, k)
  return suma

def probabilityWindDirection(solution, kClass): 
  precision = 0.017453292
  suma = 0.0
  lenClass = 2.0 * m.pi/NUMBER_OF_CLASSES

  fromClass = kClass * lenClass
  toClass = (kClass + 1) * lenClass

  angle = fromClass
  finalAngle = toClass

  while (angle < finalAngle): #emulate integral
    suma += mixtureVonMises(angle, solution) * precision
    angle += precision

  print "Prob [", fromClass, " - ", toClass, "] ", suma
  return suma

def initBins():
    global bisDirection
    for i in range(0, 29):
      bisDirection.append(0.07 * i) #each 12.6 degrees app
       

#plot
initBins()
loadData()

#Ver que onda...
#SOLUTION = [0.18310, 0.50977, 0.74453, 0.01143, 0.00857, 0.01143, 0.28434, 0.44048, 0.27518] 
#SOLUTION = [ 0.11221,0.35278,0.50727,0.62457,0.82497,0.00857,0.01000,0.00857,0.00857,0.01143,0.17639,0.12386,0.29783,0.24434,0.15759]
#SOLUTION = [0.05867, 0.00000, 0.49037, 0.00000, 0.46197, 0.48005, 0.58293, 0.24312, 0.10140 ]
SOLUTION = [ 
 1.00000, 0.07835, 1.00000, 1.00000, 0.25776, 0.99448, 
 0.72637, 0.17980, 0.81614, 0.74958, 0.32798, 1.00000, 
 0.55520, 0.68003, 0.64742, 0.86550, 0.49114, 0.69502, 
 0.00000, 0.56998, 1.00000, 0.45615, 1.00000, 0.62671, 
 0.14434, 0.88237, 0.31529, 0.00883, 1.00000, 0.60367, 
 0.68400, 0.04866, 1.00000, 1.00000, 0.24795, 0.68530, 
 0.69929, 0.00000, 0.00007, 0.00000, 0.00000, 0.00000, 
 0.00000, 0.00000, 0.00000, 0.00000, 0.00808, 0.00000, 
 0.00000, 0.00000, 0.08005, 0.00000, 0.21252, 0.00000,

]


plotData = np.arange(0.0, 2.0, 2.0/NUMBER_OF_CLASSES) #Plot more smooth
resultData = [probabilityWindDirection(SOLUTION, y) for y in range(0, NUMBER_OF_CLASSES)]

totalResultData = sum(resultData) #Adjust weigth of values.
resultData = [resultData[y] * (100.0/totalResultData) for y in range(0, len(resultData))]

plt.plot(plotData, resultData, color="red", label="PSO")

weights = [100.0/len(RAW_DATA) for i in range(0, len(RAW_DATA))]
plt.hist(RAW_DATA, NUMBER_OF_CLASSES, align = 'mid', weights = weights, histtype='bar', rwidth=1)

#plt.axis([0, 15, 0, 50])
plt.legend(bbox_to_anchor=(0.6, 0.8), loc=2, borderaxespad=0)
plt.xlabel("Direccion [rad]")
plt.ylabel("Frecuencia (%)")
plt.title(r"""$Direcci\acute{\o}n\ del\ viento\ 2015\ Valpara\acute{\imath}so$""")
plt.show()