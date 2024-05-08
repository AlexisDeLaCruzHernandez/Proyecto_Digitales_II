from PyQt6.QtWidgets import QComboBox


class ComboBox(QComboBox):
    def __init__(self, options):
        super().__init__()
        self.addItems(options)
        # TODO: Estetica del combobox
