from PyQt6.QtWidgets import QApplication
from App_python.Windows.Config_port_window import ConfigPortWindow
import serial.tools.list_ports

if __name__ == "__main__":
    # Create app
    App = QApplication([])

    cosas_serial = serial.tools.list_ports.comports()
    print(cosas_serial)
    for puerto in cosas_serial:
        print(f"Nombre: {puerto.name}, Descripción: {puerto.description}, puerto USB: {puerto.usb_info()}")

    # Create and show main window
    Window = ConfigPortWindow()
    Window.show()

    # App execution
    App.exec()
