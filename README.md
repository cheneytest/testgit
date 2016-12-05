# Clean Code Contest 2016
---
## Rules
## Subject
### Terms
Term|Explanation
----|-----------
**curly braces**|'{', '}'
**codes**| texts except comments in C/C++ source/header code files, which end with ".c", ".h", ".cpp", ".hpp", ".cc".
**exact duplicate**|totally same, in consecutive five lines of code by default
**a line of code**|one code line except blank lines and comments
**blank lines**|lines which only contains blankwhite-spaces, Tab, LF, or CR
**comments**|one line comment '//', and multi-lines comments '/* comments */'
**N consecutive lines**|N lines of C/C++ codes excluding blank lines and comments. *Exception: A curly brace should be regarded as a separate line.* This is used in comparison.
**amount of duplicate**|sum of original lines of duplicate codes in source files. This is used in result output.

### Background 
Duplicate code is a computer programming term for a sequence of source code that occurs more than once, either within a program or across different programs owned or maintained by the same entity. They are a common phenomenon in an application that has been under development for some time. Clones make it hard to change your application because you have to find and update more than one fragment. Code clones often result from developers copying code and then adapting it to its new location. It is therefore easiest to consider the degree of change that can be made before the analyzer fails to find the clone.In each case, there is a tolerance of a specific number of such modifications:
* Adding or removing blankwhite-spaces or blank lines
* Change the location of curly braces
* Rename identifiers.
* Rearrange statements.

### Content
Please make a command line tool in linux to detect duplicate C/C++ codes described as following requriment:

* As a programmer, during refactoring codes, I want to find all exact duplicate codes, so that I can eliminate them according the report.

You can find user stories in [AT/README.md](./AT/README.md) in your code space.

### Code space
 
#### Makefile
Top makefile in project.

You can change it for your project. But you have to **read its comments** carefully in advance.

#### src/
Where you can put your source files.

You may have to change Makefile according to the structure of your source files.
#### AT/
Where AT cases are put. Cases are writen with Python2.7. You can execute them on Windows or Linux.

*NOTICE*:

you can read all in this folder, but you are not permitted to change any of them.

Name|Description
 --------|-----------
 testN | A folder which represents Story N
 testN/src | A folder where target source files are put, they are input of test cases
 testN/test.py | Python scripts to run test with 'clone_detect'
 testN/expect.txt(/json) | Output expected of 'clone_detect' in Story N. txt for stdout, json for json format.
 testN/details.json | In some stories, only stdout output is examined; And it includes details of detect result. Useful in debug

#### ut/
UT cases folder.

You have to put all of your UT cases files here, and makefile in ut/build, cases source files in ut/cases. You are freely to create other folders if you need.

usage:
make all:   make application
make at:  run all auto test
make ut:  make ut application (use gtest frame work)
make ut_run: run all ut_test 

