import tkinter as tk

def createListBox(self):
    listBox = tk.Listbox(master = self.root,
                         height = 5,
                         width = 20)
    listBox.grid(row = 4,
                 column = 0,
                 columnspan = 3,
                 padx = 5,
                 pady = 5,
                 sticky = 'NESW')

    return listBox

def addEntry(listBox, date, time, depth):
    #print(listBox.curseselection())
    listBox.insert(tk.END, ' {0:^25}  {1:^25}  {2:^25}'.format(date, time, depth))