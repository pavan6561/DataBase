// -----------------------------------------------------------------------------------------------------------
// Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved Reproduction, distribution and utilization
// of this document as well as the communication of its contents to others without explicit authorization is
// prohibited. Offenders will be held liable for the payment of damages.
// -----------------------------------------------------------------------------------------------------------
// File Name   : main.cpp
//
// Purpose     : This is the place where Crypto Daemon will be called for execution of its operations.
// -----------------------------------------------------------------------------------------------------------

#include <iomanip>
#include <unistd.h>

#include "application_lifecycle.h"
int main(int argc, const char* argv[])
{
  static_cast<void>(lifecycle::ApplicationLifeCycle<ara::crypto::CryptoDaemon>::Main(argc, argv));
  return (0);
}
