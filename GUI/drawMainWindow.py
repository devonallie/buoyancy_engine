import tkinter as tk
from babel.core import Locale
import tkcalendar
import datetime
import guiListbox

class MainGUIWindow:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title('Buoyancy engine GUI')
        self.root.minsize(width = 335,
                          height = 109)
        self.PAD = 5
        self.calendarEntries = []

        self.root.columnconfigure(index = 0,
                                  weight = 1)
        self.root.rowconfigure(index = 4,
                               weight = 1)

        self.quitButton = tk.Button(master = self.root,
                                    command = self.root.destroy,
                                    text = 'Quit')
        self.quitButton.grid(row = 5,
                             column = 2,
                             padx = self.PAD,
                             pady = self.PAD,
                             sticky = 'SE')

        # timeFrame
        self.timeFrame = tk.Frame(master = self.root,
                                  borderwidth = 1)
        self.timeFrame.grid(row = 1,
                            column = 0,
                            columnspan = 4,
                            rowspan = 3,
                            padx=self.PAD,
                            pady=self.PAD,
                            sticky = 'NW')
        # dateTextBox
        self.dateEntry = tkcalendar.DateEntry(master = self.timeFrame,
                                              date_pattern = 'y-mm-dd')
        #self.dateEntry.set_date(date = datetime.datetime.date)
        self.dateEntry.grid(column = 0,
                            row = 1)
        # dateLabel
        self.dateLabel = tk.Label(master = self.timeFrame,
                                  text = 'Date in ISO format')
        self.dateLabel.grid(row = 0,
                            column = 0,
                            padx = self.PAD,
                            pady = self.PAD)
        # hourBox
        self.timeTextBox = tk.Text(master = self.timeFrame,
                                   width = 12,
                                   height = 1)
        self.timeTextBox.grid(column = 1,
                              row = 1)
        self.timeTextBox.insert(tk.END, 'HH:MM:DD')
        # hourLabel
        self.timeLabel = tk.Label(master = self.timeFrame,
                                  text = 'Time in UTC')
        self.timeLabel.grid(row = 0,
                            column = 1,
                            padx = self.PAD,
                            pady = self.PAD)
        # Depth label
        self.depthLabel = tk.Label(master = self.timeFrame,
                                   text = 'Depth in meters')
        self.depthLabel.grid(row = 0,
                             column = 2,
                             padx = self.PAD,
                             pady = self.PAD)
        # Depth textbox
        self.depthTextBox = tk.Text(master = self.timeFrame,
                                    width = 12,
                                    height = 1)
        self.depthTextBox.grid(row = 1,
                               column = 2,
                               padx = self.PAD,
                               pady = self.PAD)
        # Add button
        self.addButton = tk.Button(master = self.timeFrame,
                                   command = self.addButtonClicked,
                                   text = 'Add Entry')
        self.addButton.grid(row = 3,
                            column = 0,
                            padx = self.PAD,
                            pady = self.PAD,
                            sticky = 'SW')
        # remove button
        self.removeButton = tk.Button(master = self.timeFrame,
                                      command = self.removeButtonClicked,
                                      text = "Remove Entry",
                                      state = tk.DISABLED)
        self.removeButton.grid(row = 3,
                               column = 1,
                               padx = self.PAD,
                               pady = self.PAD,
                               sticky = 'SW')
        # Warn label
        self.warnlabel = tk.Label(master = self.timeFrame)
        self.warnlabel.grid(row = 4,
                            column = 0,
                            columnspan = 3,
                            padx = self.PAD,
                            pady = self.PAD,
                            sticky = 'SW')

        # Calendar Frame
        self.calendarFrame = tk.Frame(master = self.root)
        self.calendarFrame.grid(row = 5,
                                column = 0,
                                columnspan = 4,
                                padx = self.PAD,
                                pady = self.PAD,
                                sticky = 'NESW')
        self.calendarFrame.columnconfigure(index = 0,
                                           weight = 2)
        self.calendarFrame.rowconfigure(index = 1,
                                        weight = 2)
        # Calendar label
        self.calendaLabel = tk.Label(master = self.calendarFrame,
                                     text = 'date              time              depth         time (epoch)',
                                     width = 30,
                                     justify = 'center')
        self.calendaLabel.grid(row = 0,
                               column = 0,
                               columnspan = 3,
                               padx = self.PAD,
                               pady = self.PAD,
                               sticky = 'NESW')
        # Create listbox
        self.calendarListBox = guiListbox.createListBox(self)

        # Send Frame
        self.sendFrame = tk.Frame(master = self.root)
        self.sendFrame.grid(row = 6,
                            column = 0,
                            columnspan = 4,
                            padx = self.PAD,
                            pady = self.PAD,
                            sticky = 'NESW')
        self.sendButton = tk.Button(master = self.sendFrame,
                                    command = self.sendButtonClicked,
                                    text = 'Send Schedule')
        self.sendButton.grid(row = 0,
                             column = 0,
                             padx = self.PAD,
                             pady = self.PAD)

        self.autoUpdate()

        self.root.mainloop()


    def printWindowSize(self):
            print(self.root.winfo_width())
            print(self.root.winfo_height())


    def addButtonClicked(self):
        depthFailed = -1
        timeFailed = -2
        printstr = tk.StringVar()
        date = self.dateEntry.get_date()
        time = self.timeTextBox.get('1.0', tk.END)[:-1]

        try:
            depth = int(self.depthTextBox.get('1.0', tk.END))
        except:
            status = depthFailed
            depth = repr(self.depthTextBox.get('1.0', tk.END))[:-1]

        status, epochtime = guiListbox.addEntry(self.calendarListBox, date, time, depth)
        if (status == depthFailed):
            printstr.set('Incorrect depth ' + str(depth) + ' Depth must be below 800')
            self.warnlabel.configure(textvariable = printstr,
                                     fg = 'red')
        elif (status == timeFailed):
            printstr.set('Incorrect time format {}. Time format: HH:MM:SS'.format(time))
            self.warnlabel.configure(textvariable = printstr,
                                     fg = 'red')
        else:
            printstr.set(' ')
            self.warnlabel.configure(textvariable = printstr)


    def removeButtonClicked(self):
        epochtime = guiListbox.removeEntry(self.calendarListBox)


    def autoUpdate(self):
        # Referenced https://stackoverflow.com/questions/15672552/tkinter-listbox-getactive-method
        if (self.calendarListBox.size() > 0):
            self.removeButton.configure(state = tk.ACTIVE)
        else:
            self.removeButton.configure(state = tk.DISABLED)
        self.root.after(10, self.autoUpdate)


    def sendButtonClicked(self):
        guiListbox.getFullSchedule(self.calendarListBox)