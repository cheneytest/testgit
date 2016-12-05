# README
You are **not permitted** to change any files in this directory.
You can execute python scripts 'runtest.py' or 'cleanup.py' to run tests or clear temp files.

**Recommend** to implement stories from 1 to 6.

## How to Test
1. Put your binary executable image 'clone_detect' in the same directory of 'README.md'
1. Run all test cases with `python runtest.py`
    * 'PASS' test successful in specific test case
    * 'FAILED' test failed in specific test case
1. Run the specific test case with `python runtest.py <test number>`
    e.g. `python runtest.py 1` executing all cases in test 1

## Requirements:

`As a programmer, during refactoring codes, I want to find all exact duplicate codes, so that I can eliminate them according the report.`

#### Story 1
Caculate amount of exact duplicate codes.

* **Given** two C/C++ source files under folder 'src',
* **When** execute `clone_detect src/`,
* **Then** I get amount of duplicate codes in stdout, see in `test1/expect.txt`.

#### Story 2
Multiple consecutive white-spaces and tabs should be regarded as one white-space.

* **Given** two C/C++ source files under folder ‘src’, and their differences are white-spaces and blank lines.
* **When** execute `clone_detect src/`,
* **Then** I get amount of lines of two files as total duplication, see in `test2/expect.txt`.

#### Story 3
Comments should be ignored

* **Given** two C/C++ source files under folder 'src', and all their differences are comments,
* **When** execute `clone_detect src/ -c`,
* **Then** I get amount of lines of two files as total duplicate in stdout, see in `test3/expect.txt`.

#### Story 4
Different styles of curly braces should be regarded as the same

* **Given** two C/C++ source files under folder 'src', and all their differences are locations of braces,
* **When** execute `clone_detect src/ -b`,
* **Then** I get amount of lines of two files as total duplicate in stdout, see in `test4/expect.txt`.

#### Story 5
Print details of duplicate codes in json format.

* **Given** two C/C++ source files under folder 'src',
* **When** execute `clone_detect src/ -j clone_code.json`,
* **Then** I get details of duplication in file 'clone_code.json' in json format, see in `test5/expect.json`.

#### Story 6
Find duplicate codes of 10+ concesutive lines.

* **Given** two C/C++ source codes files under folder 'src',
* **When** execute `clone_detect src/ -n 10 -j clone_code.json`,
* **Then** I get details of duplicate codes in json format, see in `test6/expect.json`.


