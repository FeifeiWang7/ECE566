#!/bin/sh

VCL_BASE_PATH=/usr/ece566/install/

# change this to ece466 if you're in 466
CLASS=ece566 

CURRENT_DIR=`pwd`
INSTALL_DIR=`pwd`/install
BUILD_DIR=`pwd`/obj

LLVM_SRC=$VCL_BASE_PATH/llvm-3.1.src
LLVM_OBJ=$VCL_BASE_PATH/obj
LLVM_INSTALL=$VCL_BASE_PATH/bin
PYTHON=python

CC=$LLVM_INSTALL/clang
GCC46=$LLVM_INSTALL/gcc

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
    if [ -x /usr/ece566/dragonegg.so ]; then
	echo Found dragonegg.so
    else
	exit 0
    fi
fi

cd $CURRENT_DIR

if [ -d projects ]; then
    echo Found projects!  Not checking it out again.
else
    svn co file:///afs/eos.ncsu.edu/lockers/workspace/ece/$CLASS/001/common/repo/projects
    if [ -d projects ]; then
	cd projects
	./configure --with-llvmsrc=$LLVM_SRC --with-llvmobj=$LLVM_OBJ --prefix=`pwd`/install
	echo Building projects in projects directory.
	make &&  make install
    else
	echo Did not check out projects properly.
	exit 1
    fi
fi

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
    echo Found wolfbench. Not checking out wolfbench again.
else
    svn co file:///afs/eos.ncsu.edu/lockers/workspace/ece/$CLASS/001/common/repo/wolfbench
fi

source $CLASS.projects

mkdir -p p0
cd p0
../wolfbench/configure 
make && make install

echo Before doing anything else, type: source $CLASS.projects
echo 
echo Note, you will need to do this everytime you login. It sets up
echo key environment variables.
