/*---------------------------------------------------------------------------------------------------------------------
 * Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved
 * Reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 *--------------------------------------------------------------------------------------------------------------------*/
package com.avinsystems.ap.crypto.gt.generator

import com.avinsystems.ap.crypto.gt.collector.CryptoDataCollector
import com.avinsystems.ap.crypto.gt.pojo.CryptoConfigData
import com.avinsystems.ap.gt.core.AbstractManifestGenerator
import java.util.List

class CryptoManifestGenerator extends AbstractManifestGenerator{
  /**
   * cryptoConfigObj is the object of CryptoConfigData.
   * @tags SWS_CRYPT_S7.2.3_F7.6
   */
  val cryptoConfigObj = new CryptoConfigData()

  /* (non-Javadoc)
   * @see AbstractManifestGenerator#init().
   */
  override init() {
    val collector = this.dataCollector as CryptoDataCollector
    cryptoConfigObj.createCryptoManifestConfigData(collector)
  }

  /* (non-Javadoc)
   * @see AbstractManifestGenerator#getFileName().
   */
  override String getFileName() {
    return "crypto_config.json"
  }

  /* (non-Javadoc)
   * @see AbstractManifestGenerator#getOutputPath().
   */
  override List<String> getOutputPath() {
    return #["gen", "crypto-manifest"]
  }

  /* (non-Javadoc)
   * @see com.avinsystems.ap.gt.core.AbstractManifestGenerator.getFileContent().
   */
  override Object getFileContent() {
    return cryptoConfigObj
  }
}