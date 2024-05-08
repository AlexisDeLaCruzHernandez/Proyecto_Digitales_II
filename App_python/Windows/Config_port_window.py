from PyQt6.QtWidgets import QMainWindow, QFrame, QVBoxLayout
from App_python.Config.Button import Button
from App_python.Config.ComboBox import ComboBox
from App_python.Config.Colors import principal_color, background_color
import serial.tools.list_ports


class ConfigPortWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ports = ["-"]
        for port in serial.tools.list_ports.comports():
            self.ports.append(port.name)

        self.setStyleSheet(f"background-color: {background_color}")

        layout = QVBoxLayout()

        self.select_port = ComboBox(self.ports)
        self.button = Button("Ejemplo")
        layout.addWidget(self.button)
        layout.addWidget(self.select_port)

        frame = QFrame()
        frame.setLayout(layout)

        self.setCentralWidget(frame)
