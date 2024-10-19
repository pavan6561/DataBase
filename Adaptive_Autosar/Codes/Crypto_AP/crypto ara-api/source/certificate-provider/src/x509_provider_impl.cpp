// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509_provider_impl.cpp
//
// Purpose     : This file provides X509ProviderImpl class definitions.
// -----------------------------------------------------------------------------------------------------------

#include "x509_provider_impl.h"

/// @brief AUTOSAR Runtime for Adaptive Applications.
namespace ara
{
namespace crypto
{
namespace x509
{
namespace certdb
{
uint8_t const kCertDb{0};
uint8_t const kCaCertDb{1};
uint8_t const kCrlDb{2};
} // namespace certdb

sqlite3* X509ProviderImpl::stDBStore[3]{nullptr, nullptr, nullptr};

bool X509ProviderImpl::readComplete{false};

ara::log::Logger& X509ProviderImpl::aralog_{
    ara::log::CreateLogger("X5CP", "X509 Certificate Provider", ara::log::LogLevel::kVerbose)};

std::condition_variable X509ProviderImpl::cndVar_;

X509ProviderImpl::X509ProviderImpl() noexcept : X509Provider{}
{
  if (stDBStore[certdb::kCertDb] == nullptr)
  {
    int rc = sqlite3_open("/opt/crypto-core/var/cert.db", &stDBStore[certdb::kCertDb]);
    if (rc)
    {
      aralog_.LogError() << "Can't open database: " << sqlite3_errmsg(stDBStore[certdb::kCertDb]);
    }
    else
    {
      aralog_.LogDebug() << "Opened database successfully";
    }
  }

  if (stDBStore[certdb::kCaCertDb] == nullptr)
  {
    int rc = sqlite3_open("/opt/crypto-core/var/cacert.db", &stDBStore[certdb::kCaCertDb]);
    if (rc)
    {
      aralog_.LogError() << "Can't open  CA database: " << sqlite3_errmsg(stDBStore[certdb::kCertDb]);
    }
    else
    {
      aralog_.LogDebug() << "Opened CA database successfully";
      CreateTableInDB(certdb::kCaCertDb);
      if (IstableEmpty(certdb::kCaCertDb) == false)
      {
        ReadCACertificatesFromDB();
      }
    }
  }

  if (stDBStore[certdb::kCrlDb] == nullptr)
  {
    int rc = sqlite3_open("/opt/crypto-core/var/crl.db", &stDBStore[certdb::kCrlDb]);
    if (rc)
    {
      aralog_.LogError() << "Can't open CRL database: " << sqlite3_errmsg(stDBStore[certdb::kCrlDb]);
    }
    else
    {
      aralog_.LogDebug() << "Opened CRL database successfully";
      CreateTableInDB(certdb::kCrlDb);
      if (IstableEmpty(certdb::kCrlDb) == false)
      {
        ReadCRLsFromDB();
      }
    }
  }
  else
  {
    aralog_.LogDebug() << "stDBStore[certdb::kCrlDb] is not nullptr";
  }

  CreateTableInDB(0);

  if (IstableEmpty(0) == false)
  {
    ReadCertificatesFromDB();

    for (ara::core::Map<ara::core::InstanceSpecifier, CertificateImpl>::iterator it{dbCertList_.begin()};
         it != dbCertList_.end(); it++)
    {
      if (it->second.GetStatus() == Certificate::Status::kInvalid)
      {
        Certificate::Uptrc localPtr{std::make_unique<CertificateImpl>(it->second.GetCertificate())};
        Remove(std::move(localPtr));
      }
    }
  }
}

ara::core::Result<X509DN::Uptr> X509ProviderImpl::CreateEmptyDn(std::size_t capacity) noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::CreateEmptyDn()";

  static_cast<void>(capacity);

  X509DN::Uptr localPtr{std::make_unique<X509DNImpl>(nullptr)};

  return ara::core::Result<X509DN::Uptr>::FromValue(std::move(localPtr));
}

ara::core::Result<X509DN::Uptr> X509ProviderImpl::BuildDn(ara::core::StringView dn) noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::BuildDn()";

  X509_NAME* domainName{X509_NAME_new()};

  X509DN::Uptr localPtr{ std::make_unique<X509DNImpl>(domainName) };

  localPtr->SetDn(dn);

  return ara::core::Result<X509DN::Uptr>::FromValue(std::move(localPtr));
}

ara::core::Result<X509DN::Uptr> X509ProviderImpl::DecodeDn(
      ReadOnlyMemRegion dn, Serializable::FormatId formatId) noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::DecodeDn()";

  X509_NAME* domainName{nullptr};

  X509DN::Uptr localPtr{nullptr};

  if ((formatId == Serializable::kFormatDerEncoded) || (formatId == Serializable::kFormatDefault))
  {
    const uint8_t* dnAddr{ dn.data() };

    domainName = d2i_X509_NAME(NULL, &dnAddr, dn.size());

    if (domainName == nullptr)
    {
      // According to SWS_CRYPT_40613 if the cert argument cannot be parsed
      // then DecodeDn() should return SecurityErrorDomain::Errc::kInvalidArgument
      aralog_.LogError() << "Error in decoding the certificagte";
      return ara::core::Result<X509DN::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
    }
    localPtr = std::make_unique<X509DNImpl>(domainName);
  }
  else if (formatId == Serializable::kFormatRawValueOnly)
  {
    
    domainName = X509_NAME_new();

    ara::core::String dnString{ dn.begin(), dn.end() };

    localPtr = std::make_unique<X509DNImpl>(domainName);

    localPtr->SetDn(dnString);
    
  }
  else
  {
    // According to SWS_CRYPT_10711 if the formatId argument has unknown value
    // then ExportPublicly() should return SecurityErrorDomain::Errc::kUnknownIdentifier.
    aralog_.LogError() << "Invalid format Id";
    return ara::core::Result<X509DN::Uptr>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  return ara::core::Result<X509DN::Uptr>::FromValue(std::move(localPtr));
}

ara::core::Result<Certificate::Uptr> X509ProviderImpl::ParseCert(
    ReadOnlyMemRegion cert, Serializable::FormatId formatId) noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::ParseCert()";
  X509* certX509{nullptr};

