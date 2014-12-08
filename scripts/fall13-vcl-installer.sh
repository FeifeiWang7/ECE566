#!/bin/sh

# change this to ece466 if you're in 466
CLASS=ece566 

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
LLVM_INSTALL=$VCL_BASE_PATH/install
LLVM_BIN=$LLVM_INSTALL/bin
LLVM_DRAGONEGG=$VCL_BASE_PATH/dragonegg.so

PYTHON=python
GCC=$LLVM_BIN/gcc

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
touch $CLASS.projects
echo PATH=\$PATH:$INSTALL_DIR/bin > $CLASS.projects
echo PATH=\$PATH:$VCL_BASE_PATH/install/bin > $CLASS.projects
export PATH=$PATH:$INSTALL_DIR/bin

cd $CURRENT_DIR
echo Installing configuration script.

#echo PYTHONPATH=\$PYTHONPATH:\`cd $VCL_BASE_PATH/install/lib/python2.7/site-packages/\; pwd\` >> $CLASS.projects 
#echo PYTHONPATH=\$PYTHONPATH:\`cd $VCL_BASE_PATH/ply-3.4/build/lib/\; pwd\` >> $CLASS.projects 

if [ -d ./wolfbench ]; then
    echo Found wolfbench. 
else
    echo Oops. wolfbench should have been in the same directory with projects.
    exit 1
fi

source $CLASS.projects

mkdir -p p0
cd p0
../wolfbench/configure --enable-dragonegg=$LLVM_DRAGONEGG --enable-gcc=$GCC
make && make install

echo Before doing anything else, type: source $CLASS.projects
echo 
echo Note, you will need to do this everytime you login. It sets up
echo key environment variables.  You can execute the command from
echo any directory.
