from PyQt5.QtCore import *
from PyQt5.QtGui import *
from pfsp_gui.window.pfsp_node_conf import *
from pfsp_gui.window.pfsp_node_base import *
from pfsp_gui import utils

import copy

from pfsp_gui.dialog.parameter_table_dialog import ParamTableDialog
from pfsp_gui.dialog.smart_converter_dialog import SmartConverterDialog
from pfsp_gui.dialog.mode_manager_dialog import ModeMangerDialog
from pfsp_gui.dialog.error_handler_dialog import ErrorHandlerDialog

class HostComponentContent(QDMNodeContentWidget): # TODO: Tag name should be changed into Host Application
    idCounter = 1
    # templateDict = {
    #     "name": "Type service name",
    #     "id": "1",
    #     "priority": "10",
    #     # "core": "0",
    #     # "mailbox_size": "4096",
    #     "class": "CXXXService",
    #     "config file": "xxxx",
    #     "interface name": "xxxTopic", # TODO: should be transferred to smart converter
    #     "hidden": {
    #         "parameters": None,
    #         "mode_manager": None,
    #         "error_handler": None
    #     }
    # }

    def __init__(self, node, parent=None):
        self.metaData = {
            "name": "ServiceName"+str(self.__class__.idCounter),
            "id": str(self.__class__.idCounter), #"1",
            "priority": "10",
            # "core": "0",
            # "mailbox_size": "4096",
            "class": "CService"+str(self.__class__.idCounter),
            "config file": "config"+str(self.__class__.idCounter),
            # "interface name": "Topic"+str(self.__class__.idCounter), # TODO: should be transferred to smart converter
            "hidden": {
                "parameters": [
                    {'name': 'HostComponentLibName', 'value': 'libPFSP_ServiceName'+str(self.__class__.idCounter)+'.so'}
                ],
                "mode_manager": {
                    'PlayType': 'Repeat',
                    'ThreadFrequency': 20,
                    'eSWComponentOperationModeDegradation': False,
                    'eSWComponentOperationModeDiagnostics': False,
                    'eSWComponentOperationModeNormal': True
                },
                'smart_converter': {
                    "id": id(-140651930836944),
                    "scene_width": 64000,
                    "scene_height": 64000,
                    "nodes": [
                        {
                            "id": id(-140651930836896),
                            "title": "Data Structure",
                            "pos_x": -120.0,
                            "pos_y": -125.0,
                            "inputs": [
                                {
                                    "id": id(-140651723912096),
                                    "index": 0,
                                    "multi_edges": False,
                                    "position": 2,
                                    "socket_type": 1
                                }
                            ],
                            "outputs": [
                                {
                                    "id": id(-140651723912144),
                                    "index": 0,
                                    "multi_edges": True,
                                    "position": 5,
                                    "socket_type": 4
                                }
                            ],
                            "content": {
                                "object_class": "CService"+str(self.__class__.idCounter)+"InteralData",
                                "object_member_0": "type_varname:int8_t"
                            },
                            "op_code": 0
                        },
                        {
                            "id": id(-140651723912288),
                            "title": "SmartConverter - Internal To External Output",
                            "pos_x": 264.0,
                            "pos_y": -127.0,
                            "inputs": [
                                {
                                    "id": id(-140651723912336),
                                    "index": 0,
                                    "multi_edges": False,
                                    "position": 2,
                                    "socket_type": 1
                                }
                            ],
                            "outputs": [
                                {
                                    "id": id(-140651723912672),
                                    "index": 0,
                                    "multi_edges": True,
                                    "position": 5,
                                    "socket_type": 4
                                }
                            ],
                            "content": {
                                "class_initial": "CService"+str(self.__class__.idCounter),
                                "function": "getConvertedExternalData",
                                "pipelines": 1
                            },
                            "op_code": 2
                        },
                        {
                            "id": id(-140651723912768),
                            "title": "Data Structure",
                            "pos_x": 865.0,
                            "pos_y": -124.0,
                            "inputs": [
                                {
                                    "id": id(-140651723913104),
                                    "index": 0,
                                    "multi_edges": False,
                                    "position": 2,
                                    "socket_type": 1
                                }
                            ],
                            "outputs": [
                                {
                                    "id": id(-140651723913200),
                                    "index": 0,
                                    "multi_edges": True,
                                    "position": 5,
                                    "socket_type": 4
                                }
                            ],
                            "content": {
                                "object_class": "ExternalOutputFromCService"+str(self.__class__.idCounter),
                                "object_member_0": "type_varname:int8_t"
                            },
                            "op_code": 0
                        }
                    ],
                    "edges": [
                        {
                            "id": id(-140651723912048),
                            "edge_type": 2,
                            "start": id(-140651723912144),
                            "end": id(-140651723912336)
                        },
                        {
                            "id": id(-140651723913296),
                            "edge_type": 2,
                            "start": id(-140651723912672),
                            "end": id(-140651723913104)
                        }
                    ]
                },
                "error_handler": {
                    'CalibrationFailure': {
                        'DebouncingCriteria': 'eTime',
                        'ReactionMode': 'eSWComponentOperationModeNormal',
                        'ReactionState': 'eSWComponentStateTriggerInit',
                        'ThresholdToResetFailure': 100,
                        'ThresholdToTriggerFailure': 1000,
                        'Use': False
                    },
                    'FileReadWriteOpenFailure': {
                        'DebouncingCriteria': 'eTime',
                        'ReactionMode': 'eSWComponentOperationModeNormal',
                        'ReactionState': 'eSWComponentStateTriggerInit',
                        'ThresholdToResetFailure': 100,
                        'ThresholdToTriggerFailure': 1000,
                        'Use': False
                    },
                    'IndexOutOfBounds': {
                        'DebouncingCriteria': 'eCount',
                        'ReactionMode': 'eSWComponentOperationModeNormal',
                        'ReactionState': 'eSWComponentStateRun',
                        'ThresholdToResetFailure': 5,
                        'ThresholdToTriggerFailure': 5,
                        'Use': True
                    },
                    'InvalidDataSize': {
                        'DebouncingCriteria': 'eTime',
                        'ReactionMode': 'eSWComponentOperationModeNormal',
                        'ReactionState': 'eSWComponentStateTriggerInit',
                        'ThresholdToResetFailure': 100,
                        'ThresholdToTriggerFailure': 1000,
                        'Use': False
                    },
                    'InvalidDataValue': {
                        'DebouncingCriteria': 'eTime',
                        'ReactionMode': 'eSWComponentOperationModeNormal',
                        'ReactionState': 'eSWComponentStateTriggerInit',
                        'ThresholdToResetFailure': 100,
                        'ThresholdToTriggerFailure': 1000,
                        'Use': False
                    },
                    'InvalidInput': {
                        'DebouncingCriteria': 'eTime',
                        'ReactionMode': 'eSWComponentOperationModeNormal',
                        'ReactionState': 'eSWComponentStateTriggerInit',
                        'ThresholdToResetFailure': 100,
                        'ThresholdToTriggerFailure': 1000,
                        'Use': False
                    },
                    'InvalidOutput': {
                        'DebouncingCriteria': 'eTime',
                        'ReactionMode': 'eSWComponentOperationModeNormal',
                        'ReactionState': 'eSWComponentStateTriggerInit',
                        'ThresholdToResetFailure': 100,
                        'ThresholdToTriggerFailure': 1000,
                        'Use': False
                    },
                    'MemoryReadWriteFailure': {
                        'DebouncingCriteria': 'eTime',
                        'ReactionMode': 'eSWComponentOperationModeNormal',
                        'ReactionState': 'eSWComponentStateTriggerInit',
                        'ThresholdToResetFailure': 100,
                        'ThresholdToTriggerFailure': 1000,
                        'Use': False
                    },
                    'NullPointer': {
                        'DebouncingCriteria': 'eTime',
                        'ReactionMode': 'eSWComponentOperationModeNormal',
                        'ReactionState': 'eSWComponentStateTriggerInit',
                        'ThresholdToResetFailure': 100,
                        'ThresholdToTriggerFailure': 1000,
                        'Use': False
                    },
                    'TimeOutInput': {
                        'DebouncingCriteria': 'eTime',
                        'ReactionMode': 'eSWComponentOperationModeNormal',
                        'ReactionState': 'eSWComponentStateTriggerInit',
                        'ThresholdToResetFailure': 100,
                        'ThresholdToTriggerFailure': 1000,
                        'Use': False
                    },
                    'TimeOutOutput': {
                        'DebouncingCriteria': 'eTime',
                        'ReactionMode': 'eSWComponentOperationModeNormal',
                        'ReactionState': 'eSWComponentStateTriggerInit',
                        'ThresholdToResetFailure': 100,
                        'ThresholdToTriggerFailure': 1000,
                        'Use': False
                    },
                    'Unknown': {
                        'DebouncingCriteria': 'eTime',
                        'ReactionMode': 'eSWComponentOperationModeNormal',
                        'ReactionState': 'eSWComponentStateTriggerInit',
                        'ThresholdToResetFailure': 100,
                        'ThresholdToTriggerFailure': 1000,
                        'Use': False
                    }
                }
            }
        }
        self.metaData["id"] = str(self.__class__.idCounter)
        self.__class__.idCounter += 1
        super().__init__(node, parent)
        # self.initUI()
        self._parent = parent

    def initUI(self):

        self.contents = dict()

        self.vboxLayout = QVBoxLayout(self)
        self.vboxLayout.setContentsMargins(0,0,0,0)

        grid = QGridLayout()
        for i,(k,v) in enumerate(self.metaData.items()):
            if k == "hidden": continue
            tag = QLabel(k)
            w = QLineEdit()
            w.setText(v)
            w.textChanged.connect(lambda text, key=k: self.onContentChangedEvent(key, text))
            grid.addWidget(tag, i, 0)
            grid.addWidget(w, i, 1)
            self.contents[k] = w

        self.vboxLayout.addLayout(grid)

        button = QPushButton("Set Component Manager")
        button.setToolTip("Setup the Mode Manager")
        button.clicked.connect(self.onModeManagerButton)
        self.vboxLayout.addWidget(button)

        button = QPushButton("Set Configuration")
        button.setToolTip("Configure the Parameter Table")
        button.clicked.connect(self.onParameterTableButton)
        self.vboxLayout.addWidget(button)

        button = QPushButton("Set Smart Converter")
        button.setToolTip("Setup the Smart Converter")
        button.clicked.connect(self.onSmartConverterButton)
        self.vboxLayout.addWidget(button)

        button = QPushButton("Set Error Handler")
        button.setToolTip("Setup the Error Handler")
        button.clicked.connect(self.onErrorHandlerButton)
        self.vboxLayout.addWidget(button)

        # self.setLayout(self.vboxLayout)

    def updateUI(self):
        for k, v in self.metaData.items():
            if k == 'hidden': continue
            w = self.contents[k]
            w.setText(v)
            # print(k, w.text())

    def onContentChangedEvent(self, key, text):
        self.metaData[key] = text

    def onParameterTableButton(self):
        from main import mainWindow

        '''
        root = self.treeWidget.invisibleRootItem()
        signal_count = root.childCount()
        for i in range(signal_count):
            signal = root.child(i)
            print(signal)
        '''

        # for item in self.treeWidget.findItems("config file", Qt.MatchContains | Qt.MatchRecursive):
        #     # print(item.text(0), item.text(1))
        #     # print(self.treeWidget.itemWidget(item, 1))
        #     filename = self.treeWidget.itemWidget(item,1).text() #+ "." + self.__class__.fileExtension
        #     break

        dialog = ParamTableDialog(self.metaData["hidden"]["parameters"], self.metaData["name"], mainWindow)
        if dialog.exec_() == QDialog.Accepted:
            self.metaData["hidden"]["parameters"] = dialog.metaData
            print("ParamTable:")
            utils.pp(dialog.metaData)
            # self._parent.node.scene.has_been_modified = True

    def onModeManagerButton(self):
        from main import mainWindow
        dialog = ModeMangerDialog(self.metaData["hidden"]["mode_manager"], self.metaData["name"], mainWindow)
        if dialog.exec_() == QDialog.Accepted:
            self.metaData["hidden"]["mode_manager"] = dialog.metaData
            print('ModeManager:')
            utils.pp(dialog.metaData)
            # self._parent.node.scene.has_been_modified = True

    def onSmartConverterButton(self):
        from main import mainWindow

        dialog = SmartConverterDialog(self.metaData["hidden"]["smart_converter"], self.metaData["name"], self.metaData['class'], mainWindow)
        if dialog.exec_() == QDialog.Accepted:
            self.metaData["hidden"]["smart_converter"] = dialog.metaData
            print('SmartConverter:')
            utils.pp(dialog.metaData)
            # self._parent.node.scene.has_been_modified = True

    def onErrorHandlerButton(self):
        from main import mainWindow

        dialog = ErrorHandlerDialog(self.metaData["hidden"]["error_handler"], self.metaData["name"], mainWindow)
        if dialog.exec_() == QDialog.Accepted:
            self.metaData["hidden"]["error_handler"] = dialog.metaData
            print('ErrorHandler:')
            utils.pp(dialog.metaData)
            # utils.pp(self.metaData["hidden"]["error_handler"])
            # self._parent.node.scene.has_been_modified = True

    def importData(self, data, tree):
        self.itemWidgetList = []

        for k, v in data.items():
            if k == "hidden": continue
            if isinstance(v, dict):
                item = QTreeWidgetItem([k, ''])
                self.parseDict(v, item)
            elif isinstance(v, list):  # enum
                pos = v[0]
                cbox = QComboBox()
                for item in v[1]:
                    cbox.addItem(item)
                cbox.setCurrentIndex(pos)
                item = QTreeWidgetItem([k, 'enum'])
                self.itemWidgetList.append([item, cbox])
            elif isinstance(v, bool):  # checkbox
                item = QTreeWidgetItem([k, 'bool'])
                if v:
                    item.setCheckState(1, Qt.Checked);
                else:
                    item.setCheckState(1, Qt.Unchecked);
            elif isinstance(v, int):
                item = QTreeWidgetItem([k, 'int'])
                spin = QSpinBox()
                line = QLineEdit()
                spin.setLineEdit(line)
                spin.setRange(0, 999)
                self.itemWidgetList.append([item, spin])
            else:
                item = QTreeWidgetItem([k, 'str'])  # str(v)])
                line = QLineEdit()
                line.setText(v)
                self.itemWidgetList.append([item, line])

            tree.addTopLevelItem(item)

        for item, widget in self.itemWidgetList:
            tree.setItemWidget(item, 1, widget)

    def exportData(self, tree, data):
        iterator = QTreeWidgetItemIterator(tree)
        for k, v in data.items():
            if k == "hidden": continue
            item = iterator.value()
            widget = item.treeWidget().itemWidget(item, 1)
            if isinstance(v, str):
                data[k] = widget.text()
                iterator += 1
            elif isinstance(v, list):
                data[k][0] = widget.currentIndex()
                iterator += 1
            elif isinstance(v, bool):
                if item.checkState(1) == Qt.Unchecked:
                    data[k] = False
                elif item.checkState(1) == Qt.Checked:
                    data[k] = True
                iterator += 1
            elif isinstance(v, int):
                data[k] = widget.value()
                iterator += 1
            elif isinstance(v, dict):
                iterator += 1
                iterator = self.parseTree(v, iterator)

    def serialize(self):
        res = super().serialize()
        res.update(self.metaData)
        return res

    def deserialize(self, data, hashmap={}):
        res = super().deserialize(data, hashmap)
        # print("deserialize:")
        self.metaData = copy.deepcopy(data)
        self.updateUI()

        return res