  if ((formatId == Serializable::kFormatPemEncoded) || (formatId == Serializable::kFormatDefault))
  {
    // Here we are writing PEM encoded data in BIO, and read from the BIO using PEM decoding mechanism.
    aralog_.LogDebug() << "Given format Id is PEM ecoded format";
    BIO* certBio = BIO_new(BIO_s_mem());
    BIO_write(certBio, cert.data(), cert.size());
    certX509 = PEM_read_bio_X509(certBio, nullptr, 0, NULL);
    if (certX509 == nullptr)
    {
      aralog_.LogError() << "Error in parsing the certificate";
      return ara::core::Result<Certificate::Uptr>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
    }
  }
  else if (formatId == Serializable::kFormatDerEncoded)
  {
    // Here we are decoding the DER encoded data and storing it in X509 certificate.
    aralog_.LogDebug() << "Given format Id is DER ecoded format";
    const uint8_t* certAddr{cert.data()};
    aralog_.LogDebug() << "cert data size is " << certAddr;
    certX509 = d2i_X509(NULL, &certAddr, cert.size());
    if (certX509 == nullptr)
    {
      // According to SWS_CRYPT_40614 if the cert argument cannot be parsed
      // then ParseCert() should return SecurityErrorDomain::Errc::kInvalidArgument
      aralog_.LogError() << "Error in parsing the certificate";
      return ara::core::Result<Certificate::Uptr>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
    }
  }
  else
  {
    // According to SWS_CRYPT_40614 if the formatId argument has unknown value
    // then ParseCert() should return SecurityErrorDomain::Errc::kUnknownIdentifier.
    aralog_.LogError() << "Invalid format Id";
    return ara::core::Result<Certificate::Uptr>::FromError(
        ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  aralog_.LogDebug() << "Certificate parsed successfully";
  Certificate::Uptr localPtr{std::make_unique<CertificateImpl>(certX509)};

  aralog_.LogDebug() << "Uptr created";
  return ara::core::Result<Certificate::Uptr>::FromValue(std::move(localPtr));
}

ara::core::Result<std::size_t> X509ProviderImpl::CountCertsInChain(
    ReadOnlyMemRegion certChain, Serializable::FormatId formatId) const noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::CountCertsInChain()";
  ara::core::String certificates{reinterpret_cast<const char*>(certChain.data())};
  char CertToken[]{"-----BEGIN CERTIFICATE-----"};
  int64_t certStartLocation{0};
  size_t certCount{0};

  aralog_.LogDebug() << "Given format id is " << formatId;

  aralog_.LogDebug() << "Certchain length is " << certificates.length();

  if ((formatId != Serializable::kFormatPemEncoded) && (formatId != Serializable::kFormatDefault))
  {
    aralog_.LogError() << "Invalid format Id";
    return ara::core::Result<size_t>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  while (certStartLocation <= certificates.length())
  {
    certStartLocation = certificates.find(CertToken, certStartLocation);
    certStartLocation++;
    if (certStartLocation <= certificates.length())
    {
      certCount++;
    }
  }

  aralog_.LogDebug() << " Count of certificates in chain is " << certCount;

  if (certCount == 0)
  {
    aralog_.LogError() << "Invalid argument is received";
    return ara::core::Result<size_t>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
  }

  return ara::core::Result<std::size_t>::FromValue(certCount);
}

ara::core::Result<void> X509ProviderImpl::ParseCertChain(ara::core::Vector<Certificate::Uptr>& outcome,
    ReadOnlyMemRegion certChain, Serializable::FormatId formatId) noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::ParseCertChain()";
  ara::core::String certificates;
  ara::core::Vector<uint8_t> certInVtr;
  char CertStartToken[]{"-----BEGIN CERTIFICATE-----"};
  char CertEndToken[]{"-----END CERTIFICATE-----"};
  int64_t certStartLocation{0};
  ara::core::Vector<Certificate::Uptr> certVector;

  if ((formatId == Serializable::kFormatPemEncoded) || (formatId == Serializable::kFormatDefault))
  {
    certificates += reinterpret_cast<const char*>(certChain.data());
    const char* strPtr{certificates.data()};
    for (size_t index{0}; index < certificates.size(); index++)
    {
      certInVtr.emplace_back(static_cast<uint8_t>(strPtr[index]));
    }

    while (certStartLocation <= certificates.length())
    {
      certStartLocation = certificates.find(CertStartToken, certStartLocation);
      if (certStartLocation <= certificates.length())
      {
        int64_t certEndLocation{certificates.find(CertEndToken, certStartLocation)};
        ReadOnlyMemRegion signleCertificate{certInVtr.data() + certStartLocation,
            static_cast<size_t>((certEndLocation - certStartLocation) + strlen(CertEndToken))};

        ara::core::Result<Certificate::Uptr> result{ParseCert(signleCertificate, formatId)};
        if (result.HasValue())
        {
          Certificate::Uptr cert{std::move(result).Value()};
          certVector.push_back(std::move(cert));
        }
        else
        {
          return ara::core::Result<void>::FromError(std::move(result).Error());
        }
      }
      certStartLocation++;
    }
  }
  else
  {
    // According to SWS_CRYPT_40614 if the formatId argument has unknown value
    // then ParseCertChain() should return SecurityErrorDomain::Errc::kUnknownIdentifier.
    aralog_.LogError() << "Invalid format Id";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUnknownIdentifier);
  }

  if (certVector.empty() == true)
  {
    // According to SWS_CRYPT_40614 if the certChain argument cannot be parsed
    // then ParseCertChain() should return SecurityErrorDomain::Errc::kInvalidArgument.
    aralog_.LogError() << "Invalid argument is received";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
  }

  outcome.resize(certVector.size());

  size_t prevCAIndex{0};

  for (size_t outIndex{0}; outIndex < certVector.size(); outIndex++)
  {
    for (size_t inIndex{0}; inIndex < certVector.size(); inIndex++)
    {
      if (prevCAIndex == 0)
      {
        if (certVector[inIndex]->IssuerDn()->GetDnString() == certVector[inIndex]->SubjectDn()->GetDnString())
        {
          outcome[prevCAIndex++] = std::move(certVector[inIndex]);
        }
      }
      else if (prevCAIndex > 0)
      {
        if (certVector[inIndex] != nullptr)
        {
          if (outcome[prevCAIndex - 1]->SubjectDn()->GetDnString()
              == certVector[inIndex]->IssuerDn()->GetDnString())
          {
            outcome[prevCAIndex++] = std::move(certVector[inIndex]);
          }
        }
      }
    }
    if (prevCAIndex == certVector.size())
    {
      break;
    }
  }

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<void> X509ProviderImpl::ParseCertChain(ara::core::Vector<Certificate::Uptr>& outcome,
  const ara::core::Vector<ReadOnlyMemRegion>& certChain, Serializable::FormatId formatId) noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::ParseCertChain()";

  ara::core::Vector<Certificate::Uptr> certVector;

  certVector.resize(certChain.size());

  for (size_t index{0}; index < certChain.size(); index++)
  {
    ara::core::Result<Certificate::Uptr> parsResult{ParseCert(certChain[index], formatId)};
    if (parsResult.HasValue())
    {
      certVector[index] = std::move(parsResult).Value();
    }
    else
    {
      return ara::core::Result<void>::FromError(parsResult.Error());
    }
  }

  outcome.resize(certChain.size());

  size_t prevCAIndex{0};

  for (size_t outIndex{0}; outIndex < certVector.size(); outIndex++)
  {
    for (size_t inIndex{0}; inIndex < certVector.size(); inIndex++)
    {
      if (prevCAIndex == 0)
      {
        if (certVector[inIndex]->IssuerDn()->GetDnString() == certVector[inIndex]->SubjectDn()->GetDnString())
        {
          outcome[prevCAIndex++] = std::move(certVector[inIndex]);
        }
      }
      else if (prevCAIndex > 0)
      {
        if (certVector[inIndex] != nullptr)
        {
          if (outcome[prevCAIndex - 1]->SubjectDn()->GetDnString()
              == certVector[inIndex]->IssuerDn()->GetDnString())
          {
            outcome[prevCAIndex++] = std::move(certVector[inIndex]);
          }
        }
      }
    }
    if (prevCAIndex == certVector.size())
    {
      break;
    }
  }

