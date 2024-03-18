from PyQt5.QtCore import *

from pfsp_gui.window.pfsp_node_base import *
from pfsp_gui.toggle_switch import Switch

import copy

defaultContents = {
    "Use": False,
    "DebouncingCriteria": "eTime",
    "ThresholdToTriggerFailure": 1000,  # TODO: switch between [ms] and [count]
    "ThresholdToResetFailure": 100,  # TODO: switch between [ms] and [count]
    "ReactionMode": "eSWComponentOperationModeNormal",
    "ReactionState": "eSWComponentStateTriggerInit"
}

errorHandlerTemplate = {
    "CalibrationFailure": copy.deepcopy(defaultContents),
    "InvalidInput": copy.deepcopy(defaultContents),
    "InvalidOutput": copy.deepcopy(defaultContents),
    "TimeOutInput": copy.deepcopy(defaultContents),
    "TimeOutOutput": copy.deepcopy(defaultContents),
    "MemoryReadWriteFailure": copy.deepcopy(defaultContents),
    "NullPointer": copy.deepcopy(defaultContents),
    "InvalidDataValue": copy.deepcopy(defaultContents),
    "InvalidDataSize": copy.deepcopy(defaultContents),
    "IndexOutOfBounds": copy.deepcopy(defaultContents),
    "FileReadWriteOpenFailure": copy.deepcopy(defaultContents),
    "Unknown": copy.deepcopy(defaultContents)
}

errorHandlerProperty = {
    "Use": False, # eOn, eOff
    "DebouncingCriteria": [
        "eTime", "eCount"
    ],
    "ThresholdToTriggerFailure": 10000000,
    "ThresholdToResetFailure": 10000000,
    "ReactionMode": [
        "eSWComponentOperationModeNormal",
        "eSWComponentOperationModeDegradation",
        "eSWComponentOperationModeDiagnostics",
        "eSWComponentOperationModeEOL"
    ],
    "ReactionState": [
        "eSWComponentStateTriggerInit",
        "eSWComponentStateInitialized",
        "eSWComponentStateRun",
        "eSWComponentStateStop",
        "eSWComponentStateTriggerShutdown",
        "eSWComponentStateReadyToShutdown",
        "eSWComponentStateShutdownDone"
    ]
}

class ErrorProperty(QWidget):
    def __init__(self, property_dict:dict=dict(), parent=None):
        super().__init__(parent)
        if property_dict is not None and property_dict != dict():
            self.metaData = property_dict
        else:
            self.metaData = copy.deepcopy(errorHandlerTemplate["CalibrationFailure"])

        self.initUI()

    def initUI(self):
        self.contents = dict()
        self.unit_labels = []

        self.grid_layout = QGridLayout()
        for i,(k,v) in enumerate(errorHandlerProperty.items()):
            if isinstance(v, bool):
                widget = Switch(track_radius=12, thumb_radius=10)
                widget.setChecked(v)
                widget.toggled.connect(lambda state, key=k: self.onSwitchToggledEvent(key, state))
                self.grid_layout.addWidget(QLabel(k), i, 0)
                self.grid_layout.addWidget(widget, i, 1, Qt.AlignCenter)
            elif isinstance(v, int):
                widget = QSpinBox()
                widget.setRange(0, v)
                widget.setValue(self.metaData[k])
                widget.valueChanged.connect(lambda value, key=k: self.onSpinChangedEvent(key, value))
                self.grid_layout.addWidget(QLabel(k), i, 0)
                if k == "ThresholdToTriggerFailure" or k == "ThresholdToResetFailure":
                    hbox = QHBoxLayout()
                    hbox.addWidget(widget, 1)
                    unit_label = QLabel("[ms]")
                    self.unit_labels.append(unit_label)
                    hbox.addWidget(unit_label)
                    self.grid_layout.addLayout(hbox, i, 1)
                else:
                    self.grid_layout.addWidget(widget, i, 1)  # , Qt.AlignRight)
            elif isinstance(v, list):
                widget = QComboBox()
                widget.addItems(v)
                widget.setCurrentIndex(v.index(self.metaData[k]))
                widget.currentIndexChanged.connect(
                    lambda current, w=widget, key=k: self.onSelectionChangedEvent(key, w, current)
                )
                self.grid_layout.addWidget(QLabel(k), i, 0)
                self.grid_layout.addWidget(widget, i, 1)  # , Qt.AlignRight)
            self.contents[k] = widget
        self.setLayout(self.grid_layout)

    def onSwitchToggledEvent(self, key, state):
        self.metaData[key] = state

    def onSpinChangedEvent(self, key, value):
        self.metaData[key] = value

    def onSelectionChangedEvent(self, key, widget, current):
        self.metaData[key] = widget.itemText(current)
        if key == "DebouncingCriteria":
            if widget.itemText(current) == "eTime":
                for item in self.unit_labels:
                    item.setText("[ms]")
            else:
                for item in self.unit_labels:
                    item.setText("[count]")

    def updateUI(self):
        for k,v in self.metaData.items():
            w = self.contents[k]
            if isinstance(v, bool):
                w.setChecked(v)
            elif isinstance(v, int):
                w.setValue(v)
            elif isinstance(v, str):
                w.setCurrentIndex(errorHandlerProperty[k].index(v))

    def setContents(self, newer_dict):
        self.metaData = newer_dict
        self.updateUI()

    def getContents(self):
        return self.metaData

