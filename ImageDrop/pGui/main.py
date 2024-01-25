from tkinter import * 
import ctypes as c
import os

def main():
    root = Tk()
    width = 1920
    height = 1080
#    os.chdir('../')

#    test_lib = c.CDLL("./ImageDrop.so")
#    test_lib.main()
    introPage(root, width, height)
     

     root.mainloop()


def introPage(root, width, height):
    
    introMessage = "Welcome to ImageDrop\n Click the button to get started.\n"

    root.title("ImageDrop")
    root.geometry(f"{width}x{height}")

    label1 = Label(root, text = introMessage)
    label1.place(relx=0.5, rely=0.5, anchor="center")    

def buttonCheck(root):
    
if __name__ == "__main__":
    main()
