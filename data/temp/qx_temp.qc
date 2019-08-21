# qx_temp.qc
qubits 3
h q0
h q1
h q2
cx q2, q1
cx q0, q1
h q2
t q0
tdag q1
t q2
cx q2, q1
cx q0, q2
t q1
tdag q2
cx q0, q1
tdag q1
cx q0, q2
cx q2, q1
t q1
h q2
cx q2, q1
measure q0
measure q1
measure q2
error_model depolarizing_channel,0.001
display
