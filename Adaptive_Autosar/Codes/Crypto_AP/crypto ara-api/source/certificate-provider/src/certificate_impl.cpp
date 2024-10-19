// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : certificate_impl.cpp
//
// Purpose     : This file provides certificate_impl class definitions.
// -----------------------------------------------------------------------------------------------------------

#include "certificate_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
namespace crypto
{
namespace x509
{
std::uint32_t CertificateImpl::X509Version() const noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::X509Version()";
  uint32_t const kCertVersionOffset{1};
  return X509_get_version(certificatePtr_) + kCertVersionOffset;
}

bool CertificateImpl::IsRoot() const noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::IsRoot()";
  if (IssuerDn()->GetDnString() == SubjectDn()->GetDnString())
  {
    aralog_.LogDebug() << "Given Certificate is a root certificate";
    return true;
  }
  else
  {
    aralog_.LogDebug() << "Given Certificate is not a root certificate";
    return false;
  }
}

X509DN::Uptr CertificateImpl::IssuerDn() const
{
  aralog_.LogDebug() << "In CertificateImpl::IssuerDn()";
  X509DN::Uptr localPtr{std::make_unique<X509DNImpl>(issuerDomainName_)};
  return localPtr;
}

time_t CertificateImpl::StartTime() const noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::StartTime()";
  return startTime_;
}

time_t CertificateImpl::EndTime() const noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::EndTime()";
  return endTime_;
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> CertificateImpl::SerialNumber() const noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::SerialNumber()";

  ASN1_INTEGER* const serNoAsn1{X509_get_serialNumber(certificatePtr_)};

  if (serNoAsn1 != nullptr)
  {
    // Here X509_get_serialNumber ASN1_INTEGER which is an openssl structure. To convert that into printable
    // format we have to do some conversions. Here we are converting ASN1_INTEGER to BIGNUM.
    BIGNUM* const opensslBigNum{ASN1_INTEGER_to_BN(serNoAsn1, nullptr)};

    if (opensslBigNum != nullptr)
    {
      aralog_.LogWarn() << "Failed to get the Big Number";
    }

    // Here we are converting BIGNUM to ASCII decimal numbers.
    char* const serNoDec{BN_bn2dec(opensslBigNum)};

    std::stringstream printSerialNo;
    for (size_t index{0}; index < strlen(serNoDec); index++)
    {
      printSerialNo << serNoDec[index];
    }

    aralog_.LogDebug() << printSerialNo.str();

    ara::core::Vector<ara::core::Byte> const serNumResult{serNoDec, serNoDec + strlen(serNoDec)};

    BN_free(opensslBigNum);
    OPENSSL_free(serNoDec);

    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(serNumResult);
  }
  else
  {
    aralog_.LogWarn() << "Serial Number is not found";
    ara::core::Vector<ara::core::Byte> const serNumResult{};
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(serNumResult);
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> CertificateImpl::AuthorityKeyId() const noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::AuthorityKeyId()";

  // Get Authority key Id.
  // Here we get the Key ID in ASN1_OCTET_STRING which is an openssl structure. To make it printable
  // we have to do some conversions.
  ASN1_OCTET_STRING const* keyIdAsn1{X509_get0_authority_key_id(certificatePtr_)};

  uint8_t* authorityKeyId{nullptr};

  int32_t length{0};

  if (keyIdAsn1 != nullptr)
  {
    X509V3_EXT_METHOD extMethod;

    // Here we are converting ASN1_OCTET_STRING to ASCII string and storing it in char * variable.
    char* const keyId{i2s_ASN1_OCTET_STRING(&extMethod, keyIdAsn1)};

    aralog_.LogDebug() << "Extracted Authority Key Id is ";

    std::stringstream printSerialNo;
    for (size_t index{0}; index < strlen(keyId); index++)
    {
      printSerialNo << keyId[index];
    }

    aralog_.LogDebug() << printSerialNo.str();

    OPENSSL_free(keyId);

    length = i2d_ASN1_OCTET_STRING(keyIdAsn1, &authorityKeyId);
  }
  else
  {
    aralog_.LogWarn() << "Authority key Id is not found";
  }

  ara::core::Vector<ara::core::Byte> const authorityKeyIdResult{authorityKeyId, authorityKeyId + length};

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(authorityKeyIdResult);
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> CertificateImpl::SubjectKeyId() const noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::SubjectKeyId()";

  // Get Subject key Id.
  // Here we get the Key ID in ASN1_OCTET_STRING which is an openssl structure. To make it printable
  // we have to do some conversions.
  ASN1_OCTET_STRING const* keyIdAsn1{X509_get0_subject_key_id(certificatePtr_)};

  uint8_t* subjectKeyId{nullptr};

  int32_t length{0};

  if (keyIdAsn1 != nullptr)
  {
    X509V3_EXT_METHOD extMethod;

    // Here we are converting ASN1_OCTET_STRING to ASCII string and storing it in char * variable.
    char* const keyId{i2s_ASN1_OCTET_STRING(&extMethod, keyIdAsn1)};

    aralog_.LogDebug() << "Extracted Subject Key Id is ";

    std::stringstream printSerialNo;
    for (size_t index{0}; index < strlen(keyId); index++)
    {
      printSerialNo << keyId[index];
    }

    aralog_.LogDebug() << printSerialNo.str();

    OPENSSL_free(keyId);

    length = i2d_ASN1_OCTET_STRING(keyIdAsn1, &subjectKeyId);
  }
  else
  {
    aralog_.LogWarn() << "Subject key Id is not found";
  }

  ara::core::Vector<ara::core::Byte> const subjectKeyIdResult{subjectKeyId, subjectKeyId + length};

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(subjectKeyIdResult);
}

