# git submodule update --init --recursive

export WORKSPACE=$PWD
export EDK_TOOLS_PATH=$PWD/BaseTools
export PATH=$EDK_TOOLS_PATH/Bin/Linux-x86_64:$PATH

make -C BaseTools

. edksetup.sh; build -p MuffinBootPkg/MuffinBootPkg.dsc -b RELEASE -a X64 -t GCC5
