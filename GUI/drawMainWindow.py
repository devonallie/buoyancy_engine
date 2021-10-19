import tkinter as tk

class MainGUIWindow:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title('Buoyancy engine GUI')
        self.root.minsize(width = 335,
                          height = 109)
        self.PAD = 5

        self.root.columnconfigure(index = 0,
                                  weight = 1)
        self.root.rowconfigure(index = 3,
                               weight = 1)

        self.quitButton = tk.Button(master = self.root,
                                    command = self.root.destroy,
                                    text = 'Quit')
        self.quitButton.grid(row = 4,
                             column = 2,
                             padx = self.PAD,
                             pady = self.PAD,
                             sticky = 'SE')

        self.tempButton = tk.Button(self.root, command = self.printWindowSize, text='Temp').grid(row=4, column=0)

        # timeFrame
        self.timeFrame = tk.Frame(master = self.root,
                                  borderwidth = 1)
        self.timeFrame.grid(row = 1,
                            column = 0,
                            columnspan = 3,
                            rowspan = 2,
                            padx=self.PAD,
                            pady=self.PAD,
                            sticky = 'NW')
        # dateTextBox
        self.dateTextBox = tk.Text(master = self.timeFrame,
                                   height = 1,
                                   width = 12)
        self.dateTextBox.grid(column = 0,
                              row = 1)
        self.dateTextBox.insert(tk.END, 'YYYY-MM-DD')
        # dateLabel
        self.dateLabel = tk.Label(master = self.timeFrame,
                                  text = 'Date in ISO format')
        self.dateLabel.grid(row = 0,
                            column = 0,
                            padx = self.PAD,
                            pady = self.PAD)
        # hourBox
        self.hourTextBox = tk.Text(master = self.timeFrame,
                                   width = 12,
                                   height = 1)
        self.hourTextBox.grid(column = 1,
                              row = 1)
        self.hourTextBox.insert(tk.END, 'HH:MM:DD')
        # hourLabel
        self.hourLabel = tk.Label(master = self.timeFrame,
                                  text = 'Time in UTC')
        self.hourLabel.grid(row = 0,
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
        self.depthTextBox = tk.Text(master = self.timeFrame,
                                    width = 12,
                                    height = 1)
        self.depthTextBox.grid(row = 1,
                               column = 2,
                               padx = self.PAD,
                               pady = self.PAD)

        self.root.mainloop()

    def printWindowSize(self):
            print(self.root.winfo_width())
            print(self.root.winfo_height())
