# Shift-Reduce-parser
## Grammar
- E -> E + T | E - T | T
- T -> T * F | T / F | F
- F -> (E) | id | num


Expression grammar belongs to the class of LR grammars, which are suitable for bottom-up parsing. It cannot be used for top-down parsing due to its left-recursiveness.

## Shift/Reduce syntactic analysis 

## LR(0) Item DFA 

## Example

## Dependencies
1. g++
2. CMake 3.10 version (or higher)
3. GTest (for testing)
4. Flex

## Compiling 

### Parser

To get the parser library and test executables

``` cmd
$ mkdir build
$ cmake -B build -DCMAKE_BUILD_TYPE=Release -DPRINT_TABLE=ON -DDUMP_TREE=ON 
$ cmake --build build/
```

```-DPRINT_TABLE``` option is needed to get a log with a table. ```-DDUMP_TREE``` option is needed to get a dot file. 

## Run the program:

### Parser

To run the parser, you need to pass the input file and dot file if you need tree visualization.

``` cmd
$ ./build/Release/bin/parser <input_file> <dot_file> 

```

If you need to visualize the tree, use 

``` cmd
dot -Tpng <dot_file> -o file.png
```

### Test:

to run table tests

``` cmd
$ ./build/tests/UnitTestTable
```

to run tree tests

``` cmd
$ ./build/tests/UnitTestTree
```
