// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : crypto_provider_proxy_impl.cpp
//
// Purpose     : Crypto provider proxy implementation to forward functional calls from application to
//               ipc-core.
// -----------------------------------------------------------------------------------------------------------

#include "crypto_provider_proxy_impl.h"

#include "algorithm_identification.h"
#include "impl_type_cryptoalgid.h"
#include "impl_type_cryptoserviceinstanceid.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
/// @brief namespace for crypto.
namespace crypto
{
/// @brief namespace for crypto primitives.
namespace cryp
{
CryptoProviderproxyImpl::CryptoProviderproxyImpl(const CryptoServiceInstanceId serviceInstanceID) noexcept :
    logger_{ara::log::CreateLogger("CPPI", "Crypto Provider Proxy", ara::log::LogLevel::kVerbose)}
{
  logger_.LogDebug() << "CryptoProviderproxyImpl Constructor";
  logger_.LogDebug() << "Initialize server client connection for Crypto Provider";
  Initialize(serviceInstanceID);
}

void CryptoProviderproxyImpl::Initialize(const CryptoServiceInstanceId serviceInstanceID)
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;
  if (vsomeipInstance_.CreateInstance(serviceInstanceID) == true)
  {
    logger_.LogDebug() << "Server Client connection for Crypto Provider : Successful";
    cryptoProviderHandler_ = vsomeipInstance_.GetHandlerInstance();
  }
  else
  {
    logger_.LogError() << "Server Client connection for Crypto Provider : Failed";
  }
}

ara::core::Result<RandomGeneratorCtx::Uptr> CryptoProviderproxyImpl::CreateRandomGeneratorCtx(
    AlgId algId, bool initialize) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;

  RandomGeneratorCtx::Uptr rngUptr{nullptr};
  if ((algId == algorithmids::kDrbgCtrBasedRng) || (algId == kAlgIdDefault))
  {
    auto rngResult = cryptoProviderHandler_->CreateRandomGenerator(algId, initialize).GetResult();
    if (rngResult.HasValue())
    {
      CryptoServiceInstanceId instanceId = rngResult.Value().instanceId;
      logger_.LogDebug() << "IPC-APP Service InstanceId for CreateRandomGeneratorCtx : " << instanceId;
      rngUptr = std::make_unique<RandomGeneratorCtxProxyImpl>(instanceId, cryptoProviderHandler_, *this);
    }
    else
    {
      logger_.LogError() << "IPC-APP Recieved error: kBusyResource";
      return ara::core::Result<RandomGeneratorCtx::Uptr>::FromError(SecurityErrorDomain::Errc::kBusyResource);
    }
    return ara::core::Result<RandomGeneratorCtx::Uptr>(std::move(rngUptr));
  }
  else
  {
    // According to SWS_CRYPT_20741 if algorithm ID is not supported by crypto
    // provider the CreateRandomGeneratorCtx should return SecurityErrorDomain::
    // kUnknownIdentifier
    logger_.LogError() << "IPC-APP Algorithm Id is not supported by Crypto provider";
    return ara::core::Result<RandomGeneratorCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }
}

