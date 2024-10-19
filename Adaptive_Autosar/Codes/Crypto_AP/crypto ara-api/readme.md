# Crypto

## Introduction
<p>
The CryptoServiceDaemon offers applications and other Adaptive AUTOSAR Functional Cluster a standardized 
interface, which provides operations for cryptographic and related calculations. These operations include 
cryptographic operations, key management, and certificate handling. CryptoServiceDaemon manages the actual 
implementations of all operations, the configuration, and the brokering of operations from applications to 
implementations. The standardized interface is exposed by the CryptoAPI.
The CryptoServiceDaemon and its CryptoAPI supports both public-key and symmetric-key cryptography. It allows 
applications to use mechanisms such as authentication, encryption, and decryption for automotive services.
</p>

## Architecture Overview
<b>Dependencies on other FCs</b>
  * Execution Management
  * ara::log

### Component Diagram
NA

## Features
NA

## Installation Guide
Refer: [Installation Guide](install.md)

## SRS to Code Coverage(based on R19-11)
NA

## Folder Structure

This C++ code intends to show the *Diagnostic Management* demonstrator.
The folder structure is:

    .
    ├── doc
    │   └── code_report
    │       ├── codegen
    │       │   └── jprofiler
    │       │       └── jprofile_images
    │       └── module
    │           └── Static_Analysis_Results_CRYPTO_CPP_Code_V1.0.0
    ├── source
    │   ├── ara-crypto
    │   │   ├── app
    │   │   │   ├── ara-crypto-app
    │   │   │   │   ├── inc
    │   │   │   │   │   └── private
    │   │   │   │   └── src
    │   │   │   ├── files
    │   │   │   │   ├── cmake
    │   │   │   │   └── pkgconfig
    │   │   │   └── ipc-app
    │   │   │       ├── inc
    │   │   │       │   └── vsomeip
    │   │   │       │       ├── private
    │   │   │       │       └── public
    │   │   │       │           ├── crypto_provider
    │   │   │       │           └── gen
    │   │   │       │               ├── includes
    │   │   │       │               │   └── ara
    │   │   │       │               │       └── crypto
    │   │   │       │               │           ├── cryp
    │   │   │       │               │           │   ├── acc
    │   │   │       │               │           │   ├── hash
    │   │   │       │               │           │   ├── kdf
    │   │   │       │               │           │   ├── mac
    │   │   │       │               │           │   ├── rng
    │   │   │       │               │           │   ├── sbcc
    │   │   │       │               │           │   ├── scc
    │   │   │       │               │           │   └── skwc
    │   │   │       │               │           └── error
    │   │   │       │               └── vsomeip
    │   │   │       │                   └── ara
    │   │   │       │                       └── crypto
    │   │   │       │                           └── cryp
    │   │   │       │                               ├── acc
    │   │   │       │                               ├── hash
    │   │   │       │                               ├── kdf
    │   │   │       │                               ├── mac
    │   │   │       │                               ├── rng
    │   │   │       │                               ├── sbcc
    │   │   │       │                               ├── scc
    │   │   │       │                               └── skwc
    │   │   │       └── src
    │   │   │           └── vsomeip
    │   │   │               ├── crypto_provider
    │   │   │               └── gen
    │   │   │                   └── crypto_provider
    │   │   ├── autosar_crypto_header
    │   │   │   └── ara
    │   │   │       └── crypto
    │   │   │           ├── common
    │   │   │           └── cryp
    │   │   │               └── cryobj
    │   │   └── core
    │   │       ├── ara-crypto-core
    │   │       │   ├── inc
    │   │       │   │   └── public
    │   │       │   └── src
    │   │       └── ipc-core
    │   │           ├── inc
    │   │           │   └── public
    │   │           │       └── vsomeip
    │   │           │           └── gen
    │   │           │               ├── includes
    │   │           │               │   └── ara
    │   │           │               │       └── crypto
    │   │           │               │           ├── cryp
    │   │           │               │           │   ├── acc
    │   │           │               │           │   ├── hash
    │   │           │               │           │   ├── kdf
    │   │           │               │           │   ├── mac
    │   │           │               │           │   ├── rng
    │   │           │               │           │   ├── sbcc
    │   │           │               │           │   ├── scc
    │   │           │               │           │   └── skwc
    │   │           │               │           └── error
    │   │           │               └── vsomeip
    │   │           │                   └── ara
    │   │           │                       └── crypto
    │   │           │                           └── cryp
    │   │           │                               ├── acc
    │   │           │                               ├── hash
    │   │           │                               ├── kdf
    │   │           │                               ├── mac
    │   │           │                               ├── rng
    │   │           │                               ├── sbcc
    │   │           │                               ├── scc
    │   │           │                               └── skwc
    │   │           └── src
    │   │               └── vsomeip
    │   │                   └── gen
    │   ├── certificate-provider
    │   │   ├── inc
    │   │   │   └── private
    │   │   └── src
    │   ├── common
    │   │   ├── inc
    │   │   │   └── private
    │   │   └── src
    │   ├── config
    │   │   ├── inc
    │   │   │   └── private
    │   │   └── src
    │   ├── crypto-daemon
    │   │   ├── inc
    │   │   │   └── private
    │   │   └── src
    │   ├── crypto-provider
    │   │   ├── inc
    │   │   │   └── private
    │   │   │       ├── acc
    │   │   │       ├── common
    │   │   │       ├── cryobj
    │   │   │       ├── hash
    │   │   │       ├── kdf
    │   │   │       ├── mac
    │   │   │       ├── rng
    │   │   │       ├── sbcc
    │   │   │       ├── scc
    │   │   │       └── skwc
    │   │   └── src
    │   │       ├── acc
    │   │       ├── cryobj
    │   │       ├── hash
    │   │       ├── kdf
    │   │       ├── mac
    │   │       ├── rng
    │   │       ├── sbcc
    │   │       ├── scc
    │   │       └── skwc
    │   ├── key-storage
    │   │   ├── inc
    │   │   │   └── private
    │   │   └── src
    │   └── utils
    │       ├── inc
    │       │   └── private
    │       └── src
    └── tools
        └── codegen
            ├── lib
            │   └── codegen-crypto
            │       ├── features
            │       └── plugins
            └── source
                └── com.avinsystems.ap.crypto
                    ├── features
                    │   └── com.avinsystems.ap.crypto.gt.feature
                    ├── plugins
                    │   └── com.avinsystems.ap.crypto.gt
                    │       ├── bin
                    │       │   └── com
                    │       │       └── avinsystems
                    │       │           └── ap
                    │       │               └── crypto
                    │       │                   └── gt
                    │       │                       ├── check
                    │       │                       ├── collector
                    │       │                       ├── generator
                    │       │                       └── pojo
                    │       ├── META-INF
                    │       ├── resources
                    │       ├── src
                    │       │   └── com
                    │       │       └── avinsystems
                    │       │           └── ap
                    │       │               └── crypto
                    │       │                   └── gt
                    │       │                       ├── check
                    │       │                       ├── collector
                    │       │                       ├── generator
                    │       │                       └── pojo
                    │       └── xtend-gen
                    │           └── com
                    │               └── avinsystems
                    │                   └── ap
                    │                       └── crypto
                    │                           └── gt
                    │                               ├── check
                    │                               ├── collector
                    │                               ├── generator
                    │                               └── pojo
                    └── tools
                        ├── com.avinsystems.ap.crypto.launchers
                        │   ├── mvn_launchers
                        │   └── test_launchers
                        └── com.avinsystems.ap.crypto.targets
                            ├── external
                            └── oxygen



*  Folder `doc/code_report` contains the static analysis report, JProfiler and Valgrind memory leak Reports.
*  Folder `tools/codegen/lib/codegen-crypto` contains Crypto Code Generator product.
*  Folder `tools/codegen/source` contains Crypto Code Generator source.
*  Folder `source/ara-crypto` contains ara::crypto library files.
*  Folder `source/certificate-provider` contains Certificate provider files
*  Folder `source/common` contains common files which are used by all the crypto-modules.
*  Folder `source/config` contains configuration files.
*  Folder `source/crypto-daemon` contains crypto-daemon files.
*  Folder `source/crypto-provider` contains crypto-provider files.
*  Folder `source/key-storage` contains key-storage provider files.
*  Folder `source/utils` contains Crypto Utility files.

## PPMS3
Release Date: 27-August-2021<br>
Embedded Version: 1.0.0<br>
Generation Tool Version: 1.0.0<br>

### Implemented Features
    1. Cryptoprovider interface.
      1.1. Random Number Generator Context.
      1.2. Hash Function Context.

### Issues Resolved
    1. NA

### Known Limitations
    1. Code static analysis violations are partially fixed.
