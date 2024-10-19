# Debug Adaptive Platform Application Using Eclipse {#Debug_App}
## Pre requisite:
> **Note: Add meta-ara and meta-cm-use-case-avin in bblayer.conf before creating SDK**

   * Add the following image in IMAGE_INSTALL in the respective application recipe found in /yocto-layers/meta-ara/recipes-core and for CM application add in /yocto-layers/meta-ara/recipes-core/core-image-apd-common-avin.inc : 
~~~
IMAGE_INSTALL_append = "\
        ara-dropbear \
        ara-getty \
        gdbserver \
        openssh-sftp-server \
        glibc-utils \
        localedef \
        "
~~~
   * Add the following in build/conf/local.conf
   ~~~
EXTRA_IMAGE_FEATURES_append =  " eclipse-debug tools-sdk tools-debug dbg-pkgs"
IMAGE_GEN_DEBUGFS = "1"
IMAGE_FSTYPES_DEBUGFS = "tar.bz2"
~~~

## Creation of SDK:
   * Bitbake image and create SDK.
~~~
    $ bitbake core-image-apd-minimal
    $ bitbake ara-sdk
~~~
   * ara-adk creates a script in build/tmp/deploy/sdk/<script-name>.sh

   * Create a folder in home directory to install sdk.
~~~
     $ mkdir autosar_sdk && cd autosar_sdk
     $ autosar_sdk:  mkdir sdk && cd sdk
     $ autosar_sdk/sdk:  <path-to-script>.sh -y -d   ./nxpsdk
~~~


>  **Note: It is found in  /build/tmp/deploy/sdk.eg: /home/avin/Rocko_adaptive/fsl-auto-yocto-bsp/build_s32v234sbc/tmp/deploy/sdk**

## Installation of Eclipse
   * Install the Neon version of the Eclipse for C/C++ developers from eclipse.org
   * Start the Eclipse IDE.
   * Make sure you are in your Workbench and select "Install New Software" from the "Help" pull-down menu.
>   **Note: Some plugin will not be listed as it may already be installed in eclipse.**
   * Select Neon - http://download.eclipse.org/releases/neon from the "Work with:" pull-down menu.
   * Expand the box next to "Linux Tools" and select 
     * LTTng - Linux Tracing Toolkit boxes.
   * Expand the box next to "Mobile and Device Development" and select the following boxes:
     * C/C++ Remote Launch
     * Remote System Explorer End-user Runtime
     * Remote System Explorer User Actions
     * Target Management Terminal
     * TCF Remote System Explorer add-in
     * TCF Target Explorer
     ![Install Package](Images/Debug1.png)
     ![Install Package](Images/Debug2.png)
     ![Install Package](Images/Debug3.png)

   * Expand the box next to "Programming Languages" and select 
     * Autotools Support for CDT 
     * C/C++ Development Tools boxes.
     ![Install Package](Images/Debug4.png)
   * Add new link to download yocto plugin http://downloads.yoctoproject.org/releases/eclipse-plugin/2.3.2/neon and install all plugin listed for yocto.
   ![Install Package](Images/Debug5.png)


## Setup SDK gdb in eclipse
   * source SDK script before starting eclipse
~~~
    $ source  autosar_sdk/sdk/nxpsdk/environment-setup-aarch64-fsl-linux
~~~
>    **Note: “which cmake” is command to check whether the cmake path is pointing to sdk cmake.**

![SDK Check](Images/which_make.png)

   * Start Eclipse in the “Sourced” terminal.
~~~
   $ eclipse/eclipse
~~~

   * When Eclipse has started the first time the Yocto Project SDK tools configuration should be done.
   * This is done in Eclipse under the Window menu >  Preferences > Yocto Project SDK
   ![Setup SDK Configuration](Images/preference.png)

~~~
 Note: Cross compiler should be standalone pre-built toolchain.
      Point Toolchain Root location and Sysroot Location to sdk.
      Target Architecture should be available once pointed to proper sdk.
      eg: toolchain Root location:/home/avin/Autosar_sdk/sdk/nxpsdk 
      Sysroot Location: /home/avin/Autosar_sdk/sdk/nxpsdk/sysroot/
~~~

## Importing project to debug:

   * Create Build directory in autosar_sdk and build the application
   * Each sample application will have cmake command with different command line argument. Refer CMake command section at the end
~~~
    $ cd autosar_sdk
    $autosar_sdk: source sdk/nxpsdk/environment-setup-aarch64-fsl-linux
    $autosar_sdk: mkdir Build && cd Build
    $autosar_sdk/Build:  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON   -DENABLE_DOXYGEN=ON   -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -G"Eclipse CDT4 - Unix Makefiles" </path /to/application/to/debug>
