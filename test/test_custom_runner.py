from ast import Try
from cmath import e
from logging import captureWarnings
from os import popen
from platformio.public import TestRunnerBase
from platformio.test.result import TestCase, TestCaseSource, TestStatus
#from platformio.device.finder import find_serial_port
from platformio.util import strip_ansi_codes
import serial
import click
import string
from subprocess import Popen, run, PIPE, CREATE_NEW_CONSOLE
import socket
import threading
from time import sleep
import time
import sys
import os
import glob
import re

class CustomTestRunner(TestRunnerBase):
    #def __init__(self, mcuSerialPort = COM_PORT, qspyTCPPort = Q_SPY_PORT, hostAddr = HOST) -> None:
    #    super().__init__(mcuSerialPort, qspyTCPPort, hostAddr)

    TESTCASE_PARSE_REGEX = re.compile(
        r"\s?(?P<name>[\w\->:() ]+): (?P<status>FAIL|PASS) \((?P<duration>[^\)]+(?=s))"
            )

    def stage_uploading(self):
        return super().stage_uploading()

    def start_qspyrelay(self):
        click.secho("Opening QRELAY session...")
        # Find test scripts, run qutest and connect to the qspy process on the default port.  
        thisPath = os.path.dirname(os.path.realpath(__file__))
        qrelayPath = os.path.join(thisPath, 'qrelay.py') 

        self.qutest_process = Popen(['python', qrelayPath, "COM8", "5501"], stdout=PIPE, stderr=PIPE, universal_newlines=True)
        sleep(0.1)

    def start_qspy(self):
        # launch a QSPY process that is opened in a dedicated shell
        click.secho("Opening QSPY session...")
        # self.qspy_process = Popen('qspy', creationflags=CREATE_NEW_CONSOLE)
        self.qspy_process = Popen(['qspy', '-t 5501', '-u 6601'], creationflags=CREATE_NEW_CONSOLE, stdin=PIPE)

        click.echo("TestRunner connecting to qspy on port 6601")
        self.qspy_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.qspy_socket.connect(('localhost', 5501))


    def start_qutest(self):
        # Build list of test-scripts to execute for qutest.
        test_dir = self.project_config.get("platformio", "test_dir")
        test_name = self.test_suite.test_name

        # find python files starting with "test_" in the directory of the current test.
        files = glob.glob(os.path.join(test_dir, test_name, "test_*.py"))
        if(self.options.verbose):
            click.secho("Found the following test files: {}".format(files), fg='green')
        fileListString = ' '.join(files)

        # Find test scripts, run qutest and connect to the qspy process on the default port.  
        thisPath = os.path.dirname(os.path.realpath(__file__))
        qutest_path = os.path.join(thisPath, 'qutest.py') 
     
        click.secho("Running qutest: {}".format(qutest_path))
        # qutest will write its output to PIPE, that we can read periodically.
        # However, since the reading is blocking, we will delegate that to another thread, from where the output
        # will be processed.
        self.qutest_process = Popen(['python', qutest_path, fileListString, "''", "localhost:6601"], stdout=PIPE, stderr=PIPE, universal_newlines=True)
        sleep(0.1)

        # # Start polling
        self.qutest_polling_thread = threading.Thread(group=None, name="qutestPolling", target=self.qutest_polling_loop)
        self.qutest_polling_thread.start()

    # Run this function in a dedicated thread to continously poll output from the qutest-process
    # Parse the data and scan for test results.
    def qutest_polling_loop(self):
        click.secho("Starting polling of qutest...", fg='green')
        # Poll qutest output while the process is still alive.
        while self.qutest_process.poll() is None:
            #click.echo("Polling qutest...")

            try:
            
                # Read all lines in STDOUT    
                #self.qutest_process.stdout.flush()
                line = self.qutest_process.stdout.readline()
                # errors = self.qutest_process.stderr.read()

                if line is not None:
                    # if(self.options.verbose):
                    #     click.echo("QUTEST: {}".format(line), nl=False)
                    click.echo(line, nl=False)

                    #TODO: Parse lines. Similar to: https://github.com/platformio/platformio-core/blob/develop/platformio/test/runners/unity.py
                    # Example output:
                    # QUTEST: Copyright (c) 2005-2022 Quantum Leaps, www.state-machine.com
                    # QUTEST: Attaching to QSpy (localhost:7701) ... OK
                    # QUTEST: --------------------------------------------------
                    # QUTEST: Group:D:\WorkDir\AMDC-Drone\c-uav\Dependencies\qpcpp-teensy\test\test_Blinky_T4\test_blinky.py
                    # QUTEST: TIMEOUT_SIG->Blinky::inst: FAIL (3.978s):
                    # QUTEST: exp: "0000000001 Query-SM Obj=Blinky::inst,State=Blinky::off"
                    # QUTEST: got: "           Trg-ERR  QS_RX_AO_FILTER"
                    # QUTEST: timeEvt->off (tick): FAIL (4.094s):
                    # QUTEST:   @D:\WorkDir\AMDC-Drone\c-uav\Dependencies\qpcpp-teensy\test\test_Blinky_T4\test_blinky.py:44
                    # QUTEST: exp: "0000000001 TE0-Post Obj=Blinky::inst.m_timeEvt,Sig=TIMEOUT_SIG,AO=Blinky::inst"
                    # QUTEST: got: "0000000001 TE0-Post Obj=Blinky::inst.m_timeEvt,Sig=TIMEOUT_SIG,AO=AO_Blinky"
                    # QUTEST: timeEvt->on (tick): FAIL (0.000s):
                    # QUTEST:   @D:\WorkDir\AMDC-Drone\c-uav\Dependencies\qpcpp-teensy\test\test_Blinky_T4\test_blinky.py:53
                    # QUTEST: NORESET-test follows a failed test
                    # QUTEST: ============= Target: 220622_070909 ==============
                    # QUTEST: 1 Groups, 3 Tests, 3 Failures, 0 Skipped (8.156s)
                    # QUTEST:     FAIL    

                    try:
                        line = strip_ansi_codes(line or "").strip()
                        match = self.TESTCASE_PARSE_REGEX.search(line)
                        #click.echo("match: {}".format(match))
                        if not match is None:
                            data = match.groupdict()
                            #click.echo("data:  {}".format(data))
                            test_case = TestCase(
                                name=data.get("name").strip(),
                                status = TestStatus.from_string(data.get("status"))
                            )
                            self.test_suite.add_case(test_case)
                    except Exception as excpt:
                        click.secho("Error creating test case: {}".format(excpt))

                    if all(s in line for s in ("Groups", "Tests", "Failures", "Skipped")):
                        # QUtest is printing the summary. Tests are finished.
                        if(self.options.verbose):
                            click.echo("QUTest has sent the summary.")
                            break
                        #self.test_suite.on_finish()
                   
            except Exception as ex:
                click.secho("Exception polling qutest: ".format(ex), fg='red')

            # Wait for new data
            sleep(0.1)

        out, err = self.qutest_process.communicate()
        if(self.options.verbose):        
            click.echo("qutest seems to have stopped. No longer polling")
        self.isTesting = False
        click.echo(out)
        click.secho(err, fg="red")
        self.test_suite.on_finish()
        return


    def connect_mcu_serial(self, mcuSerialPort = "COM8", serBaud = 115200, tryTime_s = 15):
        self.mcuSerPort = mcuSerialPort

        startTime_ms  = time.time()

        #logging.info("Try connecting to MCU over port %s ... within %d seconds..." , self.mcuSerPort, tryTime_s)
        isSuccess = False
        while(time.time() - startTime_ms < tryTime_s):
            try:
                self.mcuPort = serial.Serial(self.mcuSerPort, serBaud, timeout = 3)
                #logging.warning("CONNECTED MCU PORT %s!", mcuSerialPort)
                click.secho("CONNECTED MCU PORT {}!".format(mcuSerialPort))
                isSuccess = True
                break
            except Exception:
                isSuccess = False
                time.sleep(0.25) # Wait 250 ms
                pass

        if (isSuccess == False):     
            #logging.error("FAILED! NO MCU FOUND...")
            sys.exit()

        return isSuccess

    def send_to_MCU(self, bytes):
        try:
            self.serial.write(bytes)
            self.serial.flush()
            click.secho("Sending data to MCU")
        except Exception as exc:
            if(self.options.verbose):
                click.secho("Error sending data to MCU: {}".format(exc))
            self.connect_mcu_serial()

    def run_Serial(self):

        # Run the serial port as long as the test runner is testing...
        while self.isTesting:

            project_options = self.project_config.items(
                env=self.test_suite.env_name, as_dict=True
            )

            # port = find_serial_port(
            #     initial_port=self.get_test_port(),
            #     board_config = self.platform.board_config(project_options["board"]),
            #     upload_protocol=project_options.get("upload_protocol"),
            #     ensure_ready=False)
            port = "COM8"
            baud = 115200

            # try to (re)open the serial port. It should be closed after uploading the program.
            startTime = time.perf_counter()
            TIMEOUT = 10
            while(time.perf_counter() - startTime <= TIMEOUT):
                try:
                    if(self.options.verbose):
                        click.secho("Attempting to open serial port: {}".format(port))
                    self.serial = serial.Serial(port, baud, parity=serial.PARITY_NONE, timeout = 5)
                    if(self.options.verbose):
                        click.secho("{} opened...".format(port))
                    break
                except Exception as e:
                    if(self.options.verbose):
                        click.secho("Could not access serial port: {}".format(port), fg='red')
                    sleep(1)

            # if serial could not be opened withit timeout, give up.
            if(self.serial is None):
                click.secho("Giving up trying to connect to serial port. Device connected?", fg='red')
                return False

            # Continously read serial output and send it to ourself.
            try:
                while(self.isTesting):
                    try:
                        self.on_testing_data_output(self.serial.read(self.serial.in_waiting or 1))
                        sys.stdout.flush()
                        sleep(0.1)
                    except:
                        self.connect_mcu_serial()
                        sleep(1)

            except Exception as exc:
                if(self.options.verbose):
                    click.secho("Exception during serial monitoring: {}".format(exc))
                self.serial = None



    def stage_testing(self):
        # Pausing for a couple of seconds to let Teensy re-establish Serial connection.
        sleep(2)
        self.isTesting = True

        # self.serialThread = threading.Thread(target=self.run_Serial)
        # self.serialThread.start()
        sleep(0.1)

        self.start_qspy()
        self.qspy_polling_thread = threading.Thread(group=None, name="qspyPollingThread", target=self.qspy_polling_loop)
        self.qspy_polling_thread.start()

        #sleep(2)
        #self.start_qspyrelay()

        sleep(2)
        self.start_qutest()

        return self.run_Serial()
        # self.qutest_polling_thread.join()
        # self.qspy_polling_thread.join()




    # This function is executed in a seperate thread and continously forwards commands
    # from QSpy to the MCU via the udp channel.
    def qspy_polling_loop(self):

        click.secho("Starting polling of qspy...", fg='green')
        while self.qspy_process.poll() is None:
            try:
                data = self.qspy_socket.recv(1024)
                if(self.options.verbose > 2):
                    click.secho("QSPY->MCU: {}".format(data), fg='yellow')
                self.send_to_MCU(data)
            except Exception as e:
                if(self.options.verbose):
                    click.secho("Exception sending data to MCU: {}".format(e), fg='red')
                self.connect_mcu_serial()

        if (self.options.verbose):
            click.secho("Stopped polling QSpy. Return code: {}".format(self.qspy_process.returncode))
        outs, errs = self.qspy_process.communicate(timeout=15)
        click.secho(outs, fg='yellow')
        click.secho(errs, fg='red')

    # Processed data received from MCU via debug port.
    # Forwards the received data to the qspy-process.
    def on_testing_data_output(self, data):
        # if (self.options.verbose):
        #     click.secho("D: {}".format(data), fg='yellow')
        self.qspy_socket.sendall(data)
        return super().on_testing_data_output(data)

    # Called when receiving a complete line from the MCU.
    # Since qspy-produces binary data we can ignore this.
    def on_testing_line_output(self, line):
        # click.secho("L: {}".format(line), fg='yellow')
        # self.qspy_socket.send(bytes(line, "utf-8"))       
        return
    
    def teardown(self):
        if(self.options.verbose):
            click.echo("tearing down tests...")
        self.isTesting = False
        sleep(2)

        click.echo("Status of test suite: {}".format(self.test_suite.status))
        # for test in self.test_suite.cases:
        #     test.

        if self.test_suite.status is TestStatus.PASSED:
            # send ESC to qspy to try to stop it.
            self.qspy_process.stdin.write(b'\x1b')
            self.qspy_process.kill()
            self.qspy_polling_thread.join()
            self.qspy_socket.close()
            self.qspy_process.terminate()
        else:
            click.secho("Close Q-SPY console to finalize the tests!", fg='yellow')
        
        return super().teardown()