ara::core::Result<Signature::Uptrc> CryptoProviderproxyImpl::CreateSignature(
    AlgId signAlgId, ReadOnlyMemRegion value, const RestrictedUseObject& key, AlgId hashAlgId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(signAlgId) == false)
  {
    // if given algorithm ID is not supported by
    // crypto provider then CreateSignature should return
    // SecurityErrorDomain::kUnknownIdentifier.
    logger_.LogError() << "IPC-APP Algorithm Id is not supported by Crypto provider";
    return ara::core::Result<Signature::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  CryptoServiceInstanceId serviceInstanceId;
  CryptoObjectType cryptoObjType = key.cryObjType;
  logger_.LogDebug() << "IPC-APP::CryptoProvider  cryptoObjectType =" << static_cast<uint32_t>(cryptoObjType);
  if (cryptoObjType == CryptoObjectType::kSecretSeed)
  {
    const ara::crypto::cryp::cryobj::SecretSeedProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::SecretSeedProxyImpl*>(&key);
    serviceInstanceId = obj->GetSecretSeedServiceInstanceId();
  }
  else if (cryptoObjType == CryptoObjectType::kSymmetricKey)
  {
    const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl*>(&key);
    serviceInstanceId = obj->GetSymmetricKeyServiceInstanceId();
  }
  else if (cryptoObjType == CryptoObjectType::kPublicKey)
  {
    const ara::crypto::cryp::cryobj::PublicKeyProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::PublicKeyProxyImpl*>(&key);
    serviceInstanceId = obj->GetPublicKeyServiceInstanceId();
  }
  else if (cryptoObjType == CryptoObjectType::kPrivateKey)
  {
    const ara::crypto::cryp::cryobj::PrivateKeyProxyImpl* obj
        = dynamic_cast<const ara::crypto::cryp::cryobj::PrivateKeyProxyImpl*>(&key);
    serviceInstanceId = obj->GetPrivateKeyServiceInstanceId();
  }

  RestrictedUseObjectStructure restrictedUseObj;
  restrictedUseObj.RestrictedUseInstanceId = serviceInstanceId;
  restrictedUseObj.RestrictedUseObjectType = static_cast<CryptoObjectTypeIPC>(cryptoObjType);
  logger_.LogDebug() << "IPC-APP::CreateSignature RestrictedUseObject serviceInstanceId = "
                     << serviceInstanceId;

  Signature::Uptrc signatureUptr{nullptr};
  CryptoReadOnlyMemRegion vtrUint8t(value.begin(), value.end());
  auto signatureResult
      = cryptoProviderHandler_->CreateSignature(signAlgId, vtrUint8t, restrictedUseObj, hashAlgId)
            .GetResult();
  if (signatureResult.HasValue())
  {
    CryptoServiceInstanceId instanceId = signatureResult.Value().signatureUptrcInstanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for CreateSignature : " << instanceId;
    signatureUptr = std::make_unique<ara::crypto::cryp::cryobj::SignatureProxyImpl>(instanceId);
    return ara::core::Result<Signature::Uptrc>(std::move(signatureUptr));
  }
  else
  {
    logger_.LogError() << "IPC-APP CreateSignature() Error: " << signatureResult.Error().Message();
    if (signatureResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kInvalidArgument)
    {
      return ara::core::Result<Signature::Uptrc>::FromError(SecurityErrorDomain::Errc::kInvalidArgument);
    }
    else if (signatureResult.Error()
        == ara::crypto::ipccom::common::error::CryptoErrorDomainErrc::kIncompatibleArguments)
    {
      return ara::core::Result<Signature::Uptrc>::FromError(
          SecurityErrorDomain::Errc::kIncompatibleArguments);
    }
    else
    {
      return ara::core::Result<Signature::Uptrc>::FromError(SecurityErrorDomain::Errc::kInvalidInputSize);
    }
  }
}

ara::core::Result<SymmetricKey::Uptrc> CryptoProviderproxyImpl::GenerateSymmetricKey(
    AlgId algId, AllowedUsageFlags allowedUsage, bool isSession, bool isExportable) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    // if given algorithm ID is not supported by
    // crypto provider then GenerateSymmetricKey should return
    // SecurityErrorDomain::kUnknownIdentifier.
    logger_.LogError() << "IPC-APP Algorithm Id is not supported by Crypto provider";
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  SymmetricKey::Uptrc symmetricKeyUptrc{nullptr};
  auto symmetricKeyResult
      = cryptoProviderHandler_->GenSymmetricKey(algId, allowedUsage, isSession, isExportable).GetResult();
  if (symmetricKeyResult.HasValue())
  {
    CryptoServiceInstanceId instanceId = symmetricKeyResult.Value().instanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for GenerateSymmetricKey : " << instanceId;
    symmetricKeyUptrc = std::make_unique<ara::crypto::cryp::cryobj::SymmetricKeyProxyImpl>(instanceId);
    return ara::core::Result<SymmetricKey::Uptrc>(std::move(symmetricKeyUptrc));
  }
  else
  {
    logger_.LogError() << "IPC-APP GenerateSymmetricKey() Error: " << symmetricKeyResult.Error().Message();
    return ara::core::Result<SymmetricKey::Uptrc>::FromError(
        SecurityErrorDomain::Errc::kIncompatibleArguments);
  }
}

