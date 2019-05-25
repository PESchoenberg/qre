# qx_temp.qc
qubits 3
h q0
x q1
y q2
z q0
s q1
t q2
tdag q1
prepz q0
cx q1, q2
cx q2, q1
cx q1, q2
swap q0, q1
cx q2, q0
cx q0, q1
s q1
h q1
cx q0, q1
h q1
cz q2, q0
h q1
cx q0, q1
h q1
t q1
cx q0, q1
t q1
h q1
s q1
x q1
s q0
cx q2, q0
cx q2, q0
cx q2, q2
cx q2, q2
h q2
cx q1, q2
tdag q2
cx q0, q2
t q2
cx q1, q2
t q2
cx q0, q2
t q1
t q2
h q2
cx q0, q1
t q0
tdag q1
cx q0, q1
toffoli q0, q1, q2
rx q1, 1.570795
ry q2, 1.570795
rz q0, 1.570795
cx q1, q2
cx q1, q2
measure q0
measure q1
measure q2
error_model depolarizing_channel,0.001
display
