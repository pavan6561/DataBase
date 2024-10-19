// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_provider_ssl_impl.cpp
//
// Purpose     : Crypto service skeleton implementation to forward functional calls to crypto daemon.
// -----------------------------------------------------------------------------------------------------------

#include "crypto_provider_skeleton_impl.h"

#include "hash_function_container.h"
#include "key_derivation_function_container.h"
#include "private_key_container.h"
#include "secret_seed_container.h"
#include "signature_container.h"
#include "symmetrickey_container.h"
#include "signer_private_container.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
CryptoProviderSkeletonImpl::CryptoProviderSkeletonImpl(CryptoServiceInstanceId instance_id,
    std::shared_ptr<ara::crypto::config::CryptoConfig>& data,
    CryptoProvider::Uptr crypProviderPtr) noexcept(false) :
    ara::crypto::ipccom::cryp::cryptoprovider::skeleton::CryptoProviderInterfaceSkeleton(
        ara::com::InstanceIdentifier(instance_id)),
    config_data_{data}, cryptoProviderPtr_{std::move(crypProviderPtr)}, serviceInstanceId_{0}
{
  logger_.LogDebug() << "CryptoProviderSkeletonImpl constructor";
}

CryptoServiceInstanceId CryptoProviderSkeletonImpl::GenerateCryptoServiceInstanceId() noexcept
{
  // Generate new instance Id for crypto service.
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::GenerateCryptoServiceInstanceId()";
  std::lock_guard<std::mutex> const lock{mutex_instanceId_};
  serviceInstanceId_ = serviceInstanceId_ + 1;
  CryptoServiceInstanceId strInstance{"SOME/IP:"};
  CryptoServiceInstanceId const tempStr{std::to_string(static_cast<uint32_t>(serviceInstanceId_))};
  strInstance.append(tempStr);
  return strInstance;
}

ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateAuthCipherOutput>
    CryptoProviderSkeletonImpl::CreateAuthCipher(const CryptoAlgId& algId)
{
  logger_.LogDebug() << "IPC-CORE CreateAuthCipher()";
  // Create authentication cipher instance. If instance get created properly then return the instance ID
  // for the newly created instance else return the error code.
  ara::core::Future<CreateAuthCipherOutput>::PromiseType promise;
  ara::core::Result<AuthCipherCtx::Uptr> accUptr{cryptoProviderPtr_->CreateAuthCipherCtx(algId)};
  if (accUptr.HasValue())
  {
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateAuthCipherOutput acc;
    CryptoServiceInstanceId acclocalInstanceId{GenerateCryptoServiceInstanceId()};
    std::unique_ptr<AuthCipherContextSkeletonImpl> localacc{
        std::make_unique<AuthCipherContextSkeletonImpl>(acclocalInstanceId, std::move(accUptr).Value())};
    localacc->OfferService();
    accContainer_.emplace(acclocalInstanceId, std::move(localacc));
    acc.instanceId = acclocalInstanceId;
    promise.set_value(acc);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider CreateAuthCipherCtx() Error: "
                       << accUptr.Error().Message();
    if (accUptr.Error() == SecurityErrorDomain::Errc::kInvalidArgument)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateDecryptorPrivateOutput>
    CryptoProviderSkeletonImpl::CreateDecryptorPrivate(const CryptoAlgId& algId)
{
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::CreateDecryptorPrivate()";
  ara::core::Future<CreateDecryptorPrivateOutput>::PromiseType promise;
  ara::core::Result<DecryptorPrivateCtx::Uptr> decryPriUptr{
      cryptoProviderPtr_->CreateDecryptorPrivateCtx(algId)};
  if (decryPriUptr.HasValue())
  {
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateDecryptorPrivateOutput decrypt;
    CryptoServiceInstanceId decryptlocalInstanceId{GenerateCryptoServiceInstanceId()};
    std::unique_ptr<DecryptorPrivateContextSkeletonImpl> localDecrypt{
        std::make_unique<DecryptorPrivateContextSkeletonImpl>(
            decryptlocalInstanceId, std::move(decryPriUptr).Value())};
    localDecrypt->OfferService();
    decrypPriContainer_.emplace(decryptlocalInstanceId, std::move(localDecrypt));
    decrypt.instanceId = decryptlocalInstanceId;
    promise.set_value(decrypt);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider CreateDecryptorPrivate() Error: "
                       << decryPriUptr.Error().Message();
    if (decryPriUptr.Error() == SecurityErrorDomain::Errc::kInvalidArgument)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateEncryptorPublicOutput>
    CryptoProviderSkeletonImpl::CreateEncryptorPublic(const CryptoAlgId& algId)
{
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::CreateEncryptorPublic()";
  ara::core::Future<CreateEncryptorPublicOutput>::PromiseType promise;
  ara::core::Result<EncryptorPublicCtx::Uptr> encryPubUptr{
      cryptoProviderPtr_->CreateEncryptorPublicCtx(algId)};
  if (encryPubUptr.HasValue())
  {
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateEncryptorPublicOutput encrypt;
    CryptoServiceInstanceId encryptlocalInstanceId{GenerateCryptoServiceInstanceId()};
    std::unique_ptr<EncryptorPrivateContextSkeletonImpl> localEncrypt{
        std::make_unique<EncryptorPrivateContextSkeletonImpl>(
            encryptlocalInstanceId, std::move(encryPubUptr).Value())};
    localEncrypt->OfferService();
    encryPubContainer_.emplace(encryptlocalInstanceId, std::move(localEncrypt));
    encrypt.instanceId = encryptlocalInstanceId;
    promise.set_value(encrypt);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider CreateEncryptorPublic() Error: "
                       << encryPubUptr.Error().Message();
    if (encryPubUptr.Error() == SecurityErrorDomain::Errc::kInvalidArgument)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateHashFunctionOutput>
    CryptoProviderSkeletonImpl::CreateHashFunction(const CryptoAlgId& algId)
{
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::CreateHashFunction()";
  // Create Hash function ctx instance. If instance get created properly then return the instance ID
  // for the newly created instance else return the error code.
  ara::core::Future<CreateHashFunctionOutput>::PromiseType promise;
  ara::core::Result<HashFunctionCtx::Uptr> hashUptr{cryptoProviderPtr_->CreateHashFunctionCtx(algId)};
  if (hashUptr.HasValue())
  {
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateHashFunctionOutput hash;
    std::shared_ptr<ara::crypto::common::crypdatastore::HashFunctionContainer> hashStorageContainer{
        ara::crypto::common::crypdatastore::HashFunctionContainer::GetInstance()};
    // create unique crypto service instance Id from skeleton data storage class for crypto objects
    CryptoServiceInstanceId hashlocalInstanceId{hashStorageContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<HashFunctionSkeletonImpl> localhash{
        std::make_unique<HashFunctionSkeletonImpl>(hashlocalInstanceId, std::move(hashUptr).Value())};
    localhash->OfferService();
    hashStorageContainer->StoreObjectToContanier(hashlocalInstanceId, std::move(localhash));
    hash.instanceId = hashlocalInstanceId;
    promise.set_value(hash);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider CreateHashFunction() Error: "
                       << hashUptr.Error().Message();
    if (hashUptr.Error() == SecurityErrorDomain::Errc::kInvalidArgument)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateKeyAgreementPrivateOutput>
    CryptoProviderSkeletonImpl::CreateKeyAgreementPrivate(const CryptoAlgId& algId)
{
  // TODO.
  // This is a dummy implementation.
  // Need to integrate and implement the Key agreement functionality inside the daemon.
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::CreateKeyAgreementPrivate()";
  static_cast<void>(algId);
  ara::core::Future<CreateKeyAgreementPrivateOutput>::PromiseType promise;
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateKeyDecapsulatorPrivateOutput>
    CryptoProviderSkeletonImpl::CreateKeyDecapsulatorPrivate(const CryptoAlgId& algId)
{
  // TODO.
  // This is a dummy implementation.
  // Need to integrate and implement the Key decapsulation functionality inside the daemon.
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::CreateKeyDecapsulatorPrivate()";
  static_cast<void>(algId);
  ara::core::Future<CreateKeyDecapsulatorPrivateOutput>::PromiseType promise;
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateKeyDerivationFunctionOutput>
    CryptoProviderSkeletonImpl::CreateKeyDerivationFunction(const CryptoAlgId& algId)
{
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::CreateKeyDerivationFunction()";
  // Create Key Derivation function ctx instance. If instance get created properly then return the instance
  // ID for the newly created instance else return the error code.
  ara::core::Future<CreateKeyDerivationFunctionOutput>::PromiseType promise;
  ara::core::Result<KeyDerivationFunctionCtx::Uptr> kdfUptr{
      cryptoProviderPtr_->CreateKeyDerivationFunctionCtx(algId)};
  if (kdfUptr.HasValue())
  {
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateKeyDerivationFunctionOutput kdf;
    std::shared_ptr<ara::crypto::common::crypdatastore::KeyDerivationFunctionContainer> kdfStorageContainer{
        ara::crypto::common::crypdatastore::KeyDerivationFunctionContainer::GetInstance()};
    // create unique crypto service instance Id from skeleton data storage class for crypto objects
    CryptoServiceInstanceId kdflocalInstanceId{kdfStorageContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<KeyDerivationFunctionskeletonImpl> localKdf{
        std::make_unique<KeyDerivationFunctionskeletonImpl>(kdflocalInstanceId, std::move(kdfUptr).Value())};
    localKdf->OfferService();
    kdfStorageContainer->StoreObjectToContanier(kdflocalInstanceId, std::move(localKdf));
    kdf.instanceId = kdflocalInstanceId;
    promise.set_value(kdf);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider CreateKeyDerivationFunction() Error: "
                       << kdfUptr.Error().Message();
    if (kdfUptr.Error() == SecurityErrorDomain::Errc::kInvalidArgument)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateKeyEncapsulatorPublicOutput>
    CryptoProviderSkeletonImpl::CreateKeyEncapsulatorPublic(const CryptoAlgId& algId)
{
  // TODO.
  // This is a dummy implementation.
  // Need to integrate and implement the Key encapsulation functionality inside the daemon.
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::CreateKeyEncapsulatorPublic()";
  static_cast<void>(algId);
  ara::core::Future<CreateKeyEncapsulatorPublicOutput>::PromiseType promise;
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateMessageAuthCodeOutput>
    CryptoProviderSkeletonImpl::CreateMessageAuthCode(const CryptoAlgId& algId)
{
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::CreateMessageAuthCode()";
  // Create Message authentication code ctx instance. If instance get created properly then return the instance
  // ID for the newly created instance else return the error code.
  ara::core::Future<CreateMessageAuthCodeOutput>::PromiseType promise;
  ara::core::Result<MessageAuthnCodeCtx::Uptr> macUptr{cryptoProviderPtr_->CreateMessageAuthCodeCtx(algId)};
  if (macUptr.HasValue())
  {
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateMessageAuthCodeOutput mac;
    CryptoServiceInstanceId maclocalInstanceId{GenerateCryptoServiceInstanceId()};
    std::unique_ptr<MessageAuthnCodeSkeletonImpl> localMac{
        std::make_unique<MessageAuthnCodeSkeletonImpl>(maclocalInstanceId, std::move(macUptr).Value())};
    localMac->OfferService();
    macContainer_.emplace(maclocalInstanceId, std::move(localMac));
    mac.instanceId = maclocalInstanceId;
    promise.set_value(mac);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider CreateMessageAuthCode() Error: "
                       << macUptr.Error().Message();
    if (macUptr.Error() == SecurityErrorDomain::Errc::kInvalidArgument)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateMsgRecoveryPublicOutput>
    CryptoProviderSkeletonImpl::CreateMsgRecoveryPublic(const CryptoAlgId& algId)
{
  logger_.LogDebug() << "IPC-CORE:Crypto-Provider CryptoProviderSkeletonImpl::CreateMsgRecoveryPublic()";
  ara::core::Future<CreateMsgRecoveryPublicOutput>::PromiseType promise;
  ara::core::Result<MsgRecoveryPublicCtx::Uptr> msgUptr{
      cryptoProviderPtr_->CreateMsgRecoveryPublicCtx(algId)};
  if (msgUptr.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE:Crypto-Provider CreateMsgRecoveryPublic() -> Successfull";
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateMsgRecoveryPublicOutput msg;
    CryptoServiceInstanceId msglocalInstanceId{GenerateCryptoServiceInstanceId()};
    std::unique_ptr<MsgRecoveryPublicContextSkeletonImpl> localMsg{
        std::make_unique<MsgRecoveryPublicContextSkeletonImpl>(
            msglocalInstanceId, std::move(msgUptr).Value())};
    localMsg->OfferService();
    logger_.LogDebug() << "IPC-CORE:Crypto-Provider CreateMsgRecoveryPublic() -> OfferService()";
    msgPubContainer_.emplace(msglocalInstanceId, std::move(localMsg));
    msg.instanceId = msglocalInstanceId;
    promise.set_value(msg);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider CreateMsgRecoveryPublic() Error: "
                       << msgUptr.Error().Message();
    if (msgUptr.Error() == SecurityErrorDomain::Errc::kInvalidArgument)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateRandomGeneratorOutput>
    CryptoProviderSkeletonImpl::CreateRandomGenerator(const CryptoAlgId& algId, const Boolean& initialize)
{
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::CreateRandomGenerator()";
  // Create Random Number Generator ctx instance. If instance get created properly then return the instance
  // ID for the newly created instance else return the error code.
  ara::core::Future<CreateRandomGeneratorOutput>::PromiseType promise;
  ara::core::Result<RandomGeneratorCtx::Uptr> rngUptr{
      cryptoProviderPtr_->CreateRandomGeneratorCtx(algId, initialize)};
  if (rngUptr.HasValue())
  {
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateRandomGeneratorOutput rng;
    CryptoServiceInstanceId rnglocalInstanceId{GenerateCryptoServiceInstanceId()};
    std::unique_ptr<RandomGeneratorSkeletonImpl> localRng{
        std::make_unique<RandomGeneratorSkeletonImpl>(rnglocalInstanceId, std::move(rngUptr).Value())};
    localRng->OfferService();
    rngContainer_.emplace(rnglocalInstanceId, std::move(localRng));
    rng.instanceId = rnglocalInstanceId;
    promise.set_value(rng);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider CreateRandomGenerator() Error: "
                       << rngUptr.Error().Message();
    if (rngUptr.Error() == SecurityErrorDomain::Errc::kBusyResource)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kBusyResource);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateSigEncodePrivateOutput>
    CryptoProviderSkeletonImpl::CreateSigEncodePrivate(const CryptoAlgId& algId)
{
  logger_.LogDebug() << "IPC-CORE:Crypto-Provider CryptoProviderSkeletonImpl::CreateSigEncodePrivate()";
  ara::core::Future<CreateSigEncodePrivateOutput>::PromiseType promise;
  ara::core::Result<SigEncodePrivateCtx::Uptr> signUptr{cryptoProviderPtr_->CreateSigEncodePrivateCtx(algId)};
  if (signUptr.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE:Crypto-Provider CreateSigEncodePrivate() -> Successfull";
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateSigEncodePrivateOutput
        signEncode;
    CryptoServiceInstanceId signlocalInstanceId{GenerateCryptoServiceInstanceId()};
    std::unique_ptr<SignEncodePrivateContextSkeletonImpl> localSign{
        std::make_unique<SignEncodePrivateContextSkeletonImpl>(
            signlocalInstanceId, std::move(signUptr).Value())};
    localSign->OfferService();
    logger_.LogDebug() << "IPC-CORE:Crypto-Provider CreateSigEncodePrivate() -> OfferService()";
    signEnPubContainer_.emplace(signlocalInstanceId, std::move(localSign));
    signEncode.instanceId = signlocalInstanceId;
    promise.set_value(signEncode);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider CreateSigEncodePrivate() Error: "
                       << signUptr.Error().Message();
    if (signUptr.Error() == SecurityErrorDomain::Errc::kInvalidArgument)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateSignerPrivateOutput>
    CryptoProviderSkeletonImpl::CreateSignerPrivate(const CryptoAlgId& algId)
{
  logger_.LogDebug() << "IPC-CORE:Crypto-Provider CryptoProviderSkeletonImpl::CreateSignerPrivate()";
  ara::core::Future<CreateSignerPrivateOutput>::PromiseType promise;
  ara::core::Result<SignerPrivateCtx::Uptr> signUptr{cryptoProviderPtr_->CreateSignerPrivateCtx(algId)};
  if (signUptr.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE:Crypto-Provider CreateSignerPrivate() -> Successfull";
    CreateSignerPrivateOutput signerOutput;
    std::shared_ptr<ara::crypto::common::crypdatastore::SignerPrivateContainer> signerPrivateStorageContainer{
        ara::crypto::common::crypdatastore::SignerPrivateContainer::GetInstance()};
    // create unique crypto service instance Id from skeleton data storage class for crypto objects
    CryptoServiceInstanceId signerlocalInstanceId{
        signerPrivateStorageContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<SignerPrivateContextSkeletonImpl> localSignUptr{
        std::make_unique<SignerPrivateContextSkeletonImpl>(
            signerlocalInstanceId, std::move(signUptr).Value())};
    localSignUptr->OfferService();
    signerPrivateStorageContainer->StoreObjectToContanier(signerlocalInstanceId, std::move(localSignUptr));
    signerOutput.instanceId = signerlocalInstanceId;
    promise.set_value(signerOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider CreateSignerPrivate() Error: "
                       << signUptr.Error().Message();
    if (signUptr.Error() == SecurityErrorDomain::Errc::kInvalidArgument)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateStreamCipherOutput>
    CryptoProviderSkeletonImpl::CreateStreamCipher(const CryptoAlgId& algId)
{
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::CreateStreamCipher()";
  // Create Stream cipher ctx instance. If instance get created properly then return the instance
  // ID for the newly created instance else return the error code.
  ara::core::Future<CreateStreamCipherOutput>::PromiseType promise;
  ara::core::Result<StreamCipherCtx::Uptr> sccUptr{cryptoProviderPtr_->CreateStreamCipherCtx(algId)};
  if (sccUptr.HasValue())
  {
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateStreamCipherOutput scc;
    CryptoServiceInstanceId scclocalInstanceId{GenerateCryptoServiceInstanceId()};
    std::unique_ptr<StreamCipherContextSkeletonImpl> localScc{
        std::make_unique<StreamCipherContextSkeletonImpl>(scclocalInstanceId, std::move(sccUptr).Value())};
    localScc->OfferService();
    sccContainer_.emplace(scclocalInstanceId, std::move(localScc));
    scc.instanceId = scclocalInstanceId;
    promise.set_value(scc);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider CreateStreamCipher() Error: "
                       << sccUptr.Error().Message();
    if (sccUptr.Error() == SecurityErrorDomain::Errc::kInvalidArgument)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateSymmetricBlockCipherOutput>
    CryptoProviderSkeletonImpl::CreateSymmetricBlockCipher(const CryptoAlgId& algId)
{
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::CreateSymmetricBlockCipher()";
  // Create Block Cipher ctx instance. If instance get created properly then return the instance
  // ID for the newly created instance else return the error code.
  ara::core::Future<CreateSymmetricBlockCipherOutput>::PromiseType promise;
  ara::core::Result<SymmetricBlockCipherCtx::Uptr> sbccUptr{
      cryptoProviderPtr_->CreateSymmetricBlockCipherCtx(algId)};
  if (sbccUptr.HasValue())
  {
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateSymmetricBlockCipherOutput sbcc;
    CryptoServiceInstanceId sbcclocalInstanceId{GenerateCryptoServiceInstanceId()};
    std::unique_ptr<SymmetricBlockCipherContextSkeletonImpl> localSbcc{
        std::make_unique<SymmetricBlockCipherContextSkeletonImpl>(
            sbcclocalInstanceId, std::move(sbccUptr).Value())};
    localSbcc->OfferService();
    sbccContainer_.emplace(sbcclocalInstanceId, std::move(localSbcc));
    sbcc.instanceId = sbcclocalInstanceId;
    promise.set_value(sbcc);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider CreateSymmetricBlockCipherOutput() Error: "
                       << sbccUptr.Error().Message();
    promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateSymmetricKeyWrapperOutput>
    CryptoProviderSkeletonImpl::CreateSymmetricKeyWrapper(const CryptoAlgId& algId)
{
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::CreateSymmetricKeyWrapper()";
  // Create Key Wrapper ctx instance. If instance get created properly then return the instance
  // ID for the newly created instance else return the error code.
  ara::core::Future<CreateSymmetricKeyWrapperOutput>::PromiseType promise;
  ara::core::Result<SymmetricKeyWrapperCtx::Uptr> skwcUptr{
      cryptoProviderPtr_->CreateSymmetricKeyWrapperCtx(algId)};
  if (skwcUptr.HasValue())
  {
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateSymmetricKeyWrapperOutput skwc;
    CryptoServiceInstanceId skwclocalInstanceId{GenerateCryptoServiceInstanceId()};
    std::unique_ptr<SymmetricKeyWrapperCtxSkeletonImpl> localSkwc{
        std::make_unique<SymmetricKeyWrapperCtxSkeletonImpl>(
            skwclocalInstanceId, std::move(skwcUptr).Value())};
    localSkwc->OfferService();
    skwcContainer_.emplace(skwclocalInstanceId, std::move(localSkwc));
    skwc.instanceId = skwclocalInstanceId;
    promise.set_value(skwc);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider CreateSymmetricKeyWrapper() Error: "
                       << skwcUptr.Error().Message();
    if (skwcUptr.Error() == SecurityErrorDomain::Errc::kInvalidArgument)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
  }
  return promise.get_future();
}

ara::core::Future<
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateVerifierPublicOutput>
    CryptoProviderSkeletonImpl::CreateVerifierPublic(const CryptoAlgId& algId)
{
  logger_.LogDebug() << "IPC-CORE:Crypto-Provider CryptoProviderSkeletonImpl::CreateVerifierPublic()";
  ara::core::Future<CreateVerifierPublicOutput>::PromiseType promise;
  ara::core::Result<VerifierPublicCtx::Uptr> verifyUptr{cryptoProviderPtr_->CreateVerifierPublicCtx(algId)};
  if (verifyUptr.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE:Crypto-Provider CreateVerifierPublic() -> Successfull";
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateVerifierPublicOutput verifyPub;
    CryptoServiceInstanceId verifylocalInstanceId{GenerateCryptoServiceInstanceId()};
    std::unique_ptr<VerifierPublicContextSkeletonImpl> localVerify{
        std::make_unique<VerifierPublicContextSkeletonImpl>(
            verifylocalInstanceId, std::move(verifyUptr).Value())};
    localVerify->OfferService();
    logger_.LogDebug() << "IPC-CORE:Crypto-Provider CreateVerifierPublic() -> OfferService()";
    verifyPubContainer_.emplace(verifylocalInstanceId, std::move(localVerify));
    verifyPub.instanceId = verifylocalInstanceId;
    promise.set_value(verifyPub);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider CreateVerifierPublic() Error: "
                       << verifyUptr.Error().Message();
    if (verifyUptr.Error() == SecurityErrorDomain::Errc::kInvalidArgument)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::GenSymmetricKeyOutput>
    CryptoProviderSkeletonImpl::GenSymmetricKey(const CryptoAlgId& algId,
        const CryptoAllowedUsageFlags& allowedUsage, const Boolean& isSession, const Boolean& isExportable)
{
  logger_.LogDebug() << "IPC-CORE:Crypto-Provider CryptoProviderSkeletonImpl::GenSymmetricKey()";
  ara::core::Future<GenSymmetricKeyOutput>::PromiseType promise;
  ara::core::Result<SymmetricKey::Uptrc> symmetricKeyUptrc{
      cryptoProviderPtr_->GenerateSymmetricKey(algId, allowedUsage, isSession, isExportable)};
  if (symmetricKeyUptrc.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE:Crypto-Provider GenSymmetricKey() -> Successfull";
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::GenSymmetricKeyOutput
        symmetricKeyOutput;
    std::shared_ptr<ara::crypto::common::datastorage::SymmetricKeyContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SymmetricKeyContainer::GetInstance()};
    // create unique crypto service instance Id from skeleton data storage class for crypto objects
    CryptoServiceInstanceId symmetricKeylocalInstanceId{dataStorageContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl> localSymmetricUtr{
        std::make_unique<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>(
            symmetricKeylocalInstanceId, std::move(symmetricKeyUptrc).Value())};
    localSymmetricUtr->OfferService();
    logger_.LogDebug() << "IPC-CORE:Crypto-Provider GenSymmetricKey() -> OfferService()";
    dataStorageContainer->StoreObjectToContanier(symmetricKeylocalInstanceId, std::move(localSymmetricUtr));
    symmetricKeyOutput.instanceId = symmetricKeylocalInstanceId;
    promise.set_value(symmetricKeyOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider GenSymmetricKey() Error: "
                       << symmetricKeyUptrc.Error().Message();
    if (symmetricKeyUptrc.Error() == SecurityErrorDomain::Errc::kUnknownIdentifier)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleArguments);
    }
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::GenPrivateKeyOutput>
    CryptoProviderSkeletonImpl::GenPrivateKey(const CryptoAlgId& algId,
        const CryptoAllowedUsageFlags& allowedUsages, const Boolean& isSession, const Boolean& isExportable)
{
  logger_.LogDebug() << "IPC-CORE:Crypto-Provider CryptoProviderSkeletonImpl::GenPrivateKey()";
  ara::core::Future<GenPrivateKeyOutput>::PromiseType promise;
  ara::core::Result<PrivateKey::Uptrc> privateKeyUptrc{
      cryptoProviderPtr_->GeneratePrivateKey(algId, allowedUsages, isSession, isExportable)};
  if (privateKeyUptrc.HasValue())
  {
    logger_.LogDebug() << "IPC-CORE:Crypto-Provider GenPrivateKey() -> Successfull";
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::GenPrivateKeyOutput privateKeyOutput;
    std::shared_ptr<ara::crypto::common::datastorage::PrivateKeyContainer> privateContainer{
        ara::crypto::common::datastorage::PrivateKeyContainer::GetInstance()};
    // create unique crypto service instance Id from skeleton data storage class for crypto objects
    CryptoServiceInstanceId privateKeylocalInstanceId{privateContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryobj::PrivateKeySkeletonImpl> localPrivateKeyUtr{
        std::make_unique<ara::crypto::cryp::cryobj::PrivateKeySkeletonImpl>(
            privateKeylocalInstanceId, std::move(privateKeyUptrc).Value())};
    localPrivateKeyUtr->OfferService();
    logger_.LogDebug() << "IPC-CORE:Crypto-Provider GenPrivateKey() -> OfferService()";
    privateContainer->StoreObjectToContanier(privateKeylocalInstanceId, std::move(localPrivateKeyUtr));
    privateKeyOutput.instanceId = privateKeylocalInstanceId;
    promise.set_value(privateKeyOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider GenPrivateKey() Error: "
                       << privateKeyUptrc.Error().Message();
    if (privateKeyUptrc.Error() == SecurityErrorDomain::Errc::kUnknownIdentifier)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleArguments);
    }
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::GenSeedOutput>
    CryptoProviderSkeletonImpl::GenSeed(const CryptoAlgId& algId,
        const CryptoAllowedUsageFlags& secretSeedAllowedUsages, const Boolean& isSession,
        const Boolean& isExportable)
{
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::GenSeed()";
  ara::core::Future<GenSeedOutput>::PromiseType promise;
  ara::core::Result<SecretSeed::Uptrc> secretSeedUptrc{
      cryptoProviderPtr_->GenerateSeed(algId, secretSeedAllowedUsages, isSession, isExportable)};
  if (secretSeedUptrc.HasValue())
  {
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::GenSeedOutput secretSeedOutput;
    std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
    CryptoServiceInstanceId secretSeedlocalInstanceId{dataStorageContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl> localSecretSeedUtr{
        std::make_unique<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>(
            secretSeedlocalInstanceId, std::move(secretSeedUptrc).Value())};
    localSecretSeedUtr->OfferService();
    dataStorageContainer->StoreObjectToContanier(secretSeedlocalInstanceId, std::move(localSecretSeedUtr));
    secretSeedOutput.instanceId = secretSeedlocalInstanceId;
    promise.set_value(secretSeedOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider GenSeed() Error: " << secretSeedUptrc.Error().Message();
    if (secretSeedUptrc.Error() == SecurityErrorDomain::Errc::kUnknownIdentifier)
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kUnknownIdentifier);
    }
    else
    {
      promise.SetError(ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleArguments);
    }
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateHashDigestOutput>
    CryptoProviderSkeletonImpl::CreateHashDigest(
        const CryptoAlgId& hashAlgId, const CryptoReadOnlyMemRegion& readOnlyValue)
{
  static_cast<void>(hashAlgId);
  static_cast<void>(readOnlyValue);
  ara::core::Future<CreateHashDigestOutput>::PromiseType promise;
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateSignatureOutput>
    CryptoProviderSkeletonImpl::CreateSignature(const CryptoAlgId& signAlgId,
        const CryptoReadOnlyMemRegion& readOnlyValue,
        const RestrictedUseObjectStructure& restrictedUseObjStruct, const CryptoAlgId& hashAlgId)
{
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::CreateSignature()";
  ara::core::Future<CreateSignatureOutput>::PromiseType promise;
  ReadOnlyMemRegion const readValue{readOnlyValue};
  CryptoServiceInstanceId& restrictedInstanceIdLocal{
      const_cast<CryptoServiceInstanceId&>(restrictedUseObjStruct.RestrictedUseInstanceId)};
  ara::core::Result<Signature::Uptrc> signatureUptrc{nullptr};
  CryptoObjectTypeIPC const objType{restrictedUseObjStruct.RestrictedUseObjectType};
  if (objType == CryptoObjectTypeIPC::SecretSeed)
  {
    std::shared_ptr<ara::crypto::common::datastorage::SecretSeedContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SecretSeedContainer::GetInstance()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>& secretSeedSkeletonObj{
        dataStorageContainer->GetObjectFromContanier(restrictedInstanceIdLocal)};

    if (secretSeedSkeletonObj->IsConstUptrSet() == true)
    {
      logger_.LogDebug() << "IPC-CORE::CryptoProvider CreateSignature() -> GetSecretSeedConstUptrInstance()";
      auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedConstUptrInstance();
      signatureUptrc = cryptoProviderPtr_->CreateSignature(signAlgId, readValue, *secretSeedUptr, hashAlgId);
    }
    else
    {
      logger_.LogDebug() << "IPC-CORE::CryptoProvider CreateSignature() -> GetSecretSeedUptrInstance()";
      auto& secretSeedUptr = secretSeedSkeletonObj->GetSecretSeedUptrInstance();
      signatureUptrc = cryptoProviderPtr_->CreateSignature(signAlgId, readValue, *secretSeedUptr, hashAlgId);
    }
  }
  else if (objType == CryptoObjectTypeIPC::SymmetricKey)
  {
    std::shared_ptr<ara::crypto::common::datastorage::SymmetricKeyContainer> dataStorageContainer{
        ara::crypto::common::datastorage::SymmetricKeyContainer::GetInstance()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>& symmetricKeySkeletonObj{
        dataStorageContainer->GetObjectFromContanier(restrictedInstanceIdLocal)};
    auto& symmetricKeyUptr{symmetricKeySkeletonObj->GetSymmetricKeyUptrInstance()};
    signatureUptrc = cryptoProviderPtr_->CreateSignature(signAlgId, readValue, *symmetricKeyUptr, hashAlgId);
  }
  else if (objType == CryptoObjectTypeIPC::PrivateKey)
  {
    std::shared_ptr<ara::crypto::common::datastorage::PrivateKeyContainer> dataStorageContainer{
        ara::crypto::common::datastorage::PrivateKeyContainer::GetInstance()};
    std::unique_ptr<ara::crypto::cryp::cryobj::PrivateKeySkeletonImpl>& privateKeySkeleton{
        dataStorageContainer->GetObjectFromContanier(restrictedInstanceIdLocal)};
    auto& privateKeyUptr{privateKeySkeleton->GetPrivateKeyUptrInstance()};
    signatureUptrc = cryptoProviderPtr_->CreateSignature(signAlgId, readValue, *privateKeyUptr, hashAlgId);
  }
  else if (objType == CryptoObjectTypeIPC::PublicKey)
  {
    std::shared_ptr<ara::crypto::common::datastorage::PublicKeyContainer> dataStorageContainer{
        ara::crypto::common::datastorage::PublicKeyContainer::GetInstance()};
    std::unique_ptr<ara::crypto::cryp::cryobj::PublicKeySkeletonImpl>& publicKeySkeleton{
        dataStorageContainer->GetObjectFromContanier(restrictedInstanceIdLocal)};
    auto& publicKeyUptr{publicKeySkeleton->GetPublicKeyUptrInstance()};
    signatureUptrc = cryptoProviderPtr_->CreateSignature(signAlgId, readValue, *publicKeyUptr, hashAlgId);
  }

  if (signatureUptrc.HasValue())
  {
    ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::CreateSignatureOutput signatureOutput;
    std::shared_ptr<ara::crypto::common::datastorage::SignatureContainer> signContainer{
        ara::crypto::common::datastorage::SignatureContainer::GetInstance()};
    CryptoServiceInstanceId signatureLocalInstanceId{signContainer->GetCryptoServiceInstanceId()};
    std::unique_ptr<ara::crypto::cryp::cryobj::SignatureSkeletonImpl> localSignatureUtr{
        std::make_unique<ara::crypto::cryp::cryobj::SignatureSkeletonImpl>(
            signatureLocalInstanceId, std::move(signatureUptrc).Value())};
    localSignatureUtr->OfferService();
    signContainer->StoreObjectToContanier(signatureLocalInstanceId, std::move(localSignatureUtr));
    signatureOutput.signatureUptrcInstanceId = signatureLocalInstanceId;
    promise.set_value(signatureOutput);
  }
  else
  {
    logger_.LogError() << "IPC-CORE:Crypto-Provider CreateSignature() Error: "
                       << signatureUptrc.Error().Message();
    promise.SetError(
        static_cast<ipccom::common::error::CryptoErrorDomainErrc>(signatureUptrc.Error().Value()));
  }
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::LoadObjectOutput>
    CryptoProviderSkeletonImpl::LoadObject(const CryptoServiceInstanceId& IOInterfaceContainerInstanceId)
{
  ara::core::Future<LoadObjectOutput>::PromiseType promise;
  static_cast<void>(IOInterfaceContainerInstanceId);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::LoadPrivateKeyOutput>
    CryptoProviderSkeletonImpl::LoadPrivateKey(const CryptoServiceInstanceId& IOInterfaceContainerInstanceId)
{
  ara::core::Future<LoadPrivateKeyOutput>::PromiseType promise;
  static_cast<void>(IOInterfaceContainerInstanceId);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::LoadPublicKeyOutput>
    CryptoProviderSkeletonImpl::LoadPublicKey(const CryptoServiceInstanceId& IOInterfaceContainerInstanceId)
{
  ara::core::Future<LoadPublicKeyOutput>::PromiseType promise;
  static_cast<void>(IOInterfaceContainerInstanceId);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::LoadSecretSeedOutput>
    CryptoProviderSkeletonImpl::LoadSecretSeed(const CryptoServiceInstanceId& IOInterfaceContainerInstanceId)
{
  ara::core::Future<LoadSecretSeedOutput>::PromiseType promise;
  static_cast<void>(IOInterfaceContainerInstanceId);
  return promise.get_future();
}

ara::core::Future<ara::crypto::ipccom::cryp::cryptoprovider::CryptoProviderInterface::LoadSymmetricKeyOutput>
    CryptoProviderSkeletonImpl::LoadSymmetricKey(
        const CryptoServiceInstanceId& IOInterfaceContainerInstanceId)
{
  ara::core::Future<LoadSymmetricKeyOutput>::PromiseType promise;
  static_cast<void>(IOInterfaceContainerInstanceId);
  return promise.get_future();
}

ara::core::Future<void> CryptoProviderSkeletonImpl::StopCryptoProviderService(
    const CryptoProviderServiceType& cpServiceId, const CryptoServiceInstanceId& instanceId)
{
  logger_.LogDebug() << "In CryptoProviderSkeletonImpl::StopCryptoProviderService()";
  switch (cpServiceId)
  {
  case CryptoProviderServiceType::kRandomGeneratorCtx:
  {
    rngContainer_[instanceId]->StopOfferService();
    rngContainer_.erase(instanceId);
    logger_.LogInfo() << "Stop offerring service for RandomGeneratorCtx with service Instance ID: "
                      << instanceId;
    break;
  }
  case CryptoProviderServiceType::kHashFunctionCtx:
  {
    std::shared_ptr<ara::crypto::common::crypdatastore::HashFunctionContainer> hashStorageContainer{
        ara::crypto::common::crypdatastore::HashFunctionContainer::GetInstance()};
    std::unique_ptr<HashFunctionSkeletonImpl>& hashUptr{
        hashStorageContainer->GetObjectFromContanier(instanceId)};
    hashUptr->StopOfferService();
    logger_.LogInfo() << "Stopped offering service for HashFunctionCtx with service Instance ID: "
                      << instanceId;
    if (hashStorageContainer->DeleteInstanceFromContainer(instanceId) == true)
    {
      logger_.LogInfo() << "successfully deleted the HashFunction from container with InstanceID: "
                        << instanceId;
    }
    else
    {
      logger_.LogError() << "Failed to delete the HashFunction from container with InstanceID: "
                         << instanceId;
    }
    break;
  }
  case CryptoProviderServiceType::kMessageAuthCodeCtx:
  {
    macContainer_[instanceId]->StopOfferService();
    macContainer_.erase(instanceId);
    logger_.LogInfo() << "Stop offerring service for MessageAuthCode with service Instance ID: "
                      << instanceId;
    break;
  }
  case CryptoProviderServiceType::kKeyDerivationFunctionCtx:
  {
    std::shared_ptr<ara::crypto::common::crypdatastore::KeyDerivationFunctionContainer> kdfStorageContainer{
        ara::crypto::common::crypdatastore::KeyDerivationFunctionContainer::GetInstance()};
    std::unique_ptr<KeyDerivationFunctionskeletonImpl>& kdfUptr{
        kdfStorageContainer->GetObjectFromContanier(instanceId)};
    kdfUptr->StopOfferService();
    logger_.LogInfo() << "Stop offering service for KeyDerivationFunction with service Instance ID: "
                      << instanceId;
    if (kdfStorageContainer->DeleteInstanceFromContainer(instanceId) == true)
    {
      logger_.LogInfo() << "successfully deleted the KeyDerivationFunction from container with InstanceID: "
                        << instanceId;
    }
    else
    {
      logger_.LogError() << "Failed to delete the KeyDerivationFunction from container with InstanceID: "
                         << instanceId;
    }
    break;
  }
  case CryptoProviderServiceType::kAuthCipherCtx:
  {
    accContainer_[instanceId]->StopOfferService();
    accContainer_.erase(instanceId);
    logger_.LogInfo() << "Stop offerring service for AuthCipherCtx with service Instance ID: " << instanceId;
    break;
  }
  case CryptoProviderServiceType::kStreamCipherCtx:
  {
    sccContainer_[instanceId]->StopOfferService();
    sccContainer_.erase(instanceId);
    logger_.LogInfo() << "Stop offerring service for StreamCipherCtx with service Instance ID: "
                      << instanceId;
    break;
  }
  case CryptoProviderServiceType::kSymmetricBlockCipherCtx:
  {
    sbccContainer_[instanceId]->StopOfferService();
    sbccContainer_.erase(instanceId);
    logger_.LogInfo() << "Stop offerring service for SymmetricBlockCipherCtx with service Instance ID: "
                      << instanceId;
    break;
  }
  case CryptoProviderServiceType::kSymmetricKeyWrapperCtx:
  {
    skwcContainer_[instanceId]->StopOfferService();
    skwcContainer_.erase(instanceId);
    logger_.LogInfo() << "Stop offerring service for StreamCipherCtx with service Instance ID: "
                      << instanceId;
    break;
  }
  case CryptoProviderServiceType::kEncryptorPublicCtx:
  {
    encryPubContainer_[instanceId]->StopOfferService();
    encryPubContainer_.erase(instanceId);
    logger_.LogInfo() << "Stop offerring service for EncryptorPublicCtx with service Instance ID: "
                      << instanceId;
    break;
  }
  case CryptoProviderServiceType::kDecryptorPrivateCtx:
  {
    decrypPriContainer_[instanceId]->StopOfferService();
    decrypPriContainer_.erase(instanceId);
    logger_.LogInfo() << "Stop offerring service for DecryptorPrivateCtx with service Instance ID: "
                      << instanceId;
    break;
  }
  case CryptoProviderServiceType::kSigEncodePrivateCtx:
  {
    signEnPubContainer_[instanceId]->StopOfferService();
    signEnPubContainer_.erase(instanceId);
    logger_.LogInfo() << "Stop offerring service for SigEncodePrivateCtx with service Instance ID: "
                      << instanceId;
    break;
  }
  case CryptoProviderServiceType::kMsgRecoveryPublicCtx:
  {
    msgPubContainer_[instanceId]->StopOfferService();
    msgPubContainer_.erase(instanceId);
    logger_.LogInfo() << "Stop offerring service for MsgRecoveryPublicCtx with service Instance ID: "
                      << instanceId;
    break;
  }
  case CryptoProviderServiceType::kSignerPrivateCtx:
  {
    std::shared_ptr<ara::crypto::common::crypdatastore::SignerPrivateContainer> signerPrivateStorageContainer{
        ara::crypto::common::crypdatastore::SignerPrivateContainer::GetInstance()};
    std::unique_ptr<SignerPrivateContextSkeletonImpl>& signerUptr{
        signerPrivateStorageContainer->GetObjectFromContanier(instanceId)};
    signerUptr->StopOfferService();
    logger_.LogInfo() << "Stopped offering service for SignerPrivateCtx with service Instance ID: "
                      << instanceId;
    if (signerPrivateStorageContainer->DeleteInstanceFromContainer(instanceId) == true)
    {
      logger_.LogInfo() << "successfully deleted the SignerPrivateContainer from container with InstanceID: "
                        << instanceId;
    }
    else
    {
      logger_.LogError() << "Failed to delete the SignerPrivateContainer from container with InstanceID: "
                         << instanceId;
    }
    break;
  }
  case CryptoProviderServiceType::kVerifierPublicCtx:
  {
    verifyPubContainer_[instanceId]->StopOfferService();
    verifyPubContainer_.erase(instanceId);
    logger_.LogInfo() << "Stop offerring service for VerifierPublicCtx with service Instance ID: "
                      << instanceId;
    break;
  }
  default:
  {
    logger_.LogError() << "default case not supported";
    break;
  }
  }
  ara::core::Future<void>::PromiseType promise;
  promise.set_value();
  return promise.get_future();
}

} // namespace cryp
} // namespace crypto
} // namespace ara
