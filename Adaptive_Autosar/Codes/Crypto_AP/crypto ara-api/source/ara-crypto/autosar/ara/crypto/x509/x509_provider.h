// --------------------------------------------------------------------------
// |              _    _ _______     .----.      _____         _____        |
// |         /\  | |  | |__   __|  .  ____ .    / ____|  /\   |  __ \       |
// |        /  \ | |  | |  | |    .  / __ \ .  | (___   /  \  | |__) |      |
// |       / /\ \| |  | |  | |   .  / / / / v   \___ \ / /\ \ |  _  /       |
// |      / /__\ \ |__| |  | |   . / /_/ /  .   ____) / /__\ \| | \ \       |
// |     /________\____/   |_|   ^ \____/  .   |_____/________\_|  \_\      |
// |                              . _ _  .                                  |
// --------------------------------------------------------------------------
//
// All Rights Reserved.
// Any use of this source code is subject to a license agreement with the
// AUTOSAR development cooperation.
// More information is available at www.autosar.org.
//
// Disclaimer
//
// This work (specification and/or software implementation) and the material
// contained in it, as released by AUTOSAR, is for the purpose of information
// only. AUTOSAR and the companies that have contributed to it shall not be
// liable for any use of the work.
//
// The material contained in this work is protected by copyright and other
// types of intellectual property rights. The commercial exploitation of the
// material contained in this work requires a license to such intellectual
// property rights.
//
// This work may be utilized or reproduced without any modification, in any
// form or by any means, for informational purposes only. For any other
// purpose, no part of the work may be utilized or reproduced, in any form
// or by any means, without permission in writing from the publisher.
//
// The work has been developed for automotive applications only. It has
// neither been developed, nor tested for non-automotive applications.
//
// The word AUTOSAR and the AUTOSAR logo are registered trademarks.
// --------------------------------------------------------------------------

/// @file
/// @brief Adaptive Autosar Crypto Stack. X.509 API. X509Provider interface definition.

#ifndef ARA_CRYPTO_X509_X509_PROVIDER_H_
#define ARA_CRYPTO_X509_X509_PROVIDER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crypto/cryp/signer_private_ctx.h"

#include "ara/crypto/x509/x509_dn.h"
#include "ara/crypto/x509/certificate.h"
#include "ara/crypto/x509/cert_sign_request.h"
#include "ara/crypto/x509/x509_extensions.h"
#include "ara/crypto/x509/ocsp_request.h"
#include "ara/crypto/x509/ocsp_response.h"

#include "ara/core/optional.h"
#include "ara/core/result.h"
#include "ara/core/vector.h"
#include "ara/core/instance_specifier.h"

namespace ara
{
namespace crypto
{
namespace x509
{

//- @interface X509Provider
///
/// @traceid{SWS_CRYPT_40600}@tracestatus{draft}
/// @uptrace{RS_CRYPTO_02306}
/// @brief X.509 Provider interface.
///       The X.509 Provider supports two internal storages: volatile (or session) and persistent.
///       All X.509 objects created by the provider should have an actual reference to their parent X.509 Provider.
///       The X.509 Provider can be destroyed only after destroying of all its daughterly objects.
///       Each method of this interface that creates a X.509 object is non-constant, because any such creation increases
///       a references counter of the X.509 Provider.
///

class X509Provider
{
public:
    /// @traceid{SWS_CRYPT_40601}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Shared smart pointer of the interface.
    using Uptr = std::unique_ptr<X509Provider>;

    /// @traceid{SWS_CRYPT_40602}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Type of an internal index inside the certificate storage.
    using StorageIndex = std::size_t;

    /// @traceid{SWS_CRYPT_40603}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Reserved "invalid index" value for navigation inside the certificate storage.
    static const StorageIndex kInvalidIndex = static_cast<std::size_t>(-1LL);

    /// @traceid{SWS_CRYPT_40604}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Destructor.
    virtual ~X509Provider() noexcept = default;

    /// @traceid{SWS_CRYPT_30226}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02004}
    /// @brief Copy-assign another X509Provider to this instance.
    /// @param other  the other instance
    /// @returns *this, containing the contents of @a other
    X509Provider& operator=(const X509Provider& other) = default;

