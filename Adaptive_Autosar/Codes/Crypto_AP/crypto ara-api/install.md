## Crypto

## Tool Installations

<b>Prerequisites:</b>

<h3> Installation of Configuration Tool</h3>

CT and GT guide.

```shell
# run as root!
sudo dpkg -i AstraSetup-x86-64-Vx-x-x.deb” in the terminal, where ASTRA debian packed is downloaded.
```

The package will be installed in “/usr/local/bin/Adaptive_GT".

<b> Note: Configuration Tool installation shall get completed with a success message.</b>

<h3> Installation of Generation Tool</h3>

```shell
1. Clone ara-api to local directory.
2. Run setup_codegen.sh as sudo from the path : LocalDirectory/ara-api/avin-ap/tools/setup.
```

<b>Note: GT installation shall get completed with a success message. Ensure executable shall have write permissions.</b>

## Dependencies

  * Execution Management
  * ara::per
  * ara::log

<h3> Yocto Build </h3>


1. If git-lfs is not already installed on system, run following commands:

```shell
$curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | sudo bash
$sudo apt-get install git-lfs
$git lfs install
```

2. Set up build environment

```shell
$sudo apt-get install gawk wget git-core diffstat unzip texinfo gcc-multilib build-essential chrpath socat libsdl1.2-dev xterm repo  curl tree iptables tree
```

3. Create a directory, eg: mkdir crypto-build
Inside crypto-build clone the ara-api, sample-app, yocto-layers repositories from dev branch into crypto-build : https://knode.avinsystems.com/bitbucket/projects/A00048

```shell
*crypto-build$ git clone https://knode.avinsystems.com/bitbucket/scm/a00048/ara-api.git
*crypto-build$ git clone https://knode.avinsystems.com/bitbucket/scm/a00048/qt-app.git
*crypto-build$ git clone https://knode.avinsystems.com/bitbucket/scm/a00048/yocto-layers.git
```

4. Clone the other supporting repositories by running the script file (fetch-dependencies.sh) at https://knode.avinsystems.com/bitbucket/scm/a00048/qt-app.git 

5. First create the initial default build directory.

```shell
*crypto-build$ source poky/oe-init-build-env
```

6. From there, add the layers meta-ara , meta-oe & meta-python.

```shell
*crypto-build/build$ bitbake-layers add-layer ../meta-openembedded/meta-oe
*crypto-build/build$ bitbake-layers add-layer ../meta-openembedded/meta-python
*crypto-build/build$ bitbake-layers add-layer ../meta-opendds
*crypto-build/build$ bitbake-layers add-layer ../yocto-layers/meta-ara
```

7. Finally, add a suitable location for Yocto <i>downloads</i> and <i>sstate-cache</i> folders to conf/local.conf

```shell
*crypto-build/build$ echo "MACHINE ??= \"qemux86\"" >> conf/local.conf
*crypto-build/build$ echo "DL_DIR = \"$HOME/yocto/downloads\"" >> conf/local.conf
*crypto-build/build$ echo "SSTATE_DIR = \"$HOME/yocto/sstate-cache\"" >> conf/local.conf
*crypto-build/build$ echo 'PREFERRED_VERSION_opendds="1.0+git%"' >> conf/local.conf
*crypto-build/build$ echo 'PREFERRED_VERSION_opendds-native="1.0+git%"' >> conf/local.conf
*crypto-build/build$ echo 'PREFERRED_VERSION_nativesdk-opendds="1.0+git%"' >> conf/local.conf
INHERIT = "own-mirrors"
SOURCE_MIRROR_URL = "http://192.168.1.17:8081/artifactory/yocto-mirror/downloads"

PREFERRED_VERSION_openssl = "3.0.0-beta1"

ASTRA_TOOL_PATH="/usr/local/bin/Adaptive_GT/AstraGTHeadless"
```

8. Command for building the Crypto image

```shell
*crypto-build/build$ bitbake core-image-apd-qt-crypto
```

9. Command for launching image

```shell
*crypto-build/build$ runqemu qemux86 core-image-apd-qt-crypto nographic
```

10. Run Execution Manager on qemu instance.

## QNX Build

NA

## Execution of AVIN Test Framework

NA

## FAQ

NA