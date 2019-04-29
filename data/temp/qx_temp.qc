# qx_temp.qc
qubits 4
h q0
h q1
h q2
h q3
cx q0, q1
h q2
h q0
cx q3, q2
h q3
measure q0
measure q1
display
# error_model depolarizing_channel,0.001
