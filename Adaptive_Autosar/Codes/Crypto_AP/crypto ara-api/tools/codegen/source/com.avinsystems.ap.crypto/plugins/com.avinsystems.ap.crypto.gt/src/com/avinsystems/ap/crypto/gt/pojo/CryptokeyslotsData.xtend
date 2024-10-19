/*---------------------------------------------------------------------------------------------------------------------
 * Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved
 * Reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 *--------------------------------------------------------------------------------------------------------------------*/
package com.avinsystems.ap.crypto.gt.pojo

import org.eclipse.xtend.lib.annotations.Accessors
import autosar40.adaptiveplatform.applicationdesign.cryptodesign.CryptoObjectTypeEnum
import autosar40.adaptiveplatform.applicationdesign.cryptodesign.CryptoKeySlotTypeEnum
import com.google.gson.annotations.SerializedName
import java.util.ArrayList
import autosar40.adaptiveplatform.platformmoduledeployment.cryptodeployment.CryptoKeySlot

/**
* This class used to fetch all required data from configuration to generate CryptokeyslotsData array in json.
*/
class CryptokeyslotsData {

 /**
  * Used to store id for Crypto keyslots.
  */
  @Accessors
  @SerializedName("uuid")
  String id
   
 /**
  * Used to store allocateShadowCopy for Crypto keyslots.
  */
  @Accessors
  @SerializedName("allocateShadowCopy")
  boolean isAllocateShadowCopy
  
 /**
  * Used to store cryptoAlgId for Crypto keyslots.
  */
  @Accessors
  @SerializedName("cryptoAlgId")
  String cryptoAlgId

 /**
  * Used to store cryptoObjectType for Crypto keyslots.
  */
  @Accessors
  @SerializedName("cryptoObjectType")
  CryptoObjectTypeEnum cryptoObjectType  
  
 /**
  * Used to store slotCapacity for Crypto keyslots.
  */
  @Accessors
  @SerializedName("slotCapacity")
  long slotCapacity  
  
 /**
  * Used to store slotType for Crypto keyslots.
  */
  @Accessors
  @SerializedName("slotType")
  CryptoKeySlotTypeEnum slotType
  
 /**
  * Used to store cryptoKeyslotAllowedModificationData for Crypto Keyslot Allowed Modification Data.
  */
  @Accessors
  @SerializedName("cryptokeyslotallowedmodification")
  val cryptokeyslotallowedmodification = new ArrayList<CryptoKeyslotAllowedModificationData>

 /**
  * Used to store cryptokeyslotcontentallowedusage for Crypto Keyslot Allowed Modification Data.
  */  
  @Accessors
  @SerializedName("cryptokeyslotcontentallowedusage")
  val cryptokeyslotcontentallowedusage = new ArrayList<CryptoKeyslotContentAllowedUsageData>


  new(){}

 /**
  * Parameterized constructor of CryptokeyslotsData.
  *
  * @param keyslots contain data from configuration.
  */  
  new(CryptoKeySlot keyslots){
        
        id=keyslots.uuid         
        isAllocateShadowCopy=keyslots.allocateShadowCopy
        cryptoAlgId=keyslots.cryptoAlgId
        cryptoObjectType=keyslots.cryptoObjectType
        slotCapacity =keyslots.slotCapacity
        slotType=keyslots.slotType
 
         /** 
          * Start fetching the element of keySlotAllowedModification. 
          */
          if(keyslots.keySlotAllowedModification!==null){
            cryptokeyslotallowedmodification.add(new 
            CryptoKeyslotAllowedModificationData(keyslots.keySlotAllowedModification))
          }
          
         /** 
          * Start fetching the element of keySlotContentAllowedUsages.
          */
          for(allowedUsageData:keyslots.keySlotContentAllowedUsages){
            cryptokeyslotcontentallowedusage.add(new CryptoKeyslotContentAllowedUsageData(allowedUsageData))
          }
    
  } 
  
}