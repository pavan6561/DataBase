#!/bin/bash
echo -e "-----------------------------------"
echo -e "BUILDING IR FILE OF CORE"
echo -e "-----------------------------------"
source ~/bauhaus/714/bauhaus-kshrc
source ~/AUTOSAR/sdks/A00056_CORE/environment-setup-core2-64-poky-linux

export BAUHAUS_CONFIG=$PWD/tools/axivion/conf
export CXX="cafeCC --strict"

if [ ! -d "tools/axivion/build" ]
then
  mkdir tools/axivion/build
fi

cd tools/axivion/build

cmake -DARA_ENABLE_TESTS=ON -DARA_RUN_TESTS=OFF -DCMAKE_CXX_COMPILER_VERSION="9.2.0" ../../../source/
make -j4
cd ../

irlink --include_unused build/core/src/libara_core.a \
    build/core-types/test/apd-core-testarray \
    build/core-types/test/apd-core-testbyte \
    build/core-types/test/apd-core-testcoreerrordomain \
    build/core-types/test/apd-core-testerrorcode \
    build/core-types/test/apd-core-testinstancespecifier \
    build/core-types/test/apd-core-testmap \
    build/core-types/test/apd-core-testfixedstring \
    build/core-types/test/apd-core-testbasicstring \
    build/core-types/test/apd-core-testpromise \
    build/core-types/test/apd-core-testfixedvector \
    build/core-types/test/apd-core-testfuture \
    build/core-types/test/apd-core-testoptional \
    build/core-types/test/apd-core-testspan \
    build/core-types/test/apd-core-teststeadyclock \
    build/core-types/test/apd-core-teststring \
    build/core-types/test/apd-core-teststringview \
    build/core-types/test/apd-core-testvariant \
    build/core-types/test/apd-core-testvector \
    build/core-types/test/apd-core-testresult_ex \
    build/core-types/test/apd-core-testresult_noex \
    build/core-types/test/apd-pmr-test \
    build/core/src/test/apd-core-testabort \
    build/core/src/test/apd-core-testinitialize \
    build/utils/config-reader/libcore-config-reader.so \
    build/utils/config-reader/src/test/test_miniparser \
    build/utils/thread/libcore-thread.a \
    build/utils/thread/src/test/test_ara_core_thread \
    build/utils/ipc/libcore-ipc.a \
    build/utils/ipc/src/test/test_message_ipc \
    build/utils/cond-var/libcore-cond-var.a \
    build/utils/cond-var/src/test/test_condition \
    build/utils/timer/libcore-timer.a \
    build/utils/timer/src/test/test_timer_thread \
    result.ir