ara::core::Result<SecretSeed::Uptrc> CryptoProviderproxyImpl::GenerateSeed(
    AlgId algId, SecretSeed::Usage allowedUsage, bool isSession, bool isExportable) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    // if given algorithm ID is not supported by
    // crypto provider then GenerateSeed should return
    // SecurityErrorDomain::kUnknownIdentifier.
    logger_.LogError() << "IPC-APP Algorithm Id is not supported by Crypto provider";
    return ara::core::Result<SecretSeed::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  SecretSeed::Uptrc secretSeedUptrc{nullptr};
  auto secretSeedResult
      = cryptoProviderHandler_->GenSeed(algId, allowedUsage, isSession, isExportable).GetResult();
  if (secretSeedResult.HasValue())
  {
    CryptoServiceInstanceId instanceId = secretSeedResult.Value().instanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for GenerateSeed : " << instanceId;
    secretSeedUptrc = std::make_unique<ara::crypto::cryp::cryobj::SecretSeedProxyImpl>(instanceId);
    return ara::core::Result<SecretSeed::Uptrc>(std::move(secretSeedUptrc));
  }
  else
  {
    logger_.LogError() << "IPC-APP GenerateSeed() Error: " << secretSeedResult.Error().Message();
    return ara::core::Result<SecretSeed::Uptrc>::FromError(SecurityErrorDomain::Errc::kIncompatibleArguments);
  }
}

ara::core::Result<PrivateKey::Uptrc> CryptoProviderproxyImpl::GeneratePrivateKey(
    AlgId algId, AllowedUsageFlags allowedUsage, bool isSession, bool isExportable) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    // if given algorithm ID is not supported by
    // crypto provider then GeneratePrivateKey should return
    // SecurityErrorDomain::kUnknownIdentifier.
    logger_.LogError() << "IPC-APP Algorithm Id is not supported by Crypto provider";
    return ara::core::Result<PrivateKey::Uptrc>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  PrivateKey::Uptrc privateKeyUptrc{nullptr};
  auto privateKeyResult
      = cryptoProviderHandler_->GenPrivateKey(algId, allowedUsage, isSession, isExportable).GetResult();
  if (privateKeyResult.HasValue())
  {
    CryptoServiceInstanceId instanceId = privateKeyResult.Value().instanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for GeneratePrivteKey : " << instanceId;
    privateKeyUptrc = std::make_unique<ara::crypto::cryp::cryobj::PrivateKeyProxyImpl>(instanceId);
    return ara::core::Result<PrivateKey::Uptrc>(std::move(privateKeyUptrc));
  }
  else
  {
    logger_.LogError() << "IPC-APP GeneratePrivteKey() Error: " << privateKeyResult.Error().Message();
    return ara::core::Result<PrivateKey::Uptrc>::FromError(SecurityErrorDomain::Errc::kIncompatibleArguments);
  }
}

