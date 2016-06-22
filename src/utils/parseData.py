import os

dir_path = "../../data/2015/csv/"
dir_path_result = "../../data/2015/parse/"

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
                d_hours[index_h].append(float(line[i]))
                index_h += 1
                dailyMean += float(line[i])
            index_h = 0
            addDataFreqList(round(dailyMean / 8.0))
        else:       #final process
            for data in d_hours:
                mean = round(sum(data) / float(len(data)))
                print "wtf ?: " + str(mean)
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
    with open(dir_path_result + "3D_data_15.csv", "w") as f:
        for data in year_mtx:
            data = [str(d) for d in data]
            line = ','.join(data)
            f.write(line + '\n')

def saveFreqYear():
     with open(dir_path_result + "freq_data_15.csv", "w") as f:
        total = 0.0 
        for data in freq_vel:
            total += data[1]
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

