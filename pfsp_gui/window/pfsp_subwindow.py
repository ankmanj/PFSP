import os, json

from collections import OrderedDict

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *

from pfsp_gui.node_editor.node_editor_widget import NodeEditorWidget
from pfsp_gui.node_editor.node_node import Node
from pfsp_gui.node_editor.node_edge import EDGE_TYPE_DIRECT, EDGE_TYPE_BEZIER
from pfsp_gui.window.pfsp_node_conf import *

from pfsp_gui import utils
from xml.dom.minidom import parseString
import xmltodict

DEBUG = False
DEBUG_CONTEXT = False

class PFSPSubWindow(NodeEditorWidget):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setAttribute(Qt.WA_DeleteOnClose)

        self.setTitle()

        self.initNewNodeActions()

        self.scene.addHasBeenModifiedListener(self.setTitle)
        self.scene.addDragEnterListener(self.onDragEnter)
        self.scene.addDropListener(self.onDrop)
        self.scene.setNodeClassSelector(self.getNodeClassFromData)

        self._close_event_listeners = []

    def getNodeClassFromData(self, data):
        if 'op_code' not in data: return Node
        return get_class_from_opcode(data['op_code'])

    def initNewNodeActions(self):
        self.node_actions = {}
        keys = list(PFSP_NODES.keys())
        keys.sort()
        for key in keys:
            node = PFSP_NODES[key]
            self.node_actions[node.op_code] = QAction(QIcon(node.icon), node.op_title)
            self.node_actions[node.op_code].setData(node.op_code)

    def initNodesContextMenu(self):
        context_menu = QMenu(self)
        keys = list(PFSP_NODES.keys())
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
        if event.mimeData().hasFormat(LISTBOX_MIMETYPE):
            event.acceptProposedAction()
        else:
            # print('... denied drag enter event')
            event.setAccepted(False)

    def onDrop(self, event):
        # print('CalcSubWnd::~onDrop')
        # print('text: \'%s\''%event.mimeData().text())
        if event.mimeData().hasFormat(LISTBOX_MIMETYPE):
            eventData = event.mimeData().data(LISTBOX_MIMETYPE)
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
                node = get_class_from_opcode(op_code)(self.scene)
                node.setPos(scene_position.x(), scene_position.y())
                self.scene.history.storeHistory('Created Node %s'% node.__class__.__name__, setModified=True)
            except Exception as e:
                utils.dumpException(e)

            event.setDropAction(Qt.MoveAction)
            event.accept()
        else:
            print(' ... drop ignored, not requested format \'%s\''%LISTBOX_MIMETYPE)
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
        if DEBUG_CONTEXT: print('CONTEXT: NODE')
        context_menu = QMenu(self)
        markDirtyAct = context_menu.addAction('Mark Dirty')
        markDirtyDescendantAct = context_menu.addAction('Mark Descendant Dirty')
        markInvalidAct = context_menu.addAction('Mark Invalid')
        unmarkInvalidAct = context_menu.addAction('Unmark Invalid')
        evalAct = context_menu.addAction('Eval')
        action = context_menu.exec_(self.mapToGlobal(event.pos()))

        selected = None
        item = self.scene.getItemAt(event.pos())
        if type(item) == QGraphicsProxyWidget:
            item = item.widget()

        if hasattr(item, 'node'): selected = item.node
        if hasattr(item, 'socket'): selected = item.socket.node

        if DEBUG_CONTEXT: print('got item:', selected)
        if selected and action == markDirtyAct: selected.markDirty()
        if selected and action == markDirtyDescendantAct: selected.markDescendantsDirty()
        if selected and action == markInvalidAct: selected.markInvalid()
        if selected and action == unmarkInvalidAct: selected.markInvalid(False)
        if selected and action == evalAct:
            val = selected.eval()
            if DEBUG_CONTEXT: print('EVALUATED:', val)

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
            new_calc_node = get_class_from_opcode(action.data())(self.scene)
            scene_pos = self.scene.getView().mapToScene(event.pos())
            new_calc_node.setPos(scene_pos.x(), scene_pos.y())
            if DEBUG_CONTEXT: print('Selected node:', new_calc_node)

    def parseSmartConverter(self, smart_converter_data: dict):
        connections = list()
        pipelines = list()
        node_content = dict()

        inbound_ids = list()
        internal_ids = list()
        internalFuncName = None
        internalInitClass = None
        outbound_ids = list()
        outboundFuncName = None
        outboundInitClass = None

        # Parsing connections
        for edge in smart_converter_data['edges']:
            connections.append([edge['start'], edge['end']])

        for item in smart_converter_data['nodes']:
            if DEBUG: print(item['title'])
            assert (len(item['inputs']) == len(item['outputs']))

            item_pipelines = dict()

            # Parsing inputs
            for dot_node in item['inputs']:
                key = dot_node['index']
                if key in item_pipelines.keys():
                    raise ValueError(f"{key} already exists more than 1 in {item['title']}")
                item_pipelines[key] = [dot_node['id']]

            # Parsing outputs
            for dot_node in item['outputs']:
                key = dot_node['index']
                if key not in item_pipelines.keys():
                    raise ValueError(f"{key} does not exist in {item_pipelines.keys()}")
                item_pipelines[key].append(dot_node['id'])

            if DEBUG:
                print('private pipelines:')
                utils.pp(item_pipelines)

            if item['title'] == 'SmartConverter - External Input To Internal':
                # Merge private pipelines into all-in-one pipelines
                pipelines.extend(item_pipelines.values())
                for pipe in item_pipelines.values():
                    inbound_ids.append(pipe[0])
                    internal_ids.append(pipe[1])
                internalFuncName = item['content']['function']
                internalInitClass = item['content']['class_initial']
            elif item['title'] == 'SmartConverter - Internal To External Output':
                # Merge private pipelines into all-in-one pipelines
                pipelines.extend(item_pipelines.values())
                for pipe in item_pipelines.values():
                    internal_ids.append(pipe[0])
                    outbound_ids.append(pipe[1])
                outboundFuncName = item['content']['function']
                outboundInitClass = item['content']['class_initial']
            elif item['title'] == 'Data Structure':
                # Parsing content
                item_content = item['content']
                class_name = item_content['object_class']
                for k in item_pipelines.keys():
                    if f'object_member_{k}' not in item_content.keys():
                        raise ValueError(f"{k} does not exist in {item_content.keys()}")

                    # Inbound node content
                    from_id = item_pipelines[k][0]
                    if from_id in node_content.keys():
                        raise ValueError(f"{from_id} already exists in {node_content.keys()}")
                    node_content[from_id] = {
                        'class_name': class_name,
                        'variable_type': item_content[f'object_member_{k}'].split(':')[-1],
                        'variable_name': item_content[f'object_member_{k}'].split(':')[0]
                    }

                    # Outbound node content
                    to_id = item_pipelines[k][1]
                    if to_id in node_content.keys():
                        raise ValueError(f"{to_id} already exists in {node_content.keys()}")
                    node_content[to_id] = {
                        'class_name': class_name,
                        'variable_type': item_content[f'object_member_{k}'].split(':')[-1],
                        'variable_name': item_content[f'object_member_{k}'].split(':')[0]
                    }

        # utils.pp(item)
        if DEBUG:
            print('Connections:')
            utils.pp(connections)
            print('Pipelines:')
            utils.pp(pipelines)
            print('Node content:')
            utils.pp(node_content)
            print('Inbound ids:')
            utils.pp(inbound_ids)
            print('Internal ids:')
            utils.pp(internal_ids)
            print('Outbound ids:')
            utils.pp(outbound_ids)

        inboundClasses = dict()
        for p in inbound_ids:
            origin_p = None
            for edge_p, edge_q in connections:
                if p == edge_p:
                    origin_p = edge_q
                elif p == edge_q:
                    origin_p = edge_p
            if origin_p is not None:
                origin_var = node_content[origin_p]
                if origin_var['class_name'] not in inboundClasses.keys():
                    inboundClasses[origin_var['class_name']] = {
                        origin_var['variable_name']: origin_var['variable_type']
                    }
                else:
                    existing_class = inboundClasses[origin_var['class_name']]
                    if origin_var['variable_name'] in existing_class.keys():
                        if DEBUG: print(f" Existing '{origin_var['variable_type']} {origin_var['variable_name']}'")
                    existing_class[origin_var['variable_name']] = origin_var['variable_type']

        internalClasses = dict()
        for p in internal_ids:
            origin_p = None
            for edge_p, edge_q in connections:
                if p == edge_p:
                    origin_p = edge_q
                elif p == edge_q:
                    origin_p = edge_p
            if origin_p is not None:
                origin_var = node_content[origin_p]
                if origin_var['class_name'] not in internalClasses.keys():
                    internalClasses[origin_var['class_name']] = {
                        origin_var['variable_name']: origin_var['variable_type']
                    }
                else:
                    existing_class = internalClasses[origin_var['class_name']]
                    if origin_var['variable_name'] in existing_class.keys():
                        if DEBUG: print(f" Existing '{origin_var['variable_type']} {origin_var['variable_name']}'")
                    existing_class[origin_var['variable_name']] = origin_var['variable_type']

        outboundClasses = dict()
        for p in outbound_ids:
            origin_p = None
            for edge_p, edge_q in connections:
                if p == edge_p:
                    origin_p = edge_q
                elif p == edge_q:
                    origin_p = edge_p
            if origin_p is not None:
                origin_var = node_content[origin_p]
                if origin_var['class_name'] not in outboundClasses.keys():
                    outboundClasses[origin_var['class_name']] = {
                        origin_var['variable_name']: origin_var['variable_type']
                    }
                else:
                    existing_class = outboundClasses[origin_var['class_name']]
                    if origin_var['variable_name'] in existing_class.keys():
                        print(f" Existing '{origin_var['variable_type']} {origin_var['variable_name']}'")
                    existing_class[origin_var['variable_name']] = origin_var['variable_type']

        if DEBUG:
            print('inboundClasses:')
            utils.pp(inboundClasses)
            print('internalClasses:')
            utils.pp(internalClasses)
            print('outboundClasses:')
            utils.pp(outboundClasses)

        mapping = list()
        for pipe_p, pipe_q in pipelines:
            origin_p = None
            for edge_p, edge_q in connections:
                if pipe_p == edge_p:
                    origin_p = edge_q
                elif pipe_p == edge_q:
                    origin_p = edge_p

            origin_q = None
            for edge_p, edge_q in connections:
                if pipe_q == edge_p:
                    origin_q = edge_q
                elif pipe_q == edge_q:
                    origin_q = edge_p

            if origin_p is not None and origin_q is not None:
                mapping.append([node_content[origin_p], node_content[origin_q]])

        if DEBUG:
            print('Mapping:')
            utils.pp(mapping)

        return inboundClasses, internalClasses, internalInitClass, internalFuncName, outboundClasses, outboundInitClass, outboundFuncName, mapping

    def fileExportAsXML(self, filename):
        assert(filename.endswith(".xml"))

        config_dict = {
            'PFSPMiddleware': {
                '@version': '1.0',
                '@xmlns': 'http://www.dmg.org/PMML-3-0',
                '@xmlns:xsi': 'http://www.w3.org/2001/XMLSchema_instance',
                'Header': {
                    '@xmlns': '',
                    '@copyright': '',
                    '#text': 'The PFSPMiddleware System Configuration for PFSPArchitecture',
                    'Application': {'@name': '/PFSP_MW', '@version': '2.0'}
                },
                'nodeTable': {
                    'node': [
                        {
                            '@name': '/pfsp_demo_first',
                            '@id': '1',
                            'connect': {
                                '@name': '/pc',
                                '@path': 'server//192.168.0.11:9009',
                                '@rx_buff_size': '1048576',
                                '@mode': 'async',
                                '@priority': '10',
                                '@class': 'CTcpConnect'
                            }
                        },
                        {
                            '@name': '/pc',
                            '@id': '2',
                            'connect': {
                                '@name': '/pfsp_demo_first',
                                '@path': 'client//192.168.0.11:9009',
                                '@rx_buff_size': '1048576',
                                '@mode': 'sync',
                                '@priority': '10',
                                '@class': 'CTcpConnect'
                            }
                        }
                    ]
                },
                'serviceTable': {
                    'service': [
                        {
                            '@name': '/time',
                            '@id': '1',
                            '@priority': '12',
                            '@core': '0',
                            '@mailbox_size': '4096',
                            '@class': 'CTimeService',
                            '@node': '/pfsp_demo_first',
                            'parameterFile': {'@file': 'time.xml'}
                        },
                        {
                            '@name': '/PluginErrorHandler',
                            '@id': '2',
                            '@priority': '1',
                            '@core': '0',
                            '@mail_size': '24692',
                            '@mailbox_size': '409600',
                            '@class': 'PfspPluginErrorHandler',
                            '@node': '/pfsp_demo_first',
                            'parameterFile': {'@file': 'plugin_error_handler.xml'}
                        },
                        {
                            '@name': '/PluginModeManager',
                            '@id': '3',
                            '@priority': '2',
                            '@core': '0',
                            '@mail_size': '24692',
                            '@mailbox_size': '409600',
                            '@class': 'PfspPluginModeManager',
                            '@node': '/pfsp_demo_first',
                            'parameterFile': {'@file': 'plugin_mode_manager.xml'}
                        }
                    ]
                },
                'topicTable': {
                    'topic': [
                        {'@name': 'Timer', '@id': '1', '@publisher': '/time'},
                        {'@name': 'ReactionReportTopic', '@id': '2', '@publisher': '/PluginErrorHandler'},
                        {'@name': 'ComponentsReactionTopic', '@id': '3', '@publisher': '/PluginModeManager'}
                    ]
                }
            }
        }

        folder_name = os.path.dirname(filename)
        if not os.path.isdir(folder_name):
            os.mkdir(folder_name)
        # print("folder_name:", folder_name)

        with open(filename, 'w') as file:
            raw_dict = self.scene.serialize()
            # utils.pp(raw_dict)

            pfsp_dict = config_dict["PFSPMiddleware"]
            if pfsp_dict["serviceTable"] is None: pfsp_dict["serviceTable"] = {"service": []}
            if pfsp_dict["topicTable"] is None: pfsp_dict["topicTable"] = {"topic": []}

            if len(pfsp_dict["serviceTable"]["service"]) > 0:
                service_id = int(pfsp_dict["serviceTable"]["service"][-1]["@id"]) + 1
            else:
                service_id = 1

            if len(pfsp_dict["topicTable"]["topic"]) > 0:
                topic_id = int(pfsp_dict["topicTable"]["topic"][-1]["@id"]) + 1
            else:
                topic_id = 1

            host_count = 0
            for item in raw_dict['nodes']:
                if item["op_code"] == PFSP_NODE_HOST_COMPONENT:
                    content_dict = item["content"]
                    pfsp_dict["serviceTable"]["service"].append(
                        {
                            "@name": "/"+content_dict["name"],
                            "@id": str(service_id),
                            # "@priority": content_dict["priority"],
                            "@priority": str(service_id-1),
                            # "@class": content_dict["name"]+"Service", #content_dict["class"],
                            "@class": content_dict["class"]+"Base",
                            "@core": "0",
                            "@mail_size": "24692",
                            "@mailbox_size": "409600",
                            "@node": '/pfsp_demo_first',
                            "parameterFile": { "@file": content_dict["config file"] + ".xml"}
                        }
                    )
                    service_id += 1

                    pfsp_dict["topicTable"]["topic"].append(
                        {
                            "@name": f"HostComponent{host_count}OutputTopic", #content_dict["interface name"],
                            "@id": str(topic_id),
                            "@publisher": "/"+content_dict["name"]
                        }
                    )
                    topic_id += 1

                    pfsp_dict["topicTable"]["topic"].append(
                        {
                            "@name": f"HostComponent{host_count}AllErrorsInfoTopic",
                            "@id": str(topic_id),
                            "@publisher": "/"+content_dict["name"]
                        }
                    )
                    topic_id += 1

                    pfsp_dict["topicTable"]["topic"].append(
                        {
                            "@name": f"HostComponent{host_count}AllModeInfoTopic",
                            "@id": str(topic_id),
                            "@publisher": "/"+content_dict["name"]
                        }
                    )
                    topic_id += 1

                    pfsp_dict["topicTable"]["topic"].append(
                        {
                            "@name": f"HostComponent{host_count}ErrorReportTopic",
                            "@id": str(topic_id),
                            "@publisher": "/"+content_dict["name"]
                        }
                    )
                    topic_id += 1

                    pfsp_dict["topicTable"]["topic"].append(
                        {
                            "@name": f"HostComponent{host_count}ModeStateReportTopic",
                            "@id": str(topic_id),
                            "@publisher": "/"+content_dict["name"]
                        }
                    )
                    topic_id += 1

                    host_count += 1

                    hidden_dict = content_dict["hidden"]
                    file_extension = ".xml"
                    app_dict = {
                        'PFSPMiddleware': {
                            '@version': '1.0',
                            '@xmlns': 'http://www.dmg.org/PMML-3-0',
                            '@xmlns:xsi': 'http://www.w3.org/2001/XMLSchema_instance',
                            'Header': {
                                '@xmlns': '',
                                '@copyright': '',
                                '#text': 'The PFSPMiddleware System Configuration for PFSPArchitecture',
                                'Application': {'@name': '/PFSP_MW', '@version': '2.0'}
                            },
                            'parameterTable': {
                                'parameter': [
                                    # {"@name": "Insert name", "@value": "Insert value"}
                                ]
                            }
                        }
                    }

                    with open(folder_name+"/"+content_dict["config file"] + ".xml", "w") as f:
                        if hidden_dict["parameters"] is not None and len(hidden_dict["parameters"]) > 0:
                            for item in hidden_dict["parameters"]:
                                app_dict["PFSPMiddleware"]["parameterTable"]['parameter'].append(
                                    { "@name": item["name"], "@value": item["value"] }
                                )
                        if hidden_dict['mode_manager'] is not None and len(hidden_dict['mode_manager']) > 0:
                            for k,v in hidden_dict["mode_manager"].items():
                                if isinstance(v, bool):
                                    app_dict["PFSPMiddleware"]["parameterTable"]["parameter"].append(
                                        {"@name": k, "@value": "eOn" if v else "eOff"}
                                    )
                                else:
                                    app_dict["PFSPMiddleware"]["parameterTable"]["parameter"].append(
                                        { "@name": k, "@value": v }
                                    )
                        if hidden_dict['error_handler'] is not None and len(hidden_dict['error_handler']) > 0:
                            for i,(error_type,error_property) in enumerate(hidden_dict["error_handler"].items()):
                                app_dict["PFSPMiddleware"]["parameterTable"]["parameter"].append(
                                    { "@name": f"Error{i}", "@value": error_type }
                                )
                                for k,v in error_property.items():
                                    if isinstance(v, bool):
                                        app_dict["PFSPMiddleware"]["parameterTable"]["parameter"].append(
                                            {"@name": f"Error{i}_" + k, "@value": "eOn" if v else "eOff"}
                                        )
                                    else:
                                        app_dict["PFSPMiddleware"]["parameterTable"]["parameter"].append(
                                            {"@name": f"Error{i}_"+k, "@value": v}
                                        )
                        xml = xmltodict.unparse(app_dict, encoding="ISO-8859-1")
                        dom = parseString(xml)
                        f.write(dom.toprettyxml())

            xml = xmltodict.unparse(config_dict, encoding="ISO-8859-1")
            # pprint(xml)
            dom = parseString(xml)
            file.write(dom.toprettyxml())

            # relative_path = os.path.relpath(folder_name, os.getcwd())
            relative_path = '../../pfsp_config/xml'
            with open(folder_name + "/" + "run.sh", "w") as f:
                f.write("PFSP_main /pfsp_demo_first ")
                f.write(f"{relative_path}/pfsp_main_config.xml ")
                f.write(f"{relative_path} /PluginErrorHandler /PluginModeManager ")
                for item in raw_dict['nodes']:
                    if item["op_code"] == PFSP_NODE_HOST_COMPONENT:
                        content_dict = item["content"]
                        f.write(f"/{content_dict['name']} ")

            with open(folder_name + "/" + "plugin_error_handler.xml", "w") as f:
                f.write('<?xml version="1.0" encoding="ISO-8859-1"?>\n')
                f.write('<PFSPMiddleware version="1.0" xmlns="http://www.dmg.org/PMML-3-0" xmlns:xsi="http://www.w3.org/2001/XMLSchema_instance">\n')
                f.write('    <Header xmlns="" copyright="">\n')
                f.write('        The PFSPMiddleware Service Configuration\n')
                f.write('        <Application name="PFSP_MW" version="2.0"/>\n')
                f.write('    </Header>\n')
                f.write('    <parameterTable>\n')
                f.write('        <!-- play once = 0, play repeatly = 1 -->\n')
                f.write('        <parameter name="PlayType" value = "eRepeat" />\n')
                f.write('        <!-- name="ThreadFrequency" value = "100" [ms] -->\n')
                f.write('        <parameter name="ThreadFrequency" value = "5" />\n')
                f.write('        <!-- For Mode Manager [~ END ]-->\n')
                f.write('    </parameterTable>\n')
                f.write('</PFSPMiddleware>\n')

            with open(folder_name + "/" + "plugin_mode_manager.xml", "w") as f:
                f.write('<?xml version="1.0" encoding="ISO-8859-1"?>\n')
                f.write('<PFSPMiddleware version="1.0" xmlns="http://www.dmg.org/PMML-3-0" xmlns:xsi="http://www.w3.org/2001/XMLSchema_instance">\n')
                f.write('    <Header xmlns="" copyright="">\n')
                f.write('        The PFSPMiddleware Service Configuration\n')
                f.write('        <Application name="PFSP_MW" version="2.0"/>\n')
                f.write('    </Header>\n')
                f.write('    <parameterTable>\n')
                f.write('        <!-- play once = 0, play repeatly = 1 -->\n')
                f.write('        <parameter name="PlayType" value = "eRepeat" />\n')
                f.write('        <!-- name="ThreadFrequency" value = "100" [ms] -->\n')
                f.write('        <parameter name="ThreadFrequency" value = "5" />\n')
                f.write('        <!-- For Mode Manager [~ END ]-->\n')
                f.write('    </parameterTable>\n')
                f.write('</PFSPMiddleware>\n')

            with open(folder_name + "/" + "time.xml", "w") as f:
                f.write('<?xml version="1.0" encoding="iso-8859-1"?>\n')
                f.write('<PFSPMiddleware version="1.0" xmlns="http://www.dmg.org/PMML-3-0" xmlns:xsi="http://www.w3.org/2001/XMLSchema_instance">\n')
                f.write('  <Header xmlns="" copyright="">\n')
                f.write('    The PFSPMiddleware for HIT.\n')
                f.write('    <Application name="PFSP_MW" version="0.9 beta"/>\n')
                f.write('  </Header>\n')
                f.write('  <parameterTable>\n')
                f.write('  	<!-- per 100ms units: 10 is 10*100ms. -->\n')
                f.write('    <parameter name="frequence" value="1" /> \n')
                f.write('    <parameter name="timerWorkerPriority" value="0" />\n')
                f.write('  </parameterTable>\n')
                f.write('</PFSPMiddleware>\n')


    def fileExportAsJSON(self, filename):
        assert(filename.endswith('.json'))
        folder_name = os.path.dirname(filename)
        # print("folder_name:", folder_name)

        raw_dict = self.scene.serialize()
        # utils.pp(raw_dict)

        config_dict = {
            'PFSPMiddleware': {
                'version': '1.0',
                'xmlns': 'http://www.dmg.org/PMML-3-0',
                'xmlns:xsi': 'http://www.w3.org/2001/XMLSchema_instance',
                'Header': {
                    'xmlns': '',
                    'copyright': '',
                    'text': 'The PFSPMiddleware System Configuration for PFSPArchitecture',
                    'Application': {'name': '/PFSP_MW', 'version': '2.0'}
                },
                'nodeTable': {
                    'node': [
                        {
                            'name': '/pfsp_demo_first',
                            'id': '1',
                            'connect': {
                                'name': '/pc',
                                'path': 'server//192.168.0.11:9009',
                                'rx_buff_size': '1048576',
                                'mode': 'async',
                                'priority': '10',
                                'class': 'CTcpConnect'
                            }
                        },
                        {
                            'name': '/pc',
                            'id': '2',
                            'connect': {
                                'name': '/pfsp_demo_first',
                                'path': 'client//192.168.0.11:9009',
                                'rx_buff_size': '1048576',
                                'mode': 'sync',
                                'priority': '10',
                                'class': 'CTcpConnect'
                            }
                        }
                    ]
                },
                'serviceTable': {
                    'service': [
                        {
                            'name': '/time',
                            'id': '1',
                            'priority': '12',
                            'core': '0',
                            'mailbox_size': '4096',
                            'class': 'CTimeService',
                            'node': '/pfsp_demo_first',
                            'parameterFile': {'file': 'time.json'}
                        },
                        {
                            'name': '/PluginErrorHandler',
                            'id': '2',
                            'priority': '1',
                            'core': '0',
                            'mail_size': '24692',
                            'mailbox_size': '409600',
                            'class': 'PfspPluginErrorHandler',
                            'node': '/pfsp_demo_first',
                            'parameterFile': {'file': 'plugin_error_handler.json'}
                        },
                        {
                            'name': '/PluginModeManager',
                            'id': '3',
                            'priority': '2',
                            'core': '0',
                            'mail_size': '24692',
                            'mailbox_size': '409600',
                            'class': 'PfspPluginModeManager',
                            'node': '/pfsp_demo_first',
                            'parameterFile': {'file': 'plugin_mode_manager.json'}
                        }
                    ]
                },
                'topicTable': {
                    'topic': [
                        {'name': 'Timer', 'id': '1', 'publisher': '/time'},
                        {'name': 'ReactionReportTopic', 'id': '2', 'publisher': '/PluginErrorHandler'},
                        {'name': 'ComponentsReactionTopic', 'id': '3', 'publisher': '/PluginModeManager'}
                    ]
                }
            }
        }

        pfsp_dict = config_dict["PFSPMiddleware"]
        if pfsp_dict["serviceTable"] is None: pfsp_dict["serviceTable"] = {"service": []}
        if pfsp_dict["topicTable"] is None: pfsp_dict["topicTable"] = {"topic": []}

        if len(pfsp_dict["serviceTable"]["service"]) > 0:
            service_id = int(pfsp_dict["serviceTable"]["service"][-1]["id"]) + 1
        else:
            service_id = 1

        if len(pfsp_dict["topicTable"]["topic"]) > 0:
            topic_id = int(pfsp_dict["topicTable"]["topic"][-1]["id"]) + 1
        else:
            topic_id = 1

        ### Main Config File
        folder_name = os.path.dirname(filename)
        if not os.path.isdir(folder_name):
            os.mkdir(folder_name)

        with open(filename, 'w') as config_file:
            data_structure_dict = {}
            host_count = 0
            for item in raw_dict['nodes']:
                if item["op_code"] == PFSP_NODE_HOST_COMPONENT:
                    content_dict = item["content"]
                    # print(content_dict["name"])
                    pfsp_dict["serviceTable"]["service"].append(
                        {
                            "name": "/" + content_dict["name"],
                            "id": str(service_id),
                            #"priority": content_dict["priority"],
                            "priority": str(service_id-1),
                            # "class": content_dict["name"]+"Service", #content_dict["class"],
                            "class": content_dict["class"]+"Base",
                            "core": "0",
                            "mail_size": "24692",
                            "mailbox_size": "409600",
                            "node": '/pfsp_demo_first',
                            "parameterFile": {"file": content_dict["config file"] + ".json"}
                        }
                    )
                    service_id += 1

                    pfsp_dict["topicTable"]["topic"].append(
                        {
                            "name": f"HostComponent{host_count}OutputTopic", #content_dict["interface name"],
                            "id": str(topic_id),
                            "publisher": "/" + content_dict["name"]
                        }
                    )
                    topic_id += 1

                    pfsp_dict["topicTable"]["topic"].append(
                        {
                            "name": f"HostComponent{host_count}AllErrorsInfoTopic",
                            "id": str(topic_id),
                            "publisher": "/" + content_dict["name"]
                        }
                    )
                    topic_id += 1

                    pfsp_dict["topicTable"]["topic"].append(
                        {
                            "name": f"HostComponent{host_count}AllModeInfoTopic",
                            "id": str(topic_id),
                            "publisher": "/" + content_dict["name"]
                        }
                    )
                    topic_id += 1

                    pfsp_dict["topicTable"]["topic"].append(
                        {
                            "name": f"HostComponent{host_count}ErrorReportTopic",
                            "id": str(topic_id),
                            "publisher": "/" + content_dict["name"]
                        }
                    )
                    topic_id += 1

                    pfsp_dict["topicTable"]["topic"].append(
                        {
                            "name": f"HostComponent{host_count}ModeStateReportTopic",
                            "id": str(topic_id),
                            "publisher": "/" + content_dict["name"]
                        }
                    )
                    topic_id += 1
                host_count += 1
            config_file.write(json.dumps(config_dict, indent=4))

        host_count = 0
        for item in raw_dict['nodes']:
            if item["op_code"] == PFSP_NODE_HOST_COMPONENT:
                content_dict = item["content"]
                hidden_dict = content_dict["hidden"]

                app_dict = {
                    'PFSPMiddleware': {
                        'version': '1.0',
                        'xmlns': 'http://www.dmg.org/PMML-3-0',
                        'xmlns:xsi': 'http://www.w3.org/2001/XMLSchema_instance',
                        'Header': {
                            'xmlns': '',
                            'copyright': '',
                            'text': 'The PFSPMiddleware System Configuration for PFSPArchitecture',
                            'Application': {'name': '/PFSP_MW', 'version': '2.0'}
                        },
                        'parameterTable': {
                            'parameter': [
                                # {"@name": "Insert name", "@value": "Insert value"}
                            ]
                        }
                    }
                }

                ### private config file for host application
                with open(folder_name + "/" + content_dict["config file"] + ".json", "w") as f:
                    if hidden_dict["parameters"] is not None and len(hidden_dict["parameters"]) > 0:
                        for item in hidden_dict["parameters"]:
                            app_dict["PFSPMiddleware"]["parameterTable"]['parameter'].append(
                                {"name": item["name"], "value": item["value"]}
                            )
                    if hidden_dict['mode_manager'] is not None and len(hidden_dict['mode_manager']) > 0:
                        for k, v in hidden_dict["mode_manager"].items():
                            if isinstance(v, bool):
                                app_dict["PFSPMiddleware"]["parameterTable"]["parameter"].append(
                                    {"name": k, "value": "eOn" if v else "eOff"}
                                )
                            else:
                                app_dict["PFSPMiddleware"]["parameterTable"]["parameter"].append(
                                    {"name": k, "value": v}
                                )
                    if hidden_dict['error_handler'] is not None and len(hidden_dict['error_handler']) > 0:
                        for i, (error_type, error_property) in enumerate(hidden_dict["error_handler"].items()):
                            app_dict["PFSPMiddleware"]["parameterTable"]["parameter"].append(
                                {"name": f"Error{i}", "value": error_type}
                            )
                            for k, v in error_property.items():
                                if isinstance(v, bool):
                                    app_dict["PFSPMiddleware"]["parameterTable"]["parameter"].append(
                                        {"name": f"Error{i}_" + k, "value": "eOn" if v else "eOff"}
                                    )
                                else:
                                    app_dict["PFSPMiddleware"]["parameterTable"]["parameter"].append(
                                        {"name": f"Error{i}_" + k, "value": v}
                                    )

                    f.write(json.dumps(app_dict, indent=4))
                host_count += 1

        # for Smart converter
        host_count = 0
        smart_converter_list = list()
        for item in raw_dict['nodes']:
            if item["op_code"] != PFSP_NODE_HOST_COMPONENT: continue

            component_name = item["content"]['name']
            hidden_dict = item["content"]["hidden"]
            if hidden_dict["smart_converter"] is None or len(hidden_dict["smart_converter"]) == 0: continue

            smart_converter_dict = hidden_dict["smart_converter"]
            inbounds,internals,internal_init_class,internal_func,outbounds,outbound_init_class,outbound_func,mapping = self.parseSmartConverter(smart_converter_dict)
            smart_converter_list.append({
                'name': component_name,
                'inbounds': inbounds,
                'internals': internals,
                'internal_initial_class': internal_init_class,
                'internal_function': internal_func,
                'outbounds': outbounds,
                'outbound_initial_class': outbound_init_class,
                'outbound_function': outbound_func,
                'mapping': mapping
            })
            host_count += 1

        if DEBUG: utils.pp(smart_converter_list)

        for host_count in range(len(smart_converter_list)):
            component_name = smart_converter_list[host_count]['name']
            inbounds = smart_converter_list[host_count]['inbounds']
            internal_init_class = smart_converter_list[host_count]['internal_initial_class']
            internal_func = smart_converter_list[host_count]['internal_function']
            internals = smart_converter_list[host_count]['internals']
            outbounds = smart_converter_list[host_count]['outbounds']
            outbound_init_class = smart_converter_list[host_count]['outbound_initial_class']
            outbound_func = smart_converter_list[host_count]['outbound_function']
            mapping = smart_converter_list[host_count]['mapping']

            reshaped_smart_converter = dict()
            if internal_init_class is not None and internal_func is not None:
                data_map = dict()
                for internal_class,internal_members in internals.items():
                    for var_name,var_type in internal_members.items():
                        for pair in mapping:
                            if pair[0]['class_name'] == internal_class and pair[0]['variable_type'] == var_type and pair[0]['variable_name'] == var_name:
                                # Find source id
                                s_id = -1
                                for i,smart_converter in enumerate(smart_converter_list):
                                    s_class_name, s_var_type, s_var_name = (
                                        pair[1]['class_name'], pair[1]['variable_type'], pair[1]['variable_name']
                                    )
                                    if s_class_name in smart_converter['outbounds'].keys() and \
                                            s_var_name in smart_converter['outbounds'][s_class_name].keys() and \
                                            s_var_type == smart_converter['outbounds'][s_class_name][s_var_name]:
                                        s_id = i
                                        break

                                if s_id == -1: raise ValueError('No matching host id')

                                matching = {
                                    "dst_var_name": var_name,
                                    "dtype": var_type,
                                    "from": pair[1]['class_name'],
                                    "assigne": pair[1]['variable_name'],
                                    "stype": pair[1]['variable_type'],
                                    "scomponent_id": s_id
                                }
                                if internal_class in data_map.keys(): data_map[internal_class].append(matching)
                                else: data_map[internal_class] = [matching]

                                break
                            elif pair[1]['class_name'] == internal_class and pair[1]['variable_type'] == var_type and pair[1]['variable_name'] == var_name:
                                # Find source id
                                s_id = -1
                                for i,smart_converter in enumerate(smart_converter_list):
                                    s_class_name, s_var_type, s_var_name = (
                                        pair[0]['class_name'], pair[0]['variable_type'], pair[0]['variable_name']
                                    )
                                    if s_class_name in smart_converter['outbounds'].keys() and \
                                            s_var_name in smart_converter['outbounds'][s_class_name].keys() and \
                                            s_var_type == smart_converter['outbounds'][s_class_name][s_var_name]:
                                        s_id = i
                                        break

                                if s_id == -1: raise ValueError('No matching host id')

                                matching = {
                                    "dst_var_name": var_name,
                                    "dtype": var_type,
                                    "from": pair[0]['class_name'],
                                    "assigne": pair[0]['variable_name'],
                                    "stype": pair[0]['variable_type'],
                                    "scomponent_id": s_id
                                }
                                if internal_class in data_map.keys(): data_map[internal_class].append(matching)
                                else: data_map[internal_class] = [matching]
                                break

                # utils.pp(data_map)
                reshaped_smart_converter['SmartConverter - External Input To Internal'] = {
                    'function': internal_func, #item['content']['function'],
                    'smart_converter_component_id': host_count,
                    'includes': ['pfsp_core_smart_converter_int.hpp'],
                    'class_initial': internal_init_class, #item['content']['class_initial'],
                    'data_map': data_map
                }

            if outbound_init_class is not None and outbound_func is not None:
                data_map = dict()
                for outbound_class,outbound_members in outbounds.items():
                    for var_name,var_type in outbound_members.items():
                        for pair in mapping:
                            if pair[0]['class_name'] == outbound_class and pair[0]['variable_type'] == var_type and pair[0]['variable_name'] == var_name:
                                matching = {
                                    "dst_var_name": var_name,
                                    "dtype": var_type,
                                    "from": pair[1]['class_name'],
                                    "assigne": pair[1]['variable_name'],
                                    "stype": pair[1]['variable_type'],
                                    "scomponent_id": host_count
                                }
                                if outbound_class in data_map.keys(): data_map[outbound_class].append(matching)
                                else: data_map[outbound_class] = [matching]

                                break
                            elif pair[1]['class_name'] == outbound_class and pair[1]['variable_type'] == var_type and pair[1]['variable_name'] == var_name:
                                matching = {
                                    "dst_var_name": var_name,
                                    "dtype": var_type,
                                    "from": pair[0]['class_name'],
                                    "assigne": pair[0]['variable_name'],
                                    "stype": pair[0]['variable_type'],
                                    "scomponent_id": host_count
                                }
                                if outbound_class in data_map.keys(): data_map[outbound_class].append(matching)
                                else: data_map[outbound_class] = [matching]
                                break

                # utils.pp(data_map)
                reshaped_smart_converter['SmartConverter - Internal To External Output'] = {
                    'function': outbound_func, #item['content']['function'],
                    'smart_converter_component_id': host_count,
                    'includes': ['pfsp_core_smart_converter_ext.hpp'],
                    'class_initial': outbound_init_class, #item['content']['class_initial'],
                    'data_map': data_map
                }

            if DEBUG:
                print('reshaped_smart_converter:')
                utils.pp(reshaped_smart_converter)

            with open(f"{folder_name}/smart_converter_{host_count}_{component_name}.json", 'w') as f:
                f.write(json.dumps(reshaped_smart_converter, indent=4))

        '''
        id_dict = {}
        mapping_list = []
        for item in raw_dict['nodes']:
            if item["op_code"] != PFSP_NODE_HOST_COMPONENT: continue

            hidden_dict = item["content"]["hidden"]
            if hidden_dict["smart_converter"] is None or len(hidden_dict["smart_converter"]) == 0: continue

            smart_converter_dict = hidden_dict["smart_converter"]
            for item in smart_converter_dict['nodes']:
                if item['title'] == 'Data Structure':
                    for x in item['inputs']:
                        key = f"object_member_{x['index']}"
                        member_name, member_type = item['content'][key].split(':')
                        id_dict[str(x['id'])] = {
                            'classname': item['content']['object_class'],
                            'type': member_type,
                            'name': member_name
                        }

                    for x in item['outputs']:
                        key = f"object_member_{x['index']}"
                        member_name, member_type = item['content'][key].split(':')
                        id_dict[str(x['id'])] = {
                            'classname': item['content']['object_class'],
                            'type': member_type,
                            'name': member_name
                        }

            for item in smart_converter_dict['edges']:
                mapping_list.append([item['start'], item['end']])

        print('id_dict:')
        utils.pp(id_dict)
        print('mapping_list:')
        utils.pp(mapping_list)

        return
        host_count = 0
        component_id = {} # assigned
        for item in raw_dict['nodes']:
            if item['op_code'] == PFSP_NODE_HOST_COMPONENT:
                hidden_dict = item["content"]["hidden"]
                if hidden_dict["smart_converter"] is not None or len(hidden_dict["smart_converter"]) > 0:
                    smart_converter_dict = hidden_dict["smart_converter"]
                    for item in smart_converter_dict['nodes']:
                        if item['title'] == 'SmartConverter - Internal To External Output':
                            for out_socket in item['outputs']:
                                out_remote_id = None
                                for x in mapping_list:
                                    if x[0] == out_socket['id']:
                                        out_remote_id = x[1]
                                        break
                                    elif x[1] == out_socket['id']:
                                        out_remote_id = x[0]
                                        break
                                # print('out_remote_id:', out_remote_id)
                                if out_remote_id is not None:
                                    classname = id_dict[str(out_remote_id)]['classname']
                                    # print('classname:', classname)
                                    component_id[classname] = host_count
                host_count += 1
        # utils.pp(component_id)

        host_count = 0
        for item in raw_dict['nodes']:
            if item["op_code"] == PFSP_NODE_HOST_COMPONENT:
                content_dict = item["content"]
                component_name = content_dict['name']
                hidden_dict = content_dict["hidden"]

                ### Smart Converter Config for host application
                # with open(f"{folder_name}/smart_converter_host_component{host_count}.json", "w") as f:
                with open(f"{folder_name}/smart_converter_{host_count}_{component_name}.json", 'w') as f:
                    # Get the instance of host application
                    reshaped_dict = {}
                    if hidden_dict["smart_converter"] and len(hidden_dict["smart_converter"]) > 0:
                        smart_converter_dict = hidden_dict["smart_converter"]
                        for item in smart_converter_dict['nodes']:
                            if item['title'] != 'SmartConverter - External Input To Internal' and item['title'] != 'SmartConverter - Internal To External Output':
                                continue

                            if item['title'] not in reshaped_dict:
                                if item['title'] == 'SmartConverter - External Input To Internal':
                                    reshaped_dict[item['title']] = {
                                        'function': item['content']['function'],
                                        'smart_converter_component_id': host_count,
                                        'includes': ['pfsp_core_smart_converter_int.hpp'],
                                        'class_initial': item['content']['class_initial'],
                                        'data_map': {}
                                    }
                                elif item['title'] == 'SmartConverter - Internal To External Output':
                                    reshaped_dict[item['title']] = {
                                        'function': item['content']['function'],
                                        'smart_converter_component_id': host_count,
                                        'includes': ['pfsp_core_smart_converter_ext.hpp'],
                                        'class_initial': item['content']['class_initial'],
                                        'data_map': {}
                                    }

                            assert (len(item['inputs']) == len(item['outputs']))
                            # utils.pp(item)
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

                                if in_remote_id is None: continue
                                in_instance = id_dict[in_remote_id]

                                out_socket = [x for x in item['outputs'] if x['index'] == in_socket['index']][0]
                                out_remote_id = None
                                for x in mapping_list:
                                    if x[0] == out_socket['id']:
                                        out_remote_id = str(x[1])
                                        break
                                    elif x[1] == out_socket['id']:
                                        out_remote_id = str(x[0])
                                        break

                                if out_remote_id is None: continue

                                out_instance = id_dict[out_remote_id]
                                if out_instance['classname'] not in reshaped_dict[item['title']]['data_map']:
                                    reshaped_dict[item['title']]['data_map'][out_instance['classname']] = []

                                reshaped_dict[item['title']]['data_map'][out_instance['classname']].append(
                                    {
                                        'dst_var_name': out_instance['name'],
                                        'dtype': out_instance['type'],
                                        'from': in_instance['classname'],
                                        'assigne': in_instance['name'],
                                        'stype': in_instance['type']
                                    }
                                )

                                if item['title'] == 'SmartConverter - External Input To Internal':
                                    print(out_instance['classname'])
                                    utils.pp(reshaped_dict[item['title']]['data_map'][out_instance['classname']])
                                    reshaped_dict[item['title']]['data_map'][out_instance['classname']][-1]['scomponent_id'] = component_id[in_instance['classname']]
                                elif item['title'] == 'SmartConverter - Internal To External Output':
                                    reshaped_dict[item['title']]['data_map'][out_instance['classname']][-1]['scomponent_id'] = host_count

                                # if in_instance['classname'] in component_id:
                                #     if 'component_id' in reshaped_dict[item['title']]:
                                #         assert(reshaped_dict[item['title']]['component_id'] == component_id[in_instance['classname']])
                                #     else:
                                #         reshaped_dict[item['title']]['component_id'] = component_id[in_instance['classname']]
                                # else:
                                #     reshaped_dict[item['title']]['component_id'] = 0

                    f.write(json.dumps(reshaped_dict, indent=4))

                host_count += 1
        '''

        # relative_path = os.path.relpath(folder_name, os.getcwd())
        relative_path = '../../pfsp_config/xml'
        with open(folder_name + "/" + "run_with_json.sh", "w") as f:
            f.write("PFSP_main /pfsp_demo_first ")
            f.write(f"{relative_path}/pfsp_main_config.json ")
            f.write(f"{relative_path} /PluginErrorHandler /PluginModeManager ")
            for item in raw_dict['nodes']:
                if item["op_code"] == PFSP_NODE_HOST_COMPONENT:
                    content_dict = item["content"]
                    f.write(f"/{content_dict['name']} ")

        with open(folder_name + "/" + "plugin_error_handler.json", "w") as f:
            app_dict = {
                'PFSPMiddleware': {
                    'version': '1.0',
                    'xmlns': 'http://www.dmg.org/PMML-3-0',
                    'xmlns:xsi': 'http://www.w3.org/2001/XMLSchema_instance',
                    'Header': {
                        'xmlns': '',
                        'copyright': '',
                        'Application': {'name': 'PFSP_MW', 'version': '2.0'},
                        'text': 'The PFSPMiddleware Service Configuration'
                    },
                    'parameterTable': {
                        'parameter': [
                            {'name':'PlayType', 'value':'eRepeat'},
                            {'name':'ThreadFrequency', 'value':'10'}
                        ]
                    }
                }
            }
            f.write(json.dumps(app_dict, indent=4))

        with open(folder_name + "/" + "plugin_mode_manager.json", "w") as f:
            app_dict = {
                'PFSPMiddleware': {
                    'version': '1.0',
                    'xmlns': 'http://www.dmg.org/PMML-3-0',
                    'xmlns:xsi': 'http://www.w3.org/2001/XMLSchema_instance',
                    'Header': {
                        'xmlns': '',
                        'copyright': '',
                        'Application': {'name': 'PFSP_MW', 'version': '2.0'},
                        'text': 'The PFSPMiddleware Service Configuration'
                    },
                    'parameterTable': {
                        'parameter': [
                            {'name':'PlayType', 'value':'eRepeat'},
                            {'name':'ThreadFrequency', 'value':'10'}
                        ]
                    }
                }
            }
            f.write(json.dumps(app_dict, indent=4))

        with open(folder_name + "/" + "time.json", "w") as f:
            app_dict = {
                'PFSPMiddleware': {
                    'version': '1.0',
                    'xmlns': 'http://www.dmg.org/PMML-3-0',
                    'xmlns:xsi': 'http://www.w3.org/2001/XMLSchema_instance',
                    'Header': {
                        'xmlns': '',
                        'copyright': '',
                        'Application': {'name': 'PFSP_MW', 'version': '0.9 beta'},
                        'text': 'The PFSPMiddleware for HIT.'
                    },
                    'parameterTable': {
                        'parameter': [
                            {'name': 'frequence', 'value': '1'},
                            {'name': 'timerWorkerPriority', 'value': '0'}
                        ]
                    }
                }
            }
            f.write(json.dumps(app_dict, indent=4))
        """
        with open(folder_name + "/base_service_components.json", "w") as f:
            base_service_dict = {}
            for item in raw_dict['nodes']:
                if item["op_code"] == PFSP_NODE_HOST_COMPONENT:
                    content_dict = item["content"]
                    # print(content_dict["name"])
                    key_str = f"service_name_{len(base_service_dict)+1}"
                    base_service_dict[key_str] = content_dict["name"]

            f.write(json.dumps(base_service_dict, indent=4))
        """

    def fileExport(self, filename):
        """
        Export this `Scene` to the user-specific file on disk.

        :param filename: where to export this scene
        :type filename: ``str``
        """
        self.fileExportAsXML(filename+".xml")
        self.fileExportAsJSON(filename+".json")

        # TODO: second parsing as the type of data_map_gen
        # id_dict = {}
        # for item in raw_dict['nodes']:
        #     if item['title'] == 'SmartConverterObjectType':
        #         for x in item['inputs']:
        #             key = f"object_member_{x['index']}"
        #             member_name,member_type = item['content'][key].split(':')
        #             id_dict[str(x['id'])] = {'classname':item['content']['object_class'], 'type':member_type, 'name':member_name}
        #
        #         for x in item['outputs']:
        #             key = f"object_member_{x['index']}"
        #             member_name,member_type = item['content'][key].split(':')
        #             id_dict[str(x['id'])] = {'classname':item['content']['object_class'], 'type':member_type, 'name':member_name}
        # # print(id_dict)
        #
        # mapping_list = []
        # for item in raw_dict['edges']:
        #     mapping_list.append([item['start'], item['end']])
        # # print(mapping_list)
        #
        # reshaped_dict = {}
        # for item in raw_dict['nodes']:
        #     if item['title'] == 'SmartConverterToInter' or item['title'] == 'SmartConverterToExt':
        #         if item['title'] not in reshaped_dict:
        #             include_files = [None]
        #             if item['title'] == 'SmartConverterToInter':
        #                 include_files = ['pfsp_core_smart_converter_int.hpp']
        #             elif item['title'] == 'SmartConverterToExt':
        #                 include_files = ['pfsp_core_smart_converter_ext.hpp']
        #
        #             reshaped_dict[item['title']] = {
        #                 'function': item['content']['function'],
        #                 'includes': include_files,
        #                 'class_initial': item['content']['class_initial'],
        #                 'data_map': {}
        #             }
        #
        #         assert(len(item['inputs']) == len(item['outputs']))
        #         no_pipelines = len(item['inputs'])
        #         for i in range(no_pipelines):
        #             in_socket = item['inputs'][i]
        #             in_remote_id = None
        #             for x in mapping_list:
        #                 if x[0] == in_socket['id']:
        #                     in_remote_id = str(x[1])
        #                     break
        #                 elif x[1] == in_socket['id']:
        #                     in_remote_id = str(x[0])
        #                     break
        #             in_instance = id_dict[in_remote_id]
        #
        #             out_socket = [ x for x in item['outputs'] if x['index'] == in_socket['index'] ][0]
        #             out_remote_id = None
        #             for x in mapping_list:
        #                 if x[0] == out_socket['id']:
        #                     out_remote_id = str(x[1])
        #                     break
        #                 elif x[1] == out_socket['id']:
        #                     out_remote_id = str(x[0])
        #                     break
        #
        #             out_instance = id_dict[out_remote_id]
        #             if out_instance['classname'] not in reshaped_dict[item['title']]['data_map']:
        #                 reshaped_dict[item['title']]['data_map'][out_instance['classname']] = []
        #
        #             pipeline = {
        #                 'dst_var_name':out_instance['name'], 'dtype':out_instance['type'],
        #                 'from':in_instance['classname'], 'assigne':in_instance['name'], 'stype':in_instance['type']
        #             }
        #
        #             reshaped_dict[item['title']]['data_map'][out_instance['classname']].append(pipeline)
        #
        # file.write(json.dumps(reshaped_dict, indent=4))

