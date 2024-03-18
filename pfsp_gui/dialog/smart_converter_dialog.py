import os
import json

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *

from pfsp_gui.node_editor.node_scene import InvalidFile

from pfsp_gui.dialog.smart_converter_window import SmartConverterWindow

DEBUG = False
DEBUG_CONTEXT = False

class SmartConverterDialog(QDialog):
    title = "The Smart Converter"

    def __init__(self, smart_converter_dict: dict = None, parent_name: str = "", class_name:str='', parent=None):
        super().__init__(parent)

        if smart_converter_dict is not None and smart_converter_dict != dict():
            self.metaData = smart_converter_dict
            for x in self.metaData['nodes']:
                if x['title'] == 'SmartConverter - External Input To Internal' or x['title'] == 'SmartConverter - Internal To External Output':
                    x['content']['class_initial'] = class_name
        else:
            self.metaData = None

        print(self.metaData)
        
        self.class_name = class_name

        self.initUI()
        self.setWindowFlag(Qt.WindowMinimizeButtonHint, True)
        self.setWindowFlag(Qt.WindowMaximizeButtonHint, True)
        self.showMaximized()

        self.setWindowTitle(self.__class__.title + " - " + class_name + '@' + parent_name)

    def initUI(self):
        self.vboxLayout = QVBoxLayout()
        self.vboxLayout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(self.vboxLayout)

        self.mainWindow = SmartConverterWindow(self.metaData, self.class_name)
        self.vboxLayout.addWidget(self.mainWindow)

        hbox = QHBoxLayout()
        hbox.addStretch(1)
        button = QPushButton("Ok")
        button.setToolTip("Save the Changes")
        button.clicked.connect(self.onOkButton)
        hbox.addWidget(button)
        button = QPushButton("Cancel")
        button.setToolTip("Ignore the Changes")
        button.clicked.connect(self.reject)
        hbox.addWidget(button)
        self.vboxLayout.addLayout(hbox)

    def onEditUndo(self):
        """Handle Edit Undo operation"""
        self.scene.history.undo()

    def onEditRedo(self):
        """Handle Edit Redo operation"""
        self.scene.history.redo()

    def onEditDelete(self):
        """Handle Delete Selected operation"""
        self.scene.getView().deleteSelected()

    def onEditCut(self):
        """Handle Edit Cut to clipboard operation"""
        data = self.scene.clipboard.serializeSelected(delete=True)
        str_data = json.dumps(data, indent=4)
        QApplication.instance().clipboard().setText(str_data)

    def onEditCopy(self):
        """Handle Edit Copy to clipboard operation"""
        data = self.scene.clipboard.serializeSelected(delete=False)
        str_data = json.dumps(data, indent=4)
        QApplication.instance().clipboard().setText(str_data)

    def onEditPaste(self):
        """Handle Edit Paste from clipboard operation"""
        raw_data = QApplication.instance().clipboard().text()

        try:
            data = json.loads(raw_data)
        except ValueError as e:
            print("Pasting of not valid json data!", e)
            return

        # check if the json data are correct
        if 'nodes' not in data:
            print("JSON does not contain any nodes!")
            return

        return self.scene.clipboard.deserializeFromClipboard(data)

    def isModified(self):
        return self.scene.isModified()

    def isFilenameSet(self):
        return self.filename is not None

    def getSelectedItems(self):
        return self.scene.getSelectedItems()

    def hasSelectedItems(self):
        return self.getSelectedItems() != []

    def canUndo(self):
        return self.scene.history.canUndo()

    def canRedo(self):
        return self.scene.history.canRedo()

    def getUserFriendlyFilename(self):
        name = os.path.basename(self.filename) if self.isFilenameSet() else 'Smart Converter Configuration'
        return name + ('*' if self.isModified() else '')

    def fileNew(self):
        self.scene.clear()
        self.filename = None
        self.scene.history.clear()
        self.scene.history.storeInitialHistoryStamp()

    def fileLoad(self, filename):
        QApplication.setOverrideCursor(Qt.WaitCursor)
        try:
            self.scene.loadFromFile(filename)
            self.filename = filename
            # clear history
            self.scene.history.clear()
            self.scene.history.storeInitialHistoryStamp()
            return True
        except InvalidFile as e:
            print(e)
            QApplication.restoreOverrideCursor()
            QMessageBox.warning(self, 'Error loading %s'%os.path.basename(filename), str(e))
            return False
        finally:
            QApplication.restoreOverrideCursor()

    def fileSave(self, filename=None):
        # when called with empty parameter, we won't store the filename
        if filename is not None: self.filename = filename
        QApplication.setOverrideCursor(Qt.WaitCursor)
        self.scene.saveToFile(self.filename)
        QApplication.restoreOverrideCursor()
        return True

    def onOkButton(self):
        # TODO: QTreeWidget -> self.metaData
        self.metaData = self.mainWindow.subWindow.scene.serialize()
        self.accept()
