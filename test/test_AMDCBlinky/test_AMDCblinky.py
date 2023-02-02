# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

def on_reset():
    expect_pause()
    continue_test()
    expect("@timestamp AMDCRecords::INFO Hello World")
    expect("@timestamp AMDCRecords::WARNING Device ID:   01 02 03 04 05")
    expect("@timestamp AMDCRecords::TESTING QUTEST checking ...")
    expect("@timestamp AMDCRecords::ERROR ERROR checking ...")
    expect("@timestamp TE0-Arm  Obj=AMDCBlinky::inst.m_timeEvt,AO=AMDCBlinky::inst,Tim=5000,Int=5000") 
    expect("@timestamp AO-Subsc Obj=AMDCBlinky::inst,Sig=ADJUST_FREQ_SIG")
    expect("===RTC===> St-Init  Obj=AMDCBlinky::inst,State=QHsm::top->AMDCBlinky::off")
    expect("@timestamp AMDCRecords::INFO LED OFF")
    expect("===RTC===> St-Entry Obj=AMDCBlinky::inst,State=AMDCBlinky::off")
    expect("@timestamp Init===> Obj=AMDCBlinky::inst,State=AMDCBlinky::off")
    expect_pause()
    continue_test()
    expect_run()

# Check normal startup ...
test("Startup")

# Check some time event
test("Timer is initially at rate 5000 (2 Hz)?")
# Set the object to be anaylzed is AMDCBlinky
current_obj(OBJ_SM_AO,"AMDCBlinky::inst")               
current_obj(OBJ_TE,"AMDCBlinky::inst.m_timeEvt")
query_curr(OBJ_SM)      # Query the state machine
expect("@timestamp Query-SM Obj=AMDCBlinky::inst,State=AMDCBlinky::off")
query_curr(OBJ_AO)      # Query active object info
expect("@timestamp Query-AO Obj=AMDCBlinky::inst,State=AMDCBlinky::off")
query_curr(OBJ_TE)      # What is the current timer?
expect("@timestamp Query-TE Obj=AMDCBlinky::inst.m_timeEvt,Rate=0,Sig=TIMEOUT_SIG,Tim=5000,Int=5000,Flags=0x80")
current_obj(OBJ_MP,"EvtPool1")
query_curr(OBJ_MP)
expect("@timestamp Query-MP Obj=EvtPool1,*")

# Check if the led frequency was changed?
test("Publish the frequency 5 Hz. Is the LED updated?")
# Set the object to be anaylzed is AMDCBlinky
current_obj(OBJ_SM_AO,"AMDCBlinky::inst")               
current_obj(OBJ_TE,"AMDCBlinky::inst.m_timeEvt")
query_curr(OBJ_SM)      # Query the state machine
expect("@timestamp Query-SM Obj=AMDCBlinky::inst,State=AMDCBlinky::off")
query_curr(OBJ_AO)      # Query active object info
expect("@timestamp Query-AO Obj=AMDCBlinky::inst,State=AMDCBlinky::off")
query_curr(OBJ_TE)      # What is the current timer?
expect("@timestamp Query-TE Obj=AMDCBlinky::inst.m_timeEvt,Rate=0,Sig=TIMEOUT_SIG,Tim=5000,Int=5000,Flags=0x80")
current_obj(OBJ_MP,"EvtPool1")
query_curr(OBJ_MP)
expect("@timestamp Query-MP Obj=EvtPool1,*")
publish("ADJUST_FREQ_SIG", pack("<I", 5))
expect("@timestamp MP-Get   Obj=EvtPool1,Free=99,Min=99")
expect("@timestamp QF-New   Sig=ADJUST_FREQ_SIG,Size=8")
expect("@timestamp QF-Pub   Sdr=QS_RX,Evt<Sig=ADJUST_FREQ_SIG,Pool=1,Ref=0>")
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AMDCBlinky::inst,Evt<Sig=ADJUST_FREQ_SIG,Pool=1,Ref=2>,Que<Free=100,Min=100>")
expect("@timestamp QF-gcA   Evt<Sig=ADJUST_FREQ_SIG,Pool=1,Ref=2>")
expect("@timestamp AO-GetL  Obj=AMDCBlinky::inst,Evt<Sig=ADJUST_FREQ_SIG,Pool=1,Ref=1>")
expect("@timestamp Disp===> Obj=AMDCBlinky::inst,Sig=ADJUST_FREQ_SIG,State=AMDCBlinky::off")
expect("@timestamp TE0-Dis  Obj=AMDCBlinky::inst.m_timeEvt,AO=AMDCBlinky::inst,Tim=5000,Int=5000")
expect("@timestamp TE0-Arm  Obj=AMDCBlinky::inst.m_timeEvt,AO=AMDCBlinky::inst,Tim=2000,Int=2000")
expect("@timestamp AMDCRecords::TESTING LED f_new =  5")
expect("@timestamp =>Intern Obj=AMDCBlinky::inst,Sig=ADJUST_FREQ_SIG,State=AMDCBlinky::off")
expect("@timestamp QF-gc    Evt<Sig=ADJUST_FREQ_SIG,Pool=1,Ref=1>")
expect("@timestamp MP-Put   Obj=EvtPool1,Free=100")
expect("@timestamp Trg-Done QS_RX_EVENT")