    /// @traceid{SWS_CRYPT_30227}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02004}
    /// @brief Move-assign another X509Provider to this instance.
    /// @param other  the other instance
    /// @returns *this, containing the contents of @a other
    X509Provider& operator=(X509Provider&& other) = default;

    /// @traceid{SWS_CRYPT_40611}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Create an empty X.500 Distinguished Name (DN) structure.
    ///       If <tt>(0 == capacity)</tt> then a maximally supported (by the implementation) capacity must be reserved.
    /// @param[in] capacity  number of bytes that should be reserved for the content of the target @c X509DN object
    /// @returns Unique smart pointer to created empty @c X509DN object
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<X509DN::Uptr> CreateEmptyDn(std::size_t capacity = 0)
        noexcept = 0;

    /// @traceid{SWS_CRYPT_40612}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Create completed X.500 Distinguished Name structure from the provided string representation.
    /// @param[in] dn  string representation of the Distinguished Name
    /// @returns unique smart pointer for the created @c X509DN object
    /// @error SecurityErrorDomain::kInvalidArgument  if the @c dn argument has incorrect format
    /// @error SecurityErrorDomain::kInvalidInputSize  if the @c dn argument has unsupported length (too large)
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<X509DN::Uptrc> BuildDn(ara::core::StringView dn) noexcept = 0;

    /// @traceid{SWS_CRYPT_40613}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Decode X.500 Distinguished Name structure from the provided serialized format.
    /// @param[in] dn  DER/PEM-encoded representation of the Distinguished Name
    /// @param[in] formatId  input format identifier (@c kFormatDefault means auto-detect)
    /// @returns unique smart pointer for the created @c X509DN object
    /// @error SecurityErrorDomain::kInvalidArgument  if the @c dn argument cannot be parsed
    /// @error SecurityErrorDomain::kUnknownIdentifier  if the @c formatId argument has unknown value
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<X509DN::Uptrc> DecodeDn(ReadOnlyMemRegion dn
        , Serializable::FormatId formatId = Serializable::kFormatDefault
    ) noexcept = 0;

    /// @traceid{SWS_CRYPT_40614}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Parse a serialized representation of the certificate and create its instance.
    ///       Off-line validation of the parsed certificate may be done via call @c VerifyCertByCrl().
    ///       After validation the certificate may be imported to the session or persistent storage for following
    ///       search and usage.
    ///       If the parsed certificate is not imported then it will be lost after destroy of the returned instance!
    ///       Only imported certificate may be found by a search and applied for automatic verifications!
    /// @param[in] cert  DER/PEM-encoded certificate
    /// @param[in] formatId  input format identifier (@c kFormatDefault means auto-detect)
    /// @returns unique smart pointer to created certificate
    /// @error SecurityErrorDomain::kInvalidArgument  if the @c cert argument cannot be parsed
    /// @error SecurityErrorDomain::kUnknownIdentifier  if the @c formatId argument has unknown value
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<Certificate::Uptr> ParseCert(ReadOnlyMemRegion cert
        , Serializable::FormatId formatId = Serializable::kFormatDefault
    ) noexcept = 0;

    /// @traceid{SWS_CRYPT_40615}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Count number of certificates in a serialized certificate chain represented by a single BLOB.
    /// @param[in] certChain  DER/PEM-encoded certificate chain (in form of a single BLOB)
    /// @param[in] formatId  input format identifier (@c kFormatDefault means auto-detect)
    /// @returns number of certificates in the chain
    /// @error SecurityErrorDomain::kInvalidArgument  if the @c certChain argument cannot be pre-parsed
    /// @error SecurityErrorDomain::kUnknownIdentifier  if the @c formatId argument has unknown value
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<std::size_t> CountCertsInChain(ReadOnlyMemRegion certChain
        , Serializable::FormatId formatId = Serializable::kFormatDefault
    ) const noexcept = 0;