  return ara::core::Result<void>::FromValue();
}

Certificate::Status X509ProviderImpl::VerifyCert(Certificate& cert, Certificate::Uptr myRoot) noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::VerifyCert";

  X509_STORE* store{X509_STORE_new()};
  X509_STORE_CTX* vrfy_ctx{X509_STORE_CTX_new()};

  // Take openssl certificate object. GetCertificate() is an internal API which is to
  // be used by Crypto-daemon only and not to be exposed to the application
  X509* certificate{cert.GetCertificate()};
  X509* caCertificate{nullptr};
  uint8_t const kNoTrust{0};
  uint8_t const kTrust{1};
  uint8_t certVerificationType{kNoTrust};

  Certificate::Status currentStatus{Certificate::Status::kInvalid};

  if (myRoot == nullptr)
  {
    aralog_.LogDebug() << "Given certificate is a root certificate";

    for (ara::core::Map<ara::core::String, CertificateImpl>::reference& it : caCertList_)
    {
      caCertificate = it.second.GetCertificate();
      if (it.second.SubjectDn()->GetDnString() == cert.IssuerDn()->GetDnString())
      {
        if (X509_STORE_add_cert(store, caCertificate) == 0)
        {
          aralog_.LogError() << "Error in adding the certificate to the store";
        }
        certVerificationType = kTrust;
      }
    }

    if (cert.IssuerDn()->GetDnString() == cert.SubjectDn()->GetDnString())
    {
      if (X509_STORE_add_cert(store, certificate) == 0)
      {
        aralog_.LogError() << "Error in adding the certificate to the store";
      }
      certVerificationType = kNoTrust;
    }
  }
  else
  {
    aralog_.LogDebug() << "Give certificate is signed by CA";
    caCertificate = myRoot->GetCertificate();
    if (X509_STORE_add_cert(store, caCertificate) == 0)
    {
      aralog_.LogError() << "Error in adding the certificate to the store";
    }
    certVerificationType = kTrust;
  }

  X509_STORE_CTX_init(vrfy_ctx, store, certificate, NULL);

  int32_t ret = X509_verify_cert(vrfy_ctx);

  if (ret == 0)
  {
    aralog_.LogDebug() << "Certification validation result is Not Ok";
  }
  else
  {
    aralog_.LogDebug() << "Certificate validation result is Ok";
    currentStatus = Certificate::Status::kValid;
  }

  time_t currentTime{std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())};

  if (cert.StartTime() > currentTime)
  {
    aralog_.LogDebug() << "Certificate will be valid in future";
    currentStatus = Certificate::Status::kFuture;
  }
  else if (cert.EndTime() < currentTime)
  {
    aralog_.LogDebug() << "Certificate is expired";
    currentStatus = Certificate::Status::kExpired;
  }
  else if (certVerificationType == kNoTrust)
  {
    aralog_.LogDebug() << "Certificate do not have trusted root certificate";
    currentStatus = Certificate::Status::kNoTrust;
  }

  cert.SetStatus(currentStatus);

  return currentStatus;
}

Certificate::Status X509ProviderImpl::VerifyCertChain(
    ara::core::Span<const Certificate::Uptr> chain, Certificate::Uptr myRoot) const noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::VerifyCertChain";

  X509_STORE* store{X509_STORE_new()};
  X509_STORE_CTX* vrfyctx{X509_STORE_CTX_new()};

  // Take openssl certificate object. GetCertificate() is an internal API which is to
  // be used by Crypto-daemon only and not to be exposed to the application
  X509* certificate{nullptr};
  Certificate::Status currentStatus{Certificate::Status::kInvalid};

  if (myRoot != nullptr)
  {
    X509* caCertificate{myRoot->GetCertificate()};
    if (X509_STORE_add_cert(store, caCertificate) == 0)
    {
      aralog_.LogError() << "Error in adding the certificate to the store";
    }
  }
  else
  {
    aralog_.LogDebug() << "My root is NULL";
    for (ara::core::Map<ara::core::String, CertificateImpl>::const_reference& it : caCertList_)
    {
      X509* caCertificate{it.second.GetCertificate()};
      if (X509_STORE_add_cert(store, caCertificate) == 0)
      {
        aralog_.LogError() << "Error in adding the certificate to the store";
      }
    }
    certificate = chain[0]->GetCertificate();

    if (X509_STORE_add_cert(store, certificate) == 0)
    {
      aralog_.LogError() << "Error in adding the certificate to the store";
    }
  }

  aralog_.LogDebug() << "Cerchain size is " << chain.size();

  for (size_t index{0}; index < chain.size(); index++)
  {
    aralog_.LogDebug() << "Getting Certificate for index " << index;
    if (chain[index] == nullptr)
    {
      aralog_.LogError() << "This is nullptr";
    }
    certificate = chain[index]->GetCertificate();
    time_t currentTime{std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())};
    aralog_.LogDebug() << "Got Certificate for index " << index;
    X509_STORE_CTX_init(vrfyctx, store, certificate, NULL);
    int32_t ret = X509_verify_cert(vrfyctx);
    if (ret != 0)
    {
      currentStatus = Certificate::Status::kValid;
      aralog_.LogDebug() << "Certificate validation result is Ok";

      if (X509_STORE_add_cert(store, certificate) == 0)
      {
        aralog_.LogError() << "Error in adding the certificate to the store";
      }
    }
    else
    {
      aralog_.LogDebug() << "Certification validation result is Not Ok";
    }
    if (chain[index]->StartTime() > currentTime)
    {
      aralog_.LogDebug() << "Certificate will be valid in future";
      currentStatus = Certificate::Status::kFuture;
    }
    else if (chain[index]->EndTime() < currentTime)
    {
      aralog_.LogDebug() << "Certificate is expired";
      currentStatus = Certificate::Status::kExpired;
    }
    chain[index]->SetStatus(currentStatus);
    if (currentStatus != Certificate::Status::kValid)
    {
      break;
    }
  }

  return currentStatus;
}

ara::core::Result<CertSignRequest::Uptrc> X509ProviderImpl::ParseCertSignRequest(
    ReadOnlyMemRegion csr, bool withMetaData) noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::ParseCertSignRequest()";
  X509_REQ* csReq{nullptr};

  if ((withMetaData == true))
  {
    // Here we are writing PEM encoded data in BIO, and read from the BIO using PEM decoding mechanism.
    aralog_.LogDebug() << "Given format Id is PEM ecoded format";
    BIO* certBio = BIO_new(BIO_s_mem());
    BIO_write(certBio, csr.data(), csr.size());
    csReq = PEM_read_bio_X509_REQ(certBio, nullptr, 0, NULL);
    if (csReq == nullptr)
    {
      aralog_.LogError() << "Error in parsing the certificate signing request in PEM format";
      const uint8_t* csrAddr{csr.data()};
      csReq = d2i_X509_REQ(NULL, &csrAddr, csr.size());
      if (csReq == nullptr)
      {
        // According to SWS_CRYPT_40634  if the provided buffer does not contain the expected format
        // then ParseCertSignRequest should return SecurityErrorDomain::kUnsupportedFormat.
        aralog_.LogError() << "Error in parsing the certificate signing request in DER format";
        return ara::core::Result<CertSignRequest::Uptrc>::FromError(
            ara::crypto::SecurityErrorDomain::Errc::kUnsupportedFormat);
      }
    }
  }
  else
  {
    // Here we are decoding the DER encoded data and storing it in X509 certificate request.
    aralog_.LogDebug() << "Given format Id is DER ecoded format";
    const uint8_t* csrAddr{csr.data()};
    csReq = d2i_X509_REQ(NULL, &csrAddr, csr.size());
    if (csReq == nullptr)
    {
      // According to SWS_CRYPT_40634  if the provided buffer does not contain the expected format
      // then ParseCertSignRequest should return SecurityErrorDomain::kUnsupportedFormat.
      aralog_.LogError() << "Error in parsing the certificate signing request in DER format";
      return ara::core::Result<CertSignRequest::Uptrc>::FromError(
          ara::crypto::SecurityErrorDomain::Errc::kUnsupportedFormat);
    }
  }

  aralog_.LogDebug() << "Certificate signing request parsed successfully";
  CertSignRequest::Uptrc localPtr{std::make_unique<CertSignRequestImpl>(csReq)};

  return ara::core::Result<CertSignRequest::Uptrc>::FromValue(std::move(localPtr));
}

