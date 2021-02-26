import serial
import socket 
import sys
import datetime

COSMOS_PORT = 8010

port = sys.argv[1]

print "Starting SDE interface on port %s."%port

ser = serial.Serial(port, 115200, timeout=0.2)

txsock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
rxsock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
rxsock.settimeout(0.1)
rxsock.bind(('localhost', 8011))

def gettime():
    now = datetime.datetime.now()
    millisecond = now.microsecond / 1000
    return now.strftime('%Y-%m-%dT%H:%M:%S')+".%d"%millisecond

def processPacket(packet):
    #print ' '.join(["%02x"%ord(x) for x in packet])
    if len(packet) < 1:
        print "Empty packet!"
        return
    opcode = ord(packet[0])
    packet = packet[1:]
    if opcode == 3:
        #EVR
        print gettime(), "EVR:",
        EVRID = ord(packet[0])
        packet = packet[1:]
        if EVRID == 0:
            print "RESERVED EVR!"
        elif EVRID == 1:
            #startup sequence
            seqloc = ord(packet[0])
            if seqloc == 0:
                print "SUS: #### Initializing ####"
            elif seqloc == 1:
                print "SUS: SPI Initialized"
            elif seqloc == 2:
                print "SUS: EEPROM Initalized"
            elif seqloc == 3:
                print "SUS: ADC Initialized"
            elif seqloc == 4:
                print "SUS: TMC429 Initialized"
            elif seqloc == 5:
                print "SUS: TMC2130s Initialized"
            elif seqloc == 7:
                print "SUS: Initialization done."
            else:
                print "Unknown startup sequence EVR received."
        elif EVRID == 2:
            #SW ver
            print "Software version: %s"%packet
        else:
            print "Undefined EVR: EVR ID = 0x%02x"%EVRID
    else:
        txsock.sendto(chr(opcode)+packet, ('127.0.0.1', COSMOS_PORT))

try:
    thisPacket = ""
    isInPacket = False
    isEscape = False
    while True:
        data = ser.read(256)
        if data:
            for char in data:
                if ord(char) == 0x7e:
                    if isInPacket:
                        isInPacket = False
                        processPacket(thisPacket)
                        thisPacket = ""
                elif ord(char) == 0x7f:
                    if not isInPacket:
                        isInPacket = True
                        thisPacket = ""
                elif ord(char) == 0x7d:
                    #Escape
                    isEscape = True
                else:
                    if isEscape:
                        thisPacket += chr(ord(char) ^ 0b00100000)
                        isEscape = False
                    else:
                        thisPacket += char
        try:
            cmddata, addr = rxsock.recvfrom(512)
        except socket.timeout:
            cmddata = None
        if cmddata:
            print "Got command"
            txbuf = chr(0x7f)
            for char in cmddata:
                if char in [0x7d, 0x7e, 0x7f]:
                    txbuf += chr(0x7d)
                    txbuf += chr(ord(char) ^ 0b00100000)
                else:
                    txbuf += char
            txbuf += chr(0x7e)
            #txbuf = 'abc'
            ser.write(txbuf)
            print "Sent %s"%' '.join(["%02x"%ord(x) for x in txbuf])


except KeyboardInterrupt:
    print "Shutting down."
    ser.close()
    txsock.close()
    rxsock.close()

except:
    ser.close()
    txsock.close()
    rxsock.close()
    raise
    
finally:
    ser.close()
    txsock.close()
    rxsock.close()