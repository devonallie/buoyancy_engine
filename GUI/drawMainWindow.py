import tkinter as tk
import tkinter.filedialog as tkfd
from babel.core import Locale
import tkcalendar
import datetime
import guiListbox
import csvModule
import os
import serial
import serial.tools.list_ports as st

class MainGUIWindow:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title('Buoyancy engine GUI')
        self.root.minsize(width = 335,
                          height = 109)
        self.PAD = 5
        self.calendarEntries = []
        self.selectedCOMPort = tk.StringVar()
        comList = st.comports()
        if (len(comList) == 0):
            comList = [0]
        self.selectedCOMPort.set(comList[0])

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
        self.timeTextBox.insert(tk.END, 'HH:MM:SS')
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
                             column = 3,
                             padx = self.PAD,
                             pady = self.PAD)
        self.exportButton = tk.Button(master = self.sendFrame,
                                      command = self.exportToCSVButton,
                                      text = 'Export to CSV')
        self.exportButton.grid(row = 0,
                               column = 1,
                               padx = self.PAD,
                               pady = self.PAD)
        self.importButton = tk.Button(master = self.sendFrame,
                                      command = self.importFromCSVButton,
                                      text = 'Import from CSV')
        self.importButton.grid(row = 0,
                               column = 0,
                               padx = self.PAD,
                               pady = self.PAD)
        
        self.comCombo = tk.OptionMenu(self.sendFrame, comList[0], *comList)
        self.comCombo.grid(row = 0,
                           column = 2,
                           padx = self.PAD,
                           pady = self.PAD)

        self.autoUpdate()

        self.root.mainloop()


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


    def exportToCSVButton(self):
        self.exportWindow = tk.Toplevel(self.root)
        self.pathLabelVar = tk.StringVar()
        self.pathLabelVar.set('Select a Path')
        self.exportWindow.title('Export to CSV')
        exportLabel = tk.Label(master = self.exportWindow,
                               text = 'Filename')
        exportLabel.grid(row = 0,
                         column = 0,
                         padx = self.PAD,
                         pady = self.PAD,
                         sticky = 'NW')
        self.exportNameTextbox = tk.Text(master = self.exportWindow,
                                    height = 1,
                                    width = 20)
        self.exportNameTextbox.insert(tk.END, 'schedule')
        self.exportNameTextbox.grid(row = 1,
                               column = 0,
                               padx = self.PAD,
                               pady = self.PAD,
                               sticky = 'NW')
        csvLabel = tk.Label(master = self.exportWindow,
                            text = '.csv')
        csvLabel.grid(row = 1,
                      column = 1)
        exportPathLabel = tk.Label(master = self.exportWindow,
                             textvariable = self.pathLabelVar)
        exportPathLabel.grid(row = 2,
                             column = 0,
                             padx = self.PAD,
                             pady = self.PAD)
        pathButton = tk.Button(master = self.exportWindow,
                               command = self.getExportPath,
                               text = 'select Path')
        pathButton.grid(row = 2,
                        column = 1,
                        padx = self.PAD,
                        pady = self.PAD)
        okButton = tk.Button(master = self.exportWindow,
                             text = 'Okay',
                             command = self.exportOkButton)
        okButton.grid(row = 3,
                      column = 1,
                      padx = self.PAD,
                      pady = self.PAD)


    def getExportPath(self):
        path = tkfd.askdirectory()
        self.pathLabelVar.set(path)
        self.exportWindow.lift()


    def exportOkButton(self):
        filename = self.exportNameTextbox.get('1.0', tk.END)
        filename = filename.replace('.csv', '')[:-1]
        path = self.pathLabelVar.get()
        if (os.path.isdir(path)):
            csvModule.exportToCSV(self.calendarListBox, filename, path)    
            self.exportWindow.destroy()
        else:
            self.pathLabelVar.set('Invalid path selected')
        

    def importFromCSVButton(self):
        self.importWindow = tk.Toplevel(self.root)
        self.importWindow.title('Import from CSV')
        self.csvPathVar = tk.StringVar()
        self.csvPathVar.set('Select a csv')
        self.importLabel = tk.Label(master = self.importWindow,
                                    textvariable = self.csvPathVar)
        self.importLabel.grid(row = 0, 
                              column = 0,
                              padx = self.PAD,
                              pady = self.PAD,
                              sticky = 'NW')
        importButton = tk.Button(master = self.importWindow,
                                 text = 'Select a file',
                                 command = self.selectImportFile)
        importButton.grid(row = 0,
                          column = 1,
                          padx = self.PAD,
                          pady = self.PAD,
                          sticky = 'NW')


    def selectImportFile(self):
        path = tkfd.askopenfilename(defaultextension = '.csv')
        csvModule.importFromCSV(self.calendarListBox, path)
        self.importWindow.destroy()
        
