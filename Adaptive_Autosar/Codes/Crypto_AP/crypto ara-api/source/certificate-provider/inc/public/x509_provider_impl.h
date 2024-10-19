// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : x509_provider_impl.h
//
// Purpose     : This file contains X.509 API. X509Provider interface definition.
// -----------------------------------------------------------------------------------------------------------

#ifndef CRYPTO_SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_X509_PROVIDER_IMPL_H_
#define CRYPTO_SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_X509_PROVIDER_IMPL_H_

#include <condition_variable>

#include <ara/log/logging.h>
#include <thread>
//#include "ara/crypto/common/serializable.h"
#include "ara/crypto/x509/x509_provider.h"
#include "cert_sign_request_impl.h"
#include "certificate_impl.h"
#include "ocsp_request_impl.h"
#include "ocsp_response_impl.h"

namespace ara
{
namespace crypto
{
namespace x509
{
/// @brief X.509 Provider interface.
/// The X.509 Provider supports two internal storages: volatile (or session) and persistent.
/// All X.509 objects created by the provider should have an actual reference to their parent X.509 Provider.
/// The X.509 Provider can be destroyed only after destroying of all its daughterly objects.
/// Each method of this interface that creates a X.509 object is non-constant, because any such creation
/// increases a references counter of the X.509 Provider.
/// @trace SWS_CRYPT_40600
class X509ProviderImpl : public X509Provider
{
 public:
  /// @brief Constructor for X509ProviderImpl class.
  X509ProviderImpl() noexcept;

  /// @brief Create an empty X.500 Distinguished Name (DN) structure.
  /// If <tt>(0 == capacity)</tt> then a maximally supported (by the implementation) capacity must be
  /// reserved.
  /// @param[in] capacity number of bytes that should be reserved for the content of the target @c X509DN
  /// object
  /// @returns Unique smart pointer to created empty @c X509DN object
  /// @trace SWS_CRYPT_40611
  ara::core::Result<X509DN::Uptr> CreateEmptyDn(std::size_t capacity = 0) noexcept override;

  /// @brief Create completed X.500 Distinguished Name structure from the provided string representation.
  /// @param[in] dn string representation of the Distinguished Name
  /// @returns unique smart pointer for the created @c X509DN object
  /// @error SecurityErrorDomain::kInvalidArgument if the @c dn argument has incorrect format
  /// @error SecurityErrorDomain::kInvalidInputSize if the @c dn argument has unsupported length (too large)
  /// @trace SWS_CRYPT_40612
  ara::core::Result<X509DN::Uptr> BuildDn(ara::core::StringView dn) noexcept override;

  /// @brief Decode X.500 Distinguished Name structure from the provided serialized format.
  /// @param[in] dn  DER/PEM-encoded representation of the Distinguished Name
  /// @param[in] formatId  input format identifier (@c kFormatDefault means auto-detect)
  /// @returns unique smart pointer for the created @c X509DN object
  /// @error SecurityErrorDomain::kInvalidArgument  if the @c dn argument cannot be parsed
  /// @error SecurityErrorDomain::kUnknownIdentifier  if the @c formatId argument has unknown value
  /// @trace SWS_CRYPT_40613
  ara::core::Result<X509DN::Uptr> DecodeDn(
      ReadOnlyMemRegion dn, Serializable::FormatId formatId = Serializable::kFormatDefault) noexcept override;

  /// @brief Parse a serialized representation of the certificate and create its instance.
  /// Off-line validation of the parsed certificate may be done via call @c VerifyCertByCrl().
  /// After validation the certificate may be imported to the session or persistent storage for following
  /// search and usage.
  /// If the parsed certificate is not imported then it will be lost after destroy of the returned instance!
  /// Only imported certificate may be found by a search and applied for automatic verifications!
  /// @param[in] cert DER/PEM-encoded certificate
  /// @param[in] formatId input format identifier (@c kFormatDefault means auto-detect)
  /// @returns unique smart pointer to created certificate
  /// @error SecurityErrorDomain::kInvalidArgument if the @c cert argument cannot be parsed
  /// @error SecurityErrorDomain::kUnknownIdentifier if the @c formatId argument has unknown value
  /// @trace SWS_CRYPT_40614
  ara::core::Result<Certificate::Uptr> ParseCert(ReadOnlyMemRegion cert,
      Serializable::FormatId formatId = Serializable::kFormatDefault) noexcept override;

