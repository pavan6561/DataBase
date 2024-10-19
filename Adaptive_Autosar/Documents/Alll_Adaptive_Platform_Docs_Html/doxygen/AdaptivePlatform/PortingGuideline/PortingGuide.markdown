# Porting Adaptive Platform to Rocko {#Porting_rocko}
## Checkout Adaptive platform and other BSPs

1. Install the Pre-requisites 
```
    $home: sudo apt-get install gawk wget git-core diffstat unzip texinfo gcc-multilib \
                        build-essential chrpath socat libsdl1.2-dev xterm repo
```

2. Download the Adaptive platform demostrator 18-06 release
```
    $ mkdir fsl-auto-yocto-bsp
    & cd fsl-auto-yocto-bsp
    $ fsl-auto-yocto-bsp$: git clone ssh://git@code.autosar.org/tf-apd/ara-api
    $ fsl-auto-yocto-bsp$: git clone ssh://git@code.autosar.org/tf-apd/sample-applications
    $ fsl-auto-yocto-bsp$: git clone ssh://git@code.autosar.org/tf-apd/yocto-layers
    $ fsl-auto-yocto-bsp$: git -C ara-api checkout  4f804c096d2b4d08b25badc4322ab02fe7484a5d
    $ fsl-auto-yocto-bsp$: git -C sample-applications checkout  e07a84a5e302a1cc54c170e04d9dd55ba73cbcc9
    $ fsl-auto-yocto-bsp$: git -C yocto-layers checkout  07d0731abf244fb1afa7b7787c0e54d11b7020a7
    ```

## Changes required to run Adaptive platform on rocko

> **Note: Following changes are required in order to build an image on Rocko. Left hand side is the actual file and right hand side is the changes to be made.** 

1. Diagnostic functional cluster has few warnings during the compilation process and these warnings are treated as an error due to compiler option -Werror is used in CMakeLists.txt. In order to build image successfully, this option is removed from corresponding CMakeLists file. 

    Path: /ara-api/diag/CmakeLists.txt
    ![/ara-api/diag/CmakeLists.txt](images/Diag.png)
   
2. "Logger g_logINT" is a global variable of dlt which gets initialized even before the application gets forked, due to this execution manager gets stuck inside fork() API. Jira ticket - 6516 has been raised in this regard. In order to avoid this issue, following work around is applied in the logmanager.cpp file.

    Path: /ara-api/log/src/
    ![/ara-api/log/src/logmanager.cpp](images/logmanager.png)

    ![/ara-api/log/src/logmanager.cpp](images/logmanager1.png)

3. Boost components are used in the ara-per in order to optimize the performance. However, this component(Boost) is causing a build issue during linking process as it's not getting linked from the respective application receipe-sysroot. To remove dependency on Boost components used by ara-per, following changes have been made in the ara-per functional clusters. 

    Path: /ara-api/per/src/fileaccess/CmakeLists.txt
    ![/ara-api/per/src/fileaccess/CmakeLists.txt](images/fileaccess.png)

    Path: /ara-api/per/src/key-value-storage/CmakeLists.txt
    ![/ara-api/per/src/key-value-storage/CmakeLists.txt](images/Keyvaluestorage.png)

4. SYSTEM is a component in the boost library which is not used by the ucm package manager and unit-test CMakeList.txt. In order to build image successfully, SYSTEM component is removed from the respective CMakeLists.txt.

    Path: /ara-api/ucm/pkgmgr/src/CmakeLists.txt
    ![/ara-api/ucm/pkgmgr/src/CmakeLists.txt](images/pkgmgr.png)

    Path: /ara-api/ucm/pkgmgr/src/test/unit_test/CmakeLists.txt
    ![/ara-api/ucm/pkgmgr/src/test/unit_test/CmakeLists.txt](images/pkgmgr1.png)

5. This change is carried out to make Test-appl1 in emo-sample as machine client so that it can interact with Execution manager to change the Machine state.

    Path: /sample-applications/emo_samples/MANIFEST-appl1.json
    ![/sample-applications/emo_samples/MANIFEST-appl1.json](images/emo_samples.png)