class ErrorHandlerDialog(QDialog):
    title = "The Error Handler"

    #class ParamTableDialog(QDMNodeContentWidget):
    def __init__(self, parameter_dict:dict=dict(), parent_name:str="", parent=None):
        super().__init__(parent) #, Qt.Popup | Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint | Qt.WindowStaysOnTopHint)

        if parameter_dict is not None and parameter_dict != dict(): self.metaData = parameter_dict
        else: self.metaData = copy.deepcopy(errorHandlerTemplate)

        self.initUI()
        self.setWindowTitle(self.__class__.title + " - " + parent_name)
        # self.show()

    def initUI(self):
        self.leftPanel = QListWidget()
        for k in self.metaData.keys():
            self.leftPanel.addItem(QListWidgetItem(k))
        self.leftPanel.currentItemChanged.connect(self.onListSelctionChangedEvent)

        add_button = QPushButton("+")
        add_button.clicked.connect(self.onAddItemEvent)
        sub_button = QPushButton("-")
        sub_button.clicked.connect(self.onDelItemEvent)

        left_frame = QFrame()
        vbox = QVBoxLayout(left_frame)
        vbox.addWidget(self.leftPanel)
        hbox = QHBoxLayout()
        # hbox.addStretch(1)
        hbox.addWidget(add_button)
        hbox.addWidget(sub_button)
        vbox.addLayout(hbox)

        self.rightPanel = ErrorProperty()#self.metaData[key_list[0]])

        splitter = QSplitter(Qt.Horizontal)
        splitter.addWidget(left_frame)
        splitter.addWidget(self.rightPanel)
        splitter.setStretchFactor(1, 1)

        self.vboxLayout = QVBoxLayout()
        self.vboxLayout.addWidget(splitter)

        hbox1 = QHBoxLayout()
        hbox1.addStretch(1)

        button = QPushButton("Ok")
        button.setToolTip("Save the Changes")
        button.clicked.connect(self.onOkButton)
        hbox1.addWidget(button)

        button = QPushButton("Cancel")
        button.setToolTip("Ignore the Changes")
        button.clicked.connect(self.reject)
        hbox1.addWidget(button)
        self.vboxLayout.addLayout(hbox1)

        self.setLayout(self.vboxLayout)

        # self.addItem("Insert name")

    def onListSelctionChangedEvent(self, curr_item, prev_item):
        if prev_item is not None:
            self.metaData[prev_item.text()] = self.rightPanel.getContents()
        self.rightPanel.setContents(self.metaData[curr_item.text()])
        # utils.pp(self.metaData)

    def addItem(self, tag_name:str):
        self.metaData[tag_name] = defaultContents
        item = QListWidgetItem(tag_name)
        # item.setFlags(item.flags() | Qt.ItemIsEditable)
        self.leftPanel.addItem(item)

    def onAddItemEvent(self):
        text,state = QInputDialog.getText(self, "Add Error Type", "Error Type:",  QLineEdit.Normal, "")
        if state and text != '':
            self.addItem(text)
            # utils.pp(self.metaData)

    def onDelItemEvent(self):
        item = self.leftPanel.currentItem()

        # self.leftPanel.removeItemWidget(item)
        self.leftPanel.removeItemWidget(self.leftPanel.takeItem(self.leftPanel.row(item)))
        del self.metaData[item.text()]
        # utils.pp(self.metaData)

    def onOkButton(self):
        # # TODO: QTreeWidget -> self.metaData
        # parameter_list = self.metaData["PFSPMiddleware"]["parameterTable"]["parameter"]
        # parameter_list.clear()
        # root = self.treeWidget.invisibleRootItem()
        # child_count = root.childCount()
        # for i in range(child_count):
        #     item = root.child(i)
        #     if self.filename.endswith(".json"):
        #         parameter_list.append( { item.text(0): self.treeWidget.itemWidget(item,1).text()} )
        #     elif self.filename.endswith(".xml"):
        #         parameter_list.append({"@"+item.text(0): self.treeWidget.itemWidget(item, 1).text()})
        #
        # # utils.pp(self.metaData)
        # if self.filename.endswith(".json"):
        #     with open(self.filename, "w") as file:
        #         file.write(json.dumps(self.metaData, indent=4))
        # elif self.filename.endswith(".xml"):
        #     with open(self.filename, "w") as file:
        #         xml = xmltodict.unparse(self.metaData, encoding="ISO-8859-1")
        #         # pprint(xml)
        #         dom = parseString(xml)
        #         file.write(dom.toprettyxml())
        self.accept()

if __name__ == "__main__":
    import sys

    app = QApplication(sys.argv)
    #print(QStyleFactory.keys())
    app.setStyle('Fusion')
    wnd = ErrorHandlerDialog(parameter_dict=None, parent_name="", parent=None)
    wnd.show()
    sys.exit(app.exec_())