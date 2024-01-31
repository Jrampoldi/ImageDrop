import kivy
from kivy.app import App
from kivy.uix.screenmanager import ScreenManager, Screen
from screenConfig import *

class ShowScreen(ScreenManager):

    #start app tree
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.add_widget(MainWindow(name="main_window"))



class MyApp(App):
    def build(self):
        self.title = "ImageDrop" 
        return ShowScreen() 