    /// @traceid{SWS_CRYPT_40616}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Parse a serialized representation of the certificate chain and create their instances.
    ///       Off-line validation of the parsed certification chain may be done via call @c VerifyCertChainByCrl().
    ///       After validation the certificates may be saved to the session or persistent storage for following
    ///       search and usage.
    ///       If the certificates are not imported then they will be lost after destroy of the returned instances!
    ///       Only imported certificates may be found by a search and applied for automatic verifications!
    ///       Certificates in the @c outcome vector will be placed from the root CA certificate (zero index) to
    ///       the final end-entity certificate (last used index of the vector).
    /// @param[out] outcome  an output vector for imported certificates
    /// @param[in] certChain  DER/PEM-encoded certificate chain (in form of a single BLOB)
    /// @param[in] formatId  input format identifier (@c kFormatDefault means auto-detect)
    /// @error SecurityErrorDomain::kInsufficientCapacity  if the capacity of @c outcome vector is less
    ///                than actual number of certificates in the chain
    /// @error SecurityErrorDomain::kInvalidArgument  if the @c certChain argument cannot be parsed
    /// @error SecurityErrorDomain::kUnknownIdentifier  if the @c formatId argument has unknown value
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<void> ParseCertChain(ara::core::Vector<Certificate::Uptr>& outcome
        , ReadOnlyMemRegion certChain
        , Serializable::FormatId formatId = Serializable::kFormatDefault
    ) noexcept = 0;

    /// @traceid{SWS_CRYPT_40617}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Parse a serialized representation of the certificate chain and create their instances.
    ///       Off-line validation of the parsed certification chain may be done via call @c VerifyCertChainByCrl().
    ///       After validation the certificates may be imported to the session or persistent storage for following
    ///       search and usage.
    ///       Capacity of the @c outcome vector must be equal to the size of the @c certChain vector.
    ///       If the certificates are not imported then they will be lost after destroy of the returned instances!
    ///       Only imported certificates may be found by a search and applied for automatic verifications!
    ///       Certificates in the @c outcome vector will be placed from the root CA certificate (zero index) to
    ///       the final end-entity certificate (last used index of the vector).
    /// @param[out] outcome  output vector of imported certificates
    /// @param[in] certChain  DER/PEM-encoded certificates chain (each certificate is presented by a separate BLOB
    ///            in the input vector)
    /// @param[in] formatId  input format identifier (@c kFormatDefault means auto-detect)
    /// @error SecurityErrorDomain::kInsufficientCapacity  if capacity of the @c outcome vector is less
    ///                than number of elements in the @c certChain
    /// @error SecurityErrorDomain::kInvalidArgument  if an element of @c certChain argument cannot be parsed
    /// @error SecurityErrorDomain::kUnknownIdentifier  if the @c formatId argument has unknown value
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<void> ParseCertChain(ara::core::Vector<Certificate::Uptr>& outcome
        , const ara::core::Vector<ReadOnlyMemRegion>& certChain
        , Serializable::FormatId formatId = Serializable::kFormatDefault
    ) noexcept = 0;

    /// @traceid{SWS_CRYPT_40618}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Verify status of the provided certificate by locally stored CA certificates and CRLs only.
    ///       This method updates the @c Certificate::Status associated with the certificate.
    /// @param[in] cert  target certificate for verification
    /// @param[in] myRoot  root certificate to be used for verification - if this is nullptr, use machine root certificates
    /// @returns verification status of the provided certificate
    /// @threadsafety{Thread-safe}
    virtual Certificate::Status VerifyCert(Certificate& cert, Certificate::Uptr myRoot=nullptr) noexcept = 0;

    /// @traceid{SWS_CRYPT_40619}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Verify status of the provided certification chain by locally stored CA certificates and CRLs only.
    ///       Verification status of the certificate chain is @c Certificate::Status::kValid only if all certificates
    ///       in the chain have such status!
    ///       Certificates in the chain (presented by container vector) must be placed from the root CA certificate
    ///       (zero index) to the target end-entity certificate (last used index of the vector). Verification is
    ///       executed in same order.
    ///       If the chain verification is failed then status of the first failed certificate is returned.
    ///       This method updates the @c Certificate::Status associated with the certificates in the chain.
    /// @param[in] chain  target certificate chain for verification
    /// @param[in] myRoot  root certificate to be used for verification - if this is nullptr, use machine root certificates
    /// @returns verification status of the provided certificate chain
    /// @threadsafety{Thread-safe}
    virtual Certificate::Status VerifyCertChain(ara::core::Span<const Certificate::Uptr> chain, Certificate::Uptr myRoot=nullptr) const noexcept = 0;

