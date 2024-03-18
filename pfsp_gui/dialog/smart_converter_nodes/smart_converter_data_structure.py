from pfsp_gui.dialog.smart_converter_node_conf import *
from pfsp_gui.dialog.smart_converter_node_base import *
from pfsp_gui.node_editor.node_socket import *
from pfsp_gui.utils import *

typeContents = [
    'int8_t', 'int16_t', 'int32_t', 'int64_t', 'uint8_t', 'uint16_t', 'uint32_t', 'uint64_t',
    'float', 'double', 'std::string'
]

class SmartConverterDataStructureContent(QDMNodeContentWidget):
    def initUI(self):
        self.vboxLayout = QVBoxLayout()
        self.vboxLayout.setContentsMargins(0,0,0,0)
        self.setLayout(self.vboxLayout)

        self.objectTypeEdit = QLineEdit("Insert type name", self)
        #self.edit.setAlignment(Qt.AlignRight)
        self.objectTypeEdit.setObjectName(self.node.content_label_objname)

        plus_button = QPushButton('+', self)
        plus_button.setFixedWidth(30)
        plus_button.clicked.connect(lambda x: self.node.addContent())

        minus_button = QPushButton('-', self)
        minus_button.setFixedWidth(30)
        minus_button.clicked.connect(lambda x: self.node.deleteContent())

        hboxLayout = QHBoxLayout()
        hboxLayout.addWidget(self.objectTypeEdit, 1) # with stretch
        hboxLayout.addWidget(plus_button)
        hboxLayout.addWidget(minus_button)

        self.vboxLayout.addLayout(hboxLayout)
        self.objectTypeList = []
        # self.setFixedHeight(dh)
        self.adjustSize()

    def serialize(self):
        res = super().serialize()
        res['object_class'] = self.objectTypeEdit.text()
        for i,items in enumerate(self.objectTypeList):
            key = f'object_member_{i}'
            res[key] = items[3].text() + ':' + items[1].currentText()
        # res['value'] = self.edit.text()
        return res

    def deserialize(self, data, hashmap={}):
        res = super().deserialize(data, hashmap)
        try:
            value = data['object_class']
            self.objectTypeEdit.setText(value)
            for i in range(len(data.keys())-1):
                key = f'object_member_{i}'
                name,type = data[key].split(':')
                self.node.addContent(name, type, with_socket=False)

            #self.edit.setText(value)
            return True & res
        except Exception as e:
            dumpException(e)
        return res

class SmartConverterDataStructureGraphicsNode(QDMGraphicsNode):
    def initSizes(self):
        super().initSizes()

        # self.dh = 36
        self.edge_roundness = 8
        self.edge_padding = 8

        self.title_height = 40
        self.title_horizontal_padding = 8
        self.title_vertical_padding = 4

        self.width = 300
        self.height = self.title_height + self.edge_padding + self.node.var_height + self.edge_padding

    def adjustHeight(self, delta):
        self.height += delta
        self.content.setFixedHeight(self.content.height() + delta)

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

@register_node(SMART_CONVERTER_NODE_DATA_STRUCTURE)
class PFSPNodeSmartConverterDataStructure(SmartConverterNode):
    icon = ''
    op_code = SMART_CONVERTER_NODE_DATA_STRUCTURE
    op_title = 'Data Structure'
    content_label_objname = 'pfsp_node_smart_converter_data_structure'

    def __init__(self, scene):
        self.var_height = 30+5
        super().__init__(scene, inputs=[], outputs=[])
        # self.eval()

    def initInnerClasses(self):
        self.content = SmartConverterDataStructureContent(self)
        self.grNode = SmartConverterDataStructureGraphicsNode(self)
        # self.grNode.adjustSize(self.content)
        # self.grNode.width,self.grNode.height = self.content.width(), self.content.height()+self.
        #self.grNode.setToolTip('')
        # self.content.edit.textChanged.connect(self.onInputChanged)

    def addSockets(self, inputs, outputs):
        # create new sockets
        counter = len(self.inputs)
        for item in inputs:
            socket = Socket(node=self, index=counter, position=self.input_socket_position,
                            socket_type=item, multi_edges=self.input_multi_edged,
                            count_on_this_node_side=len(inputs)+len(self.inputs),is_input=True
            )
            counter += 1
            self.inputs.append(socket)

        counter = len(self.outputs)
        for item in outputs:
            socket = Socket(node=self, index=counter, position=self.output_socket_position,
                            socket_type=item, multi_edges=self.output_multi_edged,
                            count_on_this_node_side=len(outputs)+len(self.outputs), is_input=False
            )
            counter += 1
            self.outputs.append(socket)

    def addContent(self, name:str='Insert var name', type_name:str='Insert var type', with_socket:bool=True):
        # print(name, type)
        var_name = QLineEdit(name)
        var_link = QLabel(' ')
        # var_type = QLineEdit(type_name)
        var_type = QComboBox()
        var_type.setStyleSheet("QComboBox {background-color: gray;}")
        var_type.clear()
        var_type.addItems(typeContents)
        var_type.setCurrentIndex(typeContents.index(type_name) if type_name in typeContents else 0)
        # index = var_type.findText('uint8_t')
        # var_type.set

        hbox = QHBoxLayout()
        hbox.addWidget(var_type)
        hbox.addWidget(var_link)
        hbox.addWidget(var_name)
        self.content.objectTypeList.append([hbox, var_type, var_link, var_name])
        self.grNode.adjustHeight(self.var_height)
        # self.content.vboxLayout.addWidget(var_name)
        self.content.vboxLayout.addLayout(hbox)
        # TODO: add socket
        if with_socket: self.addSockets(inputs=[1], outputs=[4])

    def deleteContent(self):
        if len(self.content.objectTypeList) > 0:
            # print(self.content.height())
            hbox,var_type,var_link,var_name = self.content.objectTypeList.pop()
            self.grNode.adjustHeight(-self.var_height)
            # self.content.vboxLayout.removeWidget(var_name)
            self.content.vboxLayout.removeItem(hbox)
            hbox.removeWidget(var_name)
            hbox.removeWidget(var_link)
            hbox.removeWidget(var_type)
            # TODO: remove socket
            socket = self.inputs.pop()
            self.scene.grScene.removeItem(socket.grSocket)
            socket = self.outputs.pop()
            self.scene.grScene.removeItem(socket.grSocket)

    def getSocketPosition(self, index, position, num_out_of=1):
        x = 0 if position in (LEFT_TOP, LEFT_CENTER, LEFT_BOTTOM) else self.grNode.width

        top_offset = self.grNode.title_height + self.grNode.edge_padding
        top_offset += 36 + self.grNode.edge_padding
        y = top_offset + index * self.var_height + 9

        # if x == 0:
        #     top_offset = self.grNode.title_height + 2 * self.grNode.title_vertical_padding
        #     # top_offset += self.grNode.edge_padding + 30
        #     y = top_offset + index * self.var_height + 5
        # else:
        #     top_offset = self.grNode.title_height + 2 * self.grNode.title_vertical_padding + self.grNode.edge_padding
        #     top_offset += self.grNode.edge_padding + 30
        #     y = top_offset + index * self.var_height + 10

        return [x, y]
