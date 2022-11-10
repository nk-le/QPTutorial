# include script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

# Check this out ...
# https://www.state-machine.com/qtools/namespacequtest__dsl.html
# https://www.state-machine.com/qtools/namespacequtest__dsl.html#ac04e5d768c1af3bbae0b177d0beb4d95

# preamble...
def on_setup():
    expect("@timestamp FIXTURE_SETUP")

def on_teardown():
    expect("@timestamp FIXTURE_TEARDOWN")

def command_x(par1=0, par2=0, par3=0):
    command("COMMAND_X", par1, par2, par3)

def curr_ap_obj(obj_name):
    current_obj(OBJ_AP, obj_name)


def on_reset():
    expect_pause()
    glb_filter(GRP_ALL)
    continue_test()
    expect("@timestamp TE0-Arm  Obj=dutSensor.m_timeEvt,AO=dutSensor,*")
    expect("@timestamp TE0-Arm  Obj=dutSensor.m_samplingTimerEvt,AO=dutSensor,*")
    expect("===RTC===> St-Init  Obj=dutSensor,State=QHsm::top->Sensor::off")
    expect("@timestamp FIXTURE_SETUP 0")
    expect("===RTC===> St-Entry Obj=dutSensor,State=Sensor::off")
    expect("@timestamp Init===> Obj=dutSensor,State=Sensor::off")
    expect_run()
