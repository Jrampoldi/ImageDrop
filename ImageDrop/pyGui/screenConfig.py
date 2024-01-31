import os
import ctypes
from ctypes.util import find_library
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.uix.widget import Widget
from kivy.uix.label import Label
from kivy.uix.button import Button
from kivy.graphics import Color, Rectangle
from kivy.uix.image import Image
from kivy.uix.filechooser import FileChooserIconView

lib = ctypes.CDLL("./ImageDrop.so")

class MainWindow(Screen):

    #build main page 
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.file_path = ""
        self.add_widget(self.configureLabel())
        self.add_widget(self.configureButtonOne())
        self.add_widget(self.configureButtonTwo())
        

    def configureButtonOne(self):
        self.btn = Button(
            text="Process Image",
            size_hint=(0.25, 0.1),
            pos_hint={"x": 0, "y": 0},
            color = (245/255, 245/255, 245/255, 0.8),
            on_press=self.process_file
        )
        return self.btn

    def configureButtonTwo(self):
        self.btn = Button(
            text="Select Image",
            size_hint = (0.25, 0.1),
            pos_hint={"right": 1, "bottom": 1},
            color = (245/255, 245/255, 245/255, 0.8),
            on_press=self.open_file_chooser
        )
        return self.btn

    def configureLabel(self):
        self.lbl = Label(
            text="Upload an image",
            pos_hint={"left": 1, "top": 1},
            size_hint=(1, 1), 
        )
        with self.lbl.canvas.before:
            Color(25/255, 25/255, 25/255, mode="rgba")
            self.lbl.rect = Rectangle(size=self.lbl.size, pos=self.lbl.pos)

        #correct the size of rect
        self.lbl.bind(size=self.update_rect, pos=self.update_rect)

        return self.lbl

    def configureImageDisplay(self, file_path):
        self.img = Image(
                size_hint = (1, 0.9),
                pos_hint = {"right": 1, "top": 1},
                source = file_path    
        )
        return self.img

    def get_path(self, instance, selection, *args):
        raw_path = str(selection)
        self.file_path = raw_path[2:len(raw_path) - 2]
        self.remove_widget(self.file_chooser)
        self.add_widget(self.configureImageDisplay(self.file_path))

    def update_rect(self, instance, value):
        instance.rect.size = instance.size
        instance.rect.pos = instance.pos
    
    def open_file_chooser(self, instance):
        self.file_chooser = FileChooserIconView(
                            on_submit=self.get_path,
                            path = os.path.expanduser("~/")
                            )
        self.remove_widget(self.lbl)
        self.add_widget(self.file_chooser)

    def process_file(self, instance):
        if self.file_path == "":
            pass
        else:
            lib.processImage.argtypes = [ctypes.c_char_p]
            lib.processImage.restype = None
            lib.processImage(self.file_path.encode())
            self.manager.current = "show_results"

class ResultsWindow(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.add_widget(self.generateCyanImage())
        self.add_widget(self.generateYellowImage())
        self.add_widget(self.generateMagentaImage())
        self.add_widget(self.generateKeyImage())
        self.add_widget(self.generateFinalImage())        

    def on_enter(self):
        self.cyanImg.source = "cyan_values.png"
        self.yellowImg.source = "yellow_values.png"
        self.magentaImg.source = "magenta_values.png"
        self.keyImg.source = "key_values.png"
        self.finalImg.source = "Final_IMG.png"

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

