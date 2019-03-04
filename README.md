# gre - QASM2 execution requester.




## Overview:

This project seeks to produce a program capable of requesting the execution
of QASM2 programs produced by means of any programming language on a IBM QX
quantum computer as long as the program in question is compiled following
the parameters set on project g2q, which is a Scheme to QASM compiler that
produces a .qasm file containing the QASM2 code and a .qreg file that
provides instructions to build the requests to the QX server.

You can create your own compiler in order to produce the qasm and qreg
files, or you can try g2q, which already does that; qre is language-independent.

The program is still unfinished but at this point it can successfully log
into the QX machine. I am still working on the execution request uri.


## Dependencies:

* g2q v 1.1.2 or later ( https://github.com/PESchoenberg/g2q )

* libcurl.

* A compiler capable of handling C++ 2017.

* You will need an account and an api token from the IBM Quantum Experience
(see below).


## Installation:

* Just copy the contents of the compressed file in any folder you like.


## Uninstall:

* Just delete the folder where you placed qre.


## Usage:

* Compilation (using the gcc compiler family) on Linux:

- g++ -std=c++17 -Wall -O3 -c qre0.cpp qre1.cpp 
- g++ -std=c++17 -Wall -O3 qre0.o qre1.o -o qre -lcurl

* Testing:
- You should update the api token on the provided example1.qreg file and
use your own:
  - Open examples/example1.qreg
  - Replace the string "your-token-goes-here" wherever it appears on the file
  with your new token.
  - Save the file as it is, without changin anything else.
  - See the included README.md file for further instructions on how
  to get a token on the IBM QX website.
- On your console, type 
  ./qre examples/example1.qreg test y simulator 1 example1.1 <ENT>
- The example file provided is precompiled using g2q. If you want to test run
your own programs, you will have to get a copy of g2q and write your programs
in Scheme (Lisp) or get  or develop your own compatible compiler.


## Credits and Sources:

* IBM Quantum Experience - ( https://quantumexperience.ng.bluemix.net/qx/experience )


## License:

* GPL-3.0-or-later.