    /// @traceid{SWS_CRYPT_40620}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Import Certificate Revocation List (CRL) or Delta CRL from a memory BLOB.
    ///       If the imported CRL lists some certificates kept in the persistent or volatile storages then their status
    ///       must be automatically updated to @c Status::kInvalid. If some of these certificates were already openned during
    ///       this operation, then this status change becomes available immediately (via method call
    ///       @c Certificate::GetStatus())!
    ///       All certificates with the status @c kInvalid should be automatically removed from correspondent storages
    ///       (immediately if a certificate not in use now or just after its closing otherwise).
    /// @param[in] crl  serialized CRL or Delta CRL (in form of a BLOB)
    /// @returns @c true if the CRL is valid and @c false if it is already expired
    /// @error SecurityErrorDomain::kUnexpectedValue  if the provided BLOB is not a CRL/DeltaCRL
    /// @error SecurityErrorDomain::kRuntimeFault  if the CRL validation has failed
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<bool> ImportCrl(ReadOnlyMemRegion crl) noexcept = 0;

    /// @traceid{SWS_CRYPT_40621}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Import the certificate to volatile or persistent storage.
    ///       Only imported certificate may be found by a search and applied for automatic verifications!
    ///       A certificate can be imported to only one of storage: volatile or persistent. Therefore if you
    ///       import a certificate already kept in the persistent storage to the volatile one then nothing changes.
    ///       But if you import a certificate already kept in the volatile storage to the persistent one then it is
    ///       "moved" to the persistent realm.
    ///       If an application successfully imports a certificate that correspond to a CSR existing in the storage
    ///       then this CSR should be removed.
    /// @param[in] cert  a valid certificate that should be imported
    /// @param[in] iSpecify  optionally a valid InstanceSpecifier can be provided that points to a CertificateSlot
    ///                      for persistent storage of the certificate, otherwise the certificate shall be stored
    ///                      in volatile (session) storage
    /// @error SecurityErrorDomain::kInvalidArgument  if the provided certificate is invalid
    /// @error SecurityErrorDomain::kIncompatibleObject  if provided certificate has partial collision with
    ///                a matched CSR in the storage
    /// @error SecurityErrorDomain::kContentDuplication  if the provided certificate already exists
    ///                in the storage
    /// @error SecurityErrorDomain::kAccessViolation if the InstanceSpecifier points to a CertificateSlot,
    ///                                              which the application may only read
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<void> Import(const Certificate& cert
    	, ara::core::Optional<ara::core::InstanceSpecifier> iSpecify
    ) noexcept = 0;

    /// @traceid{SWS_CRYPT_40622}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Remove specified certificate from the storage (volatile or persistent) and destroy it.
    /// @param[in] cert  a unique smart pointer to a certificate that should be removed
    /// @returns @c true if the certificate was found and removed from the storage, @c false if it was not found
    /// @threadsafety{Thread-safe}
    virtual bool Remove(Certificate::Uptrc && cert) noexcept = 0;

    /// @traceid{SWS_CRYPT_40624}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Set the "pending" status associated to the CSR that means that the CSR already sent to CA.
    ///       This method do nothing if the CSR already marked as "pending".
    ///       Only an application with permissions "CA Connector" has the right to call this method!
    /// @param[in] request  certificate signing request that should be marked as "pending"
    /// @error SecurityErrorDomain::kAccessViolation  if the method called by an application without
    ///                the "CA Connector" permission
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<void> SetPendingStatus(const CertSignRequest& request) noexcept = 0;

