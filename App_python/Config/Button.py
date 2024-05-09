from App_python.Config.Libraries_and_colors import QPushButton, principal_color, background_color, light_color


class Button(QPushButton):
    def __init__(self, text, width, height):
        super().__init__()
        self.setText(text)
        self.setFixedSize(width, height)
        self.setStyleSheet(
            "Button {"f"""
                    font: bold 15px;
                    border-radius: 10px;
                    border: 2px solid {principal_color};
                    color: {principal_color};
                    background-color: {background_color};
                """"}"
            "Button:hover {"f"""
                    background-color: {light_color};
                """"}"
            "Button:pressed {"f"""
                    border: 4px solid {principal_color};
                """"}"
        )
