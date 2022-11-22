
def on_reset():
    #glb_filter(GRP_ALL)
    expect("@timestamp RecordID::LOG_INFO dutSensor Check Sensor ID Firmware 0.1234")
    expect("@timestamp RecordID::LOG_INFO dutSensor Request Sample SFU  00 11 22 33 44 55 66 77 88 99 AA 00")
    expect("@timestamp RecordID::LOG_INFO dutSensor Parse Bytes arr:  00 11 22 33 44 55 66 77 88 99 AA 00 Result 32003033 122   1e+01   4e+00")
    expect_pause()
    continue_test()   

    # Startup secquence of all sensors
    expect("@timestamp TE0-Arm  Obj=dutSensor.m_timeEvt,AO=dutSensor,Tim=50,Int=50")
    expect("@timestamp TE0-Arm  Obj=dutSensor.m_samplingTimerEvt,AO=dutSensor,Tim=10,Int=10")
    expect("===RTC===> St-Init  Obj=dutSensor,State=QHsm::top->Sensor::off")
    expect("@timestamp RecordID::LOG_TOPIC 0")
    expect("===RTC===> St-Entry Obj=dutSensor,State=Sensor::off")
    expect("@timestamp Init===> Obj=dutSensor,State=Sensor::off")

    # Program running
    expect_run()

def on_setup ():
    expect("@timestamp CommandID::FIXTURE_SETUP")

def on_teardown():
    expect("@timestamp CommandID::FIXTURE_TEARDOWN")


# tests...
test("Assert 0")

ao_filter("dutSensor")

probe("dutSensor.testProbe", 200033)
command("CommandID::COMMAND_X")
expect("@timestamp TstProbe Fun=dutSensor.testProbe,Data=200033")
expect("@timestamp RecordID::LOG_INFO dutSensor Request Sample SFU  00 11 22 33 44 55 66 77 88 99 AA 00")
expect("@timestamp CommandID::COMMAND_X 0")
expect("@timestamp Trg-Done QS_RX_COMMAND")