ara::core::Result<std::size_t> CertificateImpl::GetFingerprint(
    ReadWriteMemRegion fingerprint, cryp::HashFunctionCtx& hashCtx) const noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::GetFingerprint()";

  if (!hashCtx.IsInitialized())
  {
    // According to SWS_CRYPT_40220 if if the hashCtx context is not initialized
    // then GetFingerprint() will return an error SecurityErrorDomain::kIncompleteArgState.
    aralog_.LogError() << "Hash ctx is not configured properly!!!!";
    return ara::core::Result<std::size_t>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kIncompleteArgState);
  }

  uint8_t* certPtr{nullptr};

  int32_t const len{i2d_X509(certificatePtr_, &certPtr)};

  ReadOnlyMemRegion const cert{certPtr, static_cast<size_t>(len)};

  // Update the hash ctx with public key.
  hashCtx.Update(cert);

  ara::core::Result<ara::core::Vector<ara::core::Byte>> hashResult{hashCtx.Finish()};

  if (hashResult.HasValue())
  {
    ara::core::Vector<ara::core::Byte> hashOutput{std::move(hashResult).Value()};

    // Here we get the output from the Hash ctx in form of ara::core::Vector<ara::core::Byte>
    // To store this value in ReadWriteMemRegion we have to convert it to ara::core::Vector<uint8_t>
    ara::core::Vector<uint8_t> hashOutInUint8{hashOutput.begin(), hashOutput.end()};

    size_t index;

    std::stringstream fingerprintValue;

    // Store the calculated Fingerprint in inout buffer until the inout buffer gets completely occupied
    // or fingerprint value gets completely copied.
    for (index = 0; (index < fingerprint.size()) && (index < hashOutInUint8.size()); index++)
    {
      fingerprintValue << std::setw(2) << std::setfill('0') << std::hex
                       << static_cast<uint16_t>(hashOutInUint8[index]);
      fingerprint[index] = hashOutInUint8[index];
    }

    aralog_.LogDebug() << "Fingerprint value is " << fingerprintValue.str();

    return ara::core::Result<std::size_t>::FromValue(index);
  }
  else
  {
    // According to SWS_CRYPT_40220 if if the hashCtx context is not initialized
    // then GetFingerprint() will return an error SecurityErrorDomain::kIncompleteArgState.
    aralog_.LogError() << "Hash ctx is not configured properly!!!!";
    return ara::core::Result<std::size_t>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kIncompleteArgState);
  }
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> CertificateImpl::ExportPublicly(
    Serializable::FormatId formatId) const noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::ExportPublicly()";

  if ((formatId != Serializable::kFormatDerEncoded) && (formatId != Serializable::kFormatPemEncoded)
      && (formatId != Serializable::kFormatDefault) && (formatId != Serializable::kFormatRawValueOnly))
  {
    // According to SWS_CRYPT_10711 if an unknown format ID was specified
    // then ExportPublicly() should return SecurityErrorDomain::Errc::kUnknownIdentifier.
    aralog_.LogError() << "Invalid format Id";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if ((formatId == Serializable::kFormatDerEncoded) || (formatId == Serializable::kFormatDefault))
  {
    uint8_t* certPtr{nullptr};

    int32_t len{i2d_X509(certificatePtr_, &certPtr)};

    if (len < 0)
    {
      aralog_.LogError() << "Error in DER encoding";
      len = 0;
    }

    ara::core::Vector<uint8_t> tempResult{certPtr, certPtr + len};
    ara::core::Vector<ara::core::Byte> const result{tempResult.begin(), tempResult.end()};

    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(result);
  }
  else if (formatId == Serializable::kFormatPemEncoded)
  {
    BIO* const certbio{BIO_new(BIO_s_mem())};

    int32_t ret{PEM_write_bio_X509(certbio, certificatePtr_)};

    if (!ret)
    {
      aralog_.LogError() << "Write operation failed.";
    }

    int32_t const certLen{BIO_pending(certbio)};

    ara::core::Vector<uint8_t> certVal;

    certVal.resize(static_cast<size_t>(certLen));

    ret = BIO_read(certbio, certVal.data(), certLen);

    if (!ret)
    {
      aralog_.LogError() << "Read Operation failed";
    }

    ara::core::Vector<ara::core::Byte> const result{certVal.begin(), certVal.end()};

    BIO_free(certbio);

    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(result);
  }
  else
  {
    // According to SWS_CRYPT_10711 if the specified format ID is not supported for this object type
    // then ExportPublicly() should return SecurityErrorDomain::Errc::kUnsupportedFormat.
    aralog_.LogError() << "Unsupported format Id";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnsupportedFormat);
  }
}

