import ctypes as c
import os
from kivy.app import App
from kivy.uix.button import Button

def main():
    width = 1920
    height = 1080

    app = App()
    app.build = build_app
    app.run()

#    os.chdir('../')

#    test_lib = c.CDLL("./ImageDrop.so")
#    test_lib.main()

def on_button_press(instance):
    print("Button Pressed");

def build_app():
    button = Button(text="Hello, World")
    button.bind(on_press=on_button_press)
    return button

if __name__ == "__main__":
    main()
