# qre - local and remote runtime for QASM2 programs.

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.2612792.svg)](https://doi.org/10.5281/zenodo.2612792)


## Overview:

This project seeks to produce a program capable of requesting the remote
execution of QASM2 programs produced by means of any programming language
on QASM - compatible quantum systems and simulators, such as IBM QX
computers. You can also run those programs on a local simulator.


Currently you cou can create the required QASM2 files in various ways:

* Create a compiler that produces QASM2 code.

* Use an existing compiler that produces QASM2 code like g2q (see below).

* Write on your own QASM2 source code.

For qre any one of those options is fine as long as you feed it standard
QASM2 files.

The program is still unfinished but at this point it can successfully log
into the QX machine. I am still working on the execution request uri.

The program has a local simulator based on the Qlib library. It is now almost
completely functional (gates u1, u2 and u3 still need work), so if you use
a compiler such as g2q to produce QASM2 code together with qre you can
have a full-stack quantum computer simulator that is independent from the API
of any provider.

Bear in mind that the local simulator is an ideal one at this point. It does
not incorporate simulated noise or other factors.


## Dependencies:

* Required:

  - g2q - v1.1.2 or later ( https://github.com/PESchoenberg/g2q )

  - curl - v7.47.0 or later.

  - g++ - v4:5.3.1 or later.

  - Qlib - v1.0.1 or later ( https://github.com/qkmaxware/Qlib ).

* Optional:

  - You will need an account and an api token from the IBM Quantum Experience
  (see below) if you want to access IBM QX computers and simulators.

  - QX Simulator - ( http://quantum-studio.net/ ).

  - Sqlite3 - ( https://www.sqlite.org/index.html ).

  - sqlp - v1.0.0 or later ( https://github.com/PESchoenberg/g2q ).

## Installation:

* Copy the contents of the compressed file in any folder you like and compile
it (see usage section).

* Once you have installed qre, you will need from the Qlib project its
core package. So you should:
- cd to the /Qlib-master/src folder.
- Copy the /core folder to your clipboard.
- Now cd to the folder containing qre.
- Create /lib/qlib if you don’t have it already within qre’s main folder.
- Paste the clipboard contents into the /lib/qlib folder of your qre folder.

* If you want to store the results of queries made by means of qre set the
following values on your qre.cfg file:
- base-results-storage=json to save them only to json files.
- base-results-storage=sqlite3 to save them to ~/qre/data/sqlite3/qre.db

* Bear in mind that:
- Unless you save your results only to json files, you will need to instal
sqlp (see dependencies above).
- Saving your results to Sqlite3 will also produce an intermediate json file.

* Lastly, you will need to make some updates:
- Overwrite:
  file /lib/qlib/core/quantum/gates/phase.hpp

  with

  file /lib/proviso/phase.hpp
- Overwrite:
  file /lib/qlib/core/quantum/gates/reset.hpp

  with

  file /lib/proviso/reset.hpp
- Overwrite:
  file /lib/qlib/core/quantum/standard_gates.hpp

  with

  file /ib/proviso/standard_gates.hpp

  This is required since the original file does not have gate S specifically
  defined, while QASM2 does.


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

  ./qre examples/example1.qasm post y qlib_simulator 1 example1_1 <ENT> to run it
  on a local qlib-based simulator.

  or

  ./qre examples/example1.qasm post y qx_simulator 1 example1_1 <ENT> to run it
  on a locally installed qx simulator.

  or

  ./qre examples/example1.qasm post y ibmqx_simulator 1 example1_1 <ENT> to run it
  on a remote IBM QX simulator.

  or

  ./qre examples/example1.qasm post y [q processor name] [max credits]
  example_1_1 <ENT> to run it on a real IBM QX quantum processor.

- You will see the results of each simulation on your console. The will also be
stored in a file. Check your /data folder.

- The example files provided were precompiled using g2q. If you want to test run
your own programs, you will have to get a copy of g2q and write your programs
in Scheme (Lisp) or write your own QASM2 source code, or get or develop your
own compatible compiler.


## Credits and Sources:

* IBM Q Experience. (2019). IBM Q Experience. [online] Available at:
https://quantumexperience.ng.bluemix.net/qx/experience [Accessed 2 Sep. 2019].

* Edronkin, P. (2019). g2q - Guile to QASM compiler. [online] g2q. Available at:
https://peschoenberg.github.io/g2q/ [Accessed 26 Aug. 2019].

* Delft University of Technology and Netherlands Organisation for Applied
Scientific Research (2019). QuTech - Research and development in quantum
technology. [online] QuTech. Available at: http://www.qutech.nl/
[Accessed 2 Sep. 2019].

* URL of this project - https://github.com/PESchoenberg/qre .

Please let me know if I forgot to add any credits or sources.


## License:

* GPL-3.0-or-later.


