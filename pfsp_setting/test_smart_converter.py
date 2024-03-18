import os
import sys
import json
import utils

import numpy as np

classHostIDs = dict() # -1: internal, >= 0: host_id
filename = 'test_smart_converter.json'

DEBUG = False

def parseSmartConverter(smart_converter_data:dict):
  connections = list()
  pipelines = list()
  node_content = dict()

  inbound_ids = list()
  internal_ids = list()
  outbound_ids = list()
    
  # Parsing connections
  for edge in smart_converter_data['edges']:
    connections.append([ edge['start'], edge['end'] ])
    
  for item in smart_converter_data['nodes']:
    if DEBUG: print(item['title'])
    assert(len(item['inputs']) == len(item['outputs']))
      
    item_pipelines = dict()
      
    # Parsing inputs
    for dot_node in item['inputs']:
      key = dot_node['index']
      if key in item_pipelines.keys():
        raise ValueError(f"{key} already exists more than 1 in {item['title']}")
      item_pipelines[key] = [ dot_node['id'] ]
          
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
    elif item['title'] == 'SmartConverter - Internal To External Output':
      # Merge private pipelines into all-in-one pipelines
      pipelines.extend(item_pipelines.values())
      for pipe in item_pipelines.values():
        internal_ids.append(pipe[0])
        outbound_ids.append(pipe[1])
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

  #utils.pp(item)
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
    for edge_p,edge_q in connections:
      if p == edge_p: origin_p = edge_q
      elif p == edge_q: origin_p = edge_p
    if origin_p is not None:
      origin_var = node_content[origin_p]
      if origin_var['class_name'] not in inboundClasses.keys():
        inboundClasses[origin_var['class_name']] = {
          origin_var['variable_name']: origin_var['variable_type']
        }
      else:
        existing_class = inboundClasses[origin_var['class_name']]
        if origin_var['variable_name'] in existing_class.keys():
          print(f" Existing '{origin_var['variable_type']} {origin_var['variable_name']}'")
        existing_class[origin_var['variable_name']] = origin_var['variable_type']
          
  internalClasses = dict()
  for p in internal_ids:
    origin_p = None
    for edge_p,edge_q in connections:
      if p == edge_p: origin_p = edge_q
      elif p == edge_q: origin_p = edge_p
    if origin_p is not None:
      origin_var = node_content[origin_p]
      if origin_var['class_name'] not in internalClasses.keys():
        internalClasses[origin_var['class_name']] = {
          origin_var['variable_name']: origin_var['variable_type']
        }
      else:
        existing_class = internalClasses[origin_var['class_name']]
        if origin_var['variable_name'] in existing_class.keys():
          print(f" Existing '{origin_var['variable_type']} {origin_var['variable_name']}'")
        existing_class[origin_var['variable_name']] = origin_var['variable_type']
          
    outboundClasses = dict()
    for p in outbound_ids:
      origin_p = None
      for edge_p,edge_q in connections:
        if p == edge_p: origin_p = edge_q
        elif p == edge_q: origin_p = edge_p
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
    for pipe_p,pipe_q in pipelines:
      origin_p = None
      for edge_p,edge_q in connections:
        if pipe_p == edge_p: origin_p = edge_q
        elif pipe_p == edge_q: origin_p = edge_p
        
      origin_q = None
      for edge_p,edge_q in connections:
        if pipe_q == edge_p: origin_q = edge_q
        elif pipe_q == edge_q: origin_q = edge_p

      if origin_p is not None and origin_q is not None:
        mapping.append([ node_content[origin_p], node_content[origin_q] ])

    if DEBUG:
      print('Mapping:')
      utils.pp(mapping)

    return inboundClasses,internalClasses,outboundClasses,mapping

if __name__ == '__main__':
  with open(filename, 'r') as file:
    raw_data = file.read()
    try:
      if sys.version_info >= (3, 9):
        parsed_data = json.loads(raw_data)
      else:
        parsed_data = json.loads(raw_data, encoding='utf-8')
    except json.JSONDecodeError:
      raise InvalidFile('%s is not a valid JSON file'%os.path.basename(filename))

  #utils.pp(parsed_data)
  if parsed_data['smart_converter'] is None or len(parsed_data['smart_converter']) == 0:
    exit(-1)

  smart_converter_data = parsed_data['smart_converter']
  ins,ints,outs,map = parseSmartConverter(smart_converter_data)

  print('ins:')
  utils.pp(ins)
  print('ints:')
  utils.pp(ints)
  print('outs:')
  utils.pp(outs)
  print('map:')
  utils.pp(map)
