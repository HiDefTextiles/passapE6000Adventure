import serial
# print(serial.__version__)
upphaf=-10
munztur=101010101
arduinoData=serial.Serial('com3',115200, timeout=1000) # Þarft að athuga hvaða port arduinoinn notar í Arduino IDE !!! BREYTILEGT !!!

while True:
	cmd=input('Please Enter Your Command: ')
	# cmd=upphaf+cmd+'!'
	cmd=f'{upphaf}{cmd}'
	arduinoData.write(cmd.encode())
	test = arduinoData.readline()
	print(test.decode())


	
