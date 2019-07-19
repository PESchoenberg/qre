# qx_temp.qc
qubits 5
h q0
h q1
h q2
h q3
h q4
swap q0, q1
swap q1, q2
swap q2, q3
swap q3, q4
swap q3, q4
swap q2, q3
swap q1, q2
swap q0, q1
cx q0, q1
cx q1, q0
cx q0, q1
cx q1, q2
cx q2, q1
cx q1, q2
cx q2, q3
cx q3, q2
cx q2, q3
cx q3, q4
cx q4, q3
cx q3, q4
cx q3, q4
cx q4, q3
cx q3, q4
cx q2, q3
cx q3, q2
cx q2, q3
cx q1, q2
cx q2, q1
cx q1, q2
cx q0, q1
cx q1, q0
cx q0, q1
measure q0
measure q1
measure q2
measure q3
measure q4
error_model depolarizing_channel,0.001
display
