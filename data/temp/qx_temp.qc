# qx_temp.qc
qubits 5
prepz q0
prepz q1
prepz q2
prepz q3
prepz q4
cx q0, q1
cx q1, q0
cx q0, q1
swap q0, q1
h q0
h q1
h q2
h q3
h q4
x q1
x q2
y q4
z q0
s q1
t q2
tdag q3
cx q2, q0
cx q3, q4
s q4
h q2
cx q4, q2
h q2
h q0
cx q2, q0
h q0
t q0
cx q2, q0
t q0
h q0
s q0
x q0
s q2
cx q3, q4
cx q3, q4
cx q2, q0
cx q2, q0
cx q2, q0
cx q2, q0
measure q0
measure q1
measure q2
measure q3
measure q4
display
