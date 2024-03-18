import os
import sys

from PyQt5.QtWidgets import *
from pfsp_gui.window.pfsp_window import PFSPWindow

# sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

from pfsp_gui import utils

'''
if __name__ == '__main__':
    app = QApplication(sys.argv)
    wnd = NodeEditorWindow()
    wnd.nodeeditor.addNodes()
    module_path = os.path.dirname(inspect.getfile(wnd.__class__))
    utils.loadStylesheet(os.path.join(module_path, 'qss/nodestyle.qss'))
    sys.exit(app.exec_())
'''

# from calc_window import CalculatorWindow
#
# if __name__ == '__main__':
#     app = QApplication(sys.argv)
#     #print(QStyleFactory.keys())
#     app.setStyle('Fusion')
#     wnd = CalculatorWindow()
#     wnd.show()
#     sys.exit(app.exec_())

mainWindow = None

if __name__ == '__main__':
    app = QApplication(sys.argv)
    #print(QStyleFactory.keys())
    app.setStyle('Fusion')
    mainWindow = PFSPWindow()

    # module_path = os.path.dirname(inspect.getfile(mainWindow.__class__))
    # utils.loadStylesheet(os.path.join(module_path, 'qss/nodestyle.qss'))

    stylesheet_filename = os.path.join(os.path.dirname(__file__), 'pfsp_gui/qss/nodeeditor.qss')
    utils.loadStylesheets(
        os.path.join(os.path.dirname(__file__), 'pfsp_gui/qss/nodeeditor-dark.qss'),
        stylesheet_filename
    )

    mainWindow.show()
    sys.exit(app.exec_())