  /// @brief Count number of certificates in a serialized certificate chain represented by a single BLOB.
  /// @param[in] certChain DER/PEM-encoded certificate chain (in form of a single BLOB)
  /// @param[in] formatId input format identifier (@c kFormatDefault means auto-detect)
  /// @returns number of certificates in the chain
  /// @error SecurityErrorDomain::kInvalidArgument if the @c certChain argument cannot be pre-parsed
  /// @error SecurityErrorDomain::kUnknownIdentifier if the @c formatId argument has unknown value
  /// @trace SWS_CRYPT_40615
  ara::core::Result<std::size_t> CountCertsInChain(ReadOnlyMemRegion certChain,
      Serializable::FormatId formatId = Serializable::kFormatDefault) const noexcept override;

  /// @brief Parse a serialized representation of the certificate chain and create their instances.
  /// Off-line validation of the parsed certification chain may be done via call @c VerifyCertChainByCrl().
  /// After validation the certificates may be saved to the session or persistent storage for following
  /// search and usage. If the certificates are not imported then they will be lost after destroy of the
  /// returned instances!
  /// Only imported certificates may be found by a search and applied for automatic verifications!
  /// Certificates in the @c outcome vector will be placed from the root CA certificate (zero index) to
  /// the final end-entity certificate (last used index of the vector).
  /// @param[out] outcome an output vector for imported certificates
  /// @param[in] certChain DER/PEM-encoded certificate chain (in form of a single BLOB)
  /// @param[in] formatId input format identifier (@c kFormatDefault means auto-detect)
  /// @error SecurityErrorDomain::kInsufficientCapacity if the capacity of @c outcome vector is less
  /// than actual number of certificates in the chain
  /// @error SecurityErrorDomain::kInvalidArgument if the @c certChain argument cannot be parsed
  /// @error SecurityErrorDomain::kUnknownIdentifier if the @c formatId argument has unknown value
  /// @trace SWS_CRYPT_40616
  ara::core::Result<void> ParseCertChain(ara::core::Vector<Certificate::Uptr>& outcome,
      ReadOnlyMemRegion certChain,
      Serializable::FormatId formatId = Serializable::kFormatDefault) noexcept override;

  /// @brief Parse a serialized representation of the certificate chain and create their instances.
  /// Off-line validation of the parsed certification chain may be done via call @c VerifyCertChainByCrl().
  /// After validation the certificates may be imported to the session or persistent storage for following
  /// search and usage.
  /// Capacity of the @c outcome vector must be equal to the size of the @c certChain vector.
  /// If the certificates are not imported then they will be lost after destroy of the returned instances!
  /// Only imported certificates may be found by a search and applied for automatic verifications!
  /// Certificates in the @c outcome vector will be placed from the root CA certificate (zero index) to
  /// the final end-entity certificate (last used index of the vector).
  /// @param[out] outcome output vector of imported certificates
  /// @param[in] certChain DER/PEM-encoded certificates chain (each certificate is presented by a separate
  /// BLOB in the input vector)
  /// @param[in] formatId input format identifier (@c kFormatDefault means auto-detect)
  /// @error SecurityErrorDomain::kInsufficientCapacity if capacity of the @c outcome vector is less
  /// than number of elements in the @c certChain
  /// @error SecurityErrorDomain::kInvalidArgument if an element of @c certChain argument cannot be parsed
  /// @error SecurityErrorDomain::kUnknownIdentifier if the @c formatId argument has unknown value
  /// @trace SWS_CRYPT_40617
  ara::core::Result<void> ParseCertChain(ara::core::Vector<Certificate::Uptr>& outcome,
      const ara::core::Vector<ReadOnlyMemRegion>& certChain,
      Serializable::FormatId formatId = Serializable::kFormatDefault) noexcept override;

  /// @brief Verify status of the provided certificate by locally stored CA certificates and CRLs only.
  /// This method updates the @c Certificate::Status associated with the certificate.
  /// @param[in] cert target certificate for verification
  /// @param[in] myRoot root certificate to be used for verification - if this is nullptr, use machine root
  /// certificates
  /// @returns verification status of the provided certificate
  /// @trace SWS_CRYPT_40618
  Certificate::Status VerifyCert(Certificate& cert, Certificate::Uptr myRoot = nullptr) noexcept override;