ara::core::Result<SymmetricBlockCipherCtx::Uptr> CryptoProviderproxyImpl::CreateSymmetricBlockCipherCtx(
    AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl:: " << __FUNCTION__;
  SymmetricBlockCipherCtx::Uptr sbccUptr;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    // According to SWS_CRYPT_20742 if given algorithm ID is not supported by
    // crypto provider then CreateSymmetricBlockCipherCtx should return
    // SecurityErrorDomain::kUnknownIdentifier.
    logger_.LogError() << "IPC-APP Algorithm Id is not supported by Crypto provider";
    return ara::core::Result<SymmetricBlockCipherCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if ((algId != algorithmids::kAES128CBC) && (algId != algorithmids::kAES192CBC)
      && (algId != algorithmids::kAES256CBC) && (algId != algorithmids::kAES128ECB)
      && (algId != algorithmids::kAES192ECB) && (algId != algorithmids::kAES256ECB))
  {
    // According to SWS_CRYPT_20742 if given algorithm ID is not supported by
    // block cipher then CreateSymmetricBlockCipherCtx should return
    // SecurityErrorDomain::kUnknownIdentifier.
    logger_.LogError() << "IPC-APP Algorithm Id is not supported by Block Cipher";
    return ara::core::Result<SymmetricBlockCipherCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  auto sbccResult = cryptoProviderHandler_->CreateSymmetricBlockCipher(algId).GetResult();
  CryptoServiceInstanceId instanceId = sbccResult.Value().instanceId;
  logger_.LogDebug() << "IPC-APP Service InstanceId for CreateSymmetricBlockCipherCtx : " << instanceId;
  sbccUptr = std::make_unique<SymmetricBlockCipherCtxProxyImpl>(instanceId, cryptoProviderHandler_, *this);
  return ara::core::Result<SymmetricBlockCipherCtx::Uptr>(std::move(sbccUptr));
}

ara::core::Result<SymmetricKeyWrapperCtx::Uptr> CryptoProviderproxyImpl::CreateSymmetricKeyWrapperCtx(
    AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;
  SymmetricKeyWrapperCtx::Uptr skwcUptr;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    // According to SWS_CRYPT_20743 if given algorithm ID is not supported by
    // crypto provider then CreateSymmetricKeyWrapperCtx should return SecurityErrorDomain::
    // kUnknownIdentifier.
    logger_.LogError() << "IPC-APP Algorithm Id is of unknown Identifier";
    return ara::core::Result<SymmetricKeyWrapperCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if ((algId == algorithmids::kKeyWrap128) || (algId == algorithmids::kKeyWrap192)
      || (algId == algorithmids::kKeyWrap256) || (algId == algorithmids::kKeyWrapPad128)
      || (algId == algorithmids::kKeyWrapPad192) || (algId == algorithmids::kKeyWrapPad256))
  {
    auto skwcResult = cryptoProviderHandler_->CreateSymmetricKeyWrapper(algId).GetResult();
    CryptoServiceInstanceId instanceId = skwcResult.Value().instanceId;
    logger_.LogDebug() << "Service InstanceId for CreateSymmetricKeyWrapperCtx : " << instanceId;
    skwcUptr = std::make_unique<SymmetrickeyWrapperCtxProxyImpl>(instanceId, cryptoProviderHandler_, *this);
    return ara::core::Result<SymmetricKeyWrapperCtx::Uptr>(std::move(skwcUptr));
  }
  else
  {
    // According to SWS_CRYPT_20743 if given algorithm ID is not supported by
    // MAC ctx then CreateSymmetricKeyWrapperCtx should return SecurityErrorDomain::
    // kInvalidArgument.
    logger_.LogError() << "IPC::APP Algorithm Id is not supported by Crypto provider";
    return ara::core::Result<SymmetricKeyWrapperCtx::Uptr>::FromError(
        SecurityErrorDomain::Errc::kInvalidArgument);
  }
}

ara::core::Result<StreamCipherCtx::Uptr> CryptoProviderproxyImpl::CreateStreamCipherCtx(AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;
  StreamCipherCtx::Uptr sccUptr;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    // According to SWS_CRYPT_20744 if given algorithm ID is not supported by
    // crypto provider then CreateStreamCipherCtx should return SecurityErrorDomain::
    // kUnknownIdentifier.
    logger_.LogError() << "IPC-APP Algorithm Id is of unknown Identifier";
    return ara::core::Result<StreamCipherCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if (IsStreamCipherAlgorithmSupported(algId) == false)
  {
    // According to SWS_CRYPT_20744 if given algorithm ID is not supported by
    // stream cipher ctx then CreateStreamCipherCtx should return SecurityErrorDomain::
    // kInvalidArgument.
    logger_.LogError() << "IPC-APP Given algorithm Id does not belongs to stream cipher ctx";
    return ara::core::Result<StreamCipherCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
  }

  auto sccResult = cryptoProviderHandler_->CreateStreamCipher(algId).GetResult();
  CryptoServiceInstanceId instanceId = sccResult.Value().instanceId;
  logger_.LogDebug() << "IPC-APP Service InstanceId for CreateStreamCipherCtx : " << instanceId;
  sccUptr = std::make_unique<StreamCipherContextProxyImpl>(instanceId, cryptoProviderHandler_, *this);
  return ara::core::Result<StreamCipherCtx::Uptr>(std::move(sccUptr));
}

ara::core::Result<AuthCipherCtx::Uptr> CryptoProviderproxyImpl::CreateAuthCipherCtx(AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;
  AuthCipherCtx::Uptr accUptr;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    // According to SWS_CRYPT_20745 if given algorithm ID is not supported by
    // crypto provider then CreateAuthCipherCtx should return SecurityErrorDomain::
    // kUnknownIdentifier.
    logger_.LogError() << "IPC-APP Algorithm Id is of unknown Identifier";
    return ara::core::Result<AuthCipherCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if (IsAuthenticationCipherAlgorithmSupported(algId) == false)
  {
    // According to SWS_CRYPT_20745 if given algorithm ID is not supported by
    // authentication cipher ctx then CreateAuthCipherCtx should return SecurityErrorDomain::
    // kInvalidArgument.
    logger_.LogError() << "IPC-APP Given algorithm Id does not belongs to Authentication cipher";
    return ara::core::Result<AuthCipherCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
  }

  auto accResult = cryptoProviderHandler_->CreateAuthCipher(algId).GetResult();

  CryptoServiceInstanceId instanceId = accResult.Value().instanceId;
  logger_.LogDebug() << "IPC-APP Service InstanceId for CreateAuthCipherCtx : " << instanceId;
  accUptr = std::make_unique<AuthCipherContextProxyImpl>(instanceId, cryptoProviderHandler_, *this);
  return ara::core::Result<AuthCipherCtx::Uptr>(std::move(accUptr));
}

ara::core::Result<MessageAuthnCodeCtx::Uptr> CryptoProviderproxyImpl::CreateMessageAuthCodeCtx(
    AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;
  MessageAuthnCodeCtx::Uptr macUptr;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    // According to SWS_CRYPT_20746 if given algorithm ID is not supported by
    // crypto provider then CreateMessageAuthCodeCtx should return SecurityErrorDomain::
    // kUnknownIdentifier.
    logger_.LogError() << "IPC-APP Algorithm Id is of unknown Identifier";
    return ara::core::Result<MessageAuthnCodeCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if ((algId == algorithmids::kHMAC) || (algId == algorithmids::kCMAC128) || (algId == algorithmids::kCMAC192)
      || (algId == algorithmids::kCMAC256) || (algId == algorithmids::kGMAC128)
      || (algId == algorithmids::kGMAC192) || (algId == algorithmids::kGMAC256))
  {
    auto macResult = cryptoProviderHandler_->CreateMessageAuthCode(algId).GetResult();
    CryptoServiceInstanceId instanceId = macResult.Value().instanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for CreateMessageAuthCode : " << instanceId;
    macUptr = std::make_unique<MessageAuthnCodeCtxProxyImpl>(instanceId, cryptoProviderHandler_, *this);
    return ara::core::Result<MessageAuthnCodeCtx::Uptr>(std::move(macUptr));
  }
  else
  {
    // According to SWS_CRYPT_20746 if given algorithm ID is not supported by
    // MAC ctx then CreateMessageAuthCodeCtx should return SecurityErrorDomain::
    // kInvalidArgument.
    logger_.LogError() << "IPC-APP Given algorithm Id does not belongs to MessageAuthnCodeCtx";
    return ara::core::Result<MessageAuthnCodeCtx::Uptr>::FromError(
        SecurityErrorDomain::Errc::kInvalidArgument);
  }
}

ara::core::Result<HashFunctionCtx::Uptr> CryptoProviderproxyImpl::CreateHashFunctionCtx(AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;
  HashFunctionCtx::Uptr hashUptr;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    // According to SWS_CRYPT_20747 if given algorithm ID is not supported by
    // crypto provider then CreateHashFunctionCtx should return SecurityErrorDomain::
    // kUnknownIdentifier.
    logger_.LogError() << "IPC::APP Algorithm Id is not supported by Crypto provider";
    return ara::core::Result<HashFunctionCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if (algId == algorithmids::ksha256Hash)
  {
    auto hashResult = cryptoProviderHandler_->CreateHashFunction(algId).GetResult();
    CryptoServiceInstanceId instanceId = hashResult.Value().instanceId;
    logger_.LogDebug() << "Service InstanceId for CreateHashFunctionCtx : " << instanceId;
    hashUptr = std::make_unique<HashFunctionCtxProxyImpl>(instanceId, cryptoProviderHandler_, *this);
    return ara::core::Result<HashFunctionCtx::Uptr>(std::move(hashUptr));
  }
  else
  {
    // According to SWS_CRYPT_20747 if given algorithm ID does not belong to Hash
    // function ctx then CreateHashFunctionCtx should return SecurityErrorDomain::
    // kInvalidArgument.
    logger_.LogError() << "IPC::APP Provided algorithm ID does not belongs to Hash function ctx";
    return ara::core::Result<HashFunctionCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
  }
}

ara::core::Result<KeyDerivationFunctionCtx::Uptr> CryptoProviderproxyImpl::CreateKeyDerivationFunctionCtx(
    AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;

  KeyDerivationFunctionCtx::Uptr kdfUptr;

  // To Do: Need to move this check in ara::crypto library.
  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    // According to SWS_CRYPT_20748 if given algorithm ID is not supported by
    // crypto provider then CreateKeyDerivationFunctionCtx should return SecurityErrorDomain::
    // kUnknownIdentifier.
    logger_.LogDebug() << "IPC-APP Algorithm Id is not supported by Crypto provider";
    return ara::core::Result<KeyDerivationFunctionCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if (algId == algorithmids::kPBKDF2)
  {
    auto kdfResult = cryptoProviderHandler_->CreateKeyDerivationFunction(algId).GetResult();
    CryptoServiceInstanceId instanceId = kdfResult.Value().instanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for CreateKeyDerivationFunction : " << instanceId;
    kdfUptr = std::make_unique<KeyDerivationFunctionCtxProxyImpl>(instanceId, cryptoProviderHandler_, *this);
    return ara::core::Result<KeyDerivationFunctionCtx::Uptr>(std::move(kdfUptr));
  }
  else
  {
    // According to SWS_CRYPT_20748 if given algorithm ID is not supported by
    // MAC ctx then KeyDerivationFunctionCtx should return SecurityErrorDomain::
    // kInvalidArgument.
    logger_.LogError() << "IPC-APP Provided algorithm ID does not belongs to KeyDerivationFunction ctx";
    return ara::core::Result<KeyDerivationFunctionCtx::Uptr>::FromError(
        SecurityErrorDomain::Errc::kInvalidArgument);
  }
}

CryptoPrimitiveId::AlgId CryptoProviderproxyImpl::ConvertToAlgId(ara::core::StringView primitiveName) const
    noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;
  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  ara::core::String tempStr{primitiveName};
  return algSptr->GetAlgoId(tempStr);
}

