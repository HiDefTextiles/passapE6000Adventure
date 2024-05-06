# Passap E6000 rebuilt and replaced console,
# Passap Electra 4600 rebuilt and replaced controller

# created by Irene Wolf
# hreinskrifað af Elíasi Lúðvíkssyni
# Notobene að ég get ekki lagfært allt þar sem ég
# 	er bara með aðgang að kóðanum en ekki hardware
# Öll output verða því að halda sér eins og þau eru þó ég dragi þau í efa

# Version 36.3, 23.7.2021
# Python code for Raspberry Pi 3, OS Debian Buster

# Importum bara öllu sem hún importaði

import random, sys
import PyCmdMessenger
import struct
import RPi.GPIO as GPIO
import sqlite3
import time
import datetime
import os


from PyQt5.QtWidgets import QApplication, QMainWindow, QVBoxLayout, QGridLayout, QAction
from PyQt5.QtWidgets import QWidget, QLineEdit, QMessageBox, QLabel, QFileDialog, QDialog
from PyQt5.QtGui import QFont, QColor
from PyQt5.QtCore import pyqtSlot
from PyQt5 import QtCore, QtWidgets, QtGui

import csv

from PIL import Image

# Bætum við numpy fyrir fylkja breytingar

import numpy as np

# --------------------------------------------------------------------------
# Setup Pin for interrupt
# --------------------------------------------------------------------------

# GPIO Pin to get signals form Arduino front lock
right_GPIO = 7
left_GPIO = 11
directionchange_GPIO = 13

GPIO.setmode(GPIO.BOARD)

GPIO.setup(right_GPIO, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(left_GPIO, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(directionchange_GPIO, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
# Útskýring fyrir pull down og pull up
# An input gpio will float between 0 and 1 if it's not connected to a voltage.

# The pull-up/downs supply that voltage so that the gpio will have a defined value UNTIL overridden by a stronger force.

# You should set a pull-down (to 0) when you expect the stronger force to pull it up to 1.

# You should set a pull-up (to 1) when you expect the stronger force to pull it down to 0.

# Otherwise the gpio will not change state and you'll never know about the external event.

# -----------------------------------------------------------------------------
# Variables
# -----------------------------------------------------------------------------

# Ég mun setja in variables hérna eftir þörf en ég dreg þær allar stranglega í efa


#--------------------------------------------------------------------------
# Communication Arduino - Raspi
#--------------------------------------------------------------------------

motor = PyCmdMessenger.ArduinoBoard('/dev/ttyUSB-ArduinoMotor',baud_rate=19200)
motor.enable_dtr=False
motor.int_bytes=4

commands =[["slowDownSpeed", "i"],["sbSlowDownSpeed", "i"],
           ["setColourChange", "?"],["sbColourChange", "?"],
           ["setRowEndStopp", "?"],["sbRowEndStopp", "?"],
           ["setFormStopp", ""],["sbFormStopp", "s"],
           ["setDrive_left", ""], ["sbDrive_left", "s"],
           ["setDrive_right",""], ["sbDrive_right", "s"]]

connectionMotor= PyCmdMessenger.CmdMessenger(motor,commands)

# connect to VNB Arduino M0 (front lock)
a = PyCmdMessenger.ArduinoBoard('/dev/ttyUSB-ArduinoVNB',115200)
a.enable_dtr=False
a.int_bytes=4
connectionVNB = PyCmdMessenger.CmdMessenger(a, [["sPat", "b*"],["sbPat", "b*"],
                                             ["setEmptyPat", "b*"], ["sbEmptyPat", "b*"],
                                             ["setNPos", "l"],["sbNPos", "l*"],
                                             ["getNPos", ""],["sendNPos", "l"],   
                                             ["setLeEnd", "i"], ["sbLeEnd", "l"],
                                             ["setRiEnd", "i"], ["sbRiEnd", "l"],
                                             ["getCursorPos", ""], ["sendCursorPos", "l"],                                           
                                             ])


# connect to HNB Arduino M0 Pro (back lock)
b = PyCmdMessenger.ArduinoBoard('/dev/ttyUSB-ArduinoHNB',115200)
b.enable_dtr=False
b.int_bytes=4
connectionHNB = PyCmdMessenger.CmdMessenger(b, [["sPat", "b*"],["sbPat", "b*"],
                                             ["setEmptyPat", "b*"], ["sbEmptyPat", "b*"],                                              
                                             ["setNPos", "l"],["sbNPos", "l*"],  
                                             ["getNPos", ""],["sendNPos", "l"],  
                                             ["setLeEnd", "i"], ["sbLeEnd", "l"],
                                             ["setRiEnd", "i"], ["sbRiEnd", "l"],
                                             ])    

def pattern_Array(listMy):
	patternArray = np.zeros(listMy.shape[0],23)
	for x in range(0,listMy.shape[0]):

		pattern = np.full((1,23),0b0)

		for m in range(0,22): # Hér er mögulega eh rugl þar sem pattern hefur lengdina 23, range(0,23) mundi þá ítra allt
			for y in range(0,8):
				n = y + m*7
				if listMy[x,n] == 0:
					pattern[0,m] = pattern[0,m].__lshift__(1)
				else:
					pattern[0,m] = pattern
		patternArray[x] = pattern
	return patternArray

def get_pattern(data):
	filename = Image.open(data)
	widthMy = filename.size[0]
	heightMy = filename.size[1]
	const0 = np.zeros((heightMy,widthMy))
	black = const0
	white = const0
	green = const0
	blue = const0
	red = const0
	for x in range(0, heightMy):
		for y in range(0, widthMy):
			pixel = (y, x)

			redMy, greenMy, blueMy = filename.getpixel(pixel)
			black[x,y] = int(redMy == 0 and greenMy == 0 and blueMy == 0)
			white[x,y] = int(redMy == 255 and greenMy == 255 and blueMy == 255)
			green[x,y] = int(redMy < 20 and greenMy > 200 and blueMy < 20)
			blue[x,y] = int(redMy < 20 and greenMy < 20 and blueMy > 200)
			red[x,y] = int(redMy > 200 and greenMy < 20 and blueMy < 20)
	return ( pattern_Array(black), 
		 pattern_Array(white), 
		 pattern_Array(green), 
		 pattern_Array(blue),
		 pattern_Array(red),
		 pattern_Array(black+white),
		pattern_Array(black+green),
		pattern_Array(black+blue),
		pattern_Array(black+red)
	)

			

