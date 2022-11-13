import socket
import sys
import click
from subprocess import Popen, run, PIPE, CREATE_NEW_CONSOLE


class QSpyVis:
    def start_qspy(self, targetPort : str, targetBaud, hostSocket = 6601):
            # launch a QSPY process that is opened in a dedicated shell
            click.secho("Opening QSPY session...")
            qspyCmd = 'qspy -c ' + targetPort + ' -b ' + str(targetBaud) + ' -t ' + str(hostSocket) 
            print(qspyCmd)
            self.qspy_process = Popen('qspy', creationflags=CREATE_NEW_CONSOLE, stdin=PIPE)

            click.echo("TestRunner connecting to qspy on port " + str(hostSocket))
            self.qspy_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.qspy_socket.connect(('localhost', hostSocket))

myPort = QSpyVis()
socketPort = 6601

if __name__ == "__main__":
    myPort.start_qspy("COM9", 921600, socketPort)