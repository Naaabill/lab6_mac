#! /usr/bin/env python3
# -*- coding: UTF-8 -*-

from PyQt5.QtWidgets import *
from PyQt5.QtCore import *
from PyQt5.QtGui import *

import sys,time

import threading
# Communication part
import struct
pipc_magic = 0xdeadbeef
import posix_ipc as pipc
mq_to_qemu = pipc.MessageQueue("/to_qemu",flags=pipc.O_CREAT, read=False, write=True)
mq_from_qemu = pipc.MessageQueue("/from_qemu",flags=pipc.O_CREAT, read=True, write=False)

class MainWindow(QMainWindow):
    def __init__(self, *args, **kwargs):
        super(MainWindow, self).__init__(*args,**kwargs)
        self.setWindowTitle("QEmu GPIO interface for STM32F303")
        self.gpios = {'A':[],'B':[],'C':[],'D':[],'F':[]}
        self.gpioIds = {'A':0,'B':1,'C':2,'D':3,'F':4}
        layoutV = QVBoxLayout()
        self.palOn = QPalette()
        self.palOn.setColor(QPalette.Base,Qt.darkGreen)
        self.palOff = QPalette()
        self.palOff.setColor(QPalette.Base,Qt.darkRed)
        self.palDis = QPalette()
        self.palDis.setColor(QPalette.Base,Qt.gray)
        
        
        for gpio in self.gpios:
            #Gpio name
            gbox=QGroupBox("GPIO"+gpio)
            #Gpio pin state
            layout = QHBoxLayout()
            for i in range(16):
                chkbox=QCheckBox("P"+str(gpio)+str(i))
                chkbox.setAutoFillBackground(True)
                chkbox.setPalette(self.palOff)
                self.gpios[gpio].append(chkbox)
                layout.addWidget(chkbox)
                #We have to copy arguments (gpio=gpio, i=i) so that 
                #there is a new lambda for each iteration
                chkbox.stateChanged.connect(lambda x, gpio=gpio,i=i: self.buttonChecked(x,self.gpioIds[gpio],i))
            gbox.setLayout(layout)
            layoutV.addWidget(gbox)
            self.setGPIO(gpio,0xFFFF,0) #set all as output / 0

        label = QLabel("checkbox in color (red/green) are configured as output. Checkbox in grey are inputs and can be checked/unchecked")
        label.setAlignment(Qt.AlignHCenter | Qt.AlignVCenter)
        layoutV.addWidget(label)
        widget = QWidget()
        widget.setLayout(layoutV)

        self.setCentralWidget(widget)

    def buttonChecked(self,state,gpioId,pin):
        """ called when a checkbox is checked """
        print("send msg: GPIO "+str(gpioId)+", pin "+str(pin)+', state '+str(state==Qt.Checked) )
        s=struct.pack("=IIII",pipc_magic,pin,state==Qt.Checked,gpioId)
        mq_to_qemu.send(s)
        

    def setGPIO(self,port,dir_mask,output):
        for pin in range(16):
            chkbox = self.gpios[port][pin]
            out = (dir_mask >> pin) & 1
            chkbox.setCheckable(not out)
            if out:
                state = (output >> pin) & 1
                chkbox.setChecked(state)
                if state:
                    pal = self.palOn
                else:
                    pal = self.palOff
            else:
                pal = self.palDis
            chkbox.setPalette(pal)

app = QApplication(sys.argv)
window = MainWindow()

def receiver():
    while True:
        msg = mq_from_qemu.receive()
        mg, changed_out, dir_mask,output,gpio = struct.unpack("=IHHHH",msg[0])
        #print("mg=",mg," pin=",pin," gpio=",gpio," state=",state) 
        if mg != pipc_magic:
            raise Exception("Wrong magic number in GPIO IPC message") 
        name = ['A','B','C','D','F']
        if gpio < 5:
            window.setGPIO(name[gpio],dir_mask,output) #TODO: thread safe?
        #required if we get too much messages to let time for the UI.
        time.sleep(.01)

window.show()

thread = threading.Thread(target=receiver)
thread.daemon = True
thread.start()

app.exec_() #event loop