~~~
~~~
Eg. Rocko_adaptive/fsl-auto-yocto-bsp/sample-applications/emo_samples/
    $autosar_sdk/Build: cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  -DENABLE_DOXYGEN=ON  -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON -G "Eclipse CDT4 - Unix Makefiles" /home/avin/Rocko_adaptive/fsl-auto-yocto-bsp/sample-applications/emo_samples/
    $autosar_sdk/Build: make
~~~
   * Import the project to eclipse(eclipse should be opened on sourced terminal)
   * Click File > Import > C/C++ > Existing project into workspace then click next
   * Browse root location to Build folder in autosar_sdk and click on Yocto CMAKE build

   * Click finish.

   * Build the project

## Setup Debug rootfs on host machine
   * Create a new folder for debug filesystem in home directory
~~~
  $ mkdir debugfs
  $ cd debugfs
  $ tar xzf <path/to>/build_s32v234sbc/tmp/deploy/images/s32v234sbc/core-image-apd-minimal-s32v234sbc-20180904061102.rootfs.tar.gz
  $ tar xjf <path/to>/build_s32v234sbc/tmp/deploy/images/s32v234sbc/core-image-apd-minimal-s32v234sbc-20180904061102-dbg.rootfs.tar.bz2
~~~

## Deploying Image to Debug

   * Select the Imported project then click on Debug As > Debug configuration, new window will apear.

   * Under C/C++ Remote Application Imported application will appear select the project.
   In Main tab under C/C++ Application browse to executable in sdk build directory.
   For eg: /home/avin/fsl_sdk/build/src/TestAppl1
   ![Debug Configuration](Images/debgug_config_main.png)
   ![Debug Configuration](Images/debgug_config_main1.png)

   * In build configuration select use active option in drop down

   * Use workspace setting should be selected.

   * In connection create a new connection
     ![SSH Connection](Images/ssh_connection.png)

   * In host give the ip address of the board and root as user

   * In command to execute before application, give cd /opt/TestAppl1
   * In Debugger Tab , GDB debugger should be pointing to GDB exe in SDK  and GDB command line should be .gdbinit in workspace
~~~
for eg : Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/bin/aarch64-fsl-linux/aarch64-fsl-linux-gdb
~~~
   
   ![Debug Configuration](Images/Debug_config.png)


   * If .gdbinit is not found create new file with following entry
~~~
      set sysroot debugfs
      set substitute-path /usr/src/debug debugfs/usr/src/debug
~~~
   * In main tab , Remote absolute path for C/C++ application should be path in filesystem of the board(If connection is made between board and host , browse option will show you boards filsystem)

   * Now select apply and then debug.(Board should be connected to the network and you should be able to ping the board )

   * Before you hit debug on eclipse following has to be done on target board.

## Boot the image on target board
   * Create SD card to boot the image
~~~
 $ sudo dd if=</path/to>build_s32v234sbc/tmp/deploy/images/s32v234sbc/core-image-apd-minimal-s32v234sbc-20180904061102.rootfs.sdcard of=/dev/mmcblk0 bs=1M && sync
~~~
   * Insert the Sd card on sbc board
   * Connect the board and the host machine with micro USB
   * Start a new terminal on host machine and start picocom
~~~
  $ sudo picocom -b 115200 /dev/ttyUSB0
~~~
   * Power on the board
   * Enter 'root' when asked for user
   * Run the execution-manager in background
~~~
   $ execution-manager&
~~~
   * Open a new terminal on host machine and connect the board through ssh
~~~
   $ ssh root@<board ip address>
~~~
   * When you logged into the board through SSH, type 'top' command and look for the pid of the application you want to debug.
   * Kill the application you want to debug.
~~~
   $ kill <pid>
~~~

## CMake command for Sample Application 
>    **Note: Change the application path and sdk path as per host machine path**

