# csc512

This is my repo for CSC 512: Compiler Construction.

## Getting Started
Environment should already be setup. To run on NCSU EOS machine:

```sh
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