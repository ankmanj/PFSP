from PyQt5.QtWidgets import *

from pfsp_gui.node_editor.node_node import Node
from pfsp_gui.node_editor.node_content_widget import QDMNodeContentWidget
from pfsp_gui.node_editor.node_graphics_node import QDMGraphicsNode
from pfsp_gui.node_editor.node_socket import LEFT_CENTER, RIGHT_CENTER


class PFSPGraphicsNode(QDMGraphicsNode):
    def initSizes(self):
        super().initSizes()
        self.width = 200
        self.height = 200
        self.edge_roundness = 6
        self.edge_padding = 0
        self.title_horizontal_padding = 8
        self.title_vertical_padding = 10

    def initAssets(self):
        super().initAssets()
        # self.icons = QImage('icons/status_icons.png')

    def paint(self, painter, QStyleOptionGraphicsItem, widget=None):
        super().paint(painter, QStyleOptionGraphicsItem, widget)

        # offset = 24.0
        # if self.node.isDirty(): offset = 0.0
        # if self.node.isInvalid(): offset = 48.0
        #
        # painter.drawImage(
        #     QRectF(-10, -10, 24.0, 24.0),
        #     self.icons,
        #     QRectF(offset, 0, 24.0, 24.0)
        # )

class PFSPContent(QDMNodeContentWidget):
    def initUI(self):
        lbl = QLabel(self.node.content_label, self)
        lbl.setObjectName(self.node.content_label_objname)

class PFSPNode(Node):
    icon = ''
    op_code = 0
    op_title = 'Undefined'
    content_label = ''
    content_label_objname = 'pfsp_node_bg'

    def __init__(self, scene, inputs=[], outputs=[1]):
        super().__init__(scene, self.__class__.op_title, inputs, outputs)

        self.value = None

        # it's really important to mark all nodes Dirty by default
        # self.markDirty()

    def initInnerClasses(self):
        self.content = PFSPContent(self)
        self.grNode = PFSPGraphicsNode(self)

    def initSettings(self):
        super().initSettings()
        self.input_socket_position = LEFT_CENTER
        self.output_socket_position = RIGHT_CENTER

    # def evalImplementation(self):
    #     return 123
    #
    # def eval(self):
    #     if not self.isDirty() and not self.isInvalid():
    #         print(' _> returning cached %s value:'%self.__class__.__name__, self.value)
    #         return self.value
    #
    #     try:
    #         val = self.evalImplementation()
    #
    #         self.markDirty(False)
    #         self.markInvalid(False)
    #
    #         return val
    #     except Exception as e:
    #         self.markInvalid()
    #         dumpException(e)
    #
    #
    # def onInputChanged(self, new_edge):
    #     print('%s::__onInputChanged'%self.__class__.__name__)
    #     self.markDirty()
    #     self.eval()

    def serialize(self):
        res = super().serialize()
        res['op_code'] = self.__class__.op_code
        return res

    def deserialize(self, data, hashmap={}, restore_id=True):
        res = super().deserialize(data, hashmap, restore_id)
        print('Deserialized PFSPNode \'%s\''%self.__class__.__name__, 'res:', res)
        return res
