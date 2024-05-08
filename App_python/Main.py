from PyQt6.QtWidgets import QApplication
from App_python.Windows.Config_port_window import ConfigPortWindow


if __name__ == "__main__":
    # Create app
    App = QApplication([])

    # Create and show main window
    Window = ConfigPortWindow()
    Window.show()

    # App execution
    App.exec()
