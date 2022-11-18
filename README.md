# Cling linkage issue
The CMakeLists.txt is copied from from the cling-demo. Note that this issue is
with the LLVM 13 branches. I have tested with Cling 0.8 and the issue did not
exist there.

## Building and running
```bash
$ mkdir build ; cd build
$ cmake .. -Dcling_build_dir=/path/to/cling-build/builddir
$ make
$ ./cling-demo
JIT session error: Symbols not found: [ _ZN14pool_item_baseI3barE13inc_referenceEv, _ZN14pool_item_baseI3barE7releaseEv ]
[runStaticInitializersOnce]: Failed to materialize symbols: { (main, { _ZN21pool_item_common_baseD2Ev, _ZN3barD0Ev, _ZTV3bar, _ZTI3bar, b, _ZTI21pool_item_common_base, _ZN14pool_item_baseI3barED2Ev, _ZN3barD2Ev, _ZTS14pool_item_baseI3barE, _ZTS21pool_item_common_base, _ZTI14pool_item_baseI3barE, $.cling-module-2.__inits.0, __orc_init_func.cling-module-2, _ZN3barD1Ev, _GLOBAL__sub_I_cling_module_2, __cxx_global_var_initcling_module_2_, _ZTS3bar, _Z15__cling_Un1Qu30Pv }) }
IncrementalExecutor::executeFunction: symbol '_ZN14pool_item_baseI3barE7releaseEv' unresolved while linking [cling interface function]!
You are probably missing the definition of pool_item_base<bar>::release()
Maybe you need to load the corresponding shared library?
IncrementalExecutor::executeFunction: symbol '_ZN14pool_item_baseI3barE13inc_referenceEv' unresolved while linking [cling interface function]!
You are probably missing the definition of pool_item_base<bar>::inc_reference()
Maybe you need to load the corresponding shared library?
```

## Environment
I have reproduced this in the following configurations:

* Arch Linux, Clang 13, Cling with LLVM 13, built with CMake

I have not been able to reproduce it on:

* Arch Linux, Clang 13, Cling 0.8, built with cpt

## How I've built Cling
```bash
#!/usr/bin/env bash

set -xeuo pipefail

srcdir="${1:-$PWD}"
llvm_url="https://github.com/vgvassilev/llvm"
llvm_branch="cling-patches-llvm13"
clang_url="https://github.com/vgvassilev/clang"
clang_branch="cling-patches-llvm13"
cling_url="https://github.com/vgvassilev/cling"
cling_branch="cling-patches-llvm13"

function prepare()
{
  if [ ! -d "$srcdir/llvm" ];
  then
    git clone -b "${llvm_branch}" --single-branch "${llvm_url}" "${srcdir}"/llvm
  fi

  if [ ! -d "$srcdir/clang" ];
  then
    git clone -b "${clang_branch}" --single-branch "${clang_url}" "${srcdir}"/clang
  fi

  if [ ! -d "$srcdir/cling" ];
  then
    git clone -b "${cling_branch}" --single-branch "${cling_url}" "${srcdir}"/cling
  fi

  if [ ! -h "$srcdir/llvm/tools/clang" ];
  then
    ln -sf "$srcdir/clang" "$srcdir/llvm/tools/clang"
  fi

  if [ ! -h "$srcdir/llvm/tools/cling" ];
  then
    ln -sf "$srcdir/cling" "$srcdir/llvm/tools/cling"
  fi
}

function build()
{
  mkdir -p "$srcdir/cling-build"
  cd "$srcdir/cling-build"

  cmake -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_INSTALL_PREFIX="/opt/cling" \
        -DLLVM_TARGETS_TO_BUILD="host;NVPTX" \
        -DLLVM_BUILD_LLVM_DYLIB=OFF \
        -DLLVM_ENABLE_RTTI=ON \
        -DLLVM_ENABLE_FFI=ON \
        -DLLVM_BUILD_DOCS=OFF \
        -DLLVM_ENABLE_SPHINX=OFF \
        -DLLVM_ENABLE_DOXYGEN=OFF \
        -DFFI_INCLUDE_DIR=$(pkg-config --cflags-only-I libffi | cut -c3-) \
        -DCLING_CXX_HEADERS=ON \
        "$srcdir/llvm"

  make -j16 -C tools/clang
  make -j16 -C tools/cling
}

prepare
build
```
