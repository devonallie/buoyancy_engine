import tkinter as tk
import datetime

from tkcalendar.calendar_ import Calendar

def createListBox(self):
    listBox = tk.Listbox(master = self.calendarFrame,
                         height = 5,
                         width = 20)
    listBox.grid(row = 1,
                 column = 0,
                 columnspan = 4,
                 padx = 5,
                 pady = 5,
                 sticky = 'NESW')

    return listBox


def addEntry(listBox, date, time, depth):
    depthFailed = -1
    timeFailed = -2
    passed = 1
    epochdate = 0
    if ((type(depth) is not str) and (depth <= 800)):
        # Depth is good
        try:
            date = datetime.datetime.strftime(date, '%Y-%m-%d')
            epochdate = datetime.datetime.strptime(date + 'T' + time, '%Y-%m-%dT%H:%M:%S').timestamp()
            listBox.insert(tk.END, '{0:^15}  {1:^15}  {2:^15}  {3:^15}'.format(date, time, depth, epochdate))
            return passed, epochdate
        except:
            return timeFailed, epochdate
    else:
        return depthFailed, epochdate


def removeEntry(listBox):
    listBox.delete(tk.ANCHOR)


def getFullSchedule(listbox):
    size = listbox.size()
    calendarList = []

    for i in range(0,size):
        currentIndex = listbox.get(i).split(' ')
        while '' in currentIndex:
            currentIndex.remove('')
        calendarList.append(CalendarEntries(depth = currentIndex[2], time = currentIndex[3]))
        calendarList[i].printEntry()


class CalendarEntries():
    def __init__(self, depth, time):
        self.depth = depth
        self.time = time

    
    def printEntry(self):
        print('Time = {}'.format(self.time))
        print('Depth = {}'.format(self.depth))