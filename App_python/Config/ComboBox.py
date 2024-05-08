from App_python.Config.Libraries_and_colors import QComboBox, principal_color, background_color


class ComboBox(QComboBox):
    def __init__(self, options):
        super().__init__()
        self.addItems(options)
        # self.setStyleSheet()
        # TODO: Estetica del combobox
