import sys
import inputs
import time
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QVBoxLayout, QComboBox, QPushButton

class USBGamepad:
    def __init__(self, name):
        self.name = name
        self.stopped = False

    def get_values(self):
        events = inputs.get_gamepad()
        values = []
        for event in events:
            if event.ev_type == "Absolute":
                values.append((event.code, event.state))
            if self.stopped:
                break
        return values

class USBDeviceSelector(QWidget):
    def __init__(self):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setWindowTitle('USB Device Selector')

        layout = QVBoxLayout()

        # Controller dropdown
        controller_label = QLabel('USB Controller:')
        self.controller_combobox = QComboBox()
        self.populate_controller_dropdown()
        self.selected_controller = None

        layout.addWidget(controller_label)
        layout.addWidget(self.controller_combobox)

        # Start button
        self.start_button = QPushButton('Start')
        self.start_button.clicked.connect(self.start_monitoring)
        layout.addWidget(self.start_button)

        # Stop button
        self.stop_button = QPushButton('Stop')
        self.stop_button.clicked.connect(self.stop_monitoring)
        self.stop_button.setEnabled(True)  # Initially disabled
        layout.addWidget(self.stop_button)

        self.setLayout(layout)
        self.show()

    def get_available_controllers(self):
        devices = inputs.devices.gamepads
        temp = []
        for device in devices: 
            temp.append(device.name)
        return temp

    def populate_controller_dropdown(self):
        controllers = self.get_available_controllers()
        self.controller_combobox.addItems(controllers)

    def start_monitoring(self):
        if self.selected_controller is not None:
            self.selected_controller = None

        self.selected_controller = USBGamepad(self.controller_combobox.currentText())
        self.start_button.setEnabled(False)
        self.stop_button.setEnabled(True)
        time.sleep(0.5)  # Add a processing delay of 0.5 seconds
        while True:
            time.sleep(0.5)
            values = self.selected_controller.get_values()
            if values:
                print(values)
            if self.selected_controller.stopped:
                break
           

    def stop_monitoring(self):
        self.selected_controller.stopped = True
        self.start_button.setEnabled(True)
        self.stop_button.setEnabled(False)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    ex = USBDeviceSelector()
    sys.exit(app.exec_())