  /// @brief Verify status of the provided certification chain by locally stored CA certificates and CRLs
  /// only.
  /// Verification status of the certificate chain is @c Certificate::Status::kValid only if all certificates
  /// in the chain have such status!
  /// Certificates in the chain (presented by container vector) must be placed from the root CA certificate
  /// (zero index) to the target end-entity certificate (last used index of the vector). Verification is
  /// executed in same order.
  /// If the chain verification is failed then status of the first failed certificate is returned.
  /// This method updates the @c Certificate::Status associated with the certificates in the chain.
  /// @param[in] chain target certificate chain for verification
  /// @param[in] myRoot root certificate to be used for verification - if this is nullptr, use machine root
  /// certificates
  /// @returns verification status of the provided certificate chain
  /// @trace SWS_CRYPT_40619
  Certificate::Status VerifyCertChain(ara::core::Span<const Certificate::Uptr> chain,
      Certificate::Uptr myRoot = nullptr) const noexcept override;

  /// @brief Import Certificate Revocation List (CRL) or Delta CRL from a memory BLOB.
  /// If the imported CRL lists some certificates kept in the persistent or volatile storages then their
  /// status must be automatically updated to @c Status::kInvalid. If some of these certificates were
  /// already openned during this operation, then this status change becomes available immediately (via
  /// method call @c Certificate::GetStatus())!
  /// All certificates with the status @c kInvalid should be automatically removed from correspondent
  /// storages (immediately if a certificate not in use now or just after its closing otherwise).
  /// @param[in] crl serialized CRL or Delta CRL (in form of a BLOB)
  /// @returns @c true if the CRL is valid and @c false if it is already expired
  /// @error SecurityErrorDomain::kUnexpectedValue if the provided BLOB is not a CRL/DeltaCRL
  /// @error SecurityErrorDomain::kRuntimeFault if the CRL validation has failed
  /// @trace SWS_CRYPT_40620
  ara::core::Result<bool> ImportCrl(ReadOnlyMemRegion crl) noexcept override;

  /// @brief Import the certificate to volatile or persistent storage.
  /// Only imported certificate may be found by a search and applied for automatic verifications!
  /// A certificate can be imported to only one of storage: volatile or persistent. Therefore if you
  /// import a certificate already kept in the persistent storage to the volatile one then nothing changes.
  /// But if you import a certificate already kept in the volatile storage to the persistent one then it is
  /// "moved" to the persistent realm.
  /// If an application successfully imports a certificate that correspond to a CSR existing in the storage
  /// then this CSR should be removed.
  /// @param[in] cert a valid certificate that should be imported
  /// @param[in] iSpecify optionally a valid InstanceSpecifier can be provided that points to a
  /// CertificateSlot for persistent storage of the certificate, otherwise the certificate shall be stored
  /// in volatile (session) storage
  /// @error SecurityErrorDomain::kInvalidArgument if the provided certificate is invalid
  /// @error SecurityErrorDomain::kIncompatibleObject if provided certificate has partial collision with
  /// a matched CSR in the storage
  /// @error SecurityErrorDomain::kContentDuplication if the provided certificate already exists
  /// in the storage
  /// @error SecurityErrorDomain::kAccessViolation if the InstanceSpecifier points to a CertificateSlot,
  /// which the application may only read
  /// @trace SWS_CRYPT_40621
  ara::core::Result<void> Import(
      const Certificate& cert, ara::core::Optional<ara::core::InstanceSpecifier> iSpecify) noexcept override;

  /// @brief Remove specified certificate from the storage (volatile or persistent) and destroy it.
  /// @param[in] cert a unique smart pointer to a certificate that should be removed
  /// @returns @c true if the certificate was found and removed from the storage, @c false if it was not
  /// found
  /// @trace SWS_CRYPT_40622
  bool Remove(Certificate::Uptrc&& cert) noexcept override;

