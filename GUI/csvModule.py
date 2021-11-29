import csv
import guiListbox
import tkinter as tk
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


def importFromCSV(listBox, path):
    with open (path, 'r') as csvFile:
        reader = csv.reader(csvFile)
        next(reader) # Skips header
        for row in reader:
            listBox.insert(tk.END, '{0:^15}  {1:^15}  {2:^15}  {3:^15}'.format(row[0], row[1], row[2], row[3]))
