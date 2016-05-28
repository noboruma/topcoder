# bin2text
This software aims at providing a solution to the following problem:

## THE GATES FOUNDATION - BINARY TO TEXT SOFTWARE PERFORMANCE CHALLENGE IN C

You can read the full challenge overview [here](https://www.topcoder.com/challenge-details/30053925/?type=develop) website or as [plain text](CHALLENGE_OVERVIEW.txt).


## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes. See deployment for notes on how to deploy the project on a live system.

### Prerequisities

This project was developed and tested on x86\_64 GNU/Linux (Linux 4.5.0) with the following configuration

```
#!text
gcc-5.3.0 
glibc-2.32
gmake-4.1
```

This project has no dependencies on 3rd party libraries other than those shipped with gcc-5.3.0.


### Installing

Unzip the zip file to your preferred directory and move into the project directory:

```
#!bash
[user@hostname]~$ unzip bin2text.zip
[user@hostname]~$ cd bin2text

```

There are three build target configurations available: all, debug, clean [default: all]. Invoke the build system with gmake:

```
#!bash
[user@hostname]~/bin2text$ make [target]
```

Please think about updating the test symbolic link to your test folder:

```
#!bash
[user@hostname]~/bin2text$ rm test && ln -s ../path/to/test
```


## Usage

The program is intended as a command line application, whose usage is as follows:

```
#!bash
Usage:   run [options] <input1> <input2>
Options:
         -n INT    number of sorted tuples for out1.txt and out2.txt [default: 0]
         -k INT    number of sorted tuples for out3.txt              [default: 0]
         -b FLOAT  bin width for out4.txt                            [default: 0]
         -s FLOAT  number of standard deviations for out5.txt
```

Input:

- 2 binary files, **<input1>** and **<input2>**; format described below
- 1 unsigned int **n** (4 bytes, 0 <= n <= 1,000,000)
- 1 unsigned int **k** (4 bytes, 0 <= k <= 100,000)
- 1 double **b** (8 bytes, 0 <= b <= 1)

Optional:

- 1 double **s** (8 bytes, -100,000 <= s <= 100,000)


Output:

- 4 or 5 ASCII text files in the current directory, depending on which Options are specified.

### Input Detail

**<input1>** in a binary file with the following representation:

```
#!text
type          | bytes       | field     | description
-----------------------------------------------------
signed int    | 4           | 0         | zero
signed int    | 4           | d         | dimension (# ints in each tuple) 
signed int    | 4           | n_vars    | number of variables total
unsigned long | 8           | n_tups    | number of tuples
double        | 8           | avg       | average of all scores
----------------------------------------------------------------
              | 28 bytes total
```

The program asserts that **<input1>** is 28 bytes as one of the safety checks before main execution.

**<input2>** is a binary file containing the data needed to run. The size of the data is determined by **<input1>**.
It has the following representation:

```
#!text
type          | bytes       | field     | description
-----------------------------------------------------
int[]         | d*4         | t1        | tuple 1
double        | 8           | t1_s      | tuple 1 score
int[]         | d*4         | t2        | tuple 2
double        | 8           | t2_s      | tuple 2 score
...            ...            ...        ...
int[]         | d*4         | tlast     | last tuple
double        | 8           | tlast_s   | last tuple score
-----------------------------------------------------------
              | n_tups*((4*d)+8) bytes total

```

### Output Detail

NOTE: IF ANY OF THESE FILES EXIST PRIOR TO EXECUTION, THE PROGRAM WILL PRINT TO STDERR AND RETURN.

**out1.txt** - top tuples sorted:

text file containing **n** tuples with the highest score, sorted by maximum score. If **n** = 0, this file is not touched.

**out2.txt** - bottom tuples sorted:

text file containing **n** tuples with the lowest score, sorted by minimum score. If **n** = 0, this file is not touched.

For out1.txt and out2.txt, tuples are printed 1 per line, values separated by a tab, and the score of the tuple at the end with 10 digits beyond the decimal points. For example, a tuple of dimension 3, with the variables 4 5 and 6 and a score of 0.01 would look like:

```
#!text
4    5    6    0.0100000000
```

**out3.txt**: sorted tuples by variable

This file contains at most (2*k) + 1 lines for each value (variable) that appears in at least 1 tuple. The first line contains the variable number, the average of scores for all tuples that contain the variable, all separated by a tab. The next k lines are the highest scoring tuples that contain the variable , sorted by maximal score. The next k lines are the lowest scoring tuples that contain the variable, sorted by minimal score. If there are fewer than k tuples that contain the value, print all of the tuples that contain the value.

**out4.txt**: histogram

The first line of this file will be the minimum and maximum score in the file, respectively, separated by a tab. If b > 0, there will be floor((max-min)/b)+1 lines following the first line. Each line has a count of how many scores fall in the range determined by the bin size b, starting at the minimum value. So line 2 should have a count of scores in the range [min, b+min), line 3 has count of scores in [min+b, (2*b)+min), etc.

For example, if b=0.01, and the scores in the input file are 0.0100, 0.0120, 0.0299, 0.0300, 0.0310, 0.060, 0.100 then the output file should have 11 lines total and look like.

```
#!text
0.0100000000    0.1000000000
2
1
2
0
0
1
0
0
0
1
```

### Example Usage
```
#!bash
[user@hostname]~/bin2text$ ./run -n 1024 -k 64 input1 input2
```

## Authors

* **Thomas Legris** - [noboruma](https://github.com/noboruma)
* **Edmund Higham** - [ehigham](https://github.com/ehigham)

## License

This project is licensed under the MIT License - see the [LICENCE.md](LICENCE.md) file for details

## Acknowledgments

* Caffeine
* Alcohol
* Nicotine
* Vim

## Known Issues

-

## Ways of Improvement

* Add multi-threading support to sorting algorithm
