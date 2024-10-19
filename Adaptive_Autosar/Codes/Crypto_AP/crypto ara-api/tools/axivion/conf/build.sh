#!/bin/bash
echo -e "-----------------------------------"
echo -e "BUILDING IR FILE OF PACKAGE MANAGER"
echo -e "-----------------------------------"
source ~/bauhaus/714/bauhaus-kshrc
source ~/AUTOSAR/sdks/A00048/environment-setup-core2-32-poky-linux

export BAUHAUS_CONFIG=$PWD/avin-ap/crypto/tools/axivion/conf
export CXX="cafeCC --strict"



if [ ! -d "avin-ap/crypto/tools/axivion/build" ]
then
  mkdir avin-ap/crypto/tools/axivion/build
fi

cd avin-ap/crypto/tools/axivion/build
cmake -DARA_ENABLE_TESTS=OFF -DARA_RUN_TESTS=OFF -DCMAKE_CXX_COMPILER_VERSION="9.2.0" ../../../source/
make
cd ../

#if [ ! -d "ara_build" ]
#then
#  mkdir ara_build
#fi

#cd ara_build
#cmake -DARA_ENABLE_TESTS=OFF -DARA_RUN_TESTS=OFF -DCMAKE_CXX_COMPILER_VERSION="9.2.0" ../../../source/ara-crypto/app
#make
#cd ../

#irlink --include_unused ara_build/ara-crypto/app/ara-crypto-app/src/libara-crypto-app.a build/crypto-daemon/src/CryptoServiceDaemon result.ir

irlink --include_unused build/crypto-daemon/src/CryptoServiceDaemon result.ir

