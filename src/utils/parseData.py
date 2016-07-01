import os
import math as m

dir_path = "../../data/2015/csv/"
dir_path_result = "../../data/2015/parse/"
TD_file_name = "3D_data_15.csv"
freq_file_name = "freq_data_15.csv"

year_mtx = [[],[],[],[],[],[],[],[],[],[],[],[]]
freq_vel = []

def is_number(s):
    try:
        int(s)
        return True
    except ValueError:
        return False

def getData(text, month):
    d_hours = [[],[],[],[],[],[],[],[]] #data per 3 hours, 3, 6 ...
    m_hours = [] #mean per 3 hours
    index_h = 0
    for line in text[5:]:   #for each line
        line = line.split(",")
        if is_number(line[1]):  #if more data
            dailyMean = 0
            for i in range(3, 19, 2): #append data per hour

                vel = 0.5 * float(line[i]) #from knots to m/s

                d_hours[index_h].append(vel)
                dailyMean += vel
                #addDataFreqList(vel)
                index_h += 1
            index_h = 0
            addDataFreqList(round(dailyMean / 8.0))
        else:       #final process
            for data in d_hours:
                mean = round(sum(data) / float(len(data)))
                m_hours.append(mean)
                #addDataFreqList(mean) #opt 1
                #for d in data:
                    #addDataFreqList(d) #opt2
            year_mtx[month - 1] = m_hours
            return

def addDataFreqList(vel):
    for data in freq_vel:
        if (data[0] == vel):
            data[1] += 1
            return
    #Add vel if no exist    
    freq_vel.append([vel, 1])


def saveYearMatrix():
    with open(dir_path_result + TD_file_name, "w") as f:
        for data in year_mtx:
            data = [str(d) for d in data]
            line = ','.join(data)
            f.write(line + '\n')

def sortFreqYear():
    freq_vel_sort = []
    global freq_vel
    lenList = len(freq_vel)
    for i in range(lenList - 1, -1, -1):
        candidate = freq_vel[i]
        for f in freq_vel:
            if candidate[0] > f[0]:
                print candidate
                candidate = f
        freq_vel_sort.append([candidate[0], candidate[1]])
        freq_vel.remove(candidate)
    freq_vel = freq_vel_sort



def saveFreqYear():
     with open(dir_path_result + freq_file_name, "w") as f:
        total = 0.0 
        sortFreqYear()
        for data in freq_vel:
            total += data[1]
        f.write(str(total) + '\n')
        for data in freq_vel:
            data[1] = round(data[1] / total, 3)   
        for data in freq_vel:
            data = [str(d) for d in data]
            line = ','.join(data)
            f.write(line + '\n')

for file_name in os.listdir(dir_path):
    with open(dir_path + file_name, "r") as f:
        print file_name
        text = f.read()
        text = text.split("\n")
        month = int(file_name.split("_")[0])
        getData(text, month)

saveYearMatrix()
saveFreqYear()

