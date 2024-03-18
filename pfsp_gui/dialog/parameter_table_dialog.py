from PyQt5.QtCore import *
from pfsp_gui.node_editor.node_serializeable import Serializable
from pfsp_gui.window.pfsp_node_base import *


# parameterTemplate = [ {"name": "HostAppLibName", "value": "libHOST_app_component.so"} ]

class ParamTableDialog(QDialog, Serializable):
    title = "The Service Configuration"

    def __init__(self, parameter_list:list=[], parent_name:str="", parent=None):
        super().__init__(parent) #, Qt.Popup | Qt.Dialog | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint | Qt.WindowStaysOnTopHint)

        if parameter_list is not None and len(parameter_list) > 0:
            parameter_list[0] = {"name": "HostComponentLibName", "value": f"libPFSP_{parent_name}.so"}
            self.metaData = parameter_list
        else:
            self.metaData = [ {"name": "HostComponentLibName", "value": f"libPFSP_{parent_name}.so"} ]

        self.initUI()
        # self.setModal(True)

        # name,ext = os.path.splitext(filename)
        # self.filename = name + self.__class__.file_extension
        # if os.path.isfile(self.filename):
        #     # TODO: json file exported into dict
        #     if self.filename.endswith(".json"):
        #         with open(self.filename, "r") as json_string:
        #             self.metaData = json.load(json_string)
        #             if "parameter" in self.metaData["PFSPMiddleware"]["parameterTable"]:
        #                 for d in self.metaData["PFSPMiddleware"]["parameterTable"]["parameter"]:
        #                     for k, v in d.items():
        #                         self.addContent(key=k, value=v)
        #     elif self.filename.endswith(".xml"):
        #         with open(self.filename, 'r') as f:
        #             self.metaData = xmltodict.parse(f.read(), encoding="ISO-8859-1", xml_attribs=True)
        #             # utils.pp(self.metaData)
        #             if self.metaData["PFSPMiddleware"]["parameterTable"] is None:
        #                 self.metaData["PFSPMiddleware"]["parameterTable"] = { "parameter":[] }
        #             else:
        #                 for d in self.metaData["PFSPMiddleware"]["parameterTable"]["parameter"]:
        #                     for k, v in d.items():
        #                         self.addContent(key=k[1:], value=v)
        # else:
        #     self.metaData = self.__class__.config_dict.copy()

        self.setWindowTitle(self.__class__.title + " - " + parent_name)
        # self.show()

    def initUI(self):

        self.contents = []

        self.resize(450, 300)
        self.vboxLayout = QVBoxLayout()
        self.vboxLayout.setContentsMargins(8, 8, 8, 8)

        plus_button = QPushButton('+', self)
        plus_button.setFixedHeight(40)
        plus_button.clicked.connect(self.onAddButton)

        minus_button = QPushButton('-', self)
        minus_button.setFixedHeight(40)
        minus_button.clicked.connect(self.onDelButton)

        hboxLayout = QHBoxLayout()
        # hboxLayout.addWidget(self.titleEdit, 1)  # with stretch
        hboxLayout.addWidget(plus_button)
        hboxLayout.addWidget(minus_button)

        self.vboxLayout.addLayout(hboxLayout)

        self.treeWidget = QTreeWidget()
        # self.treeWidget.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        # self.treeWidget.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        # self.treeWidget.resize(400, 700)
        self.treeWidget.setColumnCount(2)
        self.treeWidget.setHeaderLabels(['Name', 'Value'])

        for i,item in enumerate(self.metaData):
            if i == 0:
                self.addContent(key=item["name"], value=item["value"][3:-3], update_list=False)
            else:
                self.addContent(key=item["name"], value=item["value"], update_list=False)
        # self.importData(self.metaData, self.treeWidget)
        # self.treeWidget.header().setSectionResizeMode(0, QHeaderView.ResizeToContents)
        # self.treeWidget.header().show()
        self.treeWidget.resizeColumnToContents(0)

        self.treeWidget.expandAll()

        # scroll = QScrollArea()
        # scroll.setVerticalScrollBarPolicy(Qt.ScrollBarAlwaysOn)
        # scroll.setHorizontalScrollBarPolicy(Qt.ScrollBarAlwaysOff)
        # # scroll.setWidgetResizable(True)
        # scroll.setWidget(self.treeWidget)
        # self.vboxLayout.addWidget(scroll)
        self.vboxLayout.addWidget(self.treeWidget)

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

        # self.layout.addWidget(self.treeWidget)
        self.setLayout(self.vboxLayout)

    def addContent(self, key:str, value:str, update_list:bool=True):
        item = QTreeWidgetItem([key, "str"])  #
        item.setFlags(item.flags() | Qt.ItemIsEditable);
        self.treeWidget.addTopLevelItem(item)
        line = QLineEdit(value)
        self.treeWidget.setItemWidget(item, 1, line)
        self.contents.append(item)
        if update_list:
            self.metaData.append({"name": key, "value": value})

    def delContent(self, update_list:bool=True):
        if len(self.contents) <= 1: return
        self.treeWidget.removeItemWidget(
            self.treeWidget.takeTopLevelItem(
                self.treeWidget.indexOfTopLevelItem(self.contents.pop())
            ),
            0
        )
        if update_list:
            self.metaData.pop()

    def onAddButton(self):
        content_count = len(self.contents)
        self.addContent(f"Insert name{content_count}", f"Insert value{content_count}")

    def onDelButton(self):
        self.delContent()

    def onOkButton(self):
        # TODO: QTreeWidget -> self.metaData
        # utils.pp(self.metaData)
        root = self.treeWidget.invisibleRootItem()
        for i in range(1,root.childCount()):
            item = root.child(i)
            # print(item.text(0), self.treeWidget.itemWidget(item,1).text())
            self.metaData[i]['name'] = item.text(0)
            self.metaData[i]['value'] = self.treeWidget.itemWidget(item,1).text()

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
    wnd = ParamTableDialog(parameter_list=[], parent=None)
    wnd.show()
    sys.exit(app.exec_())
