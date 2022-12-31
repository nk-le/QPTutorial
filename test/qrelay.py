import click 
import os 
import socket
import serial 
import time
import threading
import sys 
import logging 
import coloredlogs

# Setup color for console output
coloredlogs.install()

# create logger 
config = logging.StreamHandler()

logger = logging.getLogger("QSPY_RELAY")
logger.setLevel(logging.DEBUG)
logger.addHandler(config)

COM_PORT = "COM8"
HOST = "127.0.0.1"
Q_SPY_PORT = 5501

class QRelay:

    def __init__(self, mcuSerialPort = COM_PORT, qspyTCPPort = Q_SPY_PORT, hostAddr = HOST) -> None:
        self.stopEvt = threading.Event()

        logger.warning("Starting QSpy Relaying client...")

        # Port configuration
        # QSPY TCP Port
        logging.info("Setup QSpy Socket")    
        self.qspyHostAddr = hostAddr
        self.qspyTCPPort = qspyTCPPort
        self.qspyPort = socket.socket(socket.AF_INET, socket.SOCK_STREAM)   

        # MCU Serial Port
        logging.info("Setup MCU Serial Port")
        self.mcuSerPort = mcuSerialPort
        
        # Define threading
        self.qspy2mcuThread = threading.Thread(group=None, name="Qspy2Mcu", target=self.qspy2mcu, args = (self.stopEvt,))
        self.qspy2mcuThread.daemon = True
        self.mcu2qspyThread = threading.Thread(group=None, name="Mcu2Qspy", target=self.mcu2qspy, args = (self.stopEvt,))
        self.mcu2qspyThread.daemon = True 

    def connect_qspy_tcp(self):
        try:
            logging.info("Connecting QSPY TCP Socket...")  
            self.qspyPort.connect((self.qspyHostAddr, self.qspyTCPPort))
            logging.warning("CONNECTED QSPY TCP Socket %d!", self.qspyTCPPort)
            return True
        except Exception:
            logging.critical("FAILED CONNECTING QSPY TCP SOCKET!")

        

    def connect_mcu_serial(self, mcuSerialPort: str, serBaud = 115200, tryTime_s = 15):
        self.mcuSerPort = mcuSerialPort

        startTime_ms  = time.time()

        logging.info("Try connecting to MCU over port %s ... within %d seconds..." , self.mcuSerPort, tryTime_s)
        isSuccess = False
        while(time.time() - startTime_ms < tryTime_s):
            try:
                self.mcuPort = serial.Serial(self.mcuSerPort, serBaud, timeout = 3)
                logging.warning("CONNECTED MCU PORT %s!", mcuSerialPort)
                isSuccess = True
                break
            except Exception:
                isSuccess = False
                time.sleep(0.25) # Wait 250 ms
                pass

        if (isSuccess == False):     
            logging.error("FAILED! NO MCU FOUND...")
            self.stop() # Exit the program

        return isSuccess

    def qspy2mcu(self, evt: threading.Event):
        # Init
        while True:
            try:
                # Get data from qspy port 
                try:
                    data = self.qspyPort.recv(1024)
                    logging.info("QSPY --> MCU")
                    logging.info(data)
                except Exception:
                    logging.error("Error Connection to QSpy TCP Ports")
                    data = None 
                    break            
                    
                # Send these data to the teensy                    
                try:
                    self.mcuPort.write(data)
                except:
                    logging.error("MCU Port no longer available. Trying to reconnect...")
                    # Try to reconnect to teensy
                    self.connect_mcu_serial(self.mcuSerPort)

                time.sleep(0.01)
                if evt.is_set():
                    break

            except Exception as exc:
                logging.error("Unhandled Exception raised thread qspy2mcu")
                logging.error(exc, fg='red')
           
        self.stop()
        
    def mcu2qspy(self, evt: threading.Event):
        # Init
        
        while True:
            try:
                try:
                    # Sending data from MCU to QSPY
                    data = self.mcuPort.read(1)
                    data+= self.mcuPort.read(self.mcuPort.inWaiting())

                except Exception:
                    logging.error("MCU Port no longer available.")
                    # Try to reconnect to teensy
                    self.connect_mcu_serial(self.mcuSerPort)
                    data = None

                try:
                    if data is not None:
                        self.qspyPort.sendall(data)
                except Exception:
                    logging.error("Error Connection to QSpy TCP Ports")
                    break


                time.sleep(0.01)
                if evt.is_set():
                    break

            except Exception as exc:
                logging.error("Exception unhandled in thread mcu2qspy")
                logging.error(exc)
                
        self.stop()   

    def run(self):
        logging.warning("Starting QSPY and QRELAY threads")
        
        isValid = self.connect_qspy_tcp()   
        if(isValid):
            isValid = self.connect_mcu_serial(self.mcuSerPort)     
        if(isValid):
            # Testing         
            self.mcu2qspyThread.start()
            self.qspy2mcuThread.start()        

            while True:
                try:
                    time.sleep(100)

                except (KeyboardInterrupt, SystemExit):
                    logging.warning('Received keyboard interrupt! Exit...')
                    myQRelay.stop()
                    break

    def stop(self):
        # Set the stop evt
        self.stopEvt.set()
        logging.critical("PROGRAM STOP!")
        # Wait to exit
        #sys.exit()
 
if __name__ == "__main__":
    if(len(sys.argv) == 2):
        mcuSerialPort = sys.argv[1]
        myQRelay = QRelay(mcuSerialPort)
    elif(len(sys.argv) == 3):
        mcuSerialPort = sys.argv[1]
        qspyTCPPort = int(sys.argv[2])
        myQRelay = QRelay(mcuSerialPort, qspyTCPPort)
    elif(len(sys.argv) == 4):
        mcuSerialPort = sys.argv[1]
        qspyTCPPort = int(sys.argv[2])
        hostAddr = sys.argv[3] 
        myQRelay = QRelay(mcuSerialPort, qspyTCPPort, hostAddr)
    else:
        logging.error("MCU Serial port was not given!. Exit...")
        sys.exit(1) 

    
    myQRelay.run()
    logging.critical("QSPY_RELAY END!")
