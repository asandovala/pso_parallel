import os
import math as m

PATH_RES = "../../data/2015/csv/"
PATH_OUT = "../../data/2015/parse/"
NAME_OUT = "3D_data_15.csv"

yearMeanDataPerHour = [[],[],[],[],[],[],[],[],[],[],[],[]]

def isNumber(s):
  try:
    int(s)
    return True
  except ValueError:
    return False

def getData(text, month):
  dataPerHour = [[],[],[],[],[],[],[],[]] #data per 3 hours, 3, 6 ...
  meanDataPerHour = [] #mean per 3 hours
  for line in text[5:]:   #for each line
    line = line.split(",")
    if isNumber(line[1]):  #if more data
      speeds = [0.5 * float(line[i]) for i in range(3,19,2)]
      for i in range(0, len(dataPerHour)):
        dataPerHour[i].append(speeds[i])
    else:       #No more data
      for data in dataPerHour:
        mean = round(sum(data) / float(len(data)))
        meanDataPerHour.append(mean)
      yearMeanDataPerHour[month - 1] = meanDataPerHour
      return

def saveYearData():
  with open(PATH_OUT + NAME_OUT, "w") as f:
    for data in yearMeanDataPerHour:
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

#saveYearData()
