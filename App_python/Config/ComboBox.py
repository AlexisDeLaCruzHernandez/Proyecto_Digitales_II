from App_python.Config.Libraries_and_colors import QComboBox, principal_color, background_color


class ComboBox(QComboBox):
    def __init__(self, options, width, height):
        super().__init__()
        self.addItems(options)
        self.setFixedSize(width, height)
        self.setStyleSheet(
            "ComboBox {"f"""
                      font: bold 15px;
                      border-radius: 4px;
                      border: 2px solid {principal_color};
                      color: {principal_color};
                      padding-left: 7px;
                  """"}"
            "ComboBox QListView {"f"""
                      font: bold 15px;
                      padding: 4px;
                      border: 1px solid {principal_color};
                      background-color: {background_color};
                      color: {principal_color};
                  """"}"
        )
