import os
import ctypes
import shutil
from ctypes.util import find_library
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.uix.gridlayout import GridLayout
from kivy.uix.widget import Widget
from kivy.uix.label import Label
from kivy.uix.button import Button
from kivy.graphics import Color, Rectangle
from kivy.uix.image import Image
from kivy.uix.filechooser import FileChooserIconView
from kivy.uix.textinput import TextInput
from kivy.uix.popup import Popup
from kivy.clock import Clock
from functools import partial

""" 
    Possible extension types are JPEG PNG BMP HDR
    TGA
 """

####### Global Constants #######
BGCOLOR = [35/255, 35/255, 35/255]
BTNSIZE = (0.2, 0.05)
BTNCOLOR = [183/255, 147/255, 155/255]

lib = ctypes.CDLL("./ImageDrop.so")


############### Main Page ###############

class MainWindow(Screen):

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.file_path = ""
        self.calls = 0
    
    def on_enter(self):
        #config screen
        self.add_widget(self.configureLabel())
        self.add_widget(self.configureButtonOne())
        self.add_widget(self.configureButtonTwo())
        self.fileEXT = "png" #default file extension    
    
    def on_leave(self):
        #clean screen
        self.remove_widget(self.img)
        self.remove_widget(self.file_chooser)
        self.remove_widget(self.lbl)
        self.calls += 1


####### Configure Main Buttons Start #######
    def configureButtonOne(self):
        self.btnOne = Button(
            text="Process Image",
            size_hint=BTNSIZE,
            background_color = (BTNCOLOR[0], BTNCOLOR[1], BTNCOLOR[2], 1),
            pos_hint={"x": 0, "y": 0},
            on_press=self.process_file
        )

        return self.btnOne

    def configureButtonTwo(self):
        self.btnTwo = Button(
            text="Select Image",
            size_hint = BTNSIZE,
            background_color = (BTNCOLOR[0], BTNCOLOR[1], BTNCOLOR[2], 1),
            pos_hint={"right": 1, "bottom": 1},
            on_press=self.open_file_chooser
        )

        return self.btnTwo

####### Configure Main Buttons End #######

    def configureLabel(self):
        self.lbl = Label(
            text="Upload an image",
            pos_hint={"left": 1, "top": 1},
            size_hint=(1, 1), 
        )

        with self.lbl.canvas.before:
            # Set background color
            Color(BGCOLOR[0], BGCOLOR[1] ,BGCOLOR[2], mode="rgba")
            self.lbl.rect = Rectangle(size=self.lbl.size, pos=self.lbl.pos)

        #correct the size of rect
        self.lbl.bind(size=self.update_rect, pos=self.update_rect)

        return self.lbl

    def update_rect(self, instance, value):
        instance.rect.size = instance.size
        instance.rect.pos = instance.pos

    def configureImageDisplay(self, filePath):
        self.img = Image(
                size_hint = (1, 0.9),
                pos_hint = {"right": 1, "top": 1},
                source = filePath    
        )

        return self.img

####### Main File Handling Start #######
    
    def getPath(self, instance, selection, *args):
        # filter out excess char
        raw_path = str(selection)
        self.file_path = raw_path[2:len(raw_path) - 2]
        self.remove_widget(self.file_chooser)
        self.add_widget(self.configureImageDisplay(self.file_path))

    
    def open_file_chooser(self, instance):
        self.remove_widget(self.lbl)
        self.remove_widget(self.btnOne)
        self.remove_widget(self.btnTwo)

        self.add_widget(self.configureLabel())
        self.add_widget(self.configureButtonOne())
        self.add_widget(self.configureButtonTwo())
        self.file_chooser = FileChooserIconView(
                            on_submit=self.getPath,
                            path = os.path.expanduser("~/"),
                            )
        self.lbl.text=""
        self.add_widget(self.file_chooser)

    def process_file(self, instance):
        # call Src/main.cpp to process image
        if self.file_path == "":
            pass
        else:
            # void processImage(int numOfCalls, char* fileEXT, char* filename)
            lib.processImage.argtypes = [ctypes.c_int, ctypes.c_char_p, ctypes.c_char_p]
            lib.processImage.restype = None
            lib.processImage(self.calls, self.fileEXT.encode(), self.file_path.encode())
            self.manager.current = "show_results"

####### Main File Handling End #######

