# CSC 512

This is my repo for CSC 512: Compiler Construction.

## Getting Started
Environment should already be setup. To run on NCSU EOS machine:

```sh
git clone --recurse git@github.com:briancpark/csc512.git
# OR
git clone git@github.com:briancpark/csc512.git
git submodule update --init --recursive

cd DrCCTProf
./build.sh
```

You also need to add environment variables. Need to put this in `.bashrc` to instantiate everytime you open a shell:

```sh
source /afs/unity.ncsu.edu/users/b/bcpark/drcctprof-eos-env/setup-env.sh
export drrun=/afs/unity.ncsu.edu/users/b/bcpark/csc512/DrCCTProf/build/bin64/drrun
alias drrun="/afs/unity.ncsu.edu/users/b/bcpark/csc512/DrCCTProf/build/bin64/drrun"
```

## Docker Setup
On M1 Mac, the architecture isn't suppported. So we have to run on Docker ARM container.

To build:

```sh
docker-compose build
```

To run:

```sh
docker-compose run csc512arm
```

## Project 0: DrCCTProf and Instruction Analysis
Used DrCCTProf to analyze instructions in a program.
## Project 1: Gee Parser
[Project Spec](https://xl10.github.io/CSC412-512-project1-parser/)

Made a recursive descent parser for Gee language. The parser is written in Python.
## Project 2: Gee Semantics and Types
[Project Spec (Semantics)](https://xl10.github.io/CSC412-512-project2-semantics/) [Project Spec (Types)](https://xl10.github.io/CSC412-512-project2-types/)

Used the parser from Project 1 to analyze the semantics and types of Gee programs.

## Project 3: Detecting Integer Overflow
[Project Spec](https://xl10.github.io/CSC412-512-project3-Integer-overflow/)

Used DrCCTProf to detect integer overflow in a program.