# Check if the led frequency was changed?
test("Publish the frequency 10 Hz. Is the LED updated?")
# Set the object to be anaylzed is AMDCBlinky
glb_filter(GRP_AO, 100, 101, 102, 103)  
current_obj(OBJ_SM_AO,"AMDCBlinky::inst")               
current_obj(OBJ_TE,"AMDCBlinky::inst.m_timeEvt")
query_curr(OBJ_SM)      # Query the state machine
expect("@timestamp Query-SM Obj=AMDCBlinky::inst,State=AMDCBlinky::off")
query_curr(OBJ_AO)      # Query active object info
expect("@timestamp Query-AO Obj=AMDCBlinky::inst,State=AMDCBlinky::off")
query_curr(OBJ_TE)      # What is the current timer?
expect("@timestamp Query-TE Obj=AMDCBlinky::inst.m_timeEvt,Rate=0,Sig=TIMEOUT_SIG,Tim=5000,Int=5000,Flags=0x80")
current_obj(OBJ_MP,"EvtPool1")
query_curr(OBJ_MP)
expect("@timestamp Query-MP Obj=EvtPool1,*")
publish("ADJUST_FREQ_SIG", pack("<I", 10))
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AMDCBlinky::inst,Evt<Sig=ADJUST_FREQ_SIG,Pool=1,Ref=2>,Que<Free=100,Min=100>")
expect("@timestamp AO-GetL  Obj=AMDCBlinky::inst,Evt<Sig=ADJUST_FREQ_SIG,Pool=1,Ref=1>")
expect("@timestamp AMDCRecords::TESTING LED f_new =  10")
expect("@timestamp Trg-Done QS_RX_EVENT")


test("Publish the frequency 25 Hz. Is an assertion raised?")
glb_filter(GRP_AO, 100, 101, 102, 103)  
current_obj(OBJ_SM_AO,"AMDCBlinky::inst")               
current_obj(OBJ_TE,"AMDCBlinky::inst.m_timeEvt")
query_curr(OBJ_SM)      # Query the state machine
expect("@timestamp Query-SM Obj=AMDCBlinky::inst,State=AMDCBlinky::off")
query_curr(OBJ_AO)      # Query active object info
expect("@timestamp Query-AO Obj=AMDCBlinky::inst,State=AMDCBlinky::off")
query_curr(OBJ_TE)      # What is the current timer?
expect("@timestamp Query-TE Obj=AMDCBlinky::inst.m_timeEvt,Rate=0,Sig=TIMEOUT_SIG,Tim=5000,Int=5000,Flags=0x80")
publish("ADJUST_FREQ_SIG", pack("<I", 25))
expect("@timestamp AO-Post  Sdr=QS_RX,Obj=AMDCBlinky::inst,Evt<Sig=ADJUST_FREQ_SIG,Pool=1,Ref=2>,Que<Free=100,Min=100>")
expect("@timestamp AO-GetL  Obj=AMDCBlinky::inst,Evt<Sig=ADJUST_FREQ_SIG,Pool=1,Ref=1>")
expect("@timestamp =ASSERT= M*")