ara::core::Result<ara::core::String> CryptoProviderproxyImpl::ConvertToAlgName(AlgId algId) const noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;
  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  ara::core::String algName{algSptr->GetAlgoName(algId)};
  if (algName.empty() == true)
  {
    logger_.LogError() << "IPC-APP CryptoProviderproxyImpl::ConvertToAlgName() -->kUnknownIdentifier error";
    return ara::core::Result<ara::core::String>::FromError(SecurityErrorDomain::Errc::kUnknownIdentifier);
  }
  else
  {
    logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::ConvertToAlgName() --> success";
    return ara::core::Result<ara::core::String>(algName);
  }
}

ara::core::Result<EncryptorPublicCtx::Uptr> CryptoProviderproxyImpl::CreateEncryptorPublicCtx(
    AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;

  EncryptorPublicCtx::Uptr encryPubUptr;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    // According to SWS_CRYPT_20748 if given algorithm ID is not supported by
    // crypto provider then CreateKeyDerivationFunctionCtx should return SecurityErrorDomain::
    // kUnknownIdentifier.
    logger_.LogDebug() << "IPC-APP Algorithm Id is not supported by Crypto provider";
    return ara::core::Result<EncryptorPublicCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if (algId == algorithmids::kRSA2048PKCSV1_5)
  {
    auto encryPubResult = cryptoProviderHandler_->CreateEncryptorPublic(algId).GetResult();
    CryptoServiceInstanceId instanceId = encryPubResult.Value().instanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for CreateEncryptorPublicCtx : " << instanceId;
    encryPubUptr = std::make_unique<EncryptorPublicCtxProxyImpl>(instanceId, cryptoProviderHandler_, *this);
    return ara::core::Result<EncryptorPublicCtx::Uptr>(std::move(encryPubUptr));
  }
  else
  {
    logger_.LogError() << "IPC-APP Provided algorithm ID does not belongs to EncryptorPublicCtx";
    return ara::core::Result<EncryptorPublicCtx::Uptr>::FromError(
        SecurityErrorDomain::Errc::kInvalidArgument);
  }
}

