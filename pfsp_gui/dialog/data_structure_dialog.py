import sys

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *

class DataStructureDialog(QDialog):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.initUI()

    def initUI(self):
        pass

if __name__ == '__main__':
    app = QApplication(sys.argv)
    dlg = DataStructureDialog()
    #app_comp.setGeometry(600, 50, 400, 250)
    dlg.show()
    sys.exit(app.exec_())
