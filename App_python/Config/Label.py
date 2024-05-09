from App_python.Config.Libraries_and_colors import QLabel, Qt, principal_color, background_color


class Label(QLabel):
    def __init__(self, text, width, height):
        super().__init__()
        self.setText(text)
        self.setFixedSize(width, height)
        self.setAlignment(Qt.AlignmentFlag.AlignCenter)
        self.setStyleSheet(
            "Label {"f"""
                   font: bold 15px;
                   border-bottom: 2px solid {principal_color};
                   color: {principal_color};
                   background-color: {background_color};
               """"}"
        )