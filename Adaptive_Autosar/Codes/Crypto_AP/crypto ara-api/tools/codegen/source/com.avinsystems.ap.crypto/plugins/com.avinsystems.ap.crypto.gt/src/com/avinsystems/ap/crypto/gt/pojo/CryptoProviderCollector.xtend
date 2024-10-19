/*---------------------------------------------------------------------------------------------------------------------
 * Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved
 * Reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 *--------------------------------------------------------------------------------------------------------------------*/
package com.avinsystems.ap.crypto.gt.pojo

import java.util.ArrayList
import com.avinsystems.ap.crypto.gt.collector.CryptoDataCollector

/**  
 * CryptoProviderCollector fetch all the configured array.
 */
class CryptoProviderCollector {
 
  /**
   * This function fetches the CryptoProvider Data.
   * 
   * @param collector is the object of CryptoDataCollector.
   * 
   * @return CryptoProviderData Data as per Json file requirements.
   */
    def static getCryptoProviderDataConfigData(CryptoDataCollector collector) {
    val cryptoProviderConfigData = new ArrayList<CryptoProviderData>
    var cryptoProviderId=0
    
   /**  
    * Start fetching the element of cryptoProvider 
    */
    for (cryptoProvider : collector.cryptoPovider) {
        cryptoProviderConfigData.add(new CryptoProviderData(cryptoProvider,collector,cryptoProviderId))
        cryptoProviderId++ 
    }

   /** 
    * Return the object for json creation 
    */
    return cryptoProviderConfigData
  } 
}