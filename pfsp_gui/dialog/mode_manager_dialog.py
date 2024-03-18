import sys

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from pfsp_gui.toggle_switch import Switch

import copy

modeManagerTemplate = {
  "eSWComponentOperationModeNormal": False,
  "eSWComponentOperationModeDegradation": False,
  "eSWComponentOperationModeDiagnostics": False,
  "PlayType": "Repeat",
  "ThreadFrequency": 100 # TODO: add unit as [ms]
}

modeMangerProperty = {
  "eSWComponentOperationModeNormal": False,
  "eSWComponentOperationModeDegradation": False,
  "eSWComponentOperationModeDiagnostics": False,
  "PlayType": ["Once", "Repeat"],
  "ThreadFrequency": 10000000
}

class ModeMangerDialog(QDialog):
  title = "The Mode Manager"

  def __init__(self, mode_manager_dict:dict=None, parent_name:str="", parent=None):
    super().__init__(parent)

    if mode_manager_dict is not None and mode_manager_dict != dict(): self.metaData = mode_manager_dict
    else: self.metaData = copy.deepcopy(modeManagerTemplate)

    #print(self.metaData)
    self.initUI()
    self.setWindowTitle(self.__class__.title + " - " + parent_name)

  def initUI(self):
    # self.resize(510, 410)
    self.contents = dict()

    vbox = QVBoxLayout()
    grid = QGridLayout()
    for i,(k,v) in enumerate(self.metaData.items()):
      widget = None
      if isinstance(v, bool):
        widget = Switch(track_radius=12, thumb_radius=10)
        widget.setChecked(v)
        widget.toggled.connect(lambda state, key=k: self.onSwitchToggledEvent(key, state))
        grid.addWidget(QLabel(k), i, 0)
        grid.addWidget(widget, i, 1, Qt.AlignCenter)
      elif isinstance(v, str):
        widget = QComboBox()
        widget.addItems(modeMangerProperty[k])
        widget.setCurrentIndex(modeMangerProperty[k].index(v))
        widget.currentIndexChanged.connect(lambda current, w=widget, key=k: self.onSelectionChangedEvent(key, w, current))
        grid.addWidget(QLabel(k), i, 0)
        grid.addWidget(widget, i, 1)#, Qt.AlignRight)
      elif isinstance(v, int):
        widget = QSpinBox()
        widget.setRange(0, modeMangerProperty[k])
        widget.setValue(v)
        widget.valueChanged.connect(lambda value, key=k: self.onSpinChangedEvent(key, value))
        grid.addWidget(QLabel(k), i, 0)
        grid.addWidget(widget, i, 1)#, Qt.AlignRight)
      self.contents[k] = widget
    vbox.addLayout(grid)

    hline = QFrame()
    hline.setFrameShape(QFrame.HLine)
    hline.setFrameShadow(QFrame.Sunken)
    vbox.addWidget(hline)

    hbox = QHBoxLayout()
    hbox.addStretch(1)
    ok_btn = QPushButton('Ok')
    ok_btn.setToolTip('Save the property')
    # ok_btn.clicked.connect(self.onOkClick)
    ok_btn.clicked.connect(self.accept)
    hbox.addWidget(ok_btn)
    cancel_btn = QPushButton('Cancel')
    cancel_btn.setToolTip('Quit without save')
    # cancel_btn.clicked.connect(self.onCancelClick)
    cancel_btn.clicked.connect(self.reject)
    hbox.addWidget(cancel_btn)

    vbox.addLayout(hbox)
    
    self.setLayout(vbox)

  def onSwitchToggledEvent(self, key, state):
    self.metaData[key] = state
    # utils.pp(self.metaData)

  def onSpinChangedEvent(self, key, value):
    self.metaData[key] = value
    # utils.pp(self.metaData)

  def onSelectionChangedEvent(self, key, widget, current):
    self.metaData[key] = widget.itemText(current)
    # utils.pp(self.metaData)

  def updateUI(self):
    for k, v in self.metaData.items():
      w = self.contents[k]
      if isinstance(v, bool):
        w.setChecked(v)
      elif isinstance(v, int):
        w.setValue(v)
      elif isinstance(v, str):
        w.setCurrentIndex(modeMangerProperty[k].index(v))

  def setContents(self, newer_dict):
    self.metaData = newer_dict
    self.updateUI()

  def getContents(self):
    return self.metaData

if __name__ == '__main__':
  app = QApplication(sys.argv)
  app_comp = ModeMangerDialog()
  #app_comp.setGeometry(600, 50, 400, 250)
  app_comp.show()
  sys.exit(app.exec_())
