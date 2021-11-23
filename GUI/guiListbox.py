import tkinter as tk
import datetime

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
    if ((type(depth) is not str) and (depth <= 800)):
        # Depth is good
        try:
            date = datetime.datetime.strftime(date, '%Y-%m-%d')
            epochdate = datetime.datetime.strptime(date + 'T' + time, '%Y-%m-%dT%H:%M:%S')
            listBox.insert(tk.END, '{0:^15}  {1:^15}  {2:^15}'.format(date, time, depth))
            return passed
        except:
            return timeFailed
    else:
        return depthFailed    


def removeEntry(listBox):
    listBox.delete(tk.ANCHOR)
