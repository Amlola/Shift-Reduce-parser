# Shift-Reduce-parser
## Grammar
- E -> E + T | E - T | T
- T -> T * F | T / F | F
- F -> (E) | **id** | **num**


## Dependencies
1. g++
2. CMake 3.10 version (or higher)
3. GTest (for testing)
4. Flex

## Compiling 

### Parser

To compile parser:

``` cmd
$ mkdir build
$  cmake -S ./ -B build/Release -DCMAKE_BUILD_TYPE=Release -DPRINT_TABLE=ON
$ cmake --build build/Release
```

### Test

To compile unit tests you need the ```gtest``` library:

``` cmd
$ cd test
$ mkdir build
$ cd build
$ сmake ..
$ make
```

## Run the program:

### Parser

To run the parser, you need to pass the input file. The output table will be presented in the examples folder

``` cmd
$ ./build/Release/parser examples/example1.txt
```

### Test:
``` cmd
$ ./UnitTestTable
```
