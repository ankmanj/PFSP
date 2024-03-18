from pfsp_gui.dialog.smart_converter_node_conf import *
from pfsp_gui.dialog.smart_converter_node_base import *
from pfsp_gui.node_editor.node_socket import *
from pfsp_gui.utils import *

class SmartConverterToExtContent(QDMNodeContentWidget):
    def __init__(self, node:'Node', parent:QWidget=None):
        self.node = node
        super().__init__(node, parent)

    def initUI(self):
        self.vboxLayout = QVBoxLayout()
        self.vboxLayout.setContentsMargins(0,0,0,0)
        self.setLayout(self.vboxLayout)

        self.classPrefixEdit = QLineEdit(self.node.class_name, self)
        self.classPrefixEdit.setEnabled(False)
        hbox = QHBoxLayout()
        hbox.addWidget(QLabel('Class : ',self))
        hbox.addWidget(self.classPrefixEdit)
        self.vboxLayout.addLayout(hbox)

        self.functionEdit = QLineEdit("getConvertedExternalData", self)
        self.functionEdit.setEnabled(False)
        #self.edit.setAlignment(Qt.AlignRight)
        self.functionEdit.setObjectName(self.node.content_label_objname)

        plus_button = QPushButton('+', self)
        plus_button.setFixedWidth(30)
        plus_button.clicked.connect(lambda x:self.node.addContent())

        minus_button = QPushButton('-', self)
        minus_button.setFixedWidth(30)
        minus_button.clicked.connect(lambda x:self.node.deleteContent())

        hbox = QHBoxLayout()
        hbox.addWidget(QLabel('Func : ', self))
        hbox.addWidget(self.functionEdit, 1) # with stretch
        hbox.addWidget(plus_button)
        hbox.addWidget(minus_button)
        self.vboxLayout.addLayout(hbox)
        self.objectTypeList = []
        self.adjustSize()

    def serialize(self):
        res = super().serialize()
        res['class_initial'] = self.classPrefixEdit.text()
        # res['class_remain'] = self.classPostfixEdit.text()
        res['function'] = self.functionEdit.text()
        res['pipelines'] = len(self.objectTypeList)
        # res['value'] = self.edit.text()
        return res

    def deserialize(self, data, hashmap={}):
        res = super().deserialize(data, hashmap)
        try:
            self.classPrefixEdit.setText(data['class_initial'])
            # self.classPostfixEdit.setText(data['class_remain'])
            self.classPostfixEdit = QLineEdit('class name of template_out', self)
            self.functionEdit.setText(data['function'])
            for i in range(data['pipelines']):
                self.node.addContent(with_socket=False)
            return True & res
        except Exception as e:
            dumpException(e)
        return res

class SmartConverterToExtGraphicsNode(QDMGraphicsNode):
    def initSizes(self):
        super().initSizes()
        # self.dh = 36
        self.edge_roundness = 8
        self.edge_padding = 8

        self.title_height = 40
        self.title_horizontal_padding = 8
        self.title_vertical_padding = 4

        self.width = 460
        self.height = self.title_height + 2 * self.edge_padding + 2*self.node.var_height
        # self.height = self.title_height + 2 * self.edge_padding + 1*self.node.var_height

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

@register_node(SMART_CONVERTER_NODE_TO_EXT)
class PFSPNodeSmartConverterExternal(SmartConverterNode):
    icon = ''
    op_code = SMART_CONVERTER_NODE_TO_EXT
    op_title = 'SmartConverter - Internal To External Output'
    content_label_objname = 'pfsp_node_smart_converter_to_ext'
    class_name = ''

    def __init__(self, scene):
        self.var_height = 35
        super().__init__(scene, inputs=[], outputs=[])
        self.eval()

    def initInnerClasses(self):
        self.content = SmartConverterToExtContent(self)
        self.grNode = SmartConverterToExtGraphicsNode(self)
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

    def addContent(self, with_socket=True):
        var_name = QLineEdit("------------>", self.content)
        var_name.setEnabled(False)
        var_name.setAlignment(Qt.AlignCenter)
        self.content.objectTypeList.append(var_name)
        self.grNode.adjustHeight(self.var_height)
        self.content.vboxLayout.addWidget(var_name)
        # TODO: add socket
        if with_socket: self.addSockets(inputs=[1], outputs=[4])

    def deleteContent(self):
        if len(self.content.objectTypeList) > 0:
            # print(self.content.height())
            var_name = self.content.objectTypeList.pop()
            self.grNode.adjustHeight(-self.var_height)
            self.content.vboxLayout.removeWidget(var_name)
            # self.content.vboxLayout.removeItem(var_name)
            # TODO: remove socket
            socket = self.inputs.pop()
            self.scene.grScene.removeItem(socket.grSocket)
            socket = self.outputs.pop()
            self.scene.grScene.removeItem(socket.grSocket)

    def getSocketPosition(self, index, position, num_out_of=1):
        x = 0 if position in (LEFT_TOP, LEFT_CENTER, LEFT_BOTTOM) else self.grNode.width

        top_offset = self.grNode.title_height + self.grNode.edge_padding
        top_offset += 2*self.var_height + self.var_height/2.0
        y = top_offset + index * self.var_height

        # if x == self.grNode.width:
        #     top_offset = self.grNode.title_height + 2 * self.grNode.title_vertical_padding
        #     if index == 0:
        #         # top_offset += self.grNode.edge_padding + 30
        #         y = top_offset + index * self.var_height
        #     else:
        #         top_offset += self.grNode.edge_padding + 30
        #         y = top_offset + (index+1) * self.var_height
        # else:
        #     top_offset = self.grNode.title_height + 2 * self.grNode.title_vertical_padding + self.grNode.edge_padding
        #     top_offset += self.grNode.edge_padding + 30
        #     y = top_offset + (index+2) * self.var_height

        return [x, y]