############### Results Page ############### 
class ResultsWindow(Screen):

    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.calls = 0

    def on_enter(self):
        #config screen
        self.add_widget(self.configureBackLabel())
        self.add_widget(self.generateCyanImage())
        self.add_widget(self.generateYellowImage())
        self.add_widget(self.generateMagentaImage())
        self.add_widget(self.generateKeyImage())
        self.add_widget(self.generateFinalImage())        
        self.add_widget(self.configureBackButton())
        self.add_widget(self.configureSaveButton())
        self.fileEXT = "png" #default file ext = png        

        self.cyanImg.source = f"cyan_values{self.calls}.png"
        self.yellowImg.source = f"yellow_values{self.calls}.png"
        self.magentaImg.source = f"magenta_values{self.calls}.png"
        self.keyImg.source = f"key_values{self.calls}.png"
        self.finalImg.source = f"Final_IMG{self.calls}.png"

            
    def on_leave(self):
        #clean screen
        self.remove_widget(self.cyanImg)
        self.remove_widget(self.yellowImg)
        self.remove_widget(self.magentaImg)
        self.remove_widget(self.keyImg)
        self.remove_widget(self.finalImg)
        self.remove_widget(self.backBtn)
        self.remove_widget(self.saveBtn)
        self.calls += 1

    def configureBackLabel(self):
        self.lbl = Label()
        with self.lbl.canvas.before:
            Color(BGCOLOR[0], BGCOLOR[1], BGCOLOR[2], mode="rgba")
            self.lbl.rect = Rectangle(size=self.lbl.size, pos=self.lbl.pos)

        #correct the size of rect
        self.lbl.bind(size=self.update_rect, pos=self.update_rect)

        return self.lbl

    def update_rect(self, instance, value):
        instance.rect.size = instance.size
        instance.rect.pos = instance.pos
 
####### Generate Image Start #######

    def generateCyanImage(self):
        self.cyanImg = Image(
            size_hint = (0.25, 0.45),
            pos_hint = {"top": 1, "left": 1},
        )
    
        return self.cyanImg

    def generateYellowImage(self):
        self.yellowImg = Image(
            size_hint = (0.25, 0.45),
            pos_hint={"top":1, "right":1 },
        )
        
        return self.yellowImg

    def generateMagentaImage(self):
        self.magentaImg = Image(
            size_hint = (0.25, 0.45),
            pos_hint = {"left":1 , "top":0.55},
        )

        return self.magentaImg

    def generateKeyImage(self):
        self.keyImg = Image(
            size_hint = (0.25, 0.45),
            pos_hint = {"right":1, "top": 0.55},
        )

        return self.keyImg

    def generateFinalImage(self):
        self.finalImg = Image(
            size_hint = (0.5, 0.9),
            pos_hint = {"x": 0.25, "top": 1}
        )

        return self.finalImg

####### Generate Images End #######
    
####### Configure Results Page Buttons Start #######

    def configureBackButton(self):
        self.backBtn = Button(
            text="Choose Another",
            size_hint = BTNSIZE,
            pos_hint = {"bottom":1 , "left":1 },
            background_color = (BTNCOLOR[0], BTNCOLOR[1], BTNCOLOR[2], 1),
            on_release=self.chooseAnotherImage
        )

        return self.backBtn

    def configureSaveButton(self):
        self.saveBtn = Button(
            text="Save As...",
            size_hint = BTNSIZE,
            background_color = (BTNCOLOR[0], BTNCOLOR[1], BTNCOLOR[2], 1),
            pos_hint={"right": 1, "bottom": 1},
            on_release=self.selectSave
        )
        
        return self.saveBtn

    def configureSelectPathButton(self):
        self.pathBtn = Button(
            text="Save...",
            on_release=self.get_path,
            background_color = (BTNCOLOR[0], BTNCOLOR[1], BTNCOLOR[2], 1),
            size_hint = BTNSIZE,
            pos_hint = {"bottom": 1, "right": 1}
        )
        
        return self.pathBtn
    
    def configureChooseEXTBtn(self):
        self.chooseEXT = Button(
            text=".png",
            size_hint=BTNSIZE,
            background_color = (BTNCOLOR[0], BTNCOLOR[1], BTNCOLOR[2], 1),
            pos_hint={"x":0.6, "Bottom":1},
            on_release=self.extensionOptions
        )
        
        return self.chooseEXT

####### Configure Results Page Buttons End #######
    
    def chooseAnotherImage(self, instance):
        self.manager.current = "main_window"

