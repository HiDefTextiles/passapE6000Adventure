import serial
# print(serial.__version__)
upphaf=-10
munztur=101010101
arduinoData=serial.Serial('com3',115200) # Þarft að athuga hvaða port arduinoinn notar í Arduino IDE !!! BREYTILEGT !!!

while True:
	cmd=input('Please Enter Your Command: ')
	cmd=upphaf+cmd+'!'
	arduinoData.write(cmd.encode())


	