* CMake for UCM application(pkgmgr_sample)
```
cmake -DCMAKE_NO_SYSTEM_FROM_IMPORTED=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DARAGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/aragen.py  -DJSONGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/jsongen.py  -DARA_UCM_PKGMGR_MODEL_DIR=$HOME/Rocko_adaptive/fsl-auto-yocto-bsp/ara-api/ucm/pkgmgr/files/manifests -DARA_ARXMLS_DIR=$HOME/Rocko_adaptive/fsl-auto-yocto-bsp/ara-api/apd/arxmls -DCMAKE_ECLIPSE_VERSION=4.6 -G"Eclipse CDT4 - Unix Makefiles"  $HOME/Rocko_adaptive/fsl-auto-yocto-bsp/avin-applications/ucm-use-case-avin/pkgmgr_sample
```
* CMake for Mode_Manager_ecu1 
```
cmake  -Wno-dev -DCMAKE_NO_SYSTEM_FROM_IMPORTED=1  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  -DROUTING=vsomeipd  -DLOG_LEVEL=debug -DCMAKE_BUILD_TYPE=Debug -DARAGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/aragen.py  -DJSONGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/jsongen.py  -DARA_ARXMLS_DIR=$HOME/Rocko_adaptive/fsl-auto-yocto-bsp/ara-api/apd/arxmls -DCMAKE_ECLIPSE_VERSION=4.6 -G"Eclipse CDT4 - Unix Makefiles"  $HOME/Rocko_adaptive/fsl-auto-yocto-bsp/avin-applications/em-use-case-avin/StateManager_usecase/Mode_Manager_ecu1
```
* CMake for Mode_Manager_ecu2
```
cmake  -Wno-dev -DCMAKE_NO_SYSTEM_FROM_IMPORTED=1  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON  -DROUTING=vsomeipd  -DLOG_LEVEL=debug -DCMAKE_BUILD_TYPE=Debug -DARAGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/aragen.py  -DJSONGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/jsongen.py  -DARA_ARXMLS_DIR=$HOME/Rocko_adaptive/fsl-auto-yocto-bsp/ara-api/apd/arxmls -DCMAKE_ECLIPSE_VERSION=4.6 -G"Eclipse CDT4 - Unix Makefiles"  $HOME/Rocko_adaptive/fsl-auto-yocto-bsp/avin-applications/em-use-case-avin/StateManager_usecase/Mode_Manager_ecu2
```
* CMake for emo-sample-avin
```
cmake -Wno-dev -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_VERSION=4.6 -G"Eclipse CDT4 - Unix Makefiles"  $HOME/Rocko_adaptive/fsl-auto-yocto-bsp/avin-applications/em-use-case-avin/emo-sample-avin
```
* CMake for per_usecase
```
cmake -Wno-dev -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_ECLIPSE_VERSION=4.6 -G"Eclipse CDT4 - Unix Makefiles"  $HOME/Rocko_adaptive/fsl-auto-yocto-bsp/avin-applications/per-use-case-avin/per_usecase
```
* CMake for RESTServer
```
cmake -Wno-dev -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DROUTING=vsomeipd  -DLOG_LEVEL=debug -DCMAKE_BUILD_TYPE=Debug -DARAGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/aragen.py  -DJSONGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/jsongen.py -DARA_ARXMLS_DIR=$HOME/Rocko_adaptive/fsl-auto-yocto-bsp/ara-api/apd/arxmls -DCMAKE_ECLIPSE_VERSION=4.6 -G"Eclipse CDT4 - Unix Makefiles"  $HOME/Rocko_adaptive/fsl-auto-yocto-bsp/avin-applications/rest-use-case-avin/RESTServer
```
* CMake for DoorSwc
```
cmake -Wno-dev -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DROUTING=vsomeipd  -DLOG_LEVEL=debug -DCMAKE_BUILD_TYPE=Debug -DARAGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/aragen.py  -DJSONGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/jsongen.py -DARA_ARXMLS_DIR=$HOME/Rocko_adaptive/fsl-auto-yocto-bsp/ara-api/apd/arxmls -DCMAKE_ECLIPSE_VERSION=4.6 -G"Eclipse CDT4 - Unix Makefiles" $HOME/Rocko_adaptive/fsl-auto-yocto-bsp/avin-applications/rest-use-case-avin/DoorSwc
```
* CMake for BatterySwc
```
cmake -Wno-dev -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DROUTING=vsomeipd  -DLOG_LEVEL=debug -DCMAKE_BUILD_TYPE=Debug -DARAGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/aragen.py  -DJSONGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/jsongen.py -DARA_ARXMLS_DIR=$HOME/Rocko_adaptive/fsl-auto-yocto-bsp/ara-api/apd/arxmls -DCMAKE_ECLIPSE_VERSION=4.6 -G"Eclipse CDT4 - Unix Makefiles" $HOME/Rocko_adaptive/fsl-auto-yocto-bsp/avin-applications/rest-use-case-avin/BatterySwc
```
* CMake for Performance application ecu2 (Radar)
```
cmake -Wno-dev -DCMAKE_NO_SYSTEM_FROM_IMPORTED=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DROUTING=vsomeipd  -DLOG_LEVEL=debug -DCMAKE_BUILD_TYPE=Debug -DARAGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/aragen.py  -DJSONGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/jsongen.py -DARA_ARXMLS_DIR=$HOME/Rocko_adaptive/fsl-auto-yocto-bsp/ara-api/apd/arxmls -DCMAKE_ECLIPSE_VERSION=4.6 -G"Eclipse CDT4 - Unix Makefiles" $HOME/Rocko_adaptive/fsl-auto-yocto-bsp/avin-applications/performance-use-case-avin/radar
```
* CMake for Performance application ecu1 (Fusion)
```
cmake -Wno-dev -DCMAKE_NO_SYSTEM_FROM_IMPORTED=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DROUTING=vsomeipd  -DLOG_LEVEL=debug -DCMAKE_BUILD_TYPE=Debug -DARAGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/aragen.py  -DJSONGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroot/x86_64-fslbsp-linux/usr/share/ara-gen/jsongen.py -DARA_ARXMLS_DIR=$HOME/Rocko_adaptive/fsl-auto-yocto-bsp/ara-api/apd/arxmls -DCMAKE_ECLIPSE_VERSION=4.6 -G"Eclipse CDT4 - Unix Makefiles" $HOME/Rocko_adaptive/fsl-auto-yocto-bsp/avin-applications/performance-use-case-avin/fusion
```
* CMake for cm use case
> Note: OpenCv 3.1.0 has to be installed into SDK before debugging CM usecase application and the step is provided in the following link.

    https://bugzilla.autosar.org/wiki/doku.php?id=apd_user_guide:sdk:guide:add_items_to_sdk&s[]=sdk&s[]=opencv
    
