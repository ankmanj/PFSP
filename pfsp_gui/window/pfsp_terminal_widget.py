import os
import sys
import shutil
import uuid
import logging
import time

from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

from pfsp_gui import utils
from pfsp_gui.termqt import Terminal, TerminalExecIO

class PFSPTerminalWidget(QTabWidget):
    from pfsp_gui.window.pfsp_window import PFSPWindow
    name_session = uuid.uuid4().hex

    def __init__(self, parent:PFSPWindow=None):
        super().__init__(parent)
        self.setTabPosition(QTabWidget.South)
        self.terminalProcess = QProcess(self)
        self.outputProcess = None
        self.initUI()
        self.parentWindow = parent

    def initUI(self):
        widget = QWidget()
        hbox = QHBoxLayout()

        button1 = QPushButton('Build')
        button1.pressed.connect(self.onBuildEvent)
        hbox.addWidget(button1)

        button2 = QPushButton("Run")
        button2.pressed.connect(self.onRunEvent)
        hbox.addWidget(button2)

        button3 = QPushButton("Break")
        button3.pressed.connect(self.onBreakEvent)
        hbox.addWidget(button3)

        hbox.setContentsMargins(0,0,0,0)

        widget.setLayout(hbox)
        widget.setContentsMargins(0,0,0,0)
        # self.setCornerWidget(button1, Qt.TopLeftCorner)
        # self.setCornerWidget(button2, Qt.TopRightCorner)
        self.setCornerWidget(widget)

        self.setTabsClosable(False)
        self.setMovable(False)

        ############################################
        # self.outputTab = QWidget(self)
        # vbox = QVBoxLayout()
        # self.text_edit = QPlainTextEdit()
        # self.text_edit.setStyleSheet("QPlainTextEdit {color: #e0e0e0;}")
        # self.text_edit.setReadOnly(True)
        # vbox.addWidget(self.text_edit)
        # self.outputTab.setLayout(vbox)
        # self.addTab(self.outputTab, "Output")

        # self.setCurrentWidget(self.outputTab)
        # self.outputTab.setFocus()
        #################################################
        # self.terminalTab = QWidget(self)
        # # self.terminalTab.setFixedSize(1280, 270)
        #
        # self.addTab(self.terminalTab, "Terminal")
        # self.setCurrentWidget(self.terminalTab)
        # self.terminalTab.setFocus()

        self.terminalProcess.finished.connect(self.onProcessFinishedEvent)

        # self.terminalProcess.start('urxvt', ['-embed', wid])
        # self.terminalProcess.start(
        #     'xterm',
        #     ["-fn", "-misc-fixed-*-*-*-*-18-*-*-*-*-*-*-*",
        #      '-into', str(int(self.terminalTab.winId())), '-geometry', '210x20', '+maximized', '-rightbar', '-sb',
        #      '-hold', '-e', 'bash -c \"ls\"']
        # )

        started,procId = self.terminalProcess.startDetached(
            'xterm',
            [
                '-fn', '-misc-fixed-*-*-*-*-18-*-*-*-*-*-*-*',
                '-e', 'tmux', 'new', '-s', self.name_session
            ],
            # ['/bin/bash'],
            '.'
        )

        # self.terminalLayout = QVBoxLayout(self)
        # self.terminalTab.setLayout(self.terminalLayout)


        import pgi
        pgi.install_as_gi()

        import gi
        import time

        gi.require_version("Wnck", "3.0")
        from gi.repository import Wnck, Gdk

        attempts = 0
        while attempts < 10:
            screen = Wnck.Screen.get_default()
            screen.force_update()
            time.sleep(0.2)
            while Gdk.events_pending():
                Gdk.event_get()
            for w in screen.get_windows():
                print(attempts, w.get_pid(), procId, w.get_pid() == procId)
                if w.get_pid() == procId:
                    self.terminalProcess.setParent(self)

                    window = QWindow.fromWinId(w.get_xid())
                    window.setFlags(Qt.FramelessWindowHint)

                    container = QWidget.createWindowContainer(window)
                    self.setMinimumHeight(200)
                    self.addTab(container, "Terminal")

                    return
            attempts += 1
        '''

        self.logger = logging.getLogger()
        self.logger.setLevel(logging.DEBUG)
        handler = logging.StreamHandler()
        formatter = logging.Formatter(
            "[%(asctime)s] > "
            "[%(filename)s:%(lineno)d] %(message)s"
        )
        handler.setFormatter(formatter)
        self.logger.addHandler(handler)

        # self.terminal = Terminal(400, 300, logger=logger)

        window = QWidget()
        layout = QHBoxLayout()
        self.terminal = Terminal(400, 200, logger=self.logger)
        self.terminal.maximum_line_history = 2000
        scroll = QScrollBar(Qt.Vertical, self.terminal)
        self.terminal.connect_scroll_bar(scroll)

        layout.addWidget(self.terminal)
        layout.addWidget(scroll)
        layout.setSpacing(0)
        window.setLayout(layout)

        # self.setMinimumHeight(200)
        self.addTab(window, "Terminal")

        self.terminal_io = TerminalExecIO(self.terminal.row_len, self.terminal.col_len,
                                                      "/bin/bash", logger=self.logger)
        self.terminal_io.stdout_callback = self.terminal.stdout
        self.terminal.stdin_callback = self.terminal_io.write
        self.terminal.resize_callback = self.terminal_io.resize
        time.sleep(0.5)
        self.terminal_io.spawn()
        '''

    def closeEvent(self, event):
        self.terminalProcess.terminate()
        self.terminalProcess.waitForFinished(1000)

    def message(self, s):
        self.text_edit.appendPlainText(s)

    def handle_stderr(self):
        data = self.outputProcess.readAllStandardError()
        stderr = bytes(data).decode("utf8")
        self.message(stderr)

    def handle_stdout(self):
        data = self.outputProcess.readAllStandardOutput()
        stdout = bytes(data).decode("utf8")
        self.message(stdout)

    def handle_state(self, state):
        states = {
            QProcess.NotRunning: 'Not running',
            QProcess.Starting: 'Starting',
            QProcess.Running: 'Running',
        }
        state_name = states[state]
        self.message(f"State changed: {state_name}")

    def handle_error(self, processError):
        print(processError)

    def process_finished(self, exitCode, exitStatus):
        self.message("Process finished.")
        print('exitCode:', exitCode, 'exitStatus:', exitStatus)
        self.outputProcess = None

    def onRunEvent(self):
        current_nodeeditor = self.parentWindow.getCurrentNodeEditorWidget()
        if current_nodeeditor is not None:
            try:
                if self.outputProcess is None:
                    QProcess.execute(
                        "tmux", [
                            "send-keys", "-t", self.name_session,
                            'sh simple_run.sh',
                            "Enter"
                        ]
                    )
                    '''

                    self.terminal_io.write(b'sh run_pfsp_first.sh; sh run_pfsp_second.sh\r')
                    '''
            except Exception as e:
                utils.dumpException(e)

    def onBuildEvent(self):
        current_nodeeditor = self.parentWindow.getCurrentNodeEditorWidget()
        if current_nodeeditor is not None:
            try:
                if self.outputProcess is None:
                    os.chdir("pfsp_core")

                    workDirectories = [ './pfsp_config/xml', './pfsp_config/json' ]
                    for folder_name in workDirectories:
                        if os.path.isdir(folder_name): shutil.rmtree(folder_name)
                        os.makedirs(folder_name)

                    current_nodeeditor.fileExportAsXML('pfsp_config/xml/pfsp_main_config.xml')
                    current_nodeeditor.fileExportAsJSON('pfsp_config/json/pfsp_main_config.json')

                    QProcess.execute(
                        "tmux", [
                            "send-keys", "-t", self.name_session,
                            'cd ./pfsp_core; sh run_pfsp_first.sh; sh run_pfsp_second.sh; cd ../',
                            "Enter"
                        ]
                    )
                    '''
                    self.terminal_io = TerminalExecIO(self.terminal.row_len, self.terminal.col_len,
                                                      "/bin/bash", logger=self.logger)
                    self.terminal_io.stdout_callback = self.terminal.stdout
                    self.terminal.stdin_callback = self.terminal_io.write
                    self.terminal.resize_callback = self.terminal_io.resize
                    self.terminal_io.spawn()
                    self.terminal_io.write(b'sh run_pfsp_first.sh; sh run_pfsp_second.sh\r')
                    '''
            except Exception as e:
                utils.dumpException(e)
            finally:
                os.chdir('../')

    def onBreakEvent(self):
        current_nodeeditor = self.parentWindow.getCurrentNodeEditorWidget()
        if current_nodeeditor is not None:
            try:
                if self.outputProcess is None:
                    QProcess.execute(
                        "tmux", [
                            "send-keys", "-t", self.name_session,
                            # '\032', # Ctrl+z
                            '\x03', # Ctrl+c
                            "Enter"
                        ]
                    )
                    '''
                    self.terminal_io = TerminalExecIO(self.terminal.row_len, self.terminal.col_len,
                                                      "/bin/bash", logger=self.logger)
                    self.terminal_io.stdout_callback = self.terminal.stdout
                    self.terminal.stdin_callback = self.terminal_io.write
                    self.terminal.resize_callback = self.terminal_io.resize
                    self.terminal_io.spawn()
                    self.terminal_io.write(b'sh run_pfsp_first.sh; sh run_pfsp_second.sh\r')
                    '''
            except Exception as e:
                utils.dumpException(e)

    def onStopEvent(self):
        # if self.terminalProcess:
        #     self.terminalProcess.terminate()
        #     self.terminalProcess.waitForFinished(1000)
        QProcess.execute("tmux", ["kill-session", "-t", self.name_session])

    def onProcessFinishedEvent(self):
        print("Process finished.")
        # self.process = None