####### Results Page File Handling Start #######
        
    def selectSave(self, instance):
        self.file_chooser = FileChooserIconView(
            path = os.path.expanduser("~/")
        )
        
        # Clean images
        self.remove_widget(self.cyanImg)
        self.remove_widget(self.yellowImg)
        self.remove_widget(self.magentaImg)
        self.remove_widget(self.keyImg)
        self.remove_widget(self.finalImg)
        self.remove_widget(self.saveBtn)
        self.remove_widget(self.backBtn)
        
        # Add file explorer and accessories
        self.add_widget(self.file_chooser)
        self.add_widget(self.configureSelectPathButton())
        self.add_widget(self.configureNameFile())
        self.add_widget(self.configureChooseEXTBtn())

    def get_path(self, instance):
        if self.checkFileName(self.txtBx.text):
            self.userFileName = self.txtBx.text
        else:
            self.notifyUserOfInputError()
            return
        shutil.move(f"cyan_values{self.calls}.png", self.file_chooser.path + 
                    "/" + f"cyan_{self.userFileName}." + self.fileEXT)
        shutil.move(f"yellow_values{self.calls}.png", self.file_chooser.path +
                    "/" + f"yellow_{self.userFileName}." + self.fileEXT)
        shutil.move(f"magenta_values{self.calls}.png", self.file_chooser.path + 
                    "/" + f"magenta_{self.userFileName}." + self.fileEXT)
        shutil.move(f"key_values{self.calls}.png",self.file_chooser.path + 
                    "/" + f"key_{self.userFileName}." + self.fileEXT)
        shutil.move(f"Final_IMG{self.calls}.png", self.file_chooser.path + 
                    "/" + f"Final_{self.userFileName}." + self.fileEXT)
        self.remove_widget(self.file_chooser)
        self.remove_widget(self.pathBtn)
        self.manager.current = "main_window"

    def configureNameFile(self):
        self.txtBx = TextInput(
            size_hint = (0.5, 0.05),
            pos_hint = {"x": 0.2, "y": 0},
            multiline=False
        )        

        return self.txtBx
    
####### Results Page File Handling End #######
    
####### Results Page Text Error Handling #######
    
    def checkFileName(self, fileName):
        for ltr in fileName:
            if not ltr.isalpha() and not ltr.isdigit():
                return False
        return True
    
    def notifyUserOfInputError(self):
        self.txtBx.text = "Must only be a letter or number."
        self.txtBx.focus = True

####### Results Page Text Error Handling #######

####### Extension Pop Up & Accessories #######
    def extensionOptions(self, instance):
        self.extPopUp = Popup(
            title="Choose file type...",
            size_hint = (None, None),
            size = (200, 300),
            pos_hint = {"x":0.6, "y":0.05},
            content = self.buildPopUp()
        )

        self.extPopUp.open()

    def buildPopUp(self):
        self.popLayout = GridLayout(
            rows=5
        )
        self.popLayout.add_widget(self.configurePNGBtn())
        self.popLayout.add_widget(self.configureJPGBtn())
        self.popLayout.add_widget(self.configureBMPBtn())
        self.popLayout.add_widget(self.configureTNGBtn())
        self.popLayout.add_widget(Label(size_hint=(0.2,0.1)))
        return self.popLayout

    def configurePNGBtn(self):
        self.pngBtn = Button(
            text="PNG",
            size_hint = (0.2, 0.1)
        )

        self.pngBtn.bind(on_release=partial(self.extensionSelect, "png"))

        return self.pngBtn

    def configureJPGBtn(self):
        self.jpgBtn = Button(
            text="JPG",
            size_hint=(0.2, 0.1)
        )

        self.jpgBtn.bind(on_release=partial(self.extensionSelect, "jpg"))

        return self.jpgBtn
    
    def configureBMPBtn(self):
        self.bmpBtn = Button(
            text="BMP",
            size_hint=(0.2, 0.1)
        )

        self.bmpBtn.bind(on_release=partial(self.extensionSelect, "bmp"))

        return self.bmpBtn
    def configureTNGBtn(self):
        self.tngBtn = Button(
            text="TNG",
            size_hint=(0.2, 0.1)
        )

        self.tngBtn.bind(on_release=partial(self.extensionSelect, "tng"))

        return self.tngBtn
    
    def extensionSelect(self, extension, instance):
        self.extPopUp.dismiss()
        self.chooseEXT.text = "." + extension
        self.fileEXT = extension
