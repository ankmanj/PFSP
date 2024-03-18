import json
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *

from pfsp_gui.node_editor.node_editor_widget import NodeEditorWidget
from pfsp_gui.node_editor.node_node import Node
from pfsp_gui.node_editor.node_edge import EDGE_TYPE_BEZIER, EDGE_TYPE_DIRECT

import pfsp_gui.dialog.smart_converter_node_conf as SC_node_conf
from pfsp_gui import utils

DEBUG = False
DEBUG_CONTEXT = True

class SmartConverterSubWindow(NodeEditorWidget):
    def __init__(self, config_dict:dict=dict(), class_name:str='', parent=None):
        self.class_name = class_name
        super().__init__(parent)
        self.setAttribute(Qt.WA_DeleteOnClose)

        self.setTitle()

        self.initNewNodeActions()

        self.scene.addHasBeenModifiedListener(self.setTitle)
        self.scene.addDragEnterListener(self.onDragEnter)
        self.scene.addDropListener(self.onDrop)
        self.scene.setNodeClassSelector(self.getNodeClassFromData)

        self._close_event_listeners = []

        # print("config_dict: ", config_dict)
        if config_dict:
            QApplication.setOverrideCursor(Qt.WaitCursor)
            try:
                self.scene.deserialize(config_dict)
                self.scene.has_been_modified = False
                # clear history
                self.scene.history.clear()
                self.scene.history.storeInitialHistoryStamp()
            except Exception as e:
                utils.dumpException(e)
            finally:
                QApplication.restoreOverrideCursor()

    def getNodeClassFromData(self, data):
        if 'op_code' not in data: return Node
        return SC_node_conf.get_class_from_opcode(data['op_code'])

    def initNewNodeActions(self):
        self.node_actions = {}
        keys = list(SC_node_conf.SMART_CONVERTER_NODES.keys())
        keys.sort()
        for key in keys:
            node = SC_node_conf.SMART_CONVERTER_NODES[key]
            self.node_actions[node.op_code] = QAction(QIcon(node.icon), node.op_title)
            self.node_actions[node.op_code].setData(node.op_code)

    def initNodesContextMenu(self):
        context_menu = QMenu(self)
        keys = list(SC_node_conf.SMART_CONVERTER_NODES.keys())
        keys.sort()
        for key in keys: context_menu.addAction(self.node_actions[key])
        return context_menu

    def setTitle(self):
        self.setWindowTitle(self.getUserFriendlyFilename())

    def addCloseEventListener(self, callback):
        self._close_event_listeners.append(callback)

    def closeEvent(self, event):
        for callback in self._close_event_listeners:
            callback(self, event)

    def onDragEnter(self, event):
        # print('CalcSubWnd::~onDragEnter')
        # print('text: \'%s\''%event.mimeData().text())
        if event.mimeData().hasFormat(SC_node_conf.LISTBOX_MIMETYPE):
            event.acceptProposedAction()
        else:
            # print('... denied drag enter event')
            event.setAccepted(False)

    def onDrop(self, event):
        # print('CalcSubWnd::~onDrop')
        # print('text: \'%s\''%event.mimeData().text())
        if event.mimeData().hasFormat(SC_node_conf.LISTBOX_MIMETYPE):
            eventData = event.mimeData().data(SC_node_conf.LISTBOX_MIMETYPE)
            dataStream = QDataStream(eventData, QIODevice.ReadOnly)
            pixmap = QPixmap()
            dataStream >> pixmap
            op_code = dataStream.readInt()
            text = dataStream.readQString()

            mouse_position = event.pos()
            scene_position = self.scene.grScene.views()[0].mapToScene(mouse_position)
            if DEBUG: print('GOT DROP: [%d] \'%s\''%(op_code, text), 'mouse:', mouse_position, 'scene:', scene_position)

            try:
                # node = CalcNode(self.scene, op_code, text, inputs=[1,1], outputs=[2])
                node = SC_node_conf.get_class_from_opcode(op_code)(self.scene)
                node.setPos(scene_position.x(), scene_position.y())
                self.scene.history.storeHistory('Created Node %s'% node.__class__.__name__)
            except Exception as e:
                utils.dumpException(e)


            event.setDropAction(Qt.MoveAction)
            event.accept()
        else:
            print(' ... drop ignored, not requested format \'%s\''%SC_node_conf.LISTBOX_MIMETYPE)
            event.ignore()

    def contextMenuEvent(self, event):
        try:
            item = self.scene.getItemAt(event.pos())
            if DEBUG_CONTEXT: print(item)

            if type(item) == QGraphicsProxyWidget:
                item = item.widget()

            if hasattr(item, 'node') or hasattr(item, 'socket'):
                self.handleNodeContextMenu(event)
            elif hasattr(item, 'edge'):
                self.handleEdgeContextMenu(event)
            #elif item is None:
            else:
                self.handleNewNodeContextMenu(event)

            return super().contextMenuEvent(event)
        except Exception as e:
            utils.dumpException(e)

    def handleNodeContextMenu(self, event):
        # if DEBUG_CONTEXT: print('CONTEXT: NODE')
        # context_menu = QMenu(self)
        # markDirtyAct = context_menu.addAction('Mark Dirty')
        # markDirtyDescendantAct = context_menu.addAction('Mark Descendant Dirty')
        # markInvalidAct = context_menu.addAction('Mark Invalid')
        # unmarkInvalidAct = context_menu.addAction('Unmark Invalid')
        # evalAct = context_menu.addAction('Eval')
        # action = context_menu.exec_(self.mapToGlobal(event.pos()))
        #
        # selected = None
        # item = self.scene.getItemAt(event.pos())
        # if type(item) == QGraphicsProxyWidget:
        #     item = item.widget()
        #
        # if hasattr(item, 'node'): selected = item.node
        # if hasattr(item, 'socket'): selected = item.socket.node
        #
        # if DEBUG_CONTEXT: print('got item:', selected)
        # if selected and action == markDirtyAct: selected.markDirty()
        # if selected and action == markDirtyDescendantAct: selected.markDescendantsDirty()
        # if selected and action == markInvalidAct: selected.markInvalid()
        # if selected and action == unmarkInvalidAct: selected.markInvalid(False)
        # if selected and action == evalAct:
        #     val = selected.eval()
        #     if DEBUG_CONTEXT: print('EVALUATED:', val)

        if DEBUG_CONTEXT: print('CONTEXT: NODE')
        context_menu = QMenu(self)
        deleteAct = context_menu.addAction('Delete')
        action = context_menu.exec_(self.mapToGlobal(event.pos()))

        selected = None
        item = self.scene.getItemAt(event.pos())
        if type(item) == QGraphicsProxyWidget:
            item = item.widget()

        if hasattr(item, 'node'): selected = item.node
        if hasattr(item, 'socket'): selected = item.socket.node

        if DEBUG_CONTEXT: print('got item:', selected)
        if selected and action == deleteAct:
            self.scene.getView().deleteSelected()

        return

    def handleEdgeContextMenu(self, event):
        if DEBUG_CONTEXT: print('CONTEXT: EDGE')
        context_menu = QMenu(self)
        bezierAct = context_menu.addAction('Bezier Edge')
        directAct = context_menu.addAction('Direct Edge')
        action = context_menu.exec_(self.mapToGlobal(event.pos()))

        selected = None
        item = self.scene.getItemAt(event.pos())
        if hasattr(item, 'edge'):
            selected = item.edge

        if selected and action == bezierAct: selected.edge_type = EDGE_TYPE_BEZIER
        if selected and action == directAct: selected.edge_type = EDGE_TYPE_DIRECT

    def handleNewNodeContextMenu(self, event):
        if DEBUG_CONTEXT: print('CONTEXT: EMPTY SPACE')
        context_menu = self.initNodesContextMenu()
        action = context_menu.exec_(self.mapToGlobal(event.pos()))

        if action is not None:
            new_calc_node = SC_node_conf.get_class_from_opcode(action.data())(self.scene)
            scene_pos = self.scene.getView().mapToScene(event.pos())
            new_calc_node.setPos(scene_pos.x(), scene_pos.y())
            if DEBUG_CONTEXT: print('Selected node:', new_calc_node)

    def fileExport(self, filename):
        """
        Export this `Scene` to the user-specific file on disk.

        :param filename: where to export this scene
        :type filename: ``str``
        """
        with open(filename, 'w') as file:
            raw_dict = self.scene.serialize()
            #utils.pp(raw_dict)
            # TODO: second parsing as the type of data_map_gen
            id_dict = {}
            for item in raw_dict['nodes']:
                if item['title'] == 'Data Structure':
                    for x in item['inputs']:
                        key = f"object_member_{x['index']}"
                        member_name,member_type = item['content'][key].split(':')
                        id_dict[str(x['id'])] = {'classname':item['content']['object_class'], 'type':member_type, 'name':member_name}

                    for x in item['outputs']:
                        key = f"object_member_{x['index']}"
                        member_name,member_type = item['content'][key].split(':')
                        id_dict[str(x['id'])] = {'classname':item['content']['object_class'], 'type':member_type, 'name':member_name}
            print('id_dict:')
            utils.pp(id_dict)

            mapping_list = []
            for item in raw_dict['edges']:
                mapping_list.append([item['start'], item['end']])
            print('mapping_list:')
            utils.pp(mapping_list)

            return

            reshaped_dict = {}
            for item in raw_dict['nodes']:
                if item['title'] == 'SmartConverter - External Input To Internal' or item['title'] == 'SmartConverter - Internal To External Output':
                    if item['title'] not in reshaped_dict:
                        include_files = [None]
                        if item['title'] == 'SmartConverter - External Input To Internal':
                            include_files = ['pfsp_core_smart_converter_int.hpp']
                        elif item['title'] == 'SmartConverter - Internal To External Output':
                            include_files = ['pfsp_core_smart_converter_ext.hpp']

                        reshaped_dict[item['title']] = {
                            'function': item['content']['function'],
                            'includes': include_files,
                            'class_initial': item['content']['class_initial'],
                            'data_map': {}
                        }

                    assert(len(item['inputs']) == len(item['outputs']))
                    no_pipelines = len(item['inputs'])
                    for i in range(no_pipelines):
                        in_socket = item['inputs'][i]
                        in_remote_id = None
                        for x in mapping_list:
                            if x[0] == in_socket['id']:
                                in_remote_id = str(x[1])
                                break
                            elif x[1] == in_socket['id']:
                                in_remote_id = str(x[0])
                                break
                        in_instance = id_dict[in_remote_id]

                        out_socket = [ x for x in item['outputs'] if x['index'] == in_socket['index'] ][0]
                        out_remote_id = None
                        for x in mapping_list:
                            if x[0] == out_socket['id']:
                                out_remote_id = str(x[1])
                                break
                            elif x[1] == out_socket['id']:
                                out_remote_id = str(x[0])
                                break

                        out_instance = id_dict[out_remote_id]
                        if out_instance['classname'] not in reshaped_dict[item['title']]['data_map']:
                            reshaped_dict[item['title']]['data_map'][out_instance['classname']] = []

                        pipeline = {
                            'dst_var_name':out_instance['name'], 'dtype':out_instance['type'],
                            'from':in_instance['classname'], 'assigne':in_instance['name'], 'stype':in_instance['type']
                        }

                        reshaped_dict[item['title']]['data_map'][out_instance['classname']].append(pipeline)

            file.write(json.dumps(reshaped_dict, indent=4))