X509* CertificateImpl::GetCertificate() const noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::GetCertificate()";
  return certificatePtr_;
}

Certificate::Status CertificateImpl::GetStatus() const noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::GetStatus()";
  aralog_.LogDebug() << "Certificate Status is " << static_cast<uint32_t>(certStatus_);
  return certStatus_;
}

void CertificateImpl::SetStatus(Status verificationStatus) noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::SetStatus()";
  certStatus_ = verificationStatus;
}

X509DN::Uptr CertificateImpl::SubjectDn() const noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::SubjectDn()";

  X509DN::Uptr localPtr{std::make_unique<X509DNImpl>(subjectdomainName_)};

  return localPtr;
}

bool CertificateImpl::IsCa() const noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::IsCa()";

  if (X509_check_ca(certificatePtr_) == 1)
  {
    aralog_.LogDebug() << "Given Certificate is a CA certificate";
    return true;
  }
  else
  {
    aralog_.LogDebug() << "Given Certificate is not a CA certificate";
    return false;
  }
}

X509PublicKeyInfo::Uptrc CertificateImpl::SubjectPubKey(cryp::CryptoProvider::Uptr cryptoProvider) const
    noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::SubjectPubKey()";
  cryp::CryptoProvider::Uptr crypPtr{std::make_unique<cryp::CryptoProviderSSL>()};

  int32_t const kMinUnsignedIntVal{0};

  X509PublicKeyInfo::Uptrc pKeyInfoPtr{nullptr};

  if (cryptoProvider != nullptr)
  {
    crypPtr = std::move(cryptoProvider);
  }

  EVP_PKEY* const pubKey{X509_get_pubkey(certificatePtr_)};

  // extract private key as string
  // create a place to dump the IO, in this case in memory
  BIO* const pkeyBIO{BIO_new(BIO_s_mem())};
  // dump key to IO
  int32_t ret{
      PEM_write_bio_PrivateKey(pkeyBIO, pubKey, nullptr, nullptr, kMinUnsignedIntVal, nullptr, nullptr)};

  if (ret < kMinUnsignedIntVal)
  {
    aralog_.LogError() << "Write operation in BIO is failed!!";
  }
  // get buffer length
  int32_t const pKeyLen{BIO_pending(pkeyBIO)};

  ara::core::Vector<uint8_t> pKeyChar;

  pKeyChar.resize(static_cast<size_t>(pKeyLen));

  // read the key from the buffer and put it in the char reference
  ret = BIO_read(pkeyBIO, pKeyChar.data(), pKeyLen);

  if (ret < kMinUnsignedIntVal)
  {
    aralog_.LogError() << "Read operation in BIO is failed!!";
  }

  IOInterface::Uptr ioPtr{std::make_unique<IOInterfaceVolatile>(1000, true, true, true, true)};

  uint32_t const usage{X509_get_key_usage(certificatePtr_)};

  AllowedUsageFlags usagFlags{kAllowPrototypedOnly};

  if (((usage & kConstrDigitalSignature) > kMinUnsignedIntVal)
      || ((usage & kConstrKeyCertSign) > kMinUnsignedIntVal)
      || ((usage & kConstrCrlSign) > kMinUnsignedIntVal))
  {
    usagFlags |= kAllowSignature;
  }

  if (((usage & kConstrKeyEncipherment) > kMinUnsignedIntVal)
      || ((usage & kConstrDataEncipherment) > kMinUnsignedIntVal)
      || ((usage & kConstrEncipherOnly) > kMinUnsignedIntVal))
  {
    usagFlags |= kAllowDataEncryption;
  }

  if ((usage & kConstrDecipherOnly) > kMinUnsignedIntVal)
  {
    usagFlags |= kAllowDataDecryption;
  }

  CryptoObjectUid const objectID;

  if (ioPtr->SetCryptoObject(
          objectID, CryptoObjectType::kPublicKey, GetAlgoIdFromTheCertificate(pubKey), usagFlags, pKeyChar)
      > kMinUnsignedIntVal)
  {
    aralog_.LogError() << "Write Operation failed";
  }

  X509PublicKeyInfo::Uptrc pKeyInfoTempPtr{
      std::make_unique<X509PublicKeyInfoImpl>(std::move(crypPtr), std::move(ioPtr))};

  pKeyInfoPtr = std::move(pKeyInfoTempPtr);

  return pKeyInfoPtr;
}

