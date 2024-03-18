LISTBOX_MIMETYPE = 'application/x-item'

PFSP_NODE_HOST_COMPONENT = 1
PFSP_NODE_MODULAR_INTERFACE = 2
PFSP_NODE_ERROR_HANDLER = 3
PFSP_NODE_MODE_MANAGER = 4
PFSP_NODE_SMART_CONVERTER = 5
PFSP_NODE_NODE_COMPONENT = 6


PFSP_NODES = {
}

class ConfException(Exception): pass
class InvalidNodeRegistration(ConfException): pass
class OpCodeNotRegistered(ConfException): pass

def register_node_now(op_code, class_reference):
    if op_code in PFSP_NODES:
        raise InvalidNodeRegistration('Duplicate node registration of \'%s\'. There is already %s'%(
            op_code, PFSP_NODES[op_code]
        ) )

    PFSP_NODES[op_code] = class_reference

def register_node(op_code):
    def decorator(original_class):
        register_node_now(op_code, original_class)
        return original_class
    return decorator

def get_class_from_opcode(op_code):
    if op_code not in PFSP_NODES:
        raise OpCodeNotRegistered("OpCode '{:d}' is not registered".format(op_code))
    return PFSP_NODES[op_code]

from pfsp_gui.window.pfsp_nodes import *