6. functional header file is added to resolve the dependency in the machine state manager.

    Path: /sample-applications/st_scenarios/machine_state_manager/inc/i_ms_control_server.h
    ![/sample-applications/st_scenarios/machine_state_manager/inc/i_ms_control_server.h](images/st_scenarios.png)

    Path: /sample-applications/st_scenarios/machine_state_manager/inc/ms_client_manager.h
    ![/sample-applications/st_scenarios/machine_state_manager/inc/ms_client_manager.h](images/st_scenarios1.png)

7. lib-platform dependency is added in ara-com-lib recipe file in order to resolve dependency.

    Path: /yocto-layers/meta-ara/recipes-ara/com/ara-com-lib_1.0.0.bb
    ![/yocto-layers/meta-ara/recipes-ara/com/ara-com-lib_1.0.0.bb](images/recipes_ara_com.png)

8. coreutils-native dependency is added in the execution-manager.bb file. 
   To remove auto start of Execution manager during boot process, few lines have been commented (optional).

    Path: /yocto-layers/meta-ara/recipes-ara/exec/execution-manager.bb
    ![/yocto-layers/meta-ara/recipes-ara/exec/execution-manager.bb](images/recipes-ara-exec.png)

9. boost library dependency is added ara-iam recipe file

    Path: /yocto-layers/meta-ara/recipes-ara/iam/ara-iam.bb
    ![/yocto-layers/meta-ara/recipes-ara/iam/ara-iam.bb](images/recipes-ara-iam.png)

10. Python library has been replaced with two libraries - python-native and python-lxml-native.

    Path: /yocto-layers/meta-ara/recipes-ara/sample-applications/ara-com-example.inc
![/yocto-layers/meta-ara/recipes-ara/sample-applications/ara-com-example.inc](images/recipes-ara-sample-app.png)

11. coreutils-native dependency is added in the emo-sample.bb file.

    Path: /yocto-layers/meta-ara/recipes-ara/sample-applications/emo-samples_1.0.0.bb
![/yocto-layers/meta-ara/recipes-ara/sample-applications/emo-samples_1.0.0.bb](images/recipes-ara-sample-app-emo.png)

12. ara-com-gen has been replaced with ara-com-gen-native in ucm recipe file

    Path: /yocto-layers/meta-ara/recipes-ara/ucm/ucm-package-manager.bb
![/yocto-layers/meta-ara/recipes-ara/ucm/ucm-package-manager.bb](images/recipes-ara-ucm.png)

13. The inclusion of Machine State Manager is causing packaging issue and bitbake process gets stuck. In order to avoid hanging issue during bitbake process, Machine State Manager is removed while building an image. The ROOTFS_PKGMANAGE_BOOTSTRAP variable and any references to it have been removed from Yocto - Rocko version onwards. To remove auto start of Execution manager during boot process, few lines have been commented (optional).

    Path: /yocto-layers/meta-ara/recipes-core/images/core-image-apd-common.inc
![/yocto-layers/meta-ara/recipes-core/images/core-image-apd-common.inc](images/core-image-apd-common.png)

14. ara-rest library is dependent on gtest hence this is added in CMakeLists.txt and ara-rest-lib_1.0.0.bb.

    Path: /yocto-layers/meta-ara/recipes-ara/rest/ara-rest-lib_1.0.0.bb
![/yocto-layers/meta-ara/recipes-ara/rest/ara-rest-lib_1.0.0.bb](images/recipes-ara_rest_ara-rest-lib-bb.png)
    Path: /ara-api/rest/lib/ara_rest/CmakeLists.txt
![/ara-api/rest/lib/ara_rest/CmakeLists.txt](images/ara_rest_CMakeLists.txt.png)

15. INSTALL_INTERFACE modified the path variable, hence system libraries were not found during linking.

    Path: /ara-api/per/src/kvsparser/CmakeLists.txt
![/ara-api/per/src/kvsparser/CmakeLists.txt](images/kvsparser.png)

16. INSTALL_INTERFACE modified the path variable, hence system libraries were not found during linking.

    Path: /ara-api/per/src/kvstype/CmakeLists.txt
![/ara-api/per/src/kvstype/CmakeLists.txt](images/kvstype.png)

