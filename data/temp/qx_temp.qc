# qx_temp.qc
qubits 3
x q0
h q1
h q2
h q0
h q0
cx q1, q0
tdag q0
cx q2, q0
t q0
cx q1, q0
tdag q0
cx q2, q0
t q1
t q0
h q0
cx q2, q1
t q2
tdag q1
cx q2, q1
h q1
h q2
x q1
x q2
h q1
cx q2, q1
h q1
x q1
x q2
h q1
h q2
measure q1
measure q2
error_model depolarizing_channel,0.001
display
