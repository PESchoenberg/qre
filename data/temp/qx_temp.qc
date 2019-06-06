# qx_temp.qc
qubits 5
h q0
h q3
cx q0, q2
cx q3, q2
h q0
s q0
h q3
s q3
h q1
h q2
cx q1, q2
h q1
h q2
h q4
cx q4, q2
h q3
h q4
cx q3, q2
h q3
measure q0
measure q1
measure q2
measure q3
measure q4
error_model depolarizing_channel,0.001
display
