import serial

portx = "COM4"
bps = 115200
timex = 10
ser = serial.Serial(portx, bps, timeout = timex)
print("串口详情参数：", ser)

print(ser.read(1024))
