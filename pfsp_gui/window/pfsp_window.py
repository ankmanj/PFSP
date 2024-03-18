import os
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *
from PyQt5.QtCore import *

from pfsp_gui.node_editor.node_editor_window import NodeEditorWindow
from pfsp_gui.window.pfsp_subwindow import PFSPSubWindow
from pfsp_gui.window.pfsp_drag_listbox import QDMDragListbox
from pfsp_gui.window.pfsp_node_conf import *
#from calc_conf_nodes import *

from pfsp_gui import utils

DEBUG = False

class PFSPWindow(NodeEditorWindow):
    def initUI(self):
        self.name_company = 'Unknown'
        self.name_product = 'Plugin Fractal SW Platform'

        self.empty_icon = QIcon('.')
        if DEBUG:
            print('Register nodes:')
            utils.pp(PFSP_NODES)

        self.mdiArea = QMdiArea()
        self.mdiArea.setHorizontalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        self.mdiArea.setVerticalScrollBarPolicy(Qt.ScrollBarAsNeeded)
        self.mdiArea.setViewMode(QMdiArea.TabbedView)
        self.mdiArea.setDocumentMode(True)
        self.mdiArea.setTabsClosable(True)
        self.mdiArea.setTabsMovable(True)
        self.setCentralWidget(self.mdiArea)

        self.mdiArea.subWindowActivated.connect(self.updateMenus)
        self.windowMapper = QSignalMapper(self)
        self.windowMapper.mapped[QWidget].connect(self.setActiveSubWindow)

        self.createNodesDock()

        self.createActions()
        self.createMenus()
        self.createToolBars()
        self.createStatusBar()
        self.updateMenus()

        self.readSettings()

        self.setWindowTitle('Plugin Fractal SW Platform - NodeEditor')

    def closeEvent(self, event):
        self.mdiArea.closeAllSubWindows()
        if self.mdiArea.currentSubWindow():
            event.ignore()
        else:
            self.writeSettings()
            event.accept()

    def createActions(self):
        super().createActions()

        self.actClose = QAction('Cl&ose', self, statusTip='Close the active window', triggered=self.mdiArea.closeActiveSubWindow)
        self.actCloseAll = QAction('Close &All', self, statusTip='Close all the windows', triggered=self.mdiArea.closeAllSubWindows)
        self.actTile = QAction('&Tile', self, statusTip='Tile the windows', triggered=self.mdiArea.tileSubWindows)
        self.actCascade = QAction('&Cascade', self, statusTip='Cascade the windows', triggered=self.mdiArea.cascadeSubWindows)
        self.actNext = QAction('Ne&xt', self, shortcut=QKeySequence.NextChild, statusTip='Move the focus to the next window', triggered=self.mdiArea.activateNextSubWindow)
        self.actPrevious = QAction('Pre&vious', self, shortcut=QKeySequence.PreviousChild, statusTip='Move the focus to the previous window', triggered=self.mdiArea.activatePreviousSubWindow)

        self.actSeparator = QAction(self)
        self.actSeparator.setSeparator(True)

        self.actAbout = QAction('&About', self, statusTip='Show the application\'s About box', triggered=self.about)

    def getCurrentNodeEditorWidget(self):
        ''' we're returning NodeEditorWidget here...'''
        activeSubWindow = self.mdiArea.activeSubWindow()
        if activeSubWindow:
            return activeSubWindow.widget()
        return None

    def getFileDialogDirectory(self):
        """Returns starting directory for ``QFileDialog`` file open/save"""
        defaultPath = './pfsp_setting'
        if not os.path.isdir(defaultPath):
            os.mkdir(defaultPath)
        return defaultPath

    def getFileDialogFilter(self):
        """Returns ``str`` standard file open/save filter for ``QFileDialog``"""
        return 'PFSP setting (*.json);;All files (*)'

    def onFileNew(self):
        try:
            subwnd = self.createMdiChild()
            subwnd.show()
        except Exception as e:
            utils.dumpException(e)

    def onFileOpen(self):
        fnames,filter = QFileDialog.getOpenFileNames(self, 'Open PFSP setting from file',self.getFileDialogDirectory(), self.getFileDialogFilter())
        try:
            for fname in fnames:
                if fname:
                    existing = self.findMdiChild(fname)
                    if existing:
                        self.mdiArea.setActiveSubWindow(existing)
                    else:
                        # we need to create new subWindow and open file
                        nodeeditor = PFSPSubWindow()

                        if nodeeditor.fileLoad(fname):
                            self.statusBar().showMessage('File %s loaded'%fname, 5000)
                            nodeeditor.setTitle()
                            subwnd = self.mdiArea.addSubWindow(nodeeditor)
                            subwnd.show()
                        else:
                            nodeeditor.close()
        except Exception as e:
            utils.dumpException(e)

    def onFileSaveAs(self):
        """Handle File Save As operation"""
        current_nodeeditor = self.getCurrentNodeEditorWidget()
        if current_nodeeditor is not None:
            fname, filter = QFileDialog.getSaveFileName(self, 'Save PFSP setting to file', self.getFileDialogDirectory(), self.getFileDialogFilter())
            if fname == '': return False

            self.onBeforeSaveAs(current_nodeeditor, fname)
            current_nodeeditor.fileSave(fname)
            self.statusBar().showMessage("Successfully saved as %s" % current_nodeeditor.filename, 5000)

            # support for MDI app
            if hasattr(current_nodeeditor, "setTitle"): current_nodeeditor.setTitle()
            else: self.setTitle()
            return True

    def onFileExport(self):
        """Handle File Save operation"""
        current_nodeeditor = self.getCurrentNodeEditorWidget()
        if current_nodeeditor is not None:
            fname,filter = QFileDialog.getSaveFileName(self, 'Export PFSP setting to json and XML files', self.getFileDialogDirectory(), 'PFSP setting (*.xml);;All files (*)')
            if fname == '': return False

            current_nodeeditor.fileExport(fname)
            self.statusBar().showMessage("Successfully export as %s" % fname, 5000)

            return True

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
        from pfsp_gui.window.pfsp_terminal_widget import PFSPTerminalWidget

        self.nodesListWidgets = QDMDragListbox()
        self.nodesDock = QDockWidget('Nodes')
        self.nodesDock.setWidget(self.nodesListWidgets)
        self.nodesDock.setFloating(False)
        self.addDockWidget(Qt.RightDockWidgetArea, self.nodesDock)

        self.consoleWidget = PFSPTerminalWidget(self)
        self.consoleDock = QDockWidget("Console")
        self.consoleDock.setTitleBarWidget(QWidget())
        self.consoleDock.setWidget(self.consoleWidget)
        self.consoleDock.setFloating(False)
        self.addDockWidget(Qt.BottomDockWidgetArea, self.consoleDock)

    def createStatusBar(self):
        self.statusBar().showMessage('Ready')

    def createMdiChild(self):
        nodeeditor = PFSPSubWindow()
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
