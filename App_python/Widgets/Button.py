from PyQt6.QtWidgets import QPushButton

principal_color = "#FC93AD"


class Button(QPushButton):
    def __init__(self, text):
        super().__init__()
        self.setText(text)
        self.setFixedSize(120, 35)
        self.setStyleSheet(
            "Button {"
            "font: bold 15px ;;"
            "border-radius: 13px ;;"
            "border: 2px solid" + principal_color + ";;"
            "padding: 1px 1px ;;"
            "color:" + principal_color + ";;"
            "background-color: #1C1C1C }"

            "Button:hover {"
            "background-color: #424242 }"

            "Button:pressed {"
            "border: 5px solid" + principal_color + "}"
        )
