LISTBOX_MIMETYPE = 'application/x-item'

SMART_CONVERTER_NODE_DATA_STRUCTURE = 0
SMART_CONVERTER_NODE_TO_INTER = 1
SMART_CONVERTER_NODE_TO_EXT = 2

SMART_CONVERTER_NODES = {
}

class ConfException(Exception): pass
class InvalidNodeRegistration(ConfException): pass
class OpCodeNotRegistered(ConfException): pass

def register_node_now(op_code, class_reference):
    if op_code in SMART_CONVERTER_NODES:
        raise InvalidNodeRegistration(
            'Duplicate node registration of \'%s\'. There is already %s'%(
                op_code, SMART_CONVERTER_NODES[op_code]
            )
        )

    SMART_CONVERTER_NODES[op_code] = class_reference

def register_node(op_code):
    def decorator(original_class):
        register_node_now(op_code, original_class)
        return original_class
    return decorator

def get_class_from_opcode(op_code):
    if op_code not in SMART_CONVERTER_NODES:
        raise OpCodeNotRegistered('OpCode \'%d\' is not registered'%op_code)
    return SMART_CONVERTER_NODES[op_code]

from pfsp_gui.dialog.smart_converter_nodes import *
