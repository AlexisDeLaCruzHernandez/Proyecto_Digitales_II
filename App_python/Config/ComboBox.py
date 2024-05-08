from PyQt6.QtWidgets import QComboBox


class ComboBox(QComboBox):
    def __init__(self, options):
        super().__init__()
        self.addItems(options)
        # self.setStyleSheet()
        # TODO: Estetica del combobox
