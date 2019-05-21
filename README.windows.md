This document explain how to compile Apron under Windows.
For the moment, only compilation using the OCaml Mingw port is supported.

## Prerequisites

Install an OCaml Mingw 64 bits environment by following the instructions at
https://fdopen.github.io/opam-repository-mingw, and selecting
a `mingw64` version of the OCaml compiler.

Under Cygwin, install the following Mingw libraries:
* mingw64-x86_64-gmp
* mingw64-x86_64-mpfr

## Automatic installation

Use Opam to install Apron: `opam install apron`.
Everything should work out-of-the-box.


## Manual installation

Use Opam to install Apron's dependencies:
* `opam install ocamlfind camlidl mlgmpidl conf-gmp conf-mpfr`.

Then launch Apron's `configure` script with the following options:
* `--ext-dll dll` to specify that dynamic libraries have
  a `.dll` extension.
* `--no-ppl` to deactivate support for the PPL, which gets automatically
detected (even when non-present)
* `--no-cxx` to deactivate support for the C++ API, which compilation fails
  under Cygwin
* `--prefix /path/to/installation/`

Next, make sure `perl` is in your PATH, and edit the resulting
  `Makefile.config`. `PERL` must be set to `perl` instead of
  `/usr/bin/perl`, as CamlIDL expects a Windows path instead of a Cygwin one.

Finally, compile and install as usual: `make`, `make install`.
