# test-script for QUTest unit testing harness
# see https://www.state-machine.com/qtools/qutest.html

include("test_include.pyi")

# Genera Probing
test("UART Connection")
command_x()
expect("@timestamp COMMAND_X 0")
expect("@timestamp Trg-Done QS_RX_COMMAND")
probe("myFun", 1)
command_x()
expect("@timestamp TstProbe Fun=myFun,Data=1")
expect("@timestamp COMMAND_X 1")
expect("@timestamp Trg-Done QS_RX_COMMAND")
command("COMMAND_X")
expect("@timestamp COMMAND_X 0")
expect("@timestamp Trg-Done QS_RX_COMMAND")

