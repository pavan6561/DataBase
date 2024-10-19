# -----------------------------------------------------------------------------------------------------------
# Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
# of this document as well as the communication of its contents to others without explicit authorization is
# prohibited. Offenders will be held liable for the payment of damages.
# -----------------------------------------------------------------------------------------------------------
# File Name   : CMakeLists.txt
#
# Purpose     : This file provides cmake for utils (Crypto Stack).
# -----------------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.10)

find_package(ara-core-types REQUIRED)

set(CRYPTO_PROVIDER_SRC
  ./openssl/src/crypto_provider_ssl.cpp
  ./openssl/src/auth_cipher_ctx_ssl_aes_gcm.cpp
  ./openssl/src/cryobj/secret_seed_impl.cpp
  ./openssl/src/cryobj/signature_impl.cpp
  ./openssl/src/cryobj/symmetric_key_impl.cpp
  ./openssl/src/cryobj/private_key_impl.cpp
  ./openssl/src/cryobj/public_key_impl.cpp
  ./openssl/src/hash_function_ctx_default_ssl_sha256.cpp
  ./openssl/src/key_derivation_function_ctx_ssl_default.cpp
  ./openssl/src/message_authentication_code_ctx_ssl.cpp
  ./openssl/src/random_generator_ctx_ssl_ctr_drbg.cpp
  ./openssl/src/symmetric_block_cipher_ctx_ssl_aes.cpp
  ./openssl/src/stream_cipher_ctx_ssl_aes_ctr.cpp
  ./openssl/src/symmetric_key_wrapper_ctx_ssl.cpp
  ./openssl/src/decryptor_private_ctx_ssl_rsa.cpp
  ./openssl/src/encryptor_public_ctx_ssl_rsa.cpp
  ./openssl/src/key_agreement_private_ctx_default_ssl.cpp
  ./openssl/src/key_decapsulator_private_ctx_default_ssl.cpp
  ./openssl/src/key_encapsulator_public_ctx_default_ssl.cpp
  ./openssl/src/msg_recovery_public_ctx_default_ssl.cpp
  ./openssl/src/sig_encode_private_ctx_default_ssl.cpp
  ./openssl/src/signer_private_ctx_default_ssl.cpp
  ./openssl/src/verifier_public_ctx_default_ssl.cpp
  ./openssl/src/extension_service_impl.cpp
  ./openssl/src/block_service_impl.cpp
  ./openssl/src/digest_service_impl.cpp
  ./openssl/src/crypto_service_impl.cpp
  ./openssl/src/signature_service_impl.cpp
  ./../config/src/crypto_config.cpp
  ./../common/src/algorithm_identification.cpp

)

add_library(crypto-provider STATIC ${CRYPTO_PROVIDER_SRC})

target_include_directories(crypto-provider
  PUBLIC
    openssl/inc/public/
    openssl/inc/public/cryobj/
	  ../common/inc/public

    ../autosar-crypto-files/
)

target_link_libraries(crypto-provider PUBLIC ssl)
target_link_libraries(crypto-provider PUBLIC ara::core-types)
target_link_libraries(crypto-provider PUBLIC crypto)
target_link_libraries(crypto-provider PUBLIC crypto-config)
target_link_libraries(crypto-provider PUBLIC crypto-utils)

