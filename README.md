# qre - QASM2 remote execution.




## Overview:

This project seeks to produce a program capable of requesting the execution
of QASM2 programs produced by means of any programming language on QASM -
compatible quantum systems and simulators, such as IBM QX computers.

You can create the required QASM2 files in various ways:

* Create a compiler.

* Use an existing compiler that produces QASM2 code like g2q (see below).

* Write on your own your QASM2 source code.

For qre any option is fine as long as you feed it standard QASM2 files.

The program is still unfinished but at this point it can successfully log
into the QX machine. I am still working on the execution request uri.


## Dependencies:

* g2q v 1.1.2 or later ( https://github.com/PESchoenberg/g2q )

* libcurl.

* A compiler capable of handling C++ 2017.

* You will need an account and an api token from the IBM Quantum Experience
(see below) if you want to access IBM QX computers and simulators.


## Installation:

* Just copy the contents of the compressed file in any folder you like and
compile it (see usage section).


## Uninstall:

* Just delete the folder where you placed qre and its contents.


## Usage:

* Compilation (using the gcc compiler family) on Linux:

- g++ -std=c++17 -Wall -O3 -c qre0.cpp qre1.cpp qre2.cpp
- g++ -std=c++17 -Wall -O3 qre0.o qre1.o qre2.o -o qre -lcurl

* Testing:
- You should update the api token on the provided qre.cfg file and use your
own:
  - Open qre.cfg
  - Replace the string "your-token-goes-here" wherever it appears on the file
  with your new token.
  - Save the file as it is, without changin anything else.
  - See the included README.md file for further instructions on how
  to get a token on the IBM QX website.
- On your console, type:

  ./qre examples/example1.qasm test y simulator 1 example1_1 <ENT> to perform
  a simple test and make sure about the contents of qre.cfg

  or

  ./qre examples/example1.qasm lpost y simulator 1 example1_1 <ENT> to run it
  on a local qre simulator.

  or

  ./qre examples/example1.qasm post y simulator 1 example1_1 <ENT> to run it
  on a remote QX simulator.

  or

  ./qre examples/example1.qasm post y [q processor name] [max credits]
  example_1_1 <ENT> to run it on a QX real quantum processor.
  
- The example file provided is precompiled using g2q. If you want to test run
your own programs, you will have to get a copy of g2q and write your programs
in Scheme (Lisp) or write your own QASM2 source code, or get or develop your
own compatible compiler.


## Credits and Sources:

* IBM Quantum Experience - ( https://quantumexperience.ng.bluemix.net/qx/experience )

* g2q Scheme to QASM2 compiler - - https://github.com/PESchoenberg/g2q

* URL of this project - https://github.com/PESchoenberg/qre

## License:

* GPL-3.0-or-later.