17. do_package_qa caused the bitbaking to hang hence it has been commented.

    Path: /yocto-layers/meta-ara/recipes-ara/sample-applications/machinestatemanager_1.0.0.bb
![/yocto-layers/meta-ara/recipes-ara/sample-applications/machinestatemanager_1.0.0.bb](images/machinestatemanager-1.png)

Path: /yocto-layers/meta-ara/recipes-ara/sample-applications/machinestatemanager_1.0.0.bb
![/yocto-layers/meta-ara/recipes-ara/sample-applications/machinestatemanager_1.0.0.bb](images/machinestatemanager-2.png)

18. INSTALL_INTERFACE modified the path variable, hence system libraries were not found during linking.

    Path: /ara-api/per/src/manifestaccess/CmakeLists.txt
![/ara-api/per/src/manifestaccess/CmakeLists.txt](images/manifestaccess.png)

19. When an application tries to call the Update() method in "vsomeip_event_impl.h" where the queue is empty, the application will crashes (sporadic crashes) when trying to access a map object  at /com/src/internal/e2e/e2e_binding.cpp line 84(Jira Ticket - AP-6292).

    Path: ara-api/com/include/public/ara/com/internal/vsomeip/proxy/vsomeip_event_impl.h
![/ara-api/com/include/public/ara/com/internal/vsomeip/proxy/vsomeip_event_impl.h](images/vsomeip_event_impl.h.png)

## Porting Adaptive platform to NXP s3v234sbc
1. Download NXP bsp along with rocko verison of poky and compiler.
```
    cd fsl-auto-yocto-bsp
    $fsl-auto-yocto-bsp: repo init -u https://source.codeaurora.org/external/autobsps32/auto_yocto_bsp -b alb/master
    $fsl-auto-yocto-bsp: repo sync
```
2. First time setup
```
   $fsl-auto-yocto-bsp: ./sources/meta-alb/scripts/host-prepare.sh
```
```
! Note: When you run the host script you will see the following prompt.
```
![Host Script](images/hostScript.png)

```
    The line that is highlighted in the image has to be added in the sudoer file. To add the command in sudoer follow the steps given below.
    1. Copy the command in your terminal as it can change in different host 
    2. Enter the command:  sudo /usr/sbin/visudo
    3. Paste the line in user priviledge section.
    4. Then press ctrl+x, it will prompt whether to save so press yes.
    5. Now press ctrl+m+a.
```

3. Creating Build Directories.
```
    $fsl-auto-yocto-bsp: source nxp-setup-alb.sh -m s32v234sbc
```

4. if sourcing for the second time
```
    $fsl-auto-yocto-bsp: source build_s32v234sbc/SOURCE_THIS
```

5. Add the adaptive layer in bblayer.conf
```
    $fsl-auto-yocto-bsp/build_s32v234sbc: bitbake-layers add-layer ../yocto-layers/meta-ara
```

6. Add fsl-image-auto in local.conf
```
    $fsl-auto-yocto-bsp/build_s32v234sbc: echo "IMAGE_INSTALL_append ??= \"fsl-image-auto\"" >> conf/local.conf 
```

7. Bitbake the core-image-apd-minimal
```
    $fsl-auto-yocto-bsp/build_s32v234sbc: bitbake core-image-apd-minimal-radar
```

8. Flash image in SD cards.
```
    $fsl-auto-yocto-bsp/build_s32v234sbc: cd  tmp/deploy/images/s32v234sbc
    $fsl-auto-yocto-bsp/build_s32v234sbc/tmp/deploy/images/s32v234sbc: sudo dd if="image name".sdcard of=/dev/"dev" name bs=1M && sync
```
> for example "image name" = core-image-apd-minimal-radar-s32v234sbc-2018101181325204-rootfs.sdcard 
>
>   "dev name" = sdb1, sdb, mmbclk0 (check the dev name in Disk utility)

9. For log, connect the micro-usb to host machine and run the below command on new terminal
```
    $home: sudo picocom -b 115200 /dev/ttyUSB0
``` 

10. Insert the SD card, press the reset button and it will boot automatically.
