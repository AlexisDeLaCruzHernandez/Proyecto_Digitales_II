from PyQt6.QtWidgets import QPushButton
from App_python.Config.Colors import principal_color, background_color


class Button(QPushButton):
    def __init__(self, text):
        super().__init__()
        self.setText(text)
        self.setFixedSize(85, 30)
        self.setStyleSheet(
            "Button {"f"""
                font: bold 15px;
                border-radius: 13px;
                border: 2px solid {principal_color};
                color: {principal_color};
                background-color: {background_color};
            """"}"
            "Button:hover {"f"""
                background-color: #424242;
            """"}"
            "Button:pressed {"f"""
                border: 4px solid {principal_color};
            """"}"
        )
