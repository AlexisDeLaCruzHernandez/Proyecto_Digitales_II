from App_python.Config.Button import Button
from App_python.Config.ComboBox import ComboBox
from App_python.Config.Label import Label
from App_python.Config.Libraries_and_colors import (background_color, QMainWindow,
                                                    QFrame, QVBoxLayout, comports)


class ConfigPortWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ports = ["-"]
        for port in comports():
            self.ports.append(port.name)

        self.setStyleSheet(f"background-color: {background_color}")

        layout = QVBoxLayout()

        self.select_port = ComboBox(self.ports)
        self.connect = Button("Conectar")
        self.label = Label("Puertos:")
        layout.addWidget(self.label)
        layout.addWidget(self.select_port)
        layout.addWidget(self.connect)

        frame = QFrame()
        frame.setLayout(layout)

        self.setCentralWidget(frame)
