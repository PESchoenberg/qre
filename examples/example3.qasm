// example3.qasm;
// Compiled with g2q - v1.2.6;
OPENQASM 2.0;
include "qelib1.inc";
qreg q[5];
creg c[5];
// Begin g1y;
h q[0];
h q[1];
// End g1y;
// Begin g1y;
s q[0];
s q[1];
// End g1y;
// Begin g1cxg1;
h q[0];
cx q[1],q[0];
h q[0];
// End g1cxg1;
// Begin g1y;
s q[0];
s q[1];
// End g1y;
// Begin g1y;
h q[0];
h q[1];
// End g1y;
// Begin g1y;
x q[0];
x q[1];
// End g1y;
// Begin g1cxg1;
h q[0];
cx q[1],q[0];
h q[0];
// End g1cxg1;
// Begin g1y;
x q[0];
x q[1];
// End g1y;
// Begin g1y;
h q[0];
h q[1];
// End g1y;
// Measuring;
measure q[0] -> c[0];
measure q[1] -> c[1];
