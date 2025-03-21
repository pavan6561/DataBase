// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : error_domain_cryptoerrordomain.cpp
//
// Purpose     : Implementation of file error_domain_cryptoerrordomain.cpp
// -----------------------------------------------------------------------------------------------------------
// CODE GENERATED BY        : CM Code Generator
//
// CODE GENERATOR VERSION   : 1.4.0.A-006
//
// GENERATED ON             : 2022-02-07T12:05:52+05:30
// -----------------------------------------------------------------------------------------------------------

#include "error_domain_cryptoerrordomain.h"

namespace ara
{
namespace crypto
{
namespace ipccom
{
namespace common
{
namespace error
{

char const* CryptoErrorDomainErrorDomain::Name() const noexcept{
  return "CryptoErrorDomain";
}

char const* CryptoErrorDomainErrorDomain::Message(ara::core::ErrorDomain::CodeType errorCode) const noexcept{
  char const* message;
  Errc const code{static_cast<Errc>(errorCode)};
  switch (code) {
  case Errc::kAboveBoundary:
    message = "Message text for AboveBoundary is not defined";
    break;
  case Errc::kAccessViolation:
    message = "Message text for AccessViolation is not defined";
    break;
  case Errc::kAuthTagNotValid:
    message = "Message text for AuthTagNotValid is not defined";
    break;
  case Errc::kBadObjectReference:
    message = "Message text for BadObjectReference is not defined";
    break;
  case Errc::kBelowBoundary:
    message = "Message text for BelowBoundary is not defined";
    break;
  case Errc::kBruteForceRisk:
    message = "Message text for BruteForceRisk is not defined";
    break;
  case Errc::kBusyResource:
    message = "Message text for BusyResource is not defined";
    break;
  case Errc::kContentDuplication:
    message = "Message text for ContentDuplication is not defined";
    break;
  case Errc::kContentRestrictions:
    message = "Message text for ContentRestrictions is not defined";
    break;
  case Errc::kEmptyContainer:
    message = "Message text for EmptyContainer is not defined";
    break;
  case Errc::kInOutBuffersIntersect:
    message = "Message text for InOutBuffersIntersect is not defined";
    break;
  case Errc::kIncompatibleArguments:
    message = "Message text for IncompatibleArguments is not defined";
    break;
  case Errc::kIncompatibleObject:
    message = "Message text for IncompatibleObject is not defined";
    break;
  case Errc::kIncompleteArgState:
    message = "Message text for IncompleteArgState is not defined";
    break;
  case Errc::kInsufficientCapacity:
    message = "Message text for InsufficientCapacity is not defined";
    break;
  case Errc::kInvalidArgument:
    message = "Message text for InvalidArgument is not defined";
    break;
  case Errc::kInvalidInputSize:
    message = "Message text for InvalidInputSize is not defined";
    break;
  case Errc::kInvalidUsageOrder:
    message = "Message text for InvalidUsageOrder is not defined";
    break;
  case Errc::kMissingArgument:
    message = "Message text for MissingArgument is not defined";
    break;
  case Errc::kModifiedResource:
    message = "Message text for ModifiedResource is not defined";
    break;
  case Errc::kProcessingNotFinished:
    message = "Message text for ProcessingNotFinished is not defined";
    break;
  case Errc::kProcessingNotStarted:
    message = "Message text for ProcessingNotStarted is not defined";
    break;
  case Errc::kRuntimeFault:
    message = "Message text for RuntimeFault is not defined";
    break;
  case Errc::kUnexpectedValue:
    message = "Message text for UnexpectedValue is not defined";
    break;
  case Errc::kUninitializedContext:
    message = "Message text for UninitializedContext is not defined";
    break;
  case Errc::kUnknownIdentifier:
    message = "Message text for UnknownIdentifier is not defined";
    break;
  case Errc::kUnreservedResource:
    message = "Message text for UnreservedResource is not defined";
    break;
  case Errc::kUnsupported:
    message = "Message text for Unsupported is not defined";
    break;
  case Errc::kUnsupportedFormat:
    message = "Message text for UnsupportedFormat is not defined";
    break;
  case Errc::kUsageViolation:
    message = "Message text for UsageViolation is not defined";
    break;
    default :
      message = "Unknown error";
      break;
  }
  return message;
}

void CryptoErrorDomainErrorDomain:: ThrowAsException(ara::core::ErrorCode const& errorCode) const noexcept(false){
  ara::core::ThrowOrTerminate<Exception>(errorCode);
}

} // namespace error
} // namespace common
} // namespace ipccom
} // namespace crypto
} // namespace ara

