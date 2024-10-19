// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : skeleton_data_storage_container.cpp
//
// Purpose     : Skeleton data storage container implementation to store the object into skeleton container.
// -----------------------------------------------------------------------------------------------------------

#include "skeleton_data_storage_container.h"

namespace ara
{
namespace crypto
{
namespace common
{
std::shared_ptr<SkeletonDataStorageContainer> SkeletonDataStorageContainer::skeletonDataStorageContainer_{};

std::shared_ptr<SkeletonDataStorageContainer>
  SkeletonDataStorageContainer::GetDataContainerInstance() noexcept
{
  if (skeletonDataStorageContainer_ == nullptr)
  {
    // replaces the managed object.
    skeletonDataStorageContainer_.reset(new SkeletonDataStorageContainer());
  }
  return skeletonDataStorageContainer_;
}

CryptoServiceInstanceId SkeletonDataStorageContainer::GetCryptoServiceInstanceId()
{
  // Generate new instance Id for crypto service.
  std::lock_guard<std::mutex> const lock{ mutex_instanceId_ };
  instanceId_ = instanceId_ + 1;
  CryptoServiceInstanceId strInstance{ "SOME/IP:" };
  CryptoServiceInstanceId const tempStr{ std::to_string(static_cast<uint64_t>(instanceId_)) };
  strInstance.append(tempStr);
  return strInstance;
}

void SkeletonDataStorageContainer::UpdateSymmetricKeyContainer(
  CryptoServiceInstanceId const& serviceInstanceId,
  std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl> symmetricKeyUptr)
{
  symmetricKeyDataStorageContainer_.emplace(serviceInstanceId, std::move(symmetricKeyUptr));
}

void SkeletonDataStorageContainer::UpdateSignatureContainer(CryptoServiceInstanceId const& serviceInstanceId,
  std::unique_ptr<ara::crypto::cryp::cryobj::SignatureSkeletonImpl> signatureUptr)
{
  signatureDataStorageContainer_.emplace(serviceInstanceId, std::move(signatureUptr));
}

void SkeletonDataStorageContainer::UpdatePublicKeyContainer(CryptoServiceInstanceId const& serviceInstanceId,
  std::unique_ptr<ara::crypto::cryp::cryobj::PublicKeySkeletonImpl> publicKeyUptr)
{
  publicDataStorageContainer_.emplace(serviceInstanceId, std::move(publicKeyUptr));
}

void SkeletonDataStorageContainer::UpdateSecretSeedContainer(CryptoServiceInstanceId const& serviceInstanceId,
  std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl> secretSeedUptr)
{
  secretSeedDataStorageContainer_.emplace(serviceInstanceId, std::move(secretSeedUptr));
}

void SkeletonDataStorageContainer::UpdatePrivateKeyContainer(CryptoServiceInstanceId const& serviceInstanceId,
  std::unique_ptr<ara::crypto::cryp::cryobj::PrivateKeySkeletonImpl> privateKeyUptr)
{
  privateDataStorageContainer_.emplace(serviceInstanceId, std::move(privateKeyUptr));
}

void SkeletonDataStorageContainer::UpdateRestrictedUseObjectContainer(
  CryptoServiceInstanceId const& serviceInstanceId,
  std::unique_ptr<ara::crypto::cryp::cryobj::RestrictedUseObjectSkeletonImpl> restrictedUseObjectUptr)
{
  restrictedUseObjectContainer_.emplace(serviceInstanceId, std::move(restrictedUseObjectUptr));
}

std::unique_ptr<ara::crypto::cryp::cryobj::SymmetrickeySkeletonImpl>&
  SkeletonDataStorageContainer::GetSymmetricKeySkeletonContainer(
    CryptoServiceInstanceId const& serviceInstanceId)
{
  auto obj{ symmetricKeyDataStorageContainer_.find(serviceInstanceId) };
  return obj->second;
}

std::unique_ptr<ara::crypto::cryp::cryobj::SignatureSkeletonImpl>&
  SkeletonDataStorageContainer::GetSignatureSkeletonContainer(
    CryptoServiceInstanceId const& serviceInstanceId)
{
  auto obj{ signatureDataStorageContainer_.find(serviceInstanceId) };
  return obj->second;
}

std::unique_ptr<ara::crypto::cryp::cryobj::SecretSeedSkeletonImpl>&
  SkeletonDataStorageContainer::GetSecretSeedSkeletonContainer(
    CryptoServiceInstanceId const& serviceInstanceId)
{
  auto obj{ secretSeedDataStorageContainer_.find(serviceInstanceId) };
  return obj->second;
}

std::unique_ptr<ara::crypto::cryp::cryobj::PublicKeySkeletonImpl>&
  SkeletonDataStorageContainer::GetPublicKeySkeletonContainer(
    CryptoServiceInstanceId const& serviceInstanceId)
{
  auto obj{ publicDataStorageContainer_.find(serviceInstanceId) };
  return obj->second;
}

std::unique_ptr<ara::crypto::cryp::cryobj::PrivateKeySkeletonImpl>&
  SkeletonDataStorageContainer::GetPrivateKeySkeletonContainer(
    CryptoServiceInstanceId const& serviceInstanceId)
{
  auto obj{ privateDataStorageContainer_.find(serviceInstanceId) };
  return obj->second;
}

std::unique_ptr<ara::crypto::cryp::cryobj::RestrictedUseObjectSkeletonImpl>&
  SkeletonDataStorageContainer::GetRestrictedUseObjectSkeletonContainer(
    CryptoServiceInstanceId const& serviceInstanceId)
{
  auto obj{ restrictedUseObjectContainer_.find(serviceInstanceId) };
  return obj->second;
}

} // namespace common
} // namespace crypto
} // namespace ara
