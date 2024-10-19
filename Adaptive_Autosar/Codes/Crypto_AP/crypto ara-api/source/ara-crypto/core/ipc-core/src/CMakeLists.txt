# ------------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved. 
# The reproduction, distribution and utilization of this document as well as the communication of its contents 
# to others without explicit authorization is prohibited. Offenders will be held liable for the payment of 
# damages.
# ------------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides cmake for compilation process of ipc-core source.
# ------------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE  IPC_CORE_SRC
vsomeip/gen/common/*.cpp

vsomeip/gen/cryp/cryobj/*.cpp

vsomeip/gen/cryp/cryservice/*.cpp

vsomeip/gen/cryp/*.cpp

vsomeip/gen/keys/*.cpp

vsomeip/gen/x509/*.cpp

# vsomeip/gen/ara_com_main-cryptoserverswc.cpp
     
vsomeip/gen/vsomeip_service_mapping_aracom_services.cpp
vsomeip/gen/vsomeip_service_mapping-cryptoserverexe.cpp

vsomeip/crypto-provider/cryobj/*.cpp
vsomeip/crypto-provider/cryservice/*.cpp
##vsomeip/common/iointerface_skeleton_impl.cpp

vsomeip/crypto-provider/crypto-container/crypto_context_container.cpp
vsomeip/crypto-provider/crypto-container/crypto_object_container.cpp
vsomeip/crypto-provider/crypto-container/crypto_service_container.cpp
vsomeip/crypto-provider/*.cpp
vsomeip/x509/*.cpp
vsomeip/x509/x509_data_storage/x509_data_container.cpp
#vsomeip/key-storage/key_storage_provider_skeleton_impl.cpp
#vsomeip/key-storage/key_slot_interface_skeleton_impl.cpp
)

add_library(ipc-core STATIC ${IPC_CORE_SRC})

target_link_libraries(
  ipc-core
  PUBLIC
  ara_com
  ara_vsomeipbinding
  crypto-config
  crypto-provider
  key-storage
  crypto-utils
  certificate-management
)

target_include_directories(
ipc-core
PRIVATE
    ../inc/private/
PUBLIC
     ${CMAKE_CURRENT_SOURCE_DIR}/../../../../autosar-crypto-files/
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/common/
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/crypto-provider/
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/crypto-provider/cryobj/
 ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/crypto-provider/cryobj-container/
 ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/crypto-provider/cryservice-container/
 ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/crypto-provider/cryp-context-container/
#    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/key-storage/
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/x509/
 ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/x509/x509_data_storage/

    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/common/error
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/common/iointerface
     
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/cryobj/cryptoobject
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/cryobj/symmetrickey
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/cryobj/secretseed
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/cryobj/signature
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/cryobj/privatekey
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/cryobj/pubilckey
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/cryobj/restricteduseobject

     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/authenticationcipher
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/cryptoprovider
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/hashfunction
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/keyderivationfunction
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/messageauthenticationcode
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/randomgenerator
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/streamcipher
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/symmetricblockcipher
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/symmetrickeywrapper

 ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/decryptorprivatecontext
 ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/encryptorpubliccontext
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/msgrecoverypubliccontext
 ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/signencodeprivatecontext
 ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/signerprivatecontext
 ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/verifierpubliccontext
 ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/keyagreementprivatecontext
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/keydecapsulatorprivatecontext
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/keyencapsulatorpubliccontext

    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/keys/keyslot
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/keys/keystorageprovider

${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/x509
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/x509/certificate
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/x509/csr
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/x509/ext
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/x509/ocspreq
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/x509/ocspres
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/x509/pki
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/x509/x509dn
	
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/common/error
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/common/iointerface

     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/cryobj/cryptoobject
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/cryobj/symmetrickey
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/cryobj/secretseed
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/cryobj/signature
     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/cryobj/privatekey
  ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/cryobj/pubilckey
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/cryobj/restricteduseobject

${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/authenticationcipher
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/cryptoprovider
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/hashfunction
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/keyderivationfunction
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/messageauthenticationcode
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/randomgenerator
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/streamcipher
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/symmetricblockcipher
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/symmetrickeywrapper
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/decryptorprivatecontext
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/encryptorpubliccontext
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/msgrecoverypubliccontext
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/signencodeprivatecontext
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/signerprivatecontext
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/verifierpubliccontext
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/keyagreementprivatecontext
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/keydecapsulatorprivatecontext
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/keyencapsulatorpubliccontext
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/keys/keyslot
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/keys/keystorageprovider
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/x509
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/x509/certificate
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/x509/csr
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/x509/ext
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/x509/ocspreq
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/x509/ocspres
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/x509/pki
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/x509/x509dn
${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/x509/x509dn

    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/cryservice/blockservice
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/cryservice/cryptoservice
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/cryservice/digestservice
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/cryservice/extensionservice
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/includes/ara/crypto/ipccom/cryp/cryservice/signatureservice

    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/cryservice/blockservice
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/cryservice/cryptoservice
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/cryservice/digestservice
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/cryservice/extensionservice
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/gen/vsomeip/ara/crypto/ipccom/cryp/cryservice/signatureservice

     ${CMAKE_CURRENT_SOURCE_DIR}/../inc/public/vsomeip/crypto-provider/cryservice/
)

