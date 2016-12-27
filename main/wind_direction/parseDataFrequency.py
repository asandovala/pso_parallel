import os
import math as m

PATH_RES = "../../data/2013/csv/"
PATH_OUT = "../../data/2013/parse/"
NAME_OUT = "direction_freq_data_13_SEP.csv"

allDirections = []

def getRelativeFreq():
  dic = {}
  total = 0.0
  for direction in allDirections:
    if direction in dic:
      dic[direction] += 1
      total += 1.0
    else:
      dic[direction] = 0 

  for key in dic:
    dic[key] = dic[key]/total     
  print dic    

def isNumber(s):
  try:
    int(s)
    return True
  except ValueError:
    return False

def getData(text):
  for line in text[5:]:   #for each line
    line = line.split(",")
    if isNumber(line[1]):  #if more data
      for i in range(2,18,2):
      	if (float(line[i + 1]) > 0):
      	 allDirections.append( m.pi * (float(line[i])) * 0.0055555 )   
    else:  #No more data
      return

def saveFreqYear():
  with open(PATH_OUT + NAME_OUT, "w") as f:
    global allDirections 
    allDirections = sorted(allDirections)

    for i in range(0, len(allDirections)):
      data = allDirections[i]		
      line = str(data)
      if i < len(allDirections) - 1: 
      	f.write(line + '\n')
      else: 
      	f.write(line)	

fileName = "09_SEPTIEMBRE_2013.csv"
#for fileName in os.listdir(PATH_RES):
with open(PATH_RES + fileName, "r") as f:
  print fileName

  text = f.read()
  text = text.split("\n")
  getData(text)

saveFreqYear()

#getRelativeFreq()