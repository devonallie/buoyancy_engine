import csv
import guiListbox
import os

def exportToCSV(listbox, filename, path):
    filename = filename + '.csv'
    fullPath = os.path.join(path, filename)

    with open (fullPath, 'w', newline='') as csvFile:
        writer = csv.writer(csvFile)

        writer.writerow(['Date', 'Time', 'Depth', 'epochtime'])
        size = listbox.size()
        calendarList = []

        for i in range(0,size):
            currentIndex = guiListbox.splitIndex(listbox.get(i))
            writer.writerow([currentIndex[0], currentIndex[1], currentIndex[2], currentIndex[3]])
        
        writer.close()