    /// @traceid{SWS_CRYPT_40625}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Set specified CA certificate as a "root of trust".
    ///       Only a certificate saved to the volatile or persistent storage may be marked as the "root of trust"!
    ///       Only CA certificate can be a "root of trust"!
    ///       Multiple certificates on an ECU may be marked as the "root of trust".
    ///       Only an application with permissions "Trust Master" has the right to call this method!
    /// @param[in] caCert  a valid CA certificate that should be trusted
    /// @error SecurityErrorDomain::kInvalidArgument  if the provided certificate is invalid
    /// @error SecurityErrorDomain::kIncompatibleObject  if provided certificate doesn't belong to a CA
    /// @error SecurityErrorDomain::kAccessViolation  if the method called by an application without
    ///                the "Trust Master" permission
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<void> SetAsRootOfTrust(const Certificate& caCert) noexcept = 0;

    /// @traceid{SWS_CRYPT_40626}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Create OCSP request for specified certificate.
    ///       This method may be used for implementation of the "OCSP stapling".
    /// @param[in] cert  a certificate that should be verified
    /// @param[in] signer  an optional pointer to initialized signer context (if the request should be signed)
    /// @returns unique smart pointer to the created OCSP request
    /// @error SecurityErrorDomain::kInvalidArgument  if the provided certificate is invalid
    /// @error SecurityErrorDomain::kIncompleteArgState  if the @c signer context is not initialized by a key
    /// @threadsafety{Thread-safe}
    //virtual ara::core::Result<OcspRequest::Uptrc> CreateOcspRequest(const Certificate& cert
    //    , ara::core::Optional<const cryp::SignerPrivateCtx> signer
    //) noexcept = 0;

    /// @traceid{SWS_CRYPT_40627}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Create OCSP request for specified list of certificates.
    ///       This method may be used for implementation of the "OCSP stapling".
    /// @param[in] certList  a certificates' list that should be verified
    /// @param[in] signer  an optional pointer to initialized signer context (if the request should be signed)
    /// @returns unique smart pointer to the created OCSP request
    /// @error SecurityErrorDomain::kInvalidArgument  if the provided certificates are invalid
    /// @error SecurityErrorDomain::kIncompleteArgState  if the @c signer context is not initialized by a key
    /// @threadsafety{Thread-safe}
    // virtual ara::core::Result<OcspRequest::Uptrc> CreateOcspRequest(const ara::core::Vector<const Certificate*> & certList
    //    , ara::core::Optional<const cryp::SignerPrivateCtx> signer
    // ) noexcept = 0;

    /// @traceid{SWS_CRYPT_40628}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Parse serialized OCSP response and create correspondent interface instance.
    ///       This method may be used for implementation of the "OCSP stapling".
    /// @param[in] response  a serialized OCSP response
    /// @returns unique smart pointer to the created OCSP response instance
    /// @error SecurityErrorDomain::kUnexpectedValue  if the provided BLOB @c response
    ///       doesn't keep an OCSP response
    /// @threadsafety{Thread-safe}
    virtual OcspResponse::Uptrc ParseOcspResponse(ReadOnlyMemRegion response) const noexcept = 0;

    /// @traceid{SWS_CRYPT_40629}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Check certificate status by directly provided OCSP response.
    ///       This method may be used for implementation of the "OCSP stapling".
    ///       This method updates the @c Certificate::Status associated with the certificate.
    /// @param[in] cert  a certificate that should be verified
    /// @param[in] ocspResponse  an OCSP response
    /// @returns @c true if the certificate is verified successfully and @c false otherwise
    /// @error SecurityErrorDomain::kInvalidArgument  if the @c cert is invalid
    /// @error SecurityErrorDomain::kRuntimeFault  if the @c ocspResponse is invalid
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<bool> CheckCertStatus(Certificate& cert
        , const OcspResponse& ocspResponse
    ) const noexcept = 0;

    /// @traceid{SWS_CRYPT_40630}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Check status of a certificates list by directly provided OCSP response.
    ///       This method may be used for implementation of the "OCSP stapling".
    ///       This method updates the @c Certificate::Status associated with the certificates in the list.
    /// @param[in] certList  a certificates list that should be verified
    /// @param[in] ocspResponse  an OCSP response
    /// @returns @c true if the certificates list is verified successfully and @c false otherwise
    /// @error SecurityErrorDomain::kInvalidArgument  if the provided certificates are invalid
    /// @error SecurityErrorDomain::kRuntimeFault  if the @c ocspResponse is invalid
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<bool> CheckCertStatus(const ara::core::Vector<Certificate*> & certList
        , const OcspResponse& ocspResponse
    ) const noexcept = 0;

