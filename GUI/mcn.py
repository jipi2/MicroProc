import serial

ser = serial.Serial()
ser.baudrate = 38400
ser.port = 'COM12'
ser.timeout = 1

def getValueFromSensor():
    ser.open()
    try:
        aux = ser.readline()
        line = ser.readline().decode('utf-8').strip()

        if line.startswith('Value ='):
            voltage = float(line.split('=')[1].strip().rstrip('V'))
            ser.close()
            print(f'Voltage = {voltage}')
            return voltage

    except serial.SerialException as e:
        print(f"Failed to open serial port {ser.port}: {e}")

    except Exception as e:
        print(f"An unexpected error occurred: {e}")

    finally:
        if ser.is_open:
            ser.close()


def sendValueToMCN(c):
    ser.open()
    try:
       ser.write(c.encode('utf-8'))
       ser.close() 

    except serial.SerialException as e:
        print(f"Failed to open serial port {ser.port}: {e}")

    except Exception as e:
        print(f"An unexpected error occurred: {e}")

    finally:
        if ser.is_open:
            ser.close()