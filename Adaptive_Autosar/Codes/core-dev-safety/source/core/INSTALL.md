# Run unit tests

Make sure that you have a Docker image that fits to your SDK (eg. both have been built from the same revision).
Here, the name `ara-apd:test` is mandatory and cannot be changed, otherwise the `dock` target won't find the container.

```sh
$ docker import /path/to/poky/build/tmp/deploy/images/xxx/core-image-apd-devel-xxx.tar.bz2 ara-apd:test
```

If you want to specify GTest parameters, call cmake with the optional parameter `-DARA_TEST_ARGS="<gtest_param>"`:

```sh
core$ mkdir build && cd build
build$ cmake -DARA_RUN_TESTS=ON -DARA_TEST_ARGS="--gtest_color=yes" -DCMAKE_INSTALL_PREFIX=build$OECORE_TARGET_SYSROOT/usr/ ..
build$
build$ make -j4 [-B] [install]
build$
build$ # once for the test session, start the docker container
build$ make dock
build$
build$ # if you want the dlt-daemon running for reduced time taken by the executables
build$ make rundlt
build$
build$ make test
build$ ctest -V  # more verbose output
build$ ctest [-V] -R apd-core-test(vector|map|string|optional|array|result_ex|result_noex|...)  # to run some specific test [in verbose mode]
build$ make ARGS="-V" test  # basically the same as above
build$
build$ # kill the container if you are finished with your session
build$ make undock
```

The test target might hang for a few seconds which is due to the DLT daemon not running on your system.