ara::core::Result<DecryptorPrivateCtx::Uptr> CryptoProviderproxyImpl::CreateDecryptorPrivateCtx(
    AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;

  DecryptorPrivateCtx::Uptr decryPriUptr;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    logger_.LogDebug() << "IPC-APP Algorithm Id is not supported by Crypto provider";
    return ara::core::Result<DecryptorPrivateCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if (algId == algorithmids::kRSA2048PKCSV1_5)
  {
    auto decryPriResult = cryptoProviderHandler_->CreateDecryptorPrivate(algId).GetResult();
    CryptoServiceInstanceId instanceId = decryPriResult.Value().instanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for CreateDecryptorPrivateCtx : " << instanceId;
    decryPriUptr = std::make_unique<DecryptorPrivateCtxProxyImpl>(instanceId, cryptoProviderHandler_, *this);
    return ara::core::Result<DecryptorPrivateCtx::Uptr>(std::move(decryPriUptr));
  }
  else
  {
    logger_.LogError() << "IPC-APP Provided algorithm ID does not belongs to DecryptorPrivateCtx";
    return ara::core::Result<DecryptorPrivateCtx::Uptr>::FromError(
        SecurityErrorDomain::Errc::kInvalidArgument);
  }
}

ara::core::Result<SigEncodePrivateCtx::Uptr> CryptoProviderproxyImpl::CreateSigEncodePrivateCtx(
    AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;

  SigEncodePrivateCtx::Uptr signEncPriUptr;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    logger_.LogDebug() << "IPC-APP Algorithm Id is not supported by Crypto provider";
    return ara::core::Result<SigEncodePrivateCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if (algId == algorithmids::kRSASSA2048PKCSV1_5)
  {
    auto sigEncPriResult = cryptoProviderHandler_->CreateSigEncodePrivate(algId).GetResult();
    CryptoServiceInstanceId instanceId = sigEncPriResult.Value().instanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for CreateSigEncodePrivateCtx : " << instanceId;
    signEncPriUptr
        = std::make_unique<SigEncodePrivateCtxProxyImpl>(instanceId, cryptoProviderHandler_, *this);
    return ara::core::Result<SigEncodePrivateCtx::Uptr>(std::move(signEncPriUptr));
  }
  else
  {
    logger_.LogError() << "IPC-APP Provided algorithm ID does not belongs to SigEncodePrivateCtx";
    return ara::core::Result<SigEncodePrivateCtx::Uptr>::FromError(
        SecurityErrorDomain::Errc::kInvalidArgument);
  }
}