ara::core::Result<CertSignRequest::Uptrc> X509ProviderImpl::CreateCertSignRequest(
    cryp::SignerPrivateCtx::Uptr signerCtx, ReadOnlyMemRegion derSubjectDN, ReadOnlyMemRegion x509Extensions,
    unsigned version) const noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::CreateCertSignRequest";

  static_cast<void>(x509Extensions);

  X509_REQ* csr{X509_REQ_new()};

  version -= 1;

  X509_REQ_set_version(csr, version);

  X509_NAME* name{X509_NAME_new()};

  const uint8_t* subjectDn{derSubjectDN.data()};

  d2i_X509_NAME(&name, &subjectDn, derSubjectDN.size());

  X509_REQ_set_subject_name(csr, name);

  EVP_PKEY* privKey{signerCtx->GetPrivateKey()};

  X509_REQ_set_pubkey(csr, privKey);

  if (x509Extensions.size_bytes())
  {
    const unsigned char* tempBuffer{x509Extensions.data()};
    X509_EXTENSIONS* x509Exts = d2i_X509_EXTENSIONS(nullptr, &tempBuffer, x509Extensions.size_bytes());

    X509_REQ_add_extensions(csr, x509Exts);
  }

  X509_REQ_sign(csr, privKey, EVP_sha256());

  CertSignRequest::Uptrc certReq{std::make_unique<CertSignRequestImpl>(csr)};

  return ara::core::Result<CertSignRequest::Uptrc>::FromValue(std::move(certReq));
}

ara::core::Result<bool> X509ProviderImpl::ImportCrl(ReadOnlyMemRegion crl) noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::ImportCrl";
  // Here we are writing PEM encoded data in BIO, and read from the BIO using PEM decoding mechanism.
  BIO* crlBio = BIO_new(BIO_s_mem());
  BIO_write(crlBio, crl.data(), crl.size());
  X509_CRL* certRevList = PEM_read_bio_X509_CRL(crlBio, nullptr, 0, NULL);
  if (certRevList == nullptr)
  {
    aralog_.LogError() << "Error in parsing the certificate revocation list in PEM format";
    const uint8_t* crlAddr{crl.data()};
    certRevList = d2i_X509_CRL(NULL, &crlAddr, crl.size());
    if (certRevList == nullptr)
    {
      // According to SWS_CRYPT_40634  if the provided buffer does not contain the expected format
      // then ParseCertSignRequest should return SecurityErrorDomain::kUnsupportedFormat.
      aralog_.LogError() << " Given memory region does not contain a CRL";
      return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrorDomain::Errc::kUnexpectedValue);
    }
  }

  const ASN1_TIME* nextUpdateTime{X509_CRL_get0_nextUpdate(certRevList)};

  time_t nextUpdateTimeInTimet{ASN1_GetTimeT(nextUpdateTime)};

  time_t currentTime{std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())};

  if (currentTime > nextUpdateTimeInTimet)
  {
    aralog_.LogDebug() << "Given CRL is not valid any more!!!!";
    aralog_.LogDebug() << "Please Update the CRL";
    return ara::core::Result<bool>::FromValue(false);
  }

  X509DNImpl issuerName{X509_CRL_get_issuer(certRevList)};
  Certificate::Uptrc IssuerCert{nullptr};
  for (Certificate::Uptr& certInStorage : certList_)
  {
    if (issuerName.GetDnString().Value() == certInStorage->SubjectDn()->GetDnString().Value())
    {
      IssuerCert = std::make_unique<CertificateImpl>(certInStorage->GetCertificate());
      break;
    }
  }

  if (IssuerCert == nullptr)
  {
    for (ara::core::Map<ara::core::InstanceSpecifier, CertificateImpl>::const_reference& it : dbCertList_)
    {
      if (issuerName.GetDnString().Value() == it.second.SubjectDn()->GetDnString().Value())
      {
        IssuerCert = std::make_unique<CertificateImpl>(it.second.GetCertificate());
        break;
      }
    }
  }

  if (IssuerCert == nullptr)
  {
    aralog_.LogError() << "Issuer certificate not found";
    return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrorDomain::Errc::kRuntimeFault);
  }

  X509* certPtr{IssuerCert->GetCertificate()};

  EVP_PKEY* key{X509_get_pubkey(certPtr)};

  if (X509_CRL_verify(certRevList, key) == false)
  {
    aralog_.LogError() << "Given CRL is not valid";
    return ara::core::Result<bool>::FromError(ara::crypto::SecurityErrorDomain::Errc::kRuntimeFault);
  }
  else
  {
    aralog_.LogDebug() << "Given CRL is valid";
  }

  STACK_OF(X509_REVOKED) * revokedList{X509_CRL_get_REVOKED(certRevList)};

  for (int index{0}; index < sk_X509_REVOKED_num(revokedList); index++)
  {
    X509_REVOKED* entry = sk_X509_REVOKED_value(revokedList, index);

    const ASN1_INTEGER* serNoAsn1{X509_REVOKED_get0_serialNumber(entry)};

    // Here X509_get_serialNumber ASN1_INTEGER which is an openssl structure. To convert that into printable
    // format we have to do some conversions. Here we are converting ASN1_INTEGER to BIGNUM.
    BIGNUM* opensslBigNum = ASN1_INTEGER_to_BN(serNoAsn1, NULL);

    if (opensslBigNum != 0x00)
    {
      aralog_.LogError() << "Failed to get the Big Number";
    }

    // Here we are converting BIGNUM to ASCII decimal numbers.
    char* serNoDec = BN_bn2dec(opensslBigNum);

    std::stringstream printSerialNo;
    for (size_t printindex = 0; printindex < strlen(serNoDec); printindex++)
    {
      printSerialNo << (serNoDec[printindex]);
    }

    aralog_.LogDebug() << printSerialNo.str();

    ara::core::Vector<uint8_t> serNumResult{serNoDec, serNoDec + strlen(serNoDec)};
    bool isCertFound{false};

    BN_free(opensslBigNum);
    OPENSSL_free(serNoDec);

    for (ara::core::Vector<Certificate::Uptr>::iterator it{certList_.begin()}; it != certList_.end(); it++)
    {
      ara::core::Vector<ara::core::Byte> serNoByte{(*it)->SerialNumber().Value()};
      ara::core::Vector<uint8_t> serNo{serNoByte.begin(), serNoByte.end()};
      if (serNo == serNumResult)
      {
        aralog_.LogDebug() << "Certificate found in CRL";
        isCertFound = true;
        (*it)->SetStatus(Certificate::Status::kInvalid);
        break;
      }
    }

    if (isCertFound == false)
    {
      for (ara::core::Map<ara::core::InstanceSpecifier, CertificateImpl>::iterator it{dbCertList_.begin()};
           it != dbCertList_.end(); it++)
      {
        ara::core::Vector<ara::core::Byte> serNoByte{it->second.SerialNumber().Value()};
        ara::core::Vector<uint8_t> serNo{serNoByte.begin(), serNoByte.end()};

        if (serNo == serNumResult)
        {
          aralog_.LogDebug() << "Certificate found in CRL";
          it->second.SetStatus(Certificate::Status::kInvalid);
          ara::core::String ispcfr{it->first.ToString().data()};
          UpdateStatusValue(it->second.GetStatus(), ispcfr);
          break;
        }
      }
    }
  }

  crlList_.emplace_back(certRevList);

  if (IstableEmpty(certdb::kCrlDb) == true)
  {
    CreateTableInDB(certdb::kCrlDb);
  }

  BIO* dbBio = BIO_new(BIO_s_mem());

  PEM_write_bio_X509_CRL(dbBio, certRevList);

  int dblen{BIO_pending(dbBio)};

  ara::core::Vector<uint8_t> dbData;

  dbData.resize(dblen);

  BIO_read(dbBio, dbData.data(), dblen);

  ReadOnlyMemRegion dataVal{dbData.data(), dbData.size()};

  static char id[]{"000000"};

  for (size_t index{0}; index < sizeof(id); index++)
  {
    if (id[sizeof(id) - index - 2] == '9')
    {
      id[sizeof(id) - index - 2] = '0';
      index++;
    }
    else
    {
      id[sizeof(id) - index - 2]++;
      break;
    }
  }

  ara::core::String keyValue{id};

  if (WriteDataInDB("CertRevocationList (Id, CRL, Status)", keyValue, dataVal, Certificate::Status::kValid,
          certdb::kCrlDb)
      == false)
  {
    return ara::core::Result<bool>::FromValue(false);
  }

  return ara::core::Result<bool>::FromValue(true);
}

