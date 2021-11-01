import tkinter as tk

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
    listBox.insert(tk.END, '{0:^15}  {1:^15}    {2:^15}'.format(date, time, depth))


def removeEntry(listBox):
    listBox.delete(tk.ANCHOR)