  /// @brief Set the "pending" status associated to the CSR that means that the CSR already sent to CA.
  /// This method do nothing if the CSR already marked as "pending".
  /// Only an application with permissions "CA Connector" has the right to call this method!
  /// @param[in] request certificate signing request that should be marked as "pending"
  /// @error SecurityErrorDomain::kAccessViolation if the method called by an application without
  /// the "CA Connector" permission
  /// @trace SWS_CRYPT_40624
  //virtual ara::core::Result<void> SetPendingStatus(const CertSignRequest& request) noexcept = 0;

  /// @brief Set specified CA certificate as a "root of trust".
  /// Only a certificate saved to the volatile or persistent storage may be marked as the "root of trust"!
  /// Only CA certificate can be a "root of trust"!
  /// Multiple certificates on an ECU may be marked as the "root of trust".
  /// Only an application with permissions "Trust Master" has the right to call this method!
  /// @param[in] caCert a valid CA certificate that should be trusted
  /// @error SecurityErrorDomain::kInvalidArgument if the provided certificate is invalid
  /// @error SecurityErrorDomain::kIncompatibleObject if provided certificate doesn't belong to a CA
  /// @error SecurityErrorDomain::kAccessViolation if the method called by an application without
  /// the "Trust Master" permission
  /// @trace SWS_CRYPT_40625
  ara::core::Result<void> SetAsRootOfTrust(const Certificate& caCert) noexcept override;

  /// @brief Create OCSP request for specified certificate.
  /// This method may be used for implementation of the "OCSP stapling".
  /// @param[in] cert  a certificate that should be verified
  /// @param[in] signer  an optional pointer to initialized signer context (if the request should be signed)
  /// @returns unique smart pointer to the created OCSP request
  /// @error SecurityErrorDomain::kInvalidArgument  if the provided certificate is invalid
  /// @error SecurityErrorDomain::kIncompleteArgState  if the @c signer context is not initialized by a key
  /// @trace SWS_CRYPT_40626
  ara::core::Result<OcspRequest::Uptrc> CreateOcspRequest(
      const Certificate& cert, const cryp::SignerPrivateCtx::Uptr signer) noexcept;

  /// @brief Create OCSP request for specified list of certificates.
  /// This method may be used for implementation of the "OCSP stapling".
  /// @param[in] certList  a certificates' list that should be verified
  /// @param[in] signer  an optional pointer to initialized signer context (if the request should be signed)
  /// @returns unique smart pointer to the created OCSP request
  /// @error SecurityErrorDomain::kInvalidArgument  if the provided certificates are invalid
  /// @error SecurityErrorDomain::kIncompleteArgState  if the @c signer context is not initialized by a key
  /// @trace SWS_CRYPT_40627
  //virtual ara::core::Result<OcspRequest::Uptrc> CreateOcspRequest(
  //    const ara::core::Vector<const Certificate*>& certList,
  //    ara::core::Optional<const cryp::SignerPrivateCtx> signer) noexcept
  //   = 0;

  /// @brief Parse serialized OCSP response and create correspondent interface instance.
  /// This method may be used for implementation of the "OCSP stapling".
  /// @param[in] response  a serialized OCSP response
  /// @returns unique smart pointer to the created OCSP response instance
  /// @error SecurityErrorDomain::kUnexpectedValue  if the provided BLOB @c response
  /// doesn't keep an OCSP response
  /// @trace SWS_CRYPT_40628
  OcspResponse::Uptrc ParseOcspResponse(ReadOnlyMemRegion response) const noexcept;

  /// @brief Check certificate status by directly provided OCSP response.
  /// This method may be used for implementation of the "OCSP stapling".
  /// This method updates the @c Certificate::Status associated with the certificate.
  /// @param[in] cert  a certificate that should be verified
  /// @param[in] ocspResponse  an OCSP response
  /// @returns @c true if the certificate is verified successfully and @c false otherwise
  /// @error SecurityErrorDomain::kInvalidArgument  if the @c cert is invalid
  /// @error SecurityErrorDomain::kRuntimeFault  if the @c ocspResponse is invalid
  /// @trace SWS_CRYPT_40629
  //virtual ara::core::Result<bool> CheckCertStatus(Certificate& cert, const OcspResponse& ocspResponse) const
  //    noexcept
  //    = 0;

