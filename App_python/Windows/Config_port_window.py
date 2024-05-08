from PyQt6.QtWidgets import QMainWindow
from App_python.Widgets.Button import Button


class ConfigPortWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setStyleSheet("background-color: #1C1C1C")
        self.button = Button("Ejemplo")
        self.setCentralWidget(self.button)