    /// @traceid{SWS_CRYPT_40631}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Find a certificate by the subject and issuer Distinguished Names (DN).
    ///       Argument @c certIndex represents an internal index of current certificate in the storage.
    ///       In order to start certificate search from begin, set: <tt>certIndex = kInvalidIndex</tt>
    /// @param[in] subjectDn  subject DN of the target certificate
    /// @param[in] issuerDn  issuer DN of the target certificate
    /// @param[in] validityTimePoint  a time point when the target certificate should be valid
    /// @param[in,out] certIndex  an index for iteration through all suitable certificates in the storage
    ///                (input: index of previous found cerificate, output: index of current found cerificate)
    /// @returns unique smart pointer to found certificate or @c nullptr if nothing is found
    /// @threadsafety{Thread-safe}
    virtual Certificate::Uptrc FindCertByDn(const X509DN& subjectDn
        , const X509DN& issuerDn
        , time_t validityTimePoint
        , StorageIndex & certIndex
    ) noexcept = 0;

    /// @traceid{SWS_CRYPT_40632}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Find a certificate by its SKID & AKID.
    /// @param[in] subjectKeyId  subject key identifier (SKID)
    /// @param[in] authorityKeyId  optional authority key identifier (AKID)
    /// @returns unique smart pointer to found certificate or @c nullptr if nothing is found
    /// @threadsafety{Thread-safe}
    virtual Certificate::Uptrc FindCertByKeyIds(ReadOnlyMemRegion subjectKeyId
        , ReadOnlyMemRegion authorityKeyId = ReadOnlyMemRegion()
    ) noexcept = 0;

    /// @traceid{SWS_CRYPT_40633}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Find a certificate by its serial number.
    /// @param[in] sn  serial number of the target certificate
    /// @param[in] issuerDn  authority's Distinguished Names (DN)
    /// @returns unique smart pointer to a found certificate or @c nullptr if nothing is found
    /// @threadsafety{Thread-safe}
    virtual Certificate::Uptrc FindCertBySn(ReadOnlyMemRegion sn
        , const X509DN& issuerDn
    ) noexcept = 0;

    /// @traceid{SWS_CRYPT_40634}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Parse a certificate signing request (CSR) provided by the user.
    /// @param[in] csr  the buffer containing a certificate signing request
    /// @param[in] withMetaData  specifies the format of the buffer content: @c TRUE means the object has been
    ///                          previously serialized by using the Serializable interface; @c FALSE means
    ///                          the CSR was exported using the CertSignRequest::ExportASN1CertSignRequest() interface
    /// @returns unique smart pointer to the certificate signing request
    /// @error SecurityErrorDomain::kUnsupportedFormat  is returned in case the provided buffer does not contain the expected format
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<CertSignRequest::Uptrc> ParseCertSignRequest( ReadOnlyMemRegion csr, bool withMetaData=true ) noexcept = 0;

    /// @traceid{SWS_CRYPT_40635}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Cleanup the volatile certificates storage.
    ///       After execution of this command the certificates previously imported to the volatile storage cannot be
    ///       found by a search, but it doesn't influence to already loaded @c Certificate instances!
    /// @threadsafety{Thread-safe}
    virtual void CleanupVolatileStorage() noexcept = 0;

    /// @traceid{SWS_CRYPT_40636}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Create an empty X.509 Extensions structure.
    ///       If <tt>(0 == capacity)</tt> then a maximally supported (by the implementation) capacity must be reserved.
    /// @param[in] capacity  number of bytes that should be reserved for the content of the target @c X509Extensions
    ///            object
    /// @returns Shared smart pointer to created empty @c X509X509Extensions object
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<X509Extensions::Uptr> CreateEmptyExtensions(std::size_t capacity = 0)
        noexcept = 0;