  /// @brief Check status of a certificates list by directly provided OCSP response.
  /// This method may be used for implementation of the "OCSP stapling".
  /// This method updates the @c Certificate::Status associated with the certificates in the list.
  /// @param[in] certList  a certificates list that should be verified
  /// @param[in] ocspResponse  an OCSP response
  /// @returns @c true if the certificates list is verified successfully and @c false otherwise
  /// @error SecurityErrorDomain::kInvalidArgument  if the provided certificates are invalid
  /// @error SecurityErrorDomain::kRuntimeFault  if the @c ocspResponse is invalid
  /// @trace SWS_CRYPT_40630
  //virtual ara::core::Result<bool> CheckCertStatus(
  //    const ara::core::Vector<Certificate*>& certList, const OcspResponse& ocspResponse) const noexcept
  //    = 0;

  /// @brief Find a certificate by the subject and issuer Distinguished Names (DN).
  /// Argument @c certIndex represents an internal index of current certificate in the storage.
  /// In order to start certificate search from begin, set: <tt>certIndex = kInvalidIndex</tt>
  /// @param[in] subjectDn subject DN of the target certificate
  /// @param[in] issuerDn issuer DN of the target certificate
  /// @param[in] validityTimePoint a time point when the target certificate should be valid
  /// @param[in,out] certIndex an index for iteration through all suitable certificates in the storage
  /// (input: index of previous found cerificate, output: index of current found cerificate)
  /// @returns unique smart pointer to found certificate or @c nullptr if nothing is found
  /// @trace SWS_CRYPT_40631
  Certificate::Uptrc FindCertByDn(const X509DN& subjectDn, const X509DN& issuerDn, time_t validityTimePoint,
      StorageIndex& certIndex) noexcept override;

  /// @brief Find a certificate by its SKID & AKID.
  /// @param[in] subjectKeyId subject key identifier (SKID)
  /// @param[in] authorityKeyId optional authority key identifier (AKID)
  /// @returns unique smart pointer to found certificate or @c nullptr if nothing is found
  /// @trace SWS_CRYPT_40632
  Certificate::Uptrc FindCertByKeyIds(ReadOnlyMemRegion subjectKeyId,
      ReadOnlyMemRegion authorityKeyId = ReadOnlyMemRegion()) noexcept override;

  /// @brief Find a certificate by its serial number.
  /// @param[in] sn serial number of the target certificate
  /// @param[in] issuerDn authority's Distinguished Names (DN)
  /// @returns unique smart pointer to a found certificate or @c nullptr if nothing is found
  /// @trace SWS_CRYPT_40633
  Certificate::Uptrc FindCertBySn(ReadOnlyMemRegion sn, const X509DN& issuerDn) noexcept override;

  /// @brief Parse a certificate signing request (CSR) provided by the user.
  /// @param[in] csr the buffer containing a certificate signing request
  /// @param[in] withMetaData specifies the format of the buffer content: @c TRUE means the object has been
  /// previously serialized by using the Serializable interface; @c FALSE means
  /// the CSR was exported using the CertSignRequest::ExportASN1CertSignRequest() interface
  /// @returns unique smart pointer to the certificate signing request
  /// @error SecurityErrorDomain::kUnsupportedFormat is returned in case the provided buffer does not
  /// contain the expected format
  /// @trace SWS_CRYPT_40634
  ara::core::Result<CertSignRequest::Uptrc> ParseCertSignRequest(
      ReadOnlyMemRegion csr, bool withMetaData = true) noexcept override;

  /// @brief Cleanup the volatile certificates storage.
  /// After execution of this command the certificates previously imported to the volatile storage cannot be
  /// found by a search, but it doesn't influence to already loaded @c Certificate instances!
  /// @trace SWS_CRYPT_40635
  void CleanupVolatileStorage() noexcept override;

  /// @brief Create an empty X.509 Extensions structure.
  /// If <tt>(0 == capacity)</tt> then a maximally supported (by the implementation) capacity must be
  /// reserved.
  /// @param[in] capacity  number of bytes that should be reserved for the content of the target @c
  /// X509Extensions object
  /// @returns Shared smart pointer to created empty @c X509X509Extensions object
  /// @trace SWS_CRYPT_40636
  //virtual ara::core::Result<X509Extensions::Uptr> CreateEmptyExtensions(std::size_t capacity = 0) noexcept
  //    = 0;

