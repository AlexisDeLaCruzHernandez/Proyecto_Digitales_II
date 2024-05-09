from App_python.Config.Button import Button
from App_python.Config.ComboBox import ComboBox
from App_python.Config.Label import Label
from App_python.Config.Libraries_and_colors import (QMainWindow, QFrame, QVBoxLayout, comports, background_color,
                                                    Qt, QGuiApplication)


class ConfigPortWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.ports = ["-"]
        for port in comports():
            self.ports.append(port.name)

        self.setStyleSheet(f"background-color: {background_color}")
        self.setMinimumSize(293, 132)
        self.screen_width = QGuiApplication.primaryScreen().geometry().width()
        self.screen_height = QGuiApplication.primaryScreen().geometry().height()
        print(self.screen_width, self.screen_height)

        layout = QVBoxLayout()

        self.label = Label("Puertos disponibles", 275, 30)
        self.select_port = ComboBox(self.ports, 100, 30)
        self.connect = Button("Conectar", 100, 30)

        layout.addWidget(self.label, alignment=Qt.AlignmentFlag.AlignHCenter)
        layout.addSpacing(12)
        layout.addWidget(self.select_port, alignment=Qt.AlignmentFlag.AlignHCenter)
        layout.addWidget(self.connect, alignment=Qt.AlignmentFlag.AlignHCenter)
        layout.setAlignment(Qt.AlignmentFlag.AlignCenter)

        frame = QFrame()
        frame.setLayout(layout)

        self.setCentralWidget(frame)
        self.resizeEvent = self.size_detect

    def size_detect(self, event):
        label_x = int(((275-0.7*self.screen_width)/(293-self.screen_width))*(event.size().width()-293)+275)
        select_port_connect_x = int(((100-0.2*self.screen_width)/(293-self.screen_width))*(event.size().width()-293)+100)
        scale_y = int(((30-0.05*self.screen_height)/(132-self.screen_height))*(event.size().height()-132)+30)
        self.label.setFixedSize(label_x, scale_y)
        self.select_port.setFixedSize(select_port_connect_x, scale_y)
        self.connect.setFixedSize(select_port_connect_x, scale_y)
