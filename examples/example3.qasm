// example3.qasm
//
OPENQASM 2.0;
include "qelib1.inc";
//
qreg q[5];
creg c[5];
h q[0];
h q[1];
s q[0];
s q[1];
h q[0];
cx q[1],q[0];
h q[0];
s q[0];
s q[1];
h q[0];
h q[1];
x q[0];
x q[1];
h q[0];
cx q[1],q[0];
h q[0];
x q[0];
x q[1];
h q[0];
h q[1];
// 
// Measuring
measure q[0] -> c[0];
measure q[1] -> c[1];
