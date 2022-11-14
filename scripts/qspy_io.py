import socket
import sys
import click
from subprocess import Popen, run, PIPE, CREATE_NEW_CONSOLE
import glob
import os
from time import sleep
import threading
import shlex

class QSpyVis:
    def start_qspy(self, targetPort : str, targetBaud, hostSocket = 6601):
            # launch a QSPY process that is opened in a dedicated shell
            click.secho("Opening QSPY session...")
            
            self.qspy_process = Popen(['qspy','-c', targetPort, '-b', str(targetBaud), '-u', str(hostSocket)], creationflags=CREATE_NEW_CONSOLE, stdin=PIPE)

            # click.echo("TestRunner connecting to qspy on port " + str(hostSocket))
            # self.qspy_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            # self.qspy_socket.connect(('localhost', hostSocket))

    def start_qutest(self, test_dir, test_name, qspySocket:int):
        
        # find python files starting with "test_" in the directory of the current test.
        files = glob.glob(os.path.join(test_dir, test_name, "test_*.py"))
        #if(self.options.verbose):
        click.secho("Found the following test files: {}".format(files), fg='green')
        fileListString = ' '.join(files)
        fileListString ='scripts\*\*.py'
        # Find test scripts, run qutest and connect to the qspy process on the default port.  
        thisPath = os.path.dirname(os.path.realpath(__file__))
        qutest_path = os.path.join(thisPath, 'qutest.py') 
     
        click.echo("Qutest Path " + qutest_path)
        # qutest will write its output to PIPE, that we can read periodically.
        # However, since the reading is blocking, we will delegate that to another thread, from where the output
        # will be processed.
        #self.qutest_process = Popen(['python', '-u', qutest_path, fileListString], stdout=PIPE, stderr=PIPE, universal_newlines=True)
        
        #self.qutest_process = Popen(['python', qutest_path, fileListString, '"" localhost:6601'], creationflags=CREATE_NEW_CONSOLE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
        cmd = 'python {} {} "" localhost:{}'.format(qutest_path, fileListString, qspySocket)
        print(cmd)
        self.qutest_process = Popen(cmd, stdin=PIPE, stdout=PIPE, stderr=PIPE, universal_newlines=True)
        #self.qutest_process = self.run_command(cmd)
        #sleep(0.1)

        # # Start polling
        #self.qutest_polling_thread = threading.Thread(group=None, name="qutestPolling", target=self.qutest_polling_loop)
        #self.qutest_polling_thread.start()

    def run_command(self, command):
        process = Popen(command, stdout=PIPE)
        while True:
            output = process.stdout.readline()
            if output == '' and process.poll() is not None:
                break
            if output:
                print (output.strip())
        rc = process.poll()
        return rc

    def qutest_polling_loop(self):
        click.secho("Starting polling of qutest...", fg='green')
        #Poll qutest output while the process is still alive.
        while self.qutest_process.poll() is not None:
            click.echo("Polling qutest...")

            try:
            
                # Read all lines in STDOUT    
                #self.qutest_process.stdout.flush()
                #line = self.qutest_process.stdout.readline()
                # errors = self.qutest_process.stderr.read()
                line, errors = self.qutest_process.communicate()
                if line is not None:
                    # if(self.options.verbose):
                    #     click.echo("QUTEST: {}".format(line), nl=False)
                    click.echo(line, nl=False)
                    # #TODO: Parse lines. Similar to: https://github.com/platformio/platformio-core/blob/develop/platformio/test/runners/unity.py
                    # # Example output:
                    # # QUTEST: Copyright (c) 2005-2022 Quantum Leaps, www.state-machine.com
                    # # QUTEST: Attaching to QSpy (localhost:7701) ... OK
                    # # QUTEST: --------------------------------------------------
                    # # QUTEST: Group:D:\WorkDir\AMDC-Drone\c-uav\Dependencies\qpcpp-teensy\test\test_Blinky_T4\test_blinky.py
                    # # QUTEST: TIMEOUT_SIG->Blinky::inst: FAIL (3.978s):
                    # # QUTEST: exp: "0000000001 Query-SM Obj=Blinky::inst,State=Blinky::off"
                    # # QUTEST: got: "           Trg-ERR  QS_RX_AO_FILTER"
                    # # QUTEST: timeEvt->off (tick): FAIL (4.094s):
                    # # QUTEST:   @D:\WorkDir\AMDC-Drone\c-uav\Dependencies\qpcpp-teensy\test\test_Blinky_T4\test_blinky.py:44
                    # # QUTEST: exp: "0000000001 TE0-Post Obj=Blinky::inst.m_timeEvt,Sig=TIMEOUT_SIG,AO=Blinky::inst"
                    # # QUTEST: got: "0000000001 TE0-Post Obj=Blinky::inst.m_timeEvt,Sig=TIMEOUT_SIG,AO=AO_Blinky"
                    # # QUTEST: timeEvt->on (tick): FAIL (0.000s):
                    # # QUTEST:   @D:\WorkDir\AMDC-Drone\c-uav\Dependencies\qpcpp-teensy\test\test_Blinky_T4\test_blinky.py:53
                    # # QUTEST: NORESET-test follows a failed test
                    # # QUTEST: ============= Target: 220622_070909 ==============
                    # # QUTEST: 1 Groups, 3 Tests, 3 Failures, 0 Skipped (8.156s)
                    # # QUTEST:     FAIL    

                    # try:
                    #     line = strip_ansi_codes(line or "").strip()
                    #     match = self.TESTCASE_PARSE_REGEX.search(line)
                    #     #click.echo("match: {}".format(match))
                    #     if not match is None:
                    #         data = match.groupdict()
                    #         #click.echo("data:  {}".format(data))
                    #         test_case = TestCase(
                    #             name=data.get("name").strip(),
                    #             status = TestStatus.from_string(data.get("status"))
                    #         )
                    #         self.test_suite.add_case(test_case)
                    # except Exception as excpt:
                    #     click.secho("Error creating test case: {}".format(excpt))

                    # if all(s in line for s in ("Groups", "Tests", "Failures", "Skipped")):
                    #     # QUtest is printing the summary. Tests are finished.
                    #     if(self.options.verbose):
                    #         click.echo("QUTest has sent the summary.")
                    #         break
                    #     #self.test_suite.on_finish()
                   
            except Exception as ex:
                click.secho("Exception polling qutest: ".format(ex), fg='red')

            # Wait for new data
            sleep(0.1)

        out, err = self.qutest_process.communicate()
        click.echo("qutest seems to have stopped. No longer polling")
        self.isTesting = False
        click.echo(out)
        click.secho(err, fg="red")

myPort = QSpyVis()
socketPort = 6601

if __name__ == "__main__":
    myPort.start_qspy("COM3", 921600, socketPort)

    # Build list of test-scripts to execute for qutest.
    # test_dir = self.project_config.get("platformio", "test_dir")
    # test_name = self.test_suite.test_name
    test_dir = os.path.join(os.getcwd(), "scripts")

    test_name = "test_uart_sensor"
    myPort.start_qutest(test_dir, test_name, socketPort)

    while myPort.qutest_process.stdout.readable():
        line = myPort.qutest_process.stdout.readline()

        if not line:
            print("WTF Bro")
            break


        print(line.strip())