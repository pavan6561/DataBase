#!/bin/bash
echo "[INFO ] Exectuting unit tests to prepare coverage report..."

if [ $# -lt 2 ]; then
  echo "[ERROR] Missing argument: expected arguments are path of [SDK Path] [FC Path]"
  echo "[INFO ] Usage: ./cov.sh <Absolute SDK Path> <Absolute FC Path>"
  exit 1
fi
set -x
lcov --version | grep 1.15
if [ $? -ne 0 ]
then
  echo "[ERROR] Please install lcov version 1.15 by using the following steps:"
  echo "[INFO ] sudo apt-get install alien"
  echo "[INFO ] wget https://github.com/linux-test-project/lcov/releases/download/v1.15/lcov-1.15-1.noarch.rpm"
  echo "[INFO ] sudo alien -i lcov-1.15-1.noarch.rpm"
  exit 1
fi

/usr/bin/python3 -m gcovr --version | grep 4.2
if [ $? -ne 0 ]
then
  echo "[ERROR] Please install gcovr version 4.2 for python3 by using:"
  echo "[INFO ] pip3 install gcovr"
  exit 1
fi

sdkPath=$1
fcPath=$2
sourcePath=$2/source
runPath=$(pwd)
gcovTool="${sdkPath}"/sysroots/x86_64-pokysdk-linux/usr/bin/x86_64-poky-linux/x86_64-poky-linux-gcov
cd ${fcPath}
mkdir "build"

val="core core-types utils/timer utils/config-reader utils/thread utils/ipc utils/cond-var"
rm -rf function_cov/ line_branch_cov/
# Iterate the string variable using for loop
#for /R "${fcPath}/source/" %%i in (*.xml) do copy "%%i" "${fcPath}/build" 
for StringVal in $val; 
do
echo "[INFO ] Provided arguments are .." 
echo "[INFO ] SDK path is : ${sdkPath}"
echo "[INFO ] Source path is : ${sourcePath}/${StringVal}"
 buildPath=${sourcePath}/${StringVal}/build


if [ ! -d "${buildPath}" ]
then
  echo "[INFO ] Creating build directory"
  mkdir "${buildPath}"
fi
echo "[INFO ] Build path is : ${buildPath}"
echo "[INFO ] Setting SDK environment..."
source  "${sdkPath}"/environment-setup-core2-64-poky-linux

cd "${buildPath}"

echo "[INFO ] Building executables.."

cmake -DDM_GENERATOR=OFF -DCOM_GENERATOR=OFF -DARA_ENABLE_TESTS=ON -DARA_RUN_TESTS=ON -DAPD_DOCKER_SUFFIX="${APD_DOCKER_SUFFIX:-${USER}}" \
-DARA_TEST_ARGS="--gtest_output=xml:${buildPath}/" \
      -DCMAKE_INSTALL_PREFIX=$OECORE_TARGET_SYSROOT/usr ${sourcePath}/${StringVal}
make -j$(nproc)

if [ $? -ne 0 ]
then
  echo "[Error] Unit Test Build failed."
  exit 1
fi

if [ ! -f "${buildPath}"/coverage-base.info ]
then
  # Create empty coverage data for all source files (that have *.gcno data).
  echo "[INFO ] Creating Lcov coverage-base.info" 
  lcov -o "${buildPath}"/coverage-base.info -c -i -d "${buildPath}" --gcov-tool "${gcovTool}"
fi

echo "[INFO ] Executing Tests..." 
make dock
make rundlt
ctest -V
if [ $? -ne 0 ]
then
  echo "[Error] Unit Test Build failed."
  make undock
  exit 1
fi
make undock

cd ${runPath}

if [ ! -d line_branch_cov ]
then
  mkdir line_branch_cov
fi


if [ $? -eq 0 ]
then
  # Create coverage data from last run(s).
  echo "[INFO ] Creating Lcov coverage-full.info" 
  lcov --rc lcov_branch_coverage=1 -o "${buildPath}"/coverage-full.info -c -d "${buildPath}" --gcov-tool "${gcovTool}"

  # Merge the initial empty coverage data and last run(s)' data together.
  echo "[INFO ] Merging Lcov the initial empty coverage data and last run(s)' data together into coverage-full.info" 
  lcov --rc lcov_branch_coverage=1 -a "${buildPath}"/coverage-base.info -a "${buildPath}"/coverage-full.info \
       -o "${buildPath}"/coverage-all.info --gcov-tool "${gcovTool}"

  # Keep only that coverage data that refers to files below a specific path (this excludes /usr/include, amongst others).
  echo "[INFO ] Creating Lcov coverage.info from coverage-all.info" 
  lcov --rc lcov_branch_coverage=1 -e "${buildPath}"/coverage-all.info "${sourcePath}/**" -o "${buildPath}"/coverage.info --gcov-tool "${gcovTool}"

 # Remove the test folder for the coverage part.
  echo "[INFO ] Remove the test folder for the Lcov report" 
  lcov --rc lcov_branch_coverage=1 --remove "${buildPath}"/coverage.info "*/test/*" --output-file "${buildPath}"/coverage-final.info


  echo "[INFO ] Generating Line and branch coverage report"
  /usr/bin/python3 -m gcovr \
                   --root "${fcPath}" \
                   --gcov-executable "${gcovTool}" \
                   --exclude-unreachable-branches \
                   --exclude-throw-branches \
                   -j $(nproc) \
                   --exclude .*/test/.* \
                   --exclude .*/main.cpp.* \
                   --html-details \
                   --output line_branch_cov/index.html
  echo "[INFO ] Coverage reports sucessfully generated"
fi
done
  echo "[INFO ] Generating Function coverage report." 
  genhtml -o ./function_cov --branch-coverage  -t 'Function Coverage report' --legend --demangle-cpp --prefix "$(pwd)" "${fcPath}/source/core/build"/coverage-final.info "${fcPath}/source/core-types/build"/coverage-final.info "${fcPath}/source/utils/thread/build"/coverage-final.info
"${fcPath}/source/utils/config-reader/build"/coverage-final.info "${fcPath}/source/utils/timer/build"/coverage-final.info "${fcPath}/source/utils/cond-var/build"/coverage-final.info "${fcPath}/source/utils/ipc/build"/coverage-final.info \

cp -r ${fcPath}/source/core/build/*.xml ${fcPath}/build
cp -r ${fcPath}/source/core-types/build/*.xml ${fcPath}/build
cp -r ${fcPath}/source/utils/ipc/build/*.xml ${fcPath}/build
cp -r ${fcPath}/source/utils/config-reader/build/*.xml ${fcPath}/build
cp -r ${fcPath}/source/utils/thread/build/*.xml ${fcPath}/build
cp -r ${fcPath}/source/utils/timer/build/*.xml ${fcPath}/build
cp -r ${fcPath}/source/utils/cond-var/build/*.xml ${fcPath}/build