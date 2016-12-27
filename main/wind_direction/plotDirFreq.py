import os
import math as m
import numpy as np
import matplotlib.pyplot as plt
from scipy import special

PATH_DATA = "../../data/2013/parse/direction_freq_data_13_SEP.csv"

bisDirection = []

RAW_DATA = []
LEN_DATA = 0
MIXTURE_AMOUNT = 7
LEN_SOL = MIXTURE_AMOUNT * 3
NUMBER_OF_CLASSES = 14

def loadData():
  with open(PATH_DATA, "r") as f:
    text = f.read()
    text = text.split("\n")
    LEN_DATA = len(text)
    for i in range(0, LEN_DATA):
      RAW_DATA.append(float(text[i]))

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
  #precision = 0.017453292
  suma = 0.0
  lenClass = 2.0 * m.pi/NUMBER_OF_CLASSES
  precision = lenClass / 8.0

  fromClass = kClass * lenClass
  toClass = (kClass + 1) * lenClass

  angle = fromClass
  finalAngle = toClass
  
  return mixtureVonMises(kClass, solution)

def initBins():
    global bisDirection
    for i in range(0, 29):
      bisDirection.append(0.07 * i) #each 12.6 degrees app
       

#plot
initBins()
loadData()

SOLUTION_INITIAL = [ 
 0.12247 ,0.24709 ,0.37416 ,0.60260 ,0.66171 ,0.66666 ,0.73865,
 0.00966 ,0.00932 ,0.01017 ,0.00957 ,0.00921 ,0.00918 ,0.01023,
 0.10000 ,0.19500 ,0.09000 ,0.07000 ,0.46000 ,0.06500 ,0.02000 
]

SOLUTION_FINAL = [
 0.12328, 0.24257, 0.40334, 0.64167, 0.67817, 0.66840, 0.74075,
 0.03057, 0.03476, 0.00736, 0.00000, 0.03639, 0.00000, 0.00925,
 0.11645, 0.19657, 0.10425, 0.08809, 0.42070, 0.05815, 0.01579
]



plotData = np.arange(0.0, 2.0 * m.pi + 0.1, (2.0 * m.pi)/(NUMBER_OF_CLASSES*16))
resultData = [probabilityWindDirection(SOLUTION_INITIAL, y) for y in plotData]

resultDataF = [probabilityWindDirection(SOLUTION_FINAL, y) for y in plotData]


#plt.plot(plotData, resultData, color="red", label=r"$Soluci\'on\ inicial$", linewidth="2")
#plt.plot(plotData, resultDataF, color="blue", label=r"$Soluci\'on final$", linewidth="2")

#plt.hist(RAW_DATA, NUMBER_OF_CLASSES, normed=True, facecolor='green', alpha=0.5, align = 'mid')

#plt.legend(bbox_to_anchor=(0.6, 0.8), loc=2, borderaxespad=0)
#plt.xlabel(r"""$Direcci\'on$""")
#plt.ylabel(r"""$Densidad$""")
#plt.title(r"""$Direcci\'on\ del\ viento\ Enero 2013\ Valpara\'\i{}so$""")
#plt.show()

ax = plt.subplot(111, projection='polar')
#plotData = [(radian * 180.0)/m.pi for radian in plotData]
ax.plot(plotData, resultDataF, color='b', linewidth=3)
#ax.set_rmax(2.0)
ax.grid(True)
ax.set_title(r"""$Direcci\'on\ del\ viento\ Septiembre\ 2013$""");
plt.show()