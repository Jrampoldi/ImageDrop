import tkinter as tk
from tkinter import filedialog


class MyGUI:
    #global const width, height
    WIDTH, HEIGHT = 1920, 1080

    def __init__(self):

        #init root 
        self.root = tk.Tk()
        self.root.configure(bg="#FEBAAD")
    
        self.root.geometry(f"{self.WIDTH}x{self.HEIGHT}")
        self.root.title("ImageDrop")
        
        #init frame for drag n drop
        self.drag_drop_frame = tk.Frame(self.root, borderwidth=0, highlightthickness=0)
        self.drag_drop_frame.pack(padx=10, pady=10)

        self.lbl1 = tk.Label(self.drag_drop_frame, text="Drag your photo here",width=480 ,height=720, bg="#535C63")
        self.lbl1.pack(padx=10,pady=10) 
        
        self.lbl1.bind("<Enter>", self.when_enter)
        self.lbl1.bind("<Leave>",self.when_leave)
        self.lbl1.bind("<ButtonRelease-1>", self.when_drop)

        #run mainloop
        self.root.mainloop()

    def when_enter(self, event):
        self.lbl1.config(bg="#67727B")
    def when_leave(self, event):
        self.lbl1.config(bg="#535C63")
    def when_drop(self,event):
        file_path = event.data

        print(f"{file_path}")


#init gui
def run_gui():
    MyGUI()
