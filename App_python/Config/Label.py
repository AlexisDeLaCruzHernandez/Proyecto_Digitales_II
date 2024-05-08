from App_python.Config.Libraries_and_colors import QLabel, principal_color, background_color


class Label(QLabel):
    def __init__(self, text):
        super().__init__()
        self.setText(text)
        self.setFixedSize(75, 30)
        self.setStyleSheet(
            "Label {"
            "font: bold 15px ;;"
            "border-bottom: 2px solid" + principal_color + ";;"
            "padding: 1px 1px ;;"
            "color:" + principal_color + ";;"
            "background-color: #1C1C1C }"
        )
        self.setStyleSheet(
            "Label {"f"""
                font: bold 15px;
                border-bottom: 2px solid {principal_color};
                color: {principal_color};
                background-color: {background_color};
            """"}"
        )