*  CMake command for Video Provider Right
```
cmake -DCMAKE_NO_SYSTEM_FROM_IMPORTED=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON-DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DARAGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroots/x86_64-pokysdk-linux/usr/share/ara-gen/aragen.py  -DJSONGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroots/x86_64-pokysdk-linux/usr/share/ara-gen/jsongen.py  -DARA_ARXMLS_DIR=$HOME/Rocko_adaptive/fsl-auto-yocto-bsp/ara-api/apd/arxmls -DBUILD_MACHINE_STATE_MANAGER=OFF -DBUILD_VIDEO_PROVIDER=OFF -DBUILD_VIDEO_PROVIDER_RIGHT=ON -DBUILD_VIDEO_ADAPTER=OFF -DBUILD_PREPROCESSING=ON -DBUILD_COMPUTER_VISION=ON -DBUILD_EBA=ON -DCMAKE_ECLIPSE_VERSION=4.6 -G"Eclipse CDT4 - Unix Makefiles"  $HOME/Rocko_adaptive/fsl-auto-yocto-bsp/avin-applications/cm-use-case-avin
```
*  CMake command for Video Provider Left
```
cmake -DCMAKE_NO_SYSTEM_FROM_IMPORTED=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DARAGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroots/x86_64-pokysdk-linux/usr/share/ara-gen/aragen.py  -DJSONGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroots/x86_64-pokysdk-linux/usr/share/ara-gen/jsongen.py  -DARA_ARXMLS_DIR=$HOME/Rocko_adaptive/fsl-auto-yocto-bsp/ara-api/apd/arxmls -DBUILD_MACHINE_STATE_MANAGER=OFF -DBUILD_VIDEO_PROVIDER=ON -DBUILD_VIDEO_PROVIDER_RIGHT=OFF -DBUILD_VIDEO_ADAPTER=OFF -DBUILD_PREPROCESSING=ON -DBUILD_COMPUTER_VISION=ON -DBUILD_EBA=ON -DCMAKE_ECLIPSE_VERSION=4.6 -G"Eclipse CDT4 - Unix Makefiles" $HOME/Rocko_adaptive/fsl-auto-yocto-bsp/avin-applications/cm-use-case-avin
```
*  CMake command for Video Adapter
```
cmake -DCMAKE_NO_SYSTEM_FROM_IMPORTED=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug -DARAGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroots/x86_64-pokysdk-linux/usr/share/ara-gen/aragen.py  -DJSONGEN_PATH=$HOME/Autosar_sdk/sdks/nxpsdk/sysroots/x86_64-pokysdk-linux/usr/share/ara-gen/jsongen.py  -DARA_ARXMLS_DIR=$HOME/Rocko_adaptive/fsl-auto-yocto-bsp/ara-api/apd/arxmls -DBUILD_MACHINE_STATE_MANAGER=OFF -DBUILD_VIDEO_PROVIDER=OFF -DBUILD_VIDEO_PROVIDER_RIGHT=OFF -DBUILD_VIDEO_ADAPTER=ON -DBUILD_PREPROCESSING=ON -DBUILD_COMPUTER_VISION=ON -DBUILD_EBA=ON -DCMAKE_ECLIPSE_VERSION=4.6 -G"Eclipse CDT4 - Unix Makefiles" $HOME/Rocko_adaptive/fsl-auto-yocto-bsp/avin-applications/cm-use-case-avin
```

