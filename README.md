# PSE
Pitch Spelling Algorithms
based on optimization of Engraving-based info.

to compile:
```shell
mkdir build
cd build
cmake ..
make
```
This leaves a library `pse.X.so` in `build` where `X` depends on your platform.

With this library in your path, 
you can do `import pse` in a Python3 interpreter.

The file `scripts/PSeval.py` contains some functions for using the module `pse`.

