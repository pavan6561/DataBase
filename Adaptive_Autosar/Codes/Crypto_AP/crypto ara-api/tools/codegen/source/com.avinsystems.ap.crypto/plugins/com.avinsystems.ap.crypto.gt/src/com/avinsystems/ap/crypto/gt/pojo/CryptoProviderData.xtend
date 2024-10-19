/*---------------------------------------------------------------------------------------------------------------------
 * Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved
 * Reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 *--------------------------------------------------------------------------------------------------------------------*/
package com.avinsystems.ap.crypto.gt.pojo

import org.eclipse.xtend.lib.annotations.Accessors
import java.util.ArrayList
import com.google.gson.annotations.SerializedName
import autosar40.adaptiveplatform.platformmoduledeployment.cryptodeployment.CryptoProvider
import com.avinsystems.ap.crypto.gt.collector.CryptoDataCollector

/**
 * CryptoProviderData class used to generate CryptoProvider array in manifest.
 */
class CryptoProviderData {
 /**
  * Used to store short name for Crypto Provider.
  */
  @Accessors
  @SerializedName("name")
  String name
  
 /**
  * Used to store unique id for Crypto Provider.
  */
  @Accessors
  @SerializedName("instanceid")
  String id
  
 /**
  * Used to store Crypto key slots data for Crypto Provider.
  */
  @Accessors
  @SerializedName("cryptokeyslots")
  val cryptokeyslots = new ArrayList<CryptokeyslotsData>

 /**
  * CryptoProviderData Default Constructor.
  */  
  new(){}

 /**
  * CryptoProviderData parameterized constructor used to generate manifest.
  *
  * @param cryptoProviderObj contains data of CryptoProvider container from configuration.
  * @param collector contains data of CryptoDataCollector container from configuration.
  * @param cryptoProviderId store array count used to get short name from configuration.
  */
  new(CryptoProvider cryptoProviderObj,CryptoDataCollector collector,int cryptoProviderId){
    
        var portShortName=collector.cryptoProviderToPortPrototypeMapping.get(cryptoProviderId).
        cryptoProvider.shortName
        var identifierFromPortShortName= InstanceSpecifier.HashCalculate(portShortName)
        name=cryptoProviderObj.shortName
        id="SOME/IP:"+identifierFromPortShortName.toString()
       
       /** 
        * Start fetching the element of keySlots 
        */
        for(keyslots : cryptoProviderObj.keySlots){        
          cryptokeyslots.add(new CryptokeyslotsData(keyslots))
        }
  }
}