ara::core::Result<void> X509ProviderImpl::Import(
    const Certificate& cert, ara::core::Optional<ara::core::InstanceSpecifier> iSpecify) noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::Import";

  if (cert.GetStatus() != Certificate::Status::kValid)
  {
    aralog_.LogError() << "Certificate status is not valid";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
  }

  for (Certificate::Uptr& certInStorage : certList_)
  {
    if (certInStorage->SerialNumber() == cert.SerialNumber())
    {
      return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kContentDuplication);
    }
  }

  for (ara::core::Map<ara::core::InstanceSpecifier, CertificateImpl>::const_reference& it : dbCertList_)
  {
    if (it.second.SerialNumber() == cert.SerialNumber())
    {
      return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kContentDuplication);
    }
  }

  if (iSpecify.has_value() == false)
  {
    Certificate::Uptr localPtr{std::make_unique<CertificateImpl>(cert.GetCertificate())};
    certList_.push_back(std::move(localPtr));
  }
  else
  {
    dbCertList_.insert({*iSpecify, cert.GetCertificate()});

    ara::core::String ispcfr{(*iSpecify).ToString().data()};

    SetObjectValue(cert, ispcfr, false);
  }

  return ara::core::Result<void>::FromValue();
}

Certificate::Uptrc X509ProviderImpl::FindCertBySn(ReadOnlyMemRegion sn, const X509DN& issuerDn) noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::FindCertBySn";
  Certificate::Uptrc localPtr{nullptr};
  for (Certificate::Uptr& certInStorage : certList_)
  {
    ara::core::Vector<ara::core::Byte> serNoByte{certInStorage->SerialNumber().Value()};
    ara::core::Vector<uint8_t> serNo{serNoByte.begin(), serNoByte.end()};
    ara::core::Vector<uint8_t> SerNoToBeSearched{sn.begin(), sn.end()};

    if ((SerNoToBeSearched == serNo)
        && (issuerDn.GetDnString().Value() == certInStorage->IssuerDn()->GetDnString().Value()))
    {
      localPtr = std::make_unique<CertificateImpl>(certInStorage->GetCertificate());
      break;
    }
  }

  if (localPtr == nullptr)
  {
    for (ara::core::Map<ara::core::InstanceSpecifier, CertificateImpl>::const_reference& it : dbCertList_)
    {
      ara::core::Vector<ara::core::Byte> serNoByte{it.second.SerialNumber().Value()};
      ara::core::Vector<uint8_t> serNo{serNoByte.begin(), serNoByte.end()};
      ara::core::Vector<uint8_t> SerNoToBeSearched{sn.begin(), sn.end()};

      if ((SerNoToBeSearched == serNo)
          && (issuerDn.GetDnString().Value() == it.second.IssuerDn()->GetDnString().Value()))
      {
        localPtr = std::make_unique<CertificateImpl>(it.second.GetCertificate());
        break;
      }
    }
  }

  return localPtr;
}

Certificate::Uptrc X509ProviderImpl::FindCertByDn(const X509DN& subjectDn, const X509DN& issuerDn,
    time_t validityTimePoint, StorageIndex& certIndex) noexcept
{
  static_cast<void>(certIndex);
  aralog_.LogDebug() << "In X509ProviderImpl::FindCertByDn";
  Certificate::Uptrc localPtr{nullptr};
  for (Certificate::Uptr& certInStorage : certList_)
  {
    if ((subjectDn.GetDnString().Value() == certInStorage->SubjectDn()->GetDnString().Value())
        && (issuerDn.GetDnString().Value() == certInStorage->IssuerDn()->GetDnString().Value())
        && (certInStorage->EndTime() == validityTimePoint))
    {
      localPtr = std::make_unique<CertificateImpl>(certInStorage->GetCertificate());
      break;
    }
  }

  if (localPtr == nullptr)
  {
    for (ara::core::Map<ara::core::InstanceSpecifier, CertificateImpl>::const_reference& it : dbCertList_)
    {
      if ((subjectDn.GetDnString().Value() == it.second.SubjectDn()->GetDnString().Value())
          && (issuerDn.GetDnString().Value() == it.second.IssuerDn()->GetDnString().Value())
          && (it.second.EndTime() == validityTimePoint))
      {
        localPtr = std::make_unique<CertificateImpl>(it.second.GetCertificate());
        break;
      }
    }
  }

  return localPtr;
}