BasicCertInfo::KeyConstraints CertificateImpl::GetConstraints() const noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::GetConstraints()";
  return X509_get_key_usage(certificatePtr_);
}

CryptoAlgId CertificateImpl::GetAlgoIdFromTheCertificate(EVP_PKEY* const publicKey) const noexcept
{
  int32_t pknid;
  int32_t mdnid;
  int32_t const rsaPublicKeyBitLength{2048};
  int32_t const ecDsaPublicKeyBitLength{256};
  int32_t const kMinUnsignedIntVal{0};
  CryptoAlgId algId{kAlgIdUndefined};

  int32_t const ret{X509_get_signature_info(certificatePtr_, &mdnid, &pknid, nullptr, nullptr)};

  if (ret < kMinUnsignedIntVal)
  {
    aralog_.LogError() << "Failed to get signature information";
  }

  ara::core::String const algoName{OBJ_nid2ln(pknid)};

  ara::core::String const mdName{OBJ_nid2ln(mdnid)};

  int32_t const keyLength{EVP_PKEY_get_bits(publicKey)};

  if ((algoName.find("rsa", kMinUnsignedIntVal) != std::string::npos)
      && (mdName.find("sha256", kMinUnsignedIntVal) != std::string::npos)
      && (keyLength == rsaPublicKeyBitLength))
  {
    algId = algorithmids::kRSASSA2048PKCSV1_5;
  }
  else if ((algoName.find("ec", kMinUnsignedIntVal) != std::string::npos)
      && (mdName.find("sha256", kMinUnsignedIntVal) != std::string::npos)
      && (keyLength == ecDsaPublicKeyBitLength))
  {
    algId = algorithmids::kECDSAP256;
  }
  else
  {
    // Do nothing
  }

  return algId;
}

time_t CertificateImpl::GetTimeT(ASN1_TIME const* time) noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::GetTimeT()";
  struct tm t;
  ASN1_TIME_to_tm(time, &t);
  /* Note: we did not adjust the time based on time zone information */
  return mktime(&t);
}

} // namespace x509
} // namespace crypto
} // namespace ara
