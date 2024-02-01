import kivy
from kivy.app import App
from kivy.uix.screenmanager import ScreenManager, Screen, NoTransition
from screenConfig import *

class ShowScreen(ScreenManager):

    #start app tree
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.add_widget(MainWindow(name="main_window"))
        self.add_widget(ResultsWindow(name="show_results"))


class MyApp(App):
    def build(self):
        self.title = "ImageDrop" 
        self.orientation = "vertical"
        return ShowScreen(transition=NoTransition()) 