Certificate::Uptrc X509ProviderImpl::FindCertByKeyIds(
    ReadOnlyMemRegion subjectKeyId, ReadOnlyMemRegion authorityKeyId) noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::FindCertBySn";
  Certificate::Uptrc localPtr{nullptr};
  for (Certificate::Uptr& certInStorage : certList_)
  {
    ara::core::Vector<ara::core::Byte> subKeyByte{certInStorage->SubjectKeyId().Value()};
    ara::core::Vector<uint8_t> subKey{subKeyByte.begin(), subKeyByte.end()};
    ara::core::Vector<uint8_t> subKeyToBeSearched{subjectKeyId.begin(), subjectKeyId.end()};

    ara::core::Vector<ara::core::Byte> authKeyByte{certInStorage->AuthorityKeyId().Value()};
    ara::core::Vector<uint8_t> authKey{authKeyByte.begin(), authKeyByte.end()};
    ara::core::Vector<uint8_t> authKeyToBeSearched{authorityKeyId.begin(), authorityKeyId.end()};

    if ((subKeyToBeSearched == subKey) && ((authorityKeyId.size() == 0) || (authKeyToBeSearched == authKey)))
    {
      localPtr = std::make_unique<CertificateImpl>(certInStorage->GetCertificate());
      break;
    }
  }

  if (localPtr == nullptr)
  {
    for (ara::core::Map<ara::core::InstanceSpecifier, CertificateImpl>::const_reference& it : dbCertList_)
    {
      ara::core::Vector<ara::core::Byte> subKeyByte{it.second.SubjectKeyId().Value()};
      ara::core::Vector<uint8_t> subKey{subKeyByte.begin(), subKeyByte.end()};
      ara::core::Vector<uint8_t> subKeyToBeSearched{subjectKeyId.begin(), subjectKeyId.end()};

      ara::core::Vector<ara::core::Byte> authKeyByte{it.second.AuthorityKeyId().Value()};
      ara::core::Vector<uint8_t> authKey{authKeyByte.begin(), authKeyByte.end()};
      ara::core::Vector<uint8_t> authKeyToBeSearched{authorityKeyId.begin(), authorityKeyId.end()};

      if ((subKeyToBeSearched == subKey)
          && ((authorityKeyId.size() == 0) || (authKeyToBeSearched == authKey)))
      {
        localPtr = std::make_unique<CertificateImpl>(it.second.GetCertificate());
        break;
      }
    }
  }

  return localPtr;
}

bool X509ProviderImpl::Remove(Certificate::Uptrc&& cert) noexcept
{
  aralog_.LogDebug() << "In X509ProviderImpl::FindCertBySn";
  bool isCertFound{false};

  for (ara::core::Vector<Certificate::Uptr>::iterator it{certList_.begin()}; it != certList_.end(); it++)
  {
    ara::core::Vector<ara::core::Byte> serNoByte{(*it)->SerialNumber().Value()};
    ara::core::Vector<uint8_t> serNo{serNoByte.begin(), serNoByte.end()};
    ara::core::Vector<ara::core::Byte> serNoToBeSearchedInByte{cert->SerialNumber().Value()};
    ara::core::Vector<uint8_t> serNoToBeSearched{
        serNoToBeSearchedInByte.begin(), serNoToBeSearchedInByte.end()};
    if (serNo == serNoToBeSearched)
    {
      isCertFound = true;
      certList_.erase(it);
      break;
    }
  }

  if (isCertFound == false)
  {
    for (ara::core::Map<ara::core::InstanceSpecifier, CertificateImpl>::iterator it{dbCertList_.begin()};
         it != dbCertList_.end(); it++)
    {
      ara::core::Vector<ara::core::Byte> serNoByte{it->second.SerialNumber().Value()};
      ara::core::Vector<uint8_t> serNo{serNoByte.begin(), serNoByte.end()};
      ara::core::Vector<ara::core::Byte> serNoToBeSearchedInByte{cert->SerialNumber().Value()};
      ara::core::Vector<uint8_t> serNoToBeSearched{
          serNoToBeSearchedInByte.begin(), serNoToBeSearchedInByte.end()};

      if (serNo == serNoToBeSearched)
      {
        ara::core::String ispcfr{it->first.ToString().data()};
        aralog_.LogDebug() << "InstanceSpecifier is " << ispcfr.c_str();
        DeleteCertFromDB("CryptoCertificates", "InstanceSpecifier", ispcfr, certdb::kCertDb);
        dbCertList_.erase(it);
        isCertFound = true;
        break;
      }
    }
  }

  if (isCertFound == true)
  {
    Certificate::Uptrc::pointer certPtr = cert.release();
    delete certPtr;
  }

  return isCertFound;
}

void X509ProviderImpl::CleanupVolatileStorage() noexcept
{
  aralog_.LogInfo() << "In X509ProviderImpl::CleanupVolatileStorage()";
  for (ara::core::Map<ara::core::InstanceSpecifier, CertificateImpl>::iterator it{dbCertList_.begin()};
       it != dbCertList_.end(); it++)
  {
    ara::core::String ispcfr{it->first.ToString().data()};
    DeleteCertFromDB("CryptoCertificates", "InstanceSpecifier", ispcfr, certdb::kCertDb);
  }
}

ara::core::Result<void> X509ProviderImpl::SetAsRootOfTrust(const Certificate& caCert) noexcept
{
  aralog_.LogDebug() << "X509ProviderImpl::SetAsRootOfTrust()";

  if (IstableEmpty(1) == true)
  {
    CreateTableInDB(1);
  }

  if (caCert.GetStatus() != Certificate::Status::kValid)
  {
    aralog_.LogError() << "Given certificate is not a valid certificate";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
  }
  else
  {
    aralog_.LogError() << "Given certificate is a valid certificate";
  }

  if (caCert.IsCa() == false)
  {
    aralog_.LogError() << "Given certificate is not a CA certificate";
    return ara::core::Result<void>::FromError(ara::crypto::SecurityErrorDomain::Errc::kIncompatibleObject);
  }
  else
  {
    aralog_.LogError() << "Given certificate is a CA certificate";
  }

  ara::core::Vector<ara::core::Byte> SerNoInByte{caCert.SerialNumber().Value()};
  ara::core::Vector<uint8_t> tempBuffer{SerNoInByte.begin(), SerNoInByte.end()};
  ara::core::String serNo{reinterpret_cast<const char*>(tempBuffer.data())};

  SetObjectValue(caCert, serNo, true);

  return ara::core::Result<void>::FromValue();
}

ara::core::Result<OcspRequest::Uptrc> X509ProviderImpl::CreateOcspRequest(
      const Certificate& cert, const cryp::SignerPrivateCtx::Uptr signer) noexcept
{
  aralog_.LogDebug() << "X509ProviderImpl::CreateOcspRequest()";

  /*if(cert.GetStatus() != Certificate::Status::kValid)
  {
    aralog_.LogError() << "Given Certificate is not valid!!!!";
    return ara::core::Result<OcspRequest::Uptrc>::FromError
    (ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
  }*/

  OCSP_REQUEST *request{ OCSP_REQUEST_new() };

  X509* certPtr{ cert.GetCertificate() };

  OCSP_CERTID *certId { OCSP_cert_id_new(EVP_sha256(),
                               X509_get_issuer_name(certPtr),
                               X509_get0_authority_key_id(certPtr),
                               X509_get_serialNumber(certPtr)) };
  
  if(certId != nullptr)
  {
    if(OCSP_request_add0_id(request, certId) == nullptr)
    {
      aralog_.LogError() << "Error in adding cert Id";
    }
  }
  else
  {
    aralog_.LogError() << "Could not get the CertId!!!!!";
    return ara::core::Result<OcspRequest::Uptrc>::FromError
    (ara::crypto::SecurityErrorDomain::Errc::kInvalidArgument);
  }

  /*if(signer.has_value() == true)
  {
    if(signer->IsInitialized() == true)
    {
      aralog_.LogError() << "signer context is not initialized by a key!!!!";
      return ara::core::Result<OcspRequest::Uptrc>::FromError
      (ara::crypto::SecurityErrorDomain::Errc::kIncompleteArgState);
    }

    int32_t result{ OCSP_request_sign(request, certPtr, signer->GetPrivateKey(), EVP_sha256(), nullptr, 
    OCSP_NOCERTS ) };

    if(result == 0)
    {
      aralog_.LogError() << "Signing failed for this request";
    }
  }*/

  static_cast<void>(signer);

  OcspRequest::Uptrc localPtr{ std::make_unique<OcspRequestImpl>(request)};

  return ara::core::Result<OcspRequest::Uptrc>::FromValue(std::move(localPtr));
}

