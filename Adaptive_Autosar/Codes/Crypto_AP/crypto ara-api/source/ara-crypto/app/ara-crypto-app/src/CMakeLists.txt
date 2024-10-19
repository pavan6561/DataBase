# ------------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved. 
# The reproduction, distribution and utilization of this document as well as the communication of its contents 
# to others without explicit authorization is prohibited. Offenders will be held liable for the payment of 
# damages.
# ------------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides cmake for building the ara-crypto-app package including with ara-crypto-app 
#               library.
# ------------------------------------------------------------------------------------------------------------

set(ARACRYPTO_APP ara-crypto-app)

set(CRYPTO_SRC
entry_point.cpp
)

file(GLOB_RECURSE ARA_CRYPTO_APP_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/src/vsomeip/gen/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/src/vsomeip/gen/common/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/src/vsomeip/gen/cryp/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/src/vsomeip/gen/cryp/cryobj/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/src/vsomeip/gen/cryp/cryservice/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/src/vsomeip/gen/keys/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/src/vsomeip/gen/ara/crypto/cryp/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/src/vsomeip/gen/ara/crypto/ipccom/x509/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/src/vsomeip/crypto-provider/cryobj/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/src/vsomeip/crypto-provider/cryservice/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/src/vsomeip/crypto-provider/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/src/vsomeip/x509/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../../../../common/src/algorithm_identification.cpp
)

add_library(${ARACRYPTO_APP} STATIC ${CRYPTO_SRC} ${ARA_CRYPTO_APP_SRC})

message(STATUS "  Project Name : ${PROJECT_NAME}")

target_include_directories(
${PROJECT_NAME}
PUBLIC
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/common/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/common/error/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/common/iointerface/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/cryobj/cryptoobject/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/cryobj/symmetrickey/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/cryobj/secretseed/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/cryobj/signature/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/cryobj/privatekey/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/cryobj/pubilckey/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/cryobj/restricteduseobject/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/authenticationcipher/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/cryptoprovider/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/hashfunction/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/keyderivationfunction/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/messageauthenticationcode/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/randomgenerator/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/streamcipher/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/symmetricblockcipher/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/symmetrickeywrapper/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/decryptorprivatecontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/encryptorpubliccontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/msgrecoverypubliccontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/signencodeprivatecontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/signerprivatecontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/verifierpubliccontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/keyagreementprivatecontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/keydecapsulatorprivatecontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/keyencapsulatorpubliccontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/keys/keyslot/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/keys/keystorageprovider/>

$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/x509/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/x509/certificate/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/x509/csr/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/x509/ext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/x509/ocspreq/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/x509/ocspres/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/x509/pki/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/x509/x509dn/>

$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/cryservice/blockservice/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/cryservice/cryptoservice/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/cryservice/digestservice/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/cryservice/extensionservice/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/ara/crypto/ipccom/cryp/cryservice/signatureservice/>	

$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/common/error/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/common/iointerface/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/cryobj/cryptoobject/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/cryobj/symmetrickey/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/cryobj/secretseed/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/cryobj/signature/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/cryobj/privatekey/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/cryobj/pubilckey/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/cryobj/restricteduseobject/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/authenticationcipher/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/cryptoprovider/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/hashfunction/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/keyderivationfunction/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/messageauthenticationcode/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/randomgenerator/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/streamcipher/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/symmetricblockcipher/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/symmetrickeywrapper/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/decryptorprivatecontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/encryptorpubliccontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/msgrecoverypubliccontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/signencodeprivatecontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/signerprivatecontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/verifierpubliccontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/keyagreementprivatecontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/keydecapsulatorprivatecontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/keyencapsulatorpubliccontext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/keys/keyslot/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/keys/keystorageprovider/>

$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/x509/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/x509/certificate/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/x509/csr/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/x509/ext/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/x509/ocspreq/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/x509/ocspres/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/x509/pki/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/x509/x509dn/>

$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/cryservice/blockservice/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/cryservice/cryptoservice/>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/cryservice/digestservice/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/cryservice/extensionservice/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/ara/crypto/ipccom/cryp/cryservice/signatureservice/>

#$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/common/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/crypto-provider/cryobj/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/crypto-provider/cryservice/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/crypto-provider/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/x509/>
#$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/key-storage/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../autosar/>
$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../../common/inc/public/>
$<INSTALL_INTERFACE:include>

PRIVATE
../../ipc-app/inc/vsomeip/private/
)

set_target_properties(ara-crypto-app
PROPERTIES
SOVERSION ${PROJECT_VERSION_MAJOR}
VERSION ${PROJECT_VERSION}
CXX_STANDARD 14
OUTPUT_NAME ${ARACRYPTO_APP}
)

if(PKG_CONFIG_FOUND)
  configure_file(${PROJECT_SOURCE_DIR}/files/pkgconfig/${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc" DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()

# Create *-config.cmake file
include(CMakePackageConfigHelpers)
configure_package_config_file(
  ${PROJECT_SOURCE_DIR}/files/cmake/${PROJECT_NAME}-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  NO_SET_AND_CHECK_MACRO
)

# Create *-config-version.cmake file
write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  COMPATIBILITY ExactVersion
)

# Install *-config.cmake and *-version.cmake files
install(
  FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake
  DESTINATION
    ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
  TARGETS ara-crypto-app
  EXPORT ${PROJECT_NAME}-targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install the imported target definitions
install(EXPORT ${PROJECT_NAME}-targets
  NAMESPACE apd::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
  COMPONENT dev
)

install(DIRECTORY
${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/includes/
${CMAKE_CURRENT_SOURCE_DIR}/../../ipc-app/inc/vsomeip/public/gen/vsomeip/
${CMAKE_CURRENT_SOURCE_DIR}/../../../autosar/
DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
COMPONENT dev
)


target_link_libraries(${ARACRYPTO_APP} ara_com ara_vsomeipbinding ara::log)

