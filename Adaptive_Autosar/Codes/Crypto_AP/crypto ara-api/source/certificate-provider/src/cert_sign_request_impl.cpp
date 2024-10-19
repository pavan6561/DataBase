// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : cert_sign_request_impl.cpp
//
// Purpose     : This file provides CertSignRequestImpl class definitions.
// -----------------------------------------------------------------------------------------------------------

#include "cert_sign_request_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
namespace crypto
{
namespace x509
{
bool CertSignRequestImpl::Verify() const noexcept
{
  aralog_.LogDebug() << "In CertSignRequestImpl::Verify()";

  bool csrStatus{false};

  EVP_PKEY* pubKey{X509_REQ_get_pubkey(csrReq_)};

  if (X509_REQ_verify(csrReq_, pubKey) == 1)
  {
    aralog_.LogDebug() << "Cert Sign Request Verified Succesfully";
    csrStatus = true;
  }
  else
  {
    aralog_.LogError() << "Cert Sign Request Verification Failed!!!";
  }

  OPENSSL_free(pubKey);

  return csrStatus;
}

ara::core::Result<ara::core::Vector<ara::core::Byte>>
    CertSignRequestImpl::ExportASN1CertSignRequest() noexcept
{
  aralog_.LogDebug() << "In CertSignRequestImpl::ExportASN1CertSignRequest()";

  uint8_t* certReqInDer{nullptr};

  int32_t len{i2d_X509_REQ(csrReq_, &certReqInDer)};

  if (len < 0)
  {
    aralog_.LogError() << "Required information has not beed provided!!!!";
    // According to SWS_CRYPT_40313 if not all required information has been provided
    // then ExportASN1CertSignRequest will return SecurityErrorDomain::kInvalidUsageOrder.
    ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidUsageOrder);
  }

  ara::core::Vector<ara::core::Byte> exportReq{certReqInDer, certReqInDer + len};