ara::core::Result<MsgRecoveryPublicCtx::Uptr> CryptoProviderproxyImpl::CreateMsgRecoveryPublicCtx(
    AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;

  MsgRecoveryPublicCtx::Uptr msgrecPubUptr;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    logger_.LogDebug() << "IPC-APP Algorithm Id is not supported by Crypto provider";
    return ara::core::Result<MsgRecoveryPublicCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if (algId == algorithmids::kRSASSA2048PKCSV1_5)
  {
    auto msgrecPubResult = cryptoProviderHandler_->CreateMsgRecoveryPublic(algId).GetResult();
    CryptoServiceInstanceId instanceId = msgrecPubResult.Value().instanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for CreateMsgRecoveryPublic : " << instanceId;
    msgrecPubUptr
        = std::make_unique<MsgRecoveryPublicCtxProxyImpl>(instanceId, cryptoProviderHandler_, *this);
    return ara::core::Result<MsgRecoveryPublicCtx::Uptr>(std::move(msgrecPubUptr));
  }
  else
  {
    logger_.LogError() << "IPC-APP Provided algorithm ID does not belongs to MsgRecoveryPublicCtx";
    return ara::core::Result<MsgRecoveryPublicCtx::Uptr>::FromError(
        SecurityErrorDomain::Errc::kInvalidArgument);
  }
}

