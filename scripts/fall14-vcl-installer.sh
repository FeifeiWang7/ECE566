#!/bin/sh

# change this to ece466 if you're in 466

CURRENT_DIR=`pwd`
INSTALL_DIR=`pwd`/install
BUILD_DIR=`pwd`/obj


#--------------------------------------------------------------------------------
#--------------------------------------------------------------------------------
#--------------------------------------------------------------------------------
#
# VCL INFORMATION

VCL_BASE_PATH=/usr/ece566/

LLVM_SRC=$VCL_BASE_PATH/llvm-3.3.src
LLVM_OBJ=$VCL_BASE_PATH/llvm-build
LLVM_INSTALL=/usr/local
LLVM_BIN=$LLVM_INSTALL/bin
LLVM_DRAGONEGG=$VCL_BASE_PATH/dragonegg.so

PYTHON=python
GCC=gcc

if [ -x $LLVM_INSTALL/bin/llvm-config ]; then
    echo Found LLVM.
else
    echo LLVM not found. Alter LLVM_INSTALL to point to your installation directory.
    exit 0
fi

if [ -x $LLVM_INSTALL/bin/clang ]; then
    echo Found clang.
else
    echo Clang not found.  Looking for dragonegg.so
    if [ -x $LLVM_DRAGONEGG ]; then
	echo Found dragonegg.so
    else
	
	exit 0
    fi
fi

echo Everything is sane!

cd $CURRENT_DIR

if [ -d projects ]; then
    echo Found projects!  Not checking it out again.
else
    echo Did you run this from the ECE566Projects folder?
    exit 1
fi

cd projects
./configure --with-llvmsrc=$LLVM_SRC --with-llvmobj=$LLVM_OBJ --prefix=`pwd`/install --disable-optimized
echo Building projects in projects directory.
make &&  make install

cd $CURRENT_DIR

if [ -d ./wolfbench ]; then
    echo Found wolfbench. 
else
    echo Oops. wolfbench should have been in the same directory with projects.
    exit 1
fi

mkdir -p p0-test
cd p0-test
../wolfbench/configure 
make && make install

echo "Your environment has been installed!"
