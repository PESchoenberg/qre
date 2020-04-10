# qx_temp.qc
qubits 5
h q0
h q1
h q2
h q3
h q4
measure q0
measure q1
measure q2
measure q3
measure q4
error_model depolarizing_channel,0.001
display