    ///// @traceid{SWS_CRYPT_40637}@tracestatus{draft}
    ///// @uptrace{RS_CRYPTO_02306}
    ///// @brief Get Certificate Revocation List (CRL) or Delta CRL via on-line connection.
    /////   CRL Distribution Point (CRL DP) should be extracted from the provided CA certificate.
    ///// @param[in] caCert  valid CA certificate
    ///// @returns @c true if the CRL was updated succesfully and @c false if there is no fresh updates
    ///// @error SecurityErrorDomain::kInvalidArgument  if the provided certificate is invalid
    ///// @error SecurityErrorDomain::kNoConnection  if the connection cannot be established
    ///// @threadsafety{Thread-safe}
    //
    //virtual ara::core::Result<bool> UpdateCrlOnline(const Certificate& caCert)
    //    noexcept = 0;

    ///// @traceid{SWS_CRYPT_40638}@tracestatus{draft}
    ///// @uptrace{RS_CRYPTO_02306}
    ///// @brief Send prepared certificate request to CA and save it to volatile or persistent storage.
    ///// @param[in] request  a valid certificate request that should be send to CA
    ///// @param[in] toVolatile  if this flag @c true then the request should be saved to the volatile (session)
    /////            storage, otherwise to the persistent storage
    ///// @error SecurityErrorDomain::kInvalidArgument  if the provided certification request is invalid
    ///// @error SecurityErrorDomain::kNoConnection  if connection to the CA cannot be established
    ///// @threadsafety{Thread-safe}
    //
    //virtual ara::core::Result<void> SendRequest(const cryp::X509CertRequest& request
    //    , bool toVolatile = true
    //) noexcept = 0;

    ///// @traceid{SWS_CRYPT_40639}@tracestatus{draft}
    ///// @uptrace{RS_CRYPTO_02306}
    ///// @brief Check certificate status via On-line Certificate Status Protocol (OCSP).
    ///// @param[in] cert  a certificate that should be verified
    ///// @returns certificate status
    /////       This method updates the @c Certificate::Status associated with the certificate.
    ///// @error SecurityErrorDomain::kInvalidArgument  if the provided certificate is invalid
    ///// @error SecurityErrorDomain::kNoConnection  if a connection to the OCSP responder cannot be established
    ///// @threadsafety{Thread-safe}
    //
    //virtual ara::core::Result<Certificate::Status> CheckCertStatusOnline(Certificate& cert)
    //    noexcept = 0;

    /// @traceid{SWS_CRYPT_40640}@tracestatus{draft}
    /// @uptrace{RS_CRYPTO_02306}
    /// @brief Create certification request for a private key loaded to the context.
    /// @param[in] derSubjectDN  the DER-encoded subject distinguished name (DN) of the private key owner
    /// @param[in] x509Extensions  the DER-encoded X.509 Extensions that should be included to the certification request
    /// @param[in] version  the format version of the target certification request
    /// @param[in] signerCtx  the fully-configured SignerPrivateCtx to be used for signing this certificate request
    /// @returns unique smart pointer to created certification request
    /// @error SecurityErrorDomain::kUnexpectedValue  if any of arguments has incorrect/unsupported value
    ///
    /// @threadsafety{Thread-safe}
    // virtual ara::core::Result<CertSignRequest::Uptrc> CreateCertSignRequest(
    //    	cryp::SignerPrivateCtx::Uptr signerCtx,
    //		ReadOnlyMemRegion derSubjectDN,
    //		ReadOnlyMemRegion x509Extensions = ReadOnlyMemRegion(),
	//		unsigned version = 1
    // ) const noexcept = 0;

    /// @traceid{SWS_CRYPT_40641}@tracestatus{draft}
    ///
    /// @brief Load a certificate from the persistent certificate storage.
    /// @param[in] iSpecify  the target certificate instance specifier
    /// @returns an unique smart pointer to the instantiated certificate
    ///
    /// @error SecurityErrorDomain::kUnreservedResource  if the InstanceSpecifier is incorrect (the certificate cannot be found)
    /// @threadsafety{Thread-safe}
    virtual ara::core::Result<Certificate::Uptr> LoadCertificate(ara::core::InstanceSpecifier& iSpecify) noexcept = 0;
};

}  // namespace x509
}  // namespace crypto
}  // namespace ara

#endif  // ARA_CRYPTO_X509_X509_PROVIDER_H_