  return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(exportReq);
}

unsigned CertSignRequestImpl::Version() const noexcept
{
  aralog_.LogDebug() << "In CertSignRequestImpl::Version()";

  return X509_REQ_get_version(csrReq_) + 1;
}

ara::crypto::cryp::Signature::Uptrc CertSignRequestImpl::GetSignature() const noexcept
{
  aralog_.LogDebug() << "In CertSignRequestImpl::GetSignature()";

  uint8_t* signaturValue{nullptr};

  const ASN1_BIT_STRING* sign{ASN1_BIT_STRING_new()};

  const X509_ALGOR* alg{X509_ALGOR_new()};

  X509_REQ_get0_signature(csrReq_, &sign, &alg);

  int32_t len{i2d_ASN1_BIT_STRING(sign, &signaturValue)};

  if (len < 0)
  {
    aralog_.LogError() << "Can not convert ASN1_BIT_STRING to printable format";
    len = 0;
  }

  ReadWriteMemRegion digest{signaturValue, static_cast<size_t>(len)};

  size_t index;

  std::stringstream fingerprintValue;

  // Store the calculated Fingerprint in inout buffer until the inout buffer gets completely occupied
  // or fingerprint value gets completely copied.
  for (index = 0; index < static_cast<size_t>(len); index++)
  {
    fingerprintValue << std::setw(2) << std::setfill('0') << std::hex
                     << static_cast<uint16_t>(signaturValue[index]);
  }

  aralog_.LogDebug() << "Fingerprint value is " << fingerprintValue.str();

  ara::crypto::cryp::Signature::Uptrc sigPointer = std::make_unique<ara::crypto::cryp::SignatureImpl>(digest,
      "SHA-256", AlgorithmIdentification::GetAlogIdentification()->GetAlgoId("SHA-256"), 0, 256, true, false);

  return sigPointer;
}

X509DN::Uptr CertSignRequestImpl::SubjectDn() const noexcept
{
  aralog_.LogDebug() << "In CertSignRequestImpl::SubjectDn()";
  X509DN::Uptr localPtr{std::make_unique<X509DNImpl>(subjectdomainName_)};
  return localPtr;
}

bool CertSignRequestImpl::IsCa() const noexcept
{
  aralog_.LogDebug() << "In CertSignRequestImpl::IsCa()";
  return false;
}

ara::core::Result<ara::core::Vector<ara::core::Byte>> CertSignRequestImpl::ExportPublicly(
    Serializable::FormatId formatId) const noexcept
{
  aralog_.LogDebug() << "In X509DNImpl::ExportPublicly()";

  uint8_t* csrPtr{nullptr};

  if ((formatId == Serializable::kFormatDerEncoded) || (formatId == Serializable::kFormatDefault))
  {
    int32_t len{i2d_X509_REQ(csrReq_, &csrPtr)};

    if (len < 0)
    {
      aralog_.LogError() << "Error in DER encoding";
      len = 0;
    }

    ara::core::Vector<uint8_t> tempResult{csrPtr, csrPtr + len};
    ara::core::Vector<ara::core::Byte> result{tempResult.begin(), tempResult.end()};

    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(result);
  }
  else if (formatId == Serializable::kFormatPemEncoded)
  {
    BIO* csrbio{BIO_new(BIO_s_mem())};

    PEM_write_bio_X509_REQ(csrbio, csrReq_);

    int csrLen = BIO_pending(csrbio);

    ara::core::Vector<uint8_t> csrVal;

    csrVal.resize(csrLen);

    BIO_read(csrbio, csrVal.data(), csrLen);

    ara::core::Vector<ara::core::Byte> result{csrVal.begin(), csrVal.end()};

    BIO_free(csrbio);

    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromValue(result);
  }
  else
  {
    // According to SWS_CRYPT_10711 if the formatId argument has unknown value
    // then ExportPublicly() should return SecurityErrorDomain::Errc::kUnknownIdentifier.
    aralog_.LogError() << "Invalid format Id";
    return ara::core::Result<ara::core::Vector<ara::core::Byte>>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }
}

X509PublicKeyInfo::Uptrc CertSignRequestImpl::SubjectPubKey(cryp::CryptoProvider::Uptr cryptoProvider) const
    noexcept
{
  aralog_.LogDebug() << "In CertSignRequestImpl::SubjectPubKey()";
  cryp::CryptoProvider::Uptr crypPtr{std::make_unique<cryp::CryptoProviderSSL>()};

  if (cryptoProvider != nullptr)
  {
    crypPtr = std::move(cryptoProvider);
  }

  EVP_PKEY* pubKey{X509_REQ_get_pubkey(csrReq_)};

  // extract private key as string
  // create a place to dump the IO, in this case in memory
  BIO* pkeyBIO = BIO_new(BIO_s_mem());
  // dump key to IO
  PEM_write_bio_PrivateKey(pkeyBIO, pubKey, NULL, NULL, 0, 0, NULL);
  // get buffer length
  int pKeyLen = BIO_pending(pkeyBIO);

  ara::core::Vector<uint8_t> pKeyChar;

  pKeyChar.resize(pKeyLen);
  // read the key from the buffer and put it in the char reference
  BIO_read(pkeyBIO, pKeyChar.data(), pKeyLen);

  IOInterface::Uptr ioPtr{std::make_unique<IOInterfaceVolatile>(1000, true, true, true, true)};

  ioPtr->SetObjectValue(pKeyChar);

  X509PublicKeyInfo::Uptrc pKeyInfoPtr{
      std::make_unique<X509PublicKeyInfoImpl>(std::move(crypPtr), std::move(ioPtr))};

  return pKeyInfoPtr;
}

BasicCertInfo::KeyConstraints CertSignRequestImpl::GetConstraints() const noexcept
{
  aralog_.LogDebug() << "In CertSignRequestImpl::GetConstraints()";
  return BasicCertInfo::kConstrNone;
}

CryptoAlgId CertSignRequestImpl::GetAlgoIdFromTheCSR(EVP_PKEY* public_key) const noexcept
{
  int mdnid;
  CryptoAlgId algId{kAlgIdUndefined};

  ara::core::String algoName{EVP_PKEY_get0_type_name(public_key)};

  EVP_PKEY_get_default_digest_nid(public_key, &mdnid);

  ara::core::String mdName{OBJ_nid2ln(mdnid)};

  int keyLength = EVP_PKEY_get_bits(public_key);

  if ((algoName.find("rsa", 0) != std::string::npos) && (mdName.find("sha256", 0) != std::string::npos)
      && (keyLength == 2048))
  {
    algId = algorithmids::kRSASSA2048PKCSV1_5;
  }
  else if ((algoName.find("ec", 0) != std::string::npos) && (mdName.find("sha256", 0) != std::string::npos)
      && (keyLength == 256))
  {
    algId = algorithmids::kECDSAP256;
  }
  else
  {
    // Do nothing
  }

  return algId;
}

} // namespace x509
} // namespace crypto
} // namespace ara
