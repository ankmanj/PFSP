import sys
import time
import uuid

from PyQt5 import QtCore, QtGui, QtWidgets

import gi

gi.require_version("Wnck", "3.0")
from gi.repository import Wnck, Gdk


class TerminalContainer(QtWidgets.QTabWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        lay = QtWidgets.QVBoxLayout(self)
        lay.setContentsMargins(0, 0, 0, 0)
        self.name_session = uuid.uuid4().hex

    def start(self):
        started, procId = QtCore.QProcess.startDetached(
            "xterm", ["-e", "tmux", "new", "-s", self.name_session], "."
        )
        if not started:
            QtWidgets.QMessageBox.critical(
                self, 'Command "{}" not started!'.format(command), "Eh"
            )
            return
        attempts = 0
        while attempts < 10:
            screen = Wnck.Screen.get_default()
            screen.force_update()
            time.sleep(0.1)
            while Gdk.events_pending():
                Gdk.event_get()
            for w in screen.get_windows():
                print(attempts, w.get_pid(), procId, w.get_pid() == procId)
                if w.get_pid() == procId:
                    win32w = QtGui.QWindow.fromWinId(w.get_xid())
                    widg = QtWidgets.QWidget.createWindowContainer(win32w)
                    self.layout().addWidget(widg)
                    self.resize(500, 400)
                    return
            attempts += 1
        QtWidgets.QMessageBox.critical(
            self, "Window not found", "Process started but window not found"
        )

    def stop(self):
        QtCore.QProcess.execute("tmux", ["kill-session", "-t", self.name_session])

    def send_command(self, command):
        QtCore.QProcess.execute(
            "tmux", ["send-keys", "-t", self.name_session, command, "Enter"]
        )


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self, parent=None):
        super().__init__(parent)

        self.ifconfig_btn = QtWidgets.QPushButton("ifconfig")
        self.ping_btn = QtWidgets.QPushButton("ping")
        self.terminal = TerminalContainer()

        central_widget = QtWidgets.QWidget()
        self.setCentralWidget(central_widget)

        lay = QtWidgets.QGridLayout(central_widget)
        lay.addWidget(self.ifconfig_btn, 0, 0)
        lay.addWidget(self.ping_btn, 0, 1)
        lay.addWidget(self.terminal, 1, 0, 1, 2)

        self.terminal.start()

        self.resize(640, 480)

        self.ifconfig_btn.clicked.connect(self.launch_ifconfig)
        self.ping_btn.clicked.connect(self.launch_ping)

    def launch_ifconfig(self):
        self.terminal.send_command("ifconfig")

    def launch_ping(self):
        self.terminal.send_command("ping 8.8.8.8")

    def closeEvent(self, event):
        self.terminal.stop()
        super().closeEvent(event)


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    main = MainWindow()
    main.show()
    sys.exit(app.exec_())
    