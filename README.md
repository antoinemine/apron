# The Apron Numerical Abstract Domain Library

## Introduction

Apron is a library to represent properties of numeric variables, such as variable bounds or linear relations between variables, and to manipulate these properties through semantic operations, such as variable assignments, tests, conjunctions, entailment.

Apron is intended to be used in static program analyzers, in order to infer invariants of numeric variables, i.e., properties that hold for all executions of a program. It is based on the theory of Abstract Interpretation.


## Overview

The Apron library includes several numeric abstract domains, corresponding to different classes of numeric properties with their own internal representation and algorithms, achieving various trade-offs between precision, expressiveness, and efficiency.

Apron includes the following numeric domains:
* intervals (boxes)
* polyhedra (newpolka)
* octagons
* zonotopes (taylor1plus)

Additional domains are made available through the optional PPL third-party library:
* alternate polyhedra implementation
* grids
* reduced products of polyhedra and grids

The domains are made available under a common interface, so that changing the abstract domain of interpretation in a static analysis should only take a one-line change.

The core API is in C, but optional API wrappers for additional languages are provided:
* OCaml
* Java
* C++


## Dependencies

### Base

Compiling the built-in domains with the C interface requires:
* a C compiler (gcc or clang)
* [https://gmplib.org/](GMP) version 5 or later, with development files
* [https://www.mpfr.org/](MPFR) version 3 or later, with development files
* perl
* sed


### Additional domains

Compiling the PPL-based domains requires the [Parma Polyhedra Library](http://bugseng.com/products/ppl) (tested with version 1.2) and gcc (no clang).


### Additional language support

Additional language wrappers require additional components:
* a C++ compiler for the C++ API
* a JDK >= 1.6 for the Java API
* for the OCaml API:
  * a recent OCaml compiler (tested with 4.07, but earlier 4.x should also work)
  * ocamlfind
  * camlidl
  * mlgmpidl


### Documentation

**Help needed to explain how to generate Apron's API documentation.**


## Installation instructions


### Installation on Linux

On deb-based Linux distributions (Debian, Ubuntu) a `sudo apt-get install libgmp-dev libmpfr-dev` should suffice to get the dependencies for the basic C library.

On Opam-based OCaml distributions, a `opam install ocamlfind camlidl mlgmpidl` should suffice to get the dependencies for the OCaml API.

Compilation from source could be as simple as:
* `./configure`
* `make`
* `sudo make install`

`./configure` automatically generates a `Makefile.config` file. It is also possible to write a `Makefile.config` by hand by taking some inspiration from `Makefile.config.model`.

In case some components fail to compile, it is possible to disable them through `./configure` options:
* `-no-cxx` to disable the C++  API
* `-no-java` to disable the Java API (there are known problems where the compilation fails to find `jni.h` )
* `-no-ocaml` to disable the OCaml API
* `-no-ppl` to disable the PPL domains

See `./configure -help` for more options.



### Installation on MacOS X

**Help needed for this section.**

The README.mac file is not up to date.



### Installation on Windows

See the [Windows README](README.windows.md).

