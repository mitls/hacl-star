#!/usr/bin/env bash

set -e
set -o pipefail

if [[ `uname` == "FreeBSD" ]]
then
  MAKE=$(which gmake)
else
  MAKE=$(which make)
fi

if [[ $OS == "Windows_NT" ]]; then
  # The usual issue of return codes not being forwarded.
  .ci/script.bat 2>&1 | tee log
  if grep "SUCCESS" log; then
    exit 0
  else
    exit 1
  fi
fi

# ARM cross builds are handled a little different
if [[ $ARM_CROSS_CI == "aarch64-none-linux-gnu" ]]; then
  pushd dist/gcc-compatible
  export TOOLCHAIN=$PWD/../../gcc-arm-9.2-2019.12-x86_64-aarch64-none-linux-gnu
  ./configure -target aarch64-none-linux-gnu
  $MAKE -j
  popd
  exit 0
fi

# Cross compile for other targets. Build and static library only.
if [ ! -z "$CROSS_CI" ]; then
  pushd dist/gcc-compatible
  rm -rf *.o *.d libevercrypt.a
  ./configure -target $CROSS_CI --disable-ocaml
  $MAKE -j libevercrypt.a
  popd
  exit 0
fi

if [[ $TARGET == "IA32" ]]; then
  # Test 32-bit build; Tests don't work here yet.
  pushd dist/gcc-compatible
  ./configure -target ia32
  $MAKE -j
  popd
  exit 0
fi

# For OSX... seems like the most reliable way to figure out which OpenSSL is
# installed? We have both 1.1.1d and 1.1.1f and neither can be installed on the
# other configuration.
for p in /usr/local/Cellar/openssl@1.1/*; do export CFLAGS="-I$p/include/"; export LDFLAGS="-L$p/lib"; done

# Most likely running without OCaml -- need to configure
pushd dist/gcc-compatible
./configure
$MAKE -j
popd

$MAKE -C tests -j test

# Extracted C tests -- need full kremlib, don't work on ARM because of
# intrinsics for x86 in cpu cycle count routines in testlib.c
pushd dist/test/c/
git clone https://github.com/fstarlang/kremlin --depth 10
export KREMLIN_HOME=$(pwd)/kremlin
$MAKE -C kremlin/kremlib/dist/generic -f Makefile.basic -j
$MAKE -j -k
popd

if which opam; then
  $MAKE -C dist/gcc-compatible install-hacl-star-raw
  pushd bindings/ocaml
  dune build
  dune runtest
  dune install
  popd
fi