OcspResponse::Uptrc X509ProviderImpl::ParseOcspResponse(ReadOnlyMemRegion response) const noexcept
{
  const uint8_t* responseAddr{ response.data() };
  OCSP_RESPONSE* responsePtr{ d2i_OCSP_RESPONSE(NULL, &responseAddr, response.size()) };
  if (responsePtr == nullptr)
  {
    // According to SWS_CRYPT_40634  if the provided buffer does not contain the expected format
    // then ParseCertSignRequest should return SecurityErrorDomain::kUnsupportedFormat.
    aralog_.LogError() << "Error in parsing ocsp response";
    /*return ara::core::Result<CertSignRequest::Uptrc>::FromError(
      ara::crypto::SecurityErrorDomain::Errc::kUnexpectedValue);*/
  }

  OcspResponse::Uptrc locapPtr{ std::make_unique<OcspResponseImpl>(responsePtr) };

  return locapPtr;
}

bool X509ProviderImpl::CreateTableInDB(uint8_t dbId) noexcept
{
  ara::core::String sqlCommand;

  if (dbId == certdb::kCertDb)
  {
    sqlCommand += "CREATE TABLE CryptoCertificates("  \
      "InstanceSpecifier TEXT PRIMARY KEY     NOT NULL," \
      "Certificate           TEXT          ," \
      "Status                INT    );";
  }
  else if (dbId == certdb::kCaCertDb)
  {
    sqlCommand += "CREATE TABLE CryptoCertificates("  \
      "SerialNumber TEXT PRIMARY KEY     NOT NULL," \
      "Certificate           TEXT          ," \
      "Status                INT    );";
  }
  else if (dbId == certdb::kCrlDb)
  {
    sqlCommand += "CREATE TABLE CertRevocationList("  \
      "Id TEXT PRIMARY KEY     NOT NULL," \
      "CRL           TEXT          ," \
      "Status                INT    );";
  }

  return CreateTableInSqliteDb(stDBStore[dbId], sqlCommand);
}

bool X509ProviderImpl::CreateTableInSqliteDb(sqlite3* db, ara::core::String sqlCommand) noexcept
{
  char* errorMsg;
  aralog_.LogDebug() << "In X509ProviderImpl::CreateTableInSqliteDb()";
  int32_t rc{sqlite3_exec(db, sqlCommand.data(), nullptr, 0, &errorMsg)};

  if (rc > 0)
  {
    aralog_.LogError() << "Can not create table in DB";
    return false;
  }
  else
  {
    aralog_.LogDebug() << "created table in DB";
    return true;
  }
}

void X509ProviderImpl::DeleteCertFromDB(ara::core::String tableName, ara::core::String keyValueName,
    ara::core::String keyValue, uint8_t dbId) noexcept
{
  ara::core::String command{"DELETE FROM "};
  ara::core::String argumentEnd{"';"};

  command += tableName + " WHERE " + keyValueName + " = '";
  ara::core::String sqlquery(command + keyValue + argumentEnd);

  aralog_.LogDebug() << "Formed query is " << sqlquery.c_str();

  DeleteDataFromSqliteTable(stDBStore[dbId], sqlquery);
}

void X509ProviderImpl::DeleteDataFromSqliteTable(sqlite3* db, ara::core::String sqlquery) noexcept
{
  aralog_.LogDebug() << "X509ProviderImpl::DeleteDataFromSqliteTable()";

  char* errorMsg;
  int rc = sqlite3_exec(db, sqlquery.data(), NULL, 0, &errorMsg);

  if (rc)
  {
    aralog_.LogError() << "Can't delete Value " << errorMsg;
  }
}

void X509ProviderImpl::ReadCertificatesFromDB() noexcept
{
  ara::core::String command{"SELECT * FROM CryptoCertificates"};
  char* errorMsg;

  bool opComplete{false};
  std::mutex sqliteOp;

  int rc = sqlite3_exec(stDBStore[certdb::kCertDb], command.data(), this->callback, &dbCertList_, &errorMsg);

  if (rc)
  {
    aralog_.LogError() << "Can't select Value " << errorMsg;
  }
  else
  {
    while (!opComplete)
    {
      std::unique_lock<std::mutex> lk(sqliteOp);
      if (cndVar_.wait_for(lk, std::chrono::milliseconds(1000)) == std::cv_status::timeout)
      {
        aralog_.LogDebug() << "DataBase Read Completely";
        opComplete = true;
      }
      else
      {
        aralog_.LogDebug() << "DataBase Read In Progress";
      }
      readComplete = false;
    }
  }
}

void X509ProviderImpl::ReadCACertificatesFromDB() noexcept
{
  ara::core::String command{"SELECT * FROM CryptoCertificates"};
  char* errorMsg;

  bool opComplete{false};
  std::mutex sqliteOp;

  int rc
      = sqlite3_exec(stDBStore[certdb::kCaCertDb], command.data(), this->callback, &caCertList_, &errorMsg);

  if (rc)
  {
    aralog_.LogError() << "Can't select Value " << errorMsg;
  }
  else
  {
    while (!opComplete)
    {
      std::unique_lock<std::mutex> lk(sqliteOp);
      if (cndVar_.wait_for(lk, std::chrono::milliseconds(1000)) == std::cv_status::timeout)
      {
        aralog_.LogDebug() << "CA DataBase Read Completely";
        opComplete = true;
      }
      else
      {
        aralog_.LogDebug() << "CA DataBase Read In Progress";
      }
      readComplete = false;
    }
  }
}

void X509ProviderImpl::ReadCRLsFromDB() noexcept
{
  ara::core::String command{"SELECT * FROM CertRevocationList"};
  char* errorMsg;

  bool opComplete{false};
  std::mutex sqliteOp;

  int rc = sqlite3_exec(stDBStore[certdb::kCrlDb], command.data(), this->callback, &crlList_, &errorMsg);

  if (rc)
  {
    aralog_.LogError() << "Can't select Value " << errorMsg;
  }
  else
  {
    while (!opComplete)
    {
      std::unique_lock<std::mutex> lk(sqliteOp);
      if (cndVar_.wait_for(lk, std::chrono::milliseconds(1000)) == std::cv_status::timeout)
      {
        aralog_.LogDebug() << "CRL DataBase Read Completely";
        opComplete = true;
      }
      else
      {
        aralog_.LogDebug() << "CRL DataBase Read In Progress";
      }
      readComplete = false;
    }
  }
}

