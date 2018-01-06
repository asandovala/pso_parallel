import os
import math as m

PATH_RES = "../../../data/2015/csv_oct-dic/"
PATH_OUT = "../../../data/2015/parse/"
NAME_OUT = "freq_data_15_oct-dic.csv"

frequencySpeed = []

def isNumber(s):
  try:
    int(s)
    return True
  except ValueError:
    return False

def getData(text, month):
  for line in text[5:]:   #for each line
    line = line.split(",")
    if isNumber(line[1]):  #if more data
      speeds = [0.5 * float(line[i]) for i in range(3,19,2)]
      addDataFreqList(round(sum(speeds) / len(speeds))) #Save the mean of the day.
      #for s in speeds:
      #  addDataFreqList(s)
    else:  #No more data
      return

def addDataFreqList(speed):
  for data in frequencySpeed: #data is a array of [speed,frequency]
    if (data[0] == speed):
      data[1] += 1
      return
  frequencySpeed.append([speed, 1]) #Add vel if no exist

def sortFreqYear():
  global frequencySpeed
  sortedFrequencySpeeds = []

  for i in range(len(frequencySpeed) - 1, -1, -1):
    candidate = frequencySpeed[i]
    for f in frequencySpeed:
      if candidate[0] > f[0]:
        candidate = f
    sortedFrequencySpeeds.append([candidate[0], candidate[1]])
    frequencySpeed.remove(candidate)
  frequencySpeed = sortedFrequencySpeeds

def saveFreqYear():
  with open(PATH_OUT + NAME_OUT, "w") as f:
    total = 0.0
    sortFreqYear()
    for data in frequencySpeed: #get amount of data
      total += data[1]

    for data in frequencySpeed: #get relative frequency
      data[1] = round(data[1] / total, 3)

    for data in frequencySpeed:
      data = [str(d) for d in data]
      line = ','.join(data)
      f.write(line + '\n')

for fileName in os.listdir(PATH_RES):
  with open(PATH_RES + fileName, "r") as f:
    print fileName

    text = f.read()
    text = text.split("\n")
    month = int(fileName.split("_")[0])
    getData(text, month)

saveFreqYear()