  /// @brief Create certification request for a private key loaded to the context.
  /// @param[in] derSubjectDN the DER-encoded subject distinguished name (DN) of the private key owner
  /// @param[in] x509Extensions the DER-encoded X.509 Extensions that should be included to the
  /// certification request
  /// @param[in] version the format version of the target certification request
  /// @param[in] signerCtx the fully-configured SignerPrivateCtx to be used for signing this certificate
  /// request
  /// @returns unique smart pointer to created certification request
  /// @error SecurityErrorDomain::kUnexpectedValue if any of arguments has incorrect/unsupported value
  /// @trace SWS_CRYPT_40640
  ara::core::Result<CertSignRequest::Uptrc> CreateCertSignRequest(cryp::SignerPrivateCtx::Uptr signerCtx,
      ReadOnlyMemRegion derSubjectDN, ReadOnlyMemRegion x509Extensions = ReadOnlyMemRegion(),
      unsigned version = 1) const noexcept override;

  /// @brief Load a certificate from the persistent certificate storage.
  /// @param[in] iSpecify  the target certificate instance specifier
  /// @returns an unique smart pointer to the instantiated certificate
  /// @error SecurityErrorDomain::kUnreservedResource  if the InstanceSpecifier is incorrect (the
  /// certificate cannot be found)
  /// @trace SWS_CRYPT_40641
  //virtual ara::core::Result<Certificate::Uptr> LoadCertificate(
  //    ara::core::InstanceSpecifier& iSpecify) noexcept
  //    = 0;

  /// @brief Callback function to check if table is empty.
  static int IsTableEmptycallback(void* data, int argc, char** argv, char** ColName) noexcept
  {
    static_cast<void>(ColName);

    char* ptr{reinterpret_cast<char*>(data)};

    *ptr = *(argv[0]);

    readComplete = true;

    return 0;
  }

  /// @brief call back for sqlite read operation.
  static int callback(void* data, int argc, char** argv, char** ColName) noexcept
  {
    ara::core::String keyValueStr;
    ara::core::String cert;
    bool isCA{false};
    bool isCRL{false};
    Certificate::Status certStatus{Certificate::Status::kUnknown};

    for (int index{0}; index < argc; index++)
    {
      ara::core::String coloumnName{ColName[index]};
      ara::core::String tempString{argv[index]};
      if (coloumnName == "InstanceSpecifier")
      {
        keyValueStr = tempString;
      }
      if (coloumnName == "SerialNumber")
      {
        keyValueStr = tempString;
        isCA = true;
      }
      else if (coloumnName == "Certificate")
      {
        cert = tempString;
      }
      else if (coloumnName == "Id")
      {
        isCRL = true;
      }
      else if (coloumnName == "CRL")
      {
        cert = tempString;
      }
      else
      {
        uint32_t tempStatus{0};
        const char* tempPtr{tempString.data()};
        for (size_t statusIndex{0}; statusIndex < tempString.size(); statusIndex++)
        {
          tempStatus = (tempStatus * 10) + (tempPtr[statusIndex] - 48);
        }

        certStatus = static_cast<Certificate::Status>(tempStatus);
      }
    }

    if (isCRL)
    {
      BIO* certBio = BIO_new(BIO_s_mem());
      BIO_write(certBio, cert.data(), cert.size());
      X509_CRL* crlX509{PEM_read_bio_X509_CRL(certBio, nullptr, 0, NULL)};
      if (crlX509 == nullptr)
      {
        aralog_.LogError() << "Error in reading the CRL";
      }
      else
      {
        reinterpret_cast<ara::core::Vector<X509_CRL*>*>(data)->emplace_back(crlX509);
      }
      isCRL = false;
    }
    else
    {
      BIO* certBio = BIO_new(BIO_s_mem());
      BIO_write(certBio, cert.data(), cert.size());
      X509* certX509{PEM_read_bio_X509(certBio, nullptr, 0, NULL)};
      if (certX509 == nullptr)
      {
        aralog_.LogError() << "Error in reading the certificate";
      }
      else
      {
        if (isCA == true)
        {
          aralog_.LogDebug() << "CA Certificate read succesfully from the DB";
          reinterpret_cast<ara::core::Map<ara::core::String, CertificateImpl>*>(data)->insert(
              {keyValueStr, certX509});

          ara::core::Map<ara::core::String, CertificateImpl>::iterator it{
              reinterpret_cast<ara::core::Map<ara::core::String, CertificateImpl>*>(data)->find(keyValueStr)};

          it->second.SetStatus(certStatus);
          isCA = false;
        }
        else
        {
          ara::core::InstanceSpecifier iSpcify{keyValueStr.c_str()};
          aralog_.LogDebug() << "Certificate read succesfully from the DB";
          reinterpret_cast<ara::core::Map<ara::core::InstanceSpecifier, CertificateImpl>*>(data)->insert(
              {iSpcify, certX509});

          ara::core::Map<ara::core::InstanceSpecifier, CertificateImpl>::iterator it{
              reinterpret_cast<ara::core::Map<ara::core::InstanceSpecifier, CertificateImpl>*>(data)->find(
                  iSpcify)};

          it->second.SetStatus(certStatus);
        }
      }
    }
    cndVar_.notify_all();
    return 0;
  }