class HostComponentGraphicsNode(QDMGraphicsNode):
    def initSizes(self):
        super().initSizes()
        self.width = 350
        self.height = 350
        self.edge_roundness = 8.0
        self.edge_padding = 8.0
        self.title_height = 40.0
        self.title_horizontal_padding = 8.0
        self.title_vertical_padding = 4.0

    def initAssets(self):
        super().initAssets()
        self._title_font = QFont('Ubuntu', 14)
        # self.icons = QImage('icons/status_icons.png')

    # def paint(self, painter, QStyleOptionGraphicsItem, widget=None):
    #     super().paint(painter, QStyleOptionGraphicsItem, widget)
    #
    #     offset = 24.0
    #     if self.node.isDirty(): offset = 0.0
    #     if self.node.isInvalid(): offset = 48.0
    #
    #     painter.drawImage(
    #         QRectF(-10, -10, 24.0, 24.0),
    #         self.icons,
    #         QRectF(offset, 0, 24.0, 24.0)
    #     )

@register_node(PFSP_NODE_HOST_COMPONENT)
class PFSPNodeHostComponent(PFSPNode):
    icon = ''
    op_code = PFSP_NODE_HOST_COMPONENT
    op_title = 'HostComponent'
    content_label_objname = 'pfsp_node_host_component'

    def __init__(self, scene):
        super().__init__(scene, inputs=[], outputs=[])
        self.eval()

    def initInnerClasses(self):
        self.content = HostComponentContent(self)
        self.grNode = HostComponentGraphicsNode(self)
        #self.grNode.setToolTip('')

        # self.content.edit.textChanged.connect(self.onInputChanged)

    def onDoubleClicked(self, event):
        pass
