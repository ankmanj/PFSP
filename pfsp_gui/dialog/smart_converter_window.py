from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *

from pfsp_gui.node_editor.node_editor_window import NodeEditorWindow
from pfsp_gui.dialog.smart_converter_subwindow import SmartConverterSubWindow
from pfsp_gui.dialog.smart_converter_drag_listbox import QDMDragListbox

from pfsp_gui import utils

DEBUG = False

class SmartConverterWindow(NodeEditorWindow):

    def __init__(self, smart_converter_dict: dict=dict(), class_name:str=''):
        self.metaData = smart_converter_dict
        self.class_name = class_name
        super().__init__()

    def initUI(self):
        self.name_company = 'Unknown'
        self.name_product = 'Plugin Fractal SW Platform'

        # self.stylesheet_filename = os.path.join(os.path.dirname(__file__), 'qss/nodeeditor.qss')
        # utils.loadStylesheets(
        #     os.path.join(os.path.dirname(__file__), 'qss/nodeeditor-dark.qss'),
        #     self.stylesheet_filename
        # )

        self.empty_icon = QIcon('.')

        import pfsp_gui.dialog.smart_converter_node_conf as SC_node_conf

        SC_node_conf.SMART_CONVERTER_NODES[SC_node_conf.SMART_CONVERTER_NODE_TO_INTER].class_name = self.class_name
        SC_node_conf.SMART_CONVERTER_NODES[SC_node_conf.SMART_CONVERTER_NODE_TO_EXT].class_name = self.class_name


        if DEBUG:
            print('Register nodes:')
            utils.pp(SC_node_conf.SMART_CONVERTER_NODES)

        # self.mdiArea = QMdiArea()
        # self.mdiArea.setHorizontalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        # self.mdiArea.setVerticalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        # self.mdiArea.setViewMode(QMdiArea.TabbedView)
        # self.mdiArea.setDocumentMode(True)
        # self.mdiArea.setTabsClosable(True)
        # self.mdiArea.setTabsMovable(True)
        # self.setCentralWidget(self.mdiArea)

        # self.mdiArea.subWindowActivated.connect(self.updateMenus)
        # self.windowMapper = QSignalMapper(self)
        # self.windowMapper.mapped[QWidget].connect(self.setActiveSubWindow)

        self.subWindow = SmartConverterSubWindow(self.metaData, self.class_name)
        self.setCentralWidget(self.subWindow)

        self.createNodesDock()

        # self.createActions()
        # self.createMenus()
        self.createToolBars()
        # self.createStatusBar()
        # self.updateMenus()

        self.readSettings()

        # self.setWindowTitle('Plugin Fractal SW Platform - SmartConverter')

    def closeEvent(self, event):
        self.mdiArea.closeAllSubWindows()
        if self.mdiArea.currentSubWindow():
            event.ignore()
        else:
            self.writeSettings()
            event.accept()

    def createActions(self):
        # super().createActions()
        # self.actClose = QAction('Cl&ose', self, statusTip='Close the active window', triggered=self.mdiArea.closeActiveSubWindow)
        # self.actCloseAll = QAction('Close &All', self, statusTip='Close all the windows', triggered=self.mdiArea.closeAllSubWindows)
        # self.actTile = QAction('&Tile', self, statusTip='Tile the windows', triggered=self.mdiArea.tileSubWindows)
        # self.actCascade = QAction('&Cascade', self, statusTip='Cascade the windows', triggered=self.mdiArea.cascadeSubWindows)
        # self.actNext = QAction('Ne&xt', self, shortcut=QKeySequence.NextChild, statusTip='Move the focus to the next window', triggered=self.mdiArea.activateNextSubWindow)
        # self.actPrevious = QAction('Pre&vious', self, shortcut=QKeySequence.PreviousChild, statusTip='Move the focus to the previous window', triggered=self.mdiArea.activatePreviousSubWindow)
        #
        # self.actSeparator = QAction(self)
        # self.actSeparator.setSeparator(True)
        #
        # self.actAbout = QAction('&About', self, statusTip='Show the application\'s About box', triggered=self.about)
        return

    def getCurrentNodeEditorWidget(self):
        ''' we're returning NodeEditorWidget here...'''
        # activeSubWindow = self.mdiArea.activeSubWindow()
        # if activeSubWindow:
        #     return activeSubWindow.widget()
        # return None
        return self.subWindow

    def onFileNew(self):
        try:
            subwnd = self.createMdiChild()
            subwnd.show()
        except Exception as e:
            utils.dumpException(e)

    def onFileOpen(self):
        fnames,filter = QFileDialog.getOpenFileNames(self, 'Open graph from file')
        try:
            for fname in fnames:
                if fname:
                    existing = self.findMdiChild(fname)
                    if existing:
                        self.mdiArea.setActiveSubWindow(existing)
                    else:
                        # we need to create new subWindow and open file
                        nodeeditor = SmartConverterSubWindow()

                        if nodeeditor.fileLoad(fname):
                            self.statusBar().showMessage('File %s loaded'%fname, 5000)
                            nodeeditor.setTitle()
                            subwnd = self.mdiArea.addSubWindow(nodeeditor)
                            subwnd.show()
                        else:
                            nodeeditor.close()
        except Exception as e:
            utils.dumpException(e)

    def about(self):
        QMessageBox.about(self, 'About PFSP NodeEditor',
                          'The <b>PFSP NodeEditor</b> demonstrates how to write multiple '
                          'document interface application using PyQt5 and NodeEditor. For more informations visit: '
                          '<a href=\'https://www.blenderfreak.com/\'>www.BlenderFreak.com</a>')

    def createMenus(self):
        super().createMenus()

        self.windowMenu = self.menuBar().addMenu('&Window')
        self.updateWindowMenu()
        self.windowMenu.aboutToShow.connect(self.updateWindowMenu)

        self.menuBar().addSeparator()

        self.helpMenu = self.menuBar().addMenu('&Help')
        self.helpMenu.addAction(self.actAbout)

        self.editMenu.aboutToShow.connect(self.updateEditMenu)

    def updateMenus(self):
        active = self.getCurrentNodeEditorWidget()
        hasMdiChild = (active is not None)

        self.actSave.setEnabled(hasMdiChild)
        self.actSaveAs.setEnabled(hasMdiChild)
        self.actClose.setEnabled(hasMdiChild)
        self.actCloseAll.setEnabled(hasMdiChild)
        self.actTile.setEnabled(hasMdiChild)
        self.actCascade.setEnabled(hasMdiChild)
        self.actNext.setEnabled(hasMdiChild)
        self.actPrevious.setEnabled(hasMdiChild)
        self.actSeparator.setEnabled(hasMdiChild)

        self.updateEditMenu()

    def updateEditMenu(self):
        # print('update Edit Menu')
        active = self.getCurrentNodeEditorWidget()
        hasMdiChild = (active is not None)

        self.actPaste.setEnabled(hasMdiChild)

        self.actCut.setEnabled(hasMdiChild and active.hasSelectedItems())
        self.actCopy.setEnabled(hasMdiChild and active.hasSelectedItems())
        self.actDelete.setEnabled(hasMdiChild and active.hasSelectedItems())

        self.actUndo.setEnabled(hasMdiChild and active.canUndo())
        self.actRedo.setEnabled(hasMdiChild and active.canRedo())

    def updateWindowMenu(self):
        self.windowMenu.clear()

        toolbar_nodes = self.windowMenu.addAction('Nodes Toolbar')
        toolbar_nodes.setCheckable(True)
        toolbar_nodes.triggered.connect(self.onWindowNodesToolbar)
        toolbar_nodes.setChecked(self.nodesDock.isVisible())
        self.windowMenu.addSeparator()
        self.windowMenu.addAction(self.actClose)
        self.windowMenu.addAction(self.actCloseAll)
        self.windowMenu.addSeparator()
        self.windowMenu.addAction(self.actTile)
        self.windowMenu.addAction(self.actCascade)
        self.windowMenu.addSeparator()
        self.windowMenu.addAction(self.actNext)
        self.windowMenu.addAction(self.actPrevious)
        self.windowMenu.addAction(self.actSeparator)

        windows = self.mdiArea.subWindowList()
        self.actSeparator.setVisible(len(windows) != 0)

        for i,window in enumerate(windows):
            child = window.widget()

            text = '%d %s'%(i+1, child.getUserFriendlyFilename())
            if i < 9:
                text = '&' + text

            action = self.windowMenu.addAction(text)
            action.setCheckable(True)
            action.setChecked(child is self.getCurrentNodeEditorWidget())
            action.triggered.connect(self.windowMapper.map)
            self.windowMapper.setMapping(action, window)

    def onWindowNodesToolbar(self):
        if self.nodesDock.isVisible():
            self.nodesDock.hide()
        else:
            self.nodesDock.show()

    def createToolBars(self):
        pass

    def createNodesDock(self):
        self.nodesListWidgets = QDMDragListbox()

        self.nodesDock = QDockWidget('Nodes')
        self.nodesDock.setWidget(self.nodesListWidgets)
        self.nodesDock.setFloating(False)

        self.addDockWidget(Qt.RightDockWidgetArea, self.nodesDock)

    def createStatusBar(self):
        self.statusBar().showMessage('Ready')

    def createMdiChild(self):
        nodeeditor = SmartConverterSubWindow()
        subwnd = self.mdiArea.addSubWindow(nodeeditor)
        return subwnd

    # def readSettings(self):
    #     pass

    def findMdiChild(self, filename):
        for window in self.mdiArea.subWindowList():
            if window.widget().filename == filename:
                return window
        return None

    def setActiveSubWindow(self, window):
        if window:
            try:
                self.mdiArea.setActiveSubWindow(window)
            except Exception as e:
                utils.dumpException(e)

mainWindow = None

import os, sys
import inspect

# sys.path.insert(0, os.path.join(os.path.dirname(__file__), '..', '..'))

if __name__ == '__main__':
    app = QApplication(sys.argv)
    #print(QStyleFactory.keys())
    app.setStyle('Fusion')
    # mainWindow = PFSPWindow()
    mainWindow = SmartConverterWindow()
    module_path = os.path.dirname(inspect.getfile(mainWindow.__class__))
    utils.loadStylesheet(os.path.join(module_path, 'qss/nodestyle.qss'))
    mainWindow.show()
    sys.exit(app.exec_())
