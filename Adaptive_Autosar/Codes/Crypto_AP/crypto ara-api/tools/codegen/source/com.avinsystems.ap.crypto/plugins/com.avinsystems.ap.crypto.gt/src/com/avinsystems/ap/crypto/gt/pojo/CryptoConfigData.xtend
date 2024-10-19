/*---------------------------------------------------------------------------------------------------------------------
 * Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved
 * Reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 *--------------------------------------------------------------------------------------------------------------------*/
package com.avinsystems.ap.crypto.gt.pojo

import com.avinsystems.ap.crypto.gt.collector.CryptoDataCollector
import com.google.gson.annotations.SerializedName
import java.util.ArrayList
import org.eclipse.xtend.lib.annotations.Accessors

class CryptoConfigData {

  /**
   * Used to store list of Crypto Provider.
   */
  @Accessors
  @SerializedName("cryptoprovider")
  val cryptoproviderdata = new ArrayList<CryptoProviderData>

  /**
   * This function collects the Crypto configuration Data as per Json requirements.
   * 
   * @param collector is the object of CryptoDataCollector.
   */
  def void createCryptoManifestConfigData(CryptoDataCollector collector) {
     clearAll()
     cryptoproviderdata.addAll(CryptoProviderCollector.getCryptoProviderDataConfigData(collector))
  }

  /**
   * Used to clear all the Maps.
   */
  def void clearAll() {
    cryptoproviderdata.clear()
  }
}