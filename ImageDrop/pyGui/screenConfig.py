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
            Color(113/255, 113/255, 154/255, mode="rgba")
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
