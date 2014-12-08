#!/bin/sh

WGET=wget
CURRENT_DIR=`pwd`
INSTALL_DIR=`pwd`/install
BUILD_DIR=`pwd`/llvm-build
PYTHON=python27
CLASS=566
VERSION=3.4
TARGET=

if [ "$(uname)" == "Darwin" ]; then
    WGET='curl -O ' 
    TARGET='--target=x86_64'
fi

if [ -d llvm-$VERSION ]; then
    echo Found llvm-$VERSION! Not downloading source again.
else
    $WGET http://www.llvm.org/releases/$VERSION/llvm-$VERSION.src.tar.gz
    tar xzf llvm-$VERSION.src.tar.gz
fi

if [ -d llvm-$VERSION ]; then
    echo Everything looks sane.
else
    echo Install had problems. Quitting.
    exit
fi

if [ -d llvm-$VERSION/tools ]; then
    cd llvm-$VERSION/tools
else
    echo Fail! Something is wrong with your llvm-$VERSION checkout! 
fi

if [ -d clang ]; then
    echo Found clang! Not downloading clang again.
else
    $WGET http://www.llvm.org/releases/$VERSION/clang-$VERSION.src.tar.gz
    tar xzf clang-$VERSION.src.tar.gz
    mv clang-$VERSION clang
fi

if [ -d compiler-rt ]; then
    echo Found compiler-rt! Not downloading compiler-rt again.
else
    $WGET http://www.llvm.org/releases/$VERSION/compiler-rt-$VERSION.src.tar.gz
    tar xzf compiler-rt-$VERSION.src.tar.gz
    mv compiler-rt-$VERSION compiler-rt
fi

if [ -d compiler-rt ]; then
    echo Everything looks sane.
else
    echo Install had problems. Quitting.
    exit
fi

if [ -d clang ]; then
    echo Everything looks sane.
else
    echo Install had problems. Quitting.
    exit
fi

if [ -d $BUILD_DIR ]; then
    cd $BUILD_DIR
    echo Found $BUILD_DIR.  Remove to reconfigure LLVM and Clang.
else
    mkdir -p $BUILD_DIR
    mkdir -p $INSTALL_DIR
    cd $BUILD_DIR
    ../llvm-$VERSION/configure --disable-optimized --prefix=$INSTALL_DIR
fi

if [ -x $INSTALL_DIR/bin/clang ]; then
    echo LLVM successfully installed...
else
    echo Building llvm...
    make && make install

    if [ -x $INSTALL_DIR/bin/clang ]; then
	true
    else
	echo LLVM not installed properly.
        exit 0
    fi
fi

cd $CURRENT_DIR

export PATH="$INSTALL_DIR/bin:$PATH"

if [ -d projects ]; then
    echo Found projects!  Not checking it out again.
    cd projects
    ./configure --with-llvmsrc=$CURRENT_DIR/llvm-$VERSION --with-llvmobj=$BUILD_DIR --prefix=`pwd`/install --disable-optimized $TARGET
    echo Building projects in projects directory.
    make
    make install
else
    echo Did not check out projects properly.
    exit 1
fi

cd $CURRENT_DIR
touch ece$CLASS.projects
echo PATH=$PATH:$INSTALL_DIR/bin:\$PATH > ece$CLASS.projects
export PATH=$PATH:$INSTALL_DIR/bin

#echo Setting up Python support
#cd $CURRENT_DIR

#if [ -d llvmpy ]; then
#    echo Found llvmpy. Not cloning repository again.
#else
#    git clone https://github.com/llvmpy/llvmpy.git
#    cd llvmpy
#    $PYTHON setup.py build
#    cd $CURRENT_DIR
#    echo PYTHONPATH=\$PYTHONPATH:\`cd ./llvmpy/install/lib/python2.7/site-packages/\; pwd\` >> ece$CLASS.projects 
#fi

cd $CURRENT_DIR

#if [ -d ply-3.4 ]; then
#    echo ply-3.4 found.  Not downloading again.
#else
#    wget http://www.dabeaz.com/ply/ply-3.4.tar.gz
#    tar xzf ply-3.4.tar.gz
#    cd ply-3.4
#    $PYTHON setup.py build
#    cd $CURRENT_DIR
#    echo PYTHONPATH=\$PYTHONPATH:\`cd ./ply-3.4/build/lib/\; pwd\` >> ece$CLASS.projects 
#fi

if [ -d ./wolfbench ]; then
    echo Found wolfbench. 
else
    echo You should have wolfbench already! Did you run this script from the wrong directory?
fi

mkdir -p p0
cd p0
../wolfbench/configure 
make && make install