bool X509ProviderImpl::UpdateStatusValue(Certificate::Status certStatus, ara::core::String iSpecify) noexcept
{
  aralog_.LogDebug() << "Update data";
  ara::core::String command{"UPDATE CryptoCertificates SET Status = "};
  ara::core::String subCommand1{" WHERE  InstanceSpecifier = "};
  ara::core::String InstanceSpecifier{"'"};
  ara::core::String InstanceSpecifierEnd{"'"};
  ara::core::String argumentEnd{";"};

  InstanceSpecifier += iSpecify;

  std::stringstream statusValue;

  statusValue << static_cast<std::uint32_t>(certStatus);

  ara::core::String sqlquery(
      command + statusValue.str() + subCommand1 + InstanceSpecifier + InstanceSpecifierEnd + argumentEnd);

  return StoreDataInSqlite(stDBStore[certdb::kCertDb], sqlquery);
}

bool X509ProviderImpl::SetObjectValue(const Certificate& cert, ara::core::String keyValue, bool isCa) noexcept
{
  BIO* certbio{BIO_new(BIO_s_mem())};

  PEM_write_bio_X509(certbio, cert.GetCertificate());

  int certLen = BIO_pending(certbio);

  ara::core::Vector<uint8_t> certVal;

  certVal.resize(certLen);

  BIO_read(certbio, certVal.data(), certLen);

  ReadOnlyMemRegion dataVal{certVal.data(), certVal.size()};

  if (isCa == true)
  {
    return WriteDataInDB("CryptoCertificates (SerialNumber, Certificate, Status)", keyValue, dataVal,
        cert.GetStatus(), certdb::kCaCertDb);
  }
  else
  {
    return WriteDataInDB("CryptoCertificates (InstanceSpecifier, Certificate, Status)", keyValue, dataVal,
        cert.GetStatus(), certdb::kCertDb);
  }
}

bool X509ProviderImpl::WriteDataInDB(ara::core::String tableInfo, ara::core::String keyValue,
    ReadOnlyMemRegion dataValue, Certificate::Status statusData, uint8_t dbId) noexcept
{
  static_cast<void>(dbId);
  ara::core::String command{"INSERT INTO "};
  ara::core::String subCommandStart{"VALUES "};
  ara::core::String subCommandBody{"('"};
  ara::core::String subCommandEnd{"', "};
  ara::core::String commandEnd{");"};
  ara::core::String data{"'"};

  command += tableInfo;

  subCommandBody += keyValue;

  for (size_t index = 0; index < dataValue.size(); index++)
  {
    data.push_back(dataValue[index]);
  }

  std::stringstream statusValue;

  statusValue << static_cast<std::uint32_t>(statusData);

  ara::core::String sqlquery(command + subCommandStart + subCommandBody + subCommandEnd + data + subCommandEnd
      + statusValue.str() + commandEnd);

  return StoreDataInSqlite(stDBStore[dbId], sqlquery);
}

bool X509ProviderImpl::StoreDataInSqlite(sqlite3* db, ara::core::String sqlquery) noexcept
{
  char* errorMsg;
  int32_t rc{sqlite3_exec(db, sqlquery.data(), NULL, 0, &errorMsg)};

  if (rc)
  {
    aralog_.LogDebug() << "Can't update CA certificate" << errorMsg;
    return false;
  }
  else
  {
    aralog_.LogDebug() << "CA Certificate Updated successfully";
  }

  return true;
}

bool X509ProviderImpl::IstableEmpty(uint8_t dbId) noexcept
{
  ara::core::String command;

  aralog_.LogDebug() << "In X509ProviderImpl::IstableEmpty()";

  char count;
  char* errorMsg;

  readComplete = false;

  if (dbId < 2)
  {
    command += "SELECT count(*) FROM CryptoCertificates;";
  }
  else
  {
    command += "SELECT count(*) FROM CertRevocationList;";
  }

  int rc{sqlite3_exec(stDBStore[dbId], command.data(), this->IsTableEmptycallback, &count, &errorMsg)};

  if (rc)
  {
    aralog_.LogError() << "Can't select Value " << errorMsg;
  }

  while (!readComplete)
  {
  }

  if (count == '0')
  {
    aralog_.LogDebug() << "Table is empty";
    return true;
  }
  else
  {
    aralog_.LogDebug() << "Table is not empty";
    return false;
  }
}

size_t X509ProviderImpl::GetDBContentLength(ara::core::String& instanceSpecifier) const noexcept
{
  ara::core::String command{"SELECT LENGTH(Certificate) FROM CryptoCertificates WHERE InstanceSpecifier = '"};
  ara::core::String argumentEnd{"';"};
  char* errorMsg;
  size_t length;

  aralog_.LogError() << "Getting length for instanceSpecifier " << instanceSpecifier.c_str();

  ara::core::String sqlquery(command + instanceSpecifier + argumentEnd);

  readComplete = false;

  int rc = sqlite3_exec(stDBStore[certdb::kCertDb], sqlquery.data(), this->callback, &length, &errorMsg);

  if (rc)
  {
    aralog_.LogError() << "Can't select Value " << errorMsg;
  }
  else
  {
    while (!readComplete)
    {
    }
  }

  return length;
}

time_t X509ProviderImpl::ASN1_GetTimeT(const ASN1_TIME* time) noexcept
{
  aralog_.LogDebug() << "In CertificateImpl::ASN1_GetTimeT()";
  struct tm t;
  const char* str = reinterpret_cast<const char*>(time->data);
  size_t i = 0;
  memset(&t, 0, sizeof(t));
  if (time->type == V_ASN1_UTCTIME)
  { /* two digit year */
    aralog_.LogDebug() << "UTCTIME";
    t.tm_year = (str[i++] - '0') * 10;
    t.tm_year += (str[i++] - '0');
    if (t.tm_year < 70)
      t.tm_year += 100;
  }
  else if (time->type == V_ASN1_GENERALIZEDTIME)
  { /* four digit year */
    aralog_.LogDebug() << "GENERALIZEDTIME";
    t.tm_year = (str[i++] - '0') * 1000;
    t.tm_year += (str[i++] - '0') * 100;
    t.tm_year += (str[i++] - '0') * 10;
    t.tm_year += (str[i++] - '0');
    t.tm_year -= 1900;
  }
  t.tm_mon = (str[i++] - '0') * 10;
  // -1 since January is 0 not 1.
  t.tm_mon += (str[i++] - '0') - 1;
  t.tm_mday = (str[i++] - '0') * 10;
  t.tm_mday += (str[i++] - '0');
  t.tm_hour = (str[i++] - '0') * 10;
  t.tm_hour += (str[i++] - '0');
  t.tm_min = (str[i++] - '0') * 10;
  t.tm_min += (str[i++] - '0');
  t.tm_sec = (str[i++] - '0') * 10;
  t.tm_sec += (str[i++] - '0');
  /* Note: we did not adjust the time based on time zone information */
  return mktime(&t);
}

} // namespace x509
} // namespace crypto
} // namespace ara
