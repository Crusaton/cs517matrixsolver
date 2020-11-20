# CS517-Matrix-Solver

# Requirements

    * g++ v5.4 or newer
    * Make

# Compilation

The code can be compiled with the provided makefile using the standard 'make' command.

# Sample Execution & Output

If run without command line arguments using

```
./parseTemps
```

the following usage message will be displayed.

```
Usage: ./parseTemps input_file_name
```

If run using

```
./parseTemps sensors-2018.12.26.txt
```

output *similar* to

```
 xk <= x <   xk+1; y_i      =        c0      +        c1x; type
    0 <= x <     30; y_0      =        61.0000 +    0.6333x; interpolation
   30 <= x <     60; y_1      =        98.0000 +   -0.6000x; interpolation
   60 <= x <     90; y_2      =        20.0000 +    0.7000x; interpolation
   90 <= x <    120; y_3      =       128.0000 +   -0.5000x; interpolation
  120 <= x <    150; y_4      =        12.0000 +    0.4667x; interpolation
  150 <= x <    180; y_5      =       112.0000 +   -0.2000x; interpolation
``` 

will be printed to text files. The number of text files will vary based off the amount of core data provided.
The program will determine the number of cores based off columns present in input file. Output files names will take the form of
{basename}-core-{core number}.{txt}.

Note that precision of calculations may vary based upon your architecture/system.