ara::core::Result<SignerPrivateCtx::Uptr> CryptoProviderproxyImpl::CreateSignerPrivateCtx(
    AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;

  SignerPrivateCtx::Uptr signerPriUptr;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    logger_.LogDebug() << "IPC-APP Algorithm Id is not supported by Crypto provider";
    return ara::core::Result<SignerPrivateCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if ((algId == algorithmids::kECDSA) || (algId == algorithmids::kECDSAP256)
      || (algId == algorithmids::kRSASSA2048PKCSV1_5))
  {
    auto signPriResult = cryptoProviderHandler_->CreateSignerPrivate(algId).GetResult();
    CryptoServiceInstanceId instanceId = signPriResult.Value().instanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for CreateSignerPrivateCtx : " << instanceId;
    signerPriUptr = std::make_unique<SignerPrivateCtxProxyImpl>(instanceId, cryptoProviderHandler_, *this);
    return ara::core::Result<SignerPrivateCtx::Uptr>(std::move(signerPriUptr));
  }
  else
  {
    logger_.LogError() << "IPC-APP Provided algorithm ID does not belongs to SignerPrivateCtx";
    return ara::core::Result<SignerPrivateCtx::Uptr>::FromError(SecurityErrorDomain::Errc::kInvalidArgument);
  }
}

ara::core::Result<VerifierPublicCtx::Uptr> CryptoProviderproxyImpl::CreateVerifierPublicCtx(
    AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;

  VerifierPublicCtx::Uptr verifyPubUptr;

  std::shared_ptr<AlgorithmIdentification> algSptr = AlgorithmIdentification::GetAlogIdentification();
  if (algSptr->IsAlgorithmSupported(algId) == false)
  {
    logger_.LogDebug() << "IPC-APP Algorithm Id is not supported by Crypto provider";
    return ara::core::Result<VerifierPublicCtx::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if ((algId == algorithmids::kECDSA) || (algId == algorithmids::kECDSAP256)
      || (algId == algorithmids::kRSASSA2048PKCSV1_5))
  {
    auto verifyPubResult = cryptoProviderHandler_->CreateVerifierPublic(algId).GetResult();
    CryptoServiceInstanceId instanceId = verifyPubResult.Value().instanceId;
    logger_.LogDebug() << "IPC-APP Service InstanceId for CreateVerifierPublicCtx : " << instanceId;
    verifyPubUptr = std::make_unique<VerifierPublicCtxProxyImpl>(instanceId, cryptoProviderHandler_, *this);
    return ara::core::Result<VerifierPublicCtx::Uptr>(std::move(verifyPubUptr));
  }
  else
  {
    logger_.LogError() << "IPC-APP Provided algorithm ID does not belongs to VerifierPublicCtx";
    return ara::core::Result<VerifierPublicCtx::Uptr>::FromError(SecurityErrorDomain::Errc::kInvalidArgument);
  }
}

/*
ara::core::Result<KeyEncapsulatorPublicCtx::Uptr> CryptoProviderproxyImpl::CreateKeyEncapsulatorPublicCtx(AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;
  static_cast<void>(algId);
  return ara::core::Result<KeyEncapsulatorPublicCtx::Uptr>(nullptr);
}

ara::core::Result<KeyDecapsulatorPrivateCtx::Uptr> CryptoProviderproxyImpl::CreateKeyDecapsulatorPrivateCtx(AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;
  static_cast<void>(algId);
  return ara::core::Result<KeyDecapsulatorPrivateCtx::Uptr>(nullptr);
}

ara::core::Result<KeyAgreementPrivateCtx::Uptr> CryptoProviderproxyImpl::CreateKeyAgreementPrivateCtx(AlgId algId) noexcept
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;
  static_cast<void>(algId);
  return ara::core::Result<KeyAgreementPrivateCtx::Uptr>(nullptr);
}

*/

bool CryptoProviderproxyImpl::IsStreamCipherAlgorithmSupported(AlgId algId)
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;
  if ((algId == algorithmids::kAES128CTR) || (algId == algorithmids::kAES192CTR)
      || (algId == algorithmids::kAES256CTR))
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool CryptoProviderproxyImpl::IsAuthenticationCipherAlgorithmSupported(AlgId algId)
{
  logger_.LogDebug() << "IPC-APP CryptoProviderproxyImpl::" << __FUNCTION__;
  if ((algId == algorithmids::kAES128GCM) || (algId == algorithmids::kAES192GCM)
      || (algId == algorithmids::kAES256GCM))
  {
    return true;
  }
  else
  {
    return false;
  }
}

} // namespace cryp
} // namespace crypto
} // namespace ara
