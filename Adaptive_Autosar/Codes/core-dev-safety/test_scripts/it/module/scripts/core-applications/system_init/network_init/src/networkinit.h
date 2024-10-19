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

#ifndef NETWORK_INIT_INCLUDE_NETWORKINIT_NETWORKINIT_H_
#define NETWORK_INIT_INCLUDE_NETWORKINIT_NETWORKINIT_H_

namespace networkinit
{
class NetworkInit
{
 public:
  explicit NetworkInit();
  virtual ~NetworkInit();

  /**
     * \brief Performs one-time setup of the application.
     */
  void Initialize();

  /**
     * \brief Executes the workload of the application.
     *
     * For a one-shot workload, the method may run through and then return.
     * For a periodic workload, the method must implement its own loop.
     * Note that in either case, the method must return as soon as possible when exit_requested_ has been set to true.
     */
  void Run();

  /**
     * \brief Performs one-time cleanup of the application.
     *
     * After this method has run, the application is ready to be terminated.
     * When the application has spawned threads, all threads must be destroyed before this method returns.
     */
  void Shutdown();
};

} /* namespace networkinit */

#endif // NETWORK_INIT_INCLUDE_NETWORKINIT_NETWORKINIT_H_