 private:
  /// @brief method to convert ASN1_TIME type to time_t.
  time_t ASN1_GetTimeT(const ASN1_TIME* time) noexcept;

  /// @brief method to create table in DB independent of the underlying DB.
  bool CreateTableInDB(uint8_t dbId) noexcept;

  /// @brief method to create table in sqlite DB.
  bool CreateTableInSqliteDb(sqlite3* db, ara::core::String sqlCommand) noexcept;

  /// @brief method to read certificate from the DB.
  void ReadCertificatesFromDB() noexcept;

  /// @brief method to Delete data from the sqlite DB.
  void DeleteDataFromSqliteTable(sqlite3* db, ara::core::String keyValue) noexcept;

  /// @brief method to get the length of the content in the table.
  size_t GetDBContentLength(ara::core::String& instanceSpecifier) const noexcept;

  /// @brief method to set the object value is the certificate table.
  bool SetObjectValue(const Certificate& cert, ara::core::String keyValue, bool isCa) noexcept;

  /// @brief method to Update the status of the certificate is in the table.
  bool UpdateStatusValue(Certificate::Status certStatus, ara::core::String iSpecify) noexcept;

  /// @brief method to check if table is empty.
  bool IstableEmpty(uint8_t dbId) noexcept;

  /// @brief method to delete the table from the DB.
  void DeleteCertFromDB(ara::core::String tableName, ara::core::String keyValueName,
      ara::core::String keyValue, uint8_t dbId) noexcept;

  /// @brief read CA certificate from the DB.
  void ReadCACertificatesFromDB() noexcept;

  /// @brief Method to write data in sqlite DB.
  bool StoreDataInSqlite(sqlite3* db, ara::core::String sqlquery) noexcept;

  /// @brief Method to write data in DB independent of the underlying DB.
  bool WriteDataInDB(ara::core::String tableInfo, ara::core::String keyValue, ReadOnlyMemRegion dataValue,
      Certificate::Status statusValue, uint8_t dbId) noexcept;

  /// @brief Method to read CRLs from the DB.
  void ReadCRLsFromDB() noexcept;

  /// @brief logger variable for ara::log.
  static ara::log::Logger& aralog_;

  /// @brief variable to store the sqlite DB pointers.
  static sqlite3* stDBStore[3];

  /// @brief variable to store the flag to indicate completion of read operation.
  static bool readComplete;

  /// @brief storage for volatile certificates.
  ara::core::Vector<Certificate::Uptr> certList_;

  /// @brief storage for persistent certificates.
  ara::core::Map<ara::core::InstanceSpecifier, CertificateImpl> dbCertList_;

  /// @brief storage for session certificates.
  ara::core::Map<ara::core::String, CertificateImpl> caCertList_;

  /// @brief condition Var for callback.
  static std::condition_variable cndVar_;

  /// @brief storage for CRL.
  ara::core::Vector<X509_CRL*> crlList_;
};

} // namespace x509
} // namespace crypto
} // namespace ara

#endif // CRYPTO_SOURCE_CERTIFICATE_PROVIDER_INC_PUBLIC_X509_PROVIDER_IMPL_H_
