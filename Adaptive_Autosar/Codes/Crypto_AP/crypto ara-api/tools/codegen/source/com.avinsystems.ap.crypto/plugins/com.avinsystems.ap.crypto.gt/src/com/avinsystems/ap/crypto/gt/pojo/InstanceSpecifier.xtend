/*---------------------------------------------------------------------------------------------------------------------
 * Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved
 * Reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 *--------------------------------------------------------------------------------------------------------------------*/
package com.avinsystems.ap.crypto.gt.pojo

import com.avinsystems.ap.crypto.gt.collector.CryptoDataCollector
import autosar40.adaptiveplatform.applicationdesign.applicationstructure.
AdaptiveApplicationSwComponentType
import autosar40.swcomponent.components.RPortPrototype
import autosar40.adaptiveplatform.applicationdesign.cryptodesign.CryptoProviderInterface
import autosar40.adaptiveplatform.applicationdesign.cryptodesign.CryptoCertificateInterface
import autosar40.adaptiveplatform.applicationdesign.cryptodesign.CryptoKeySlotInterface

/**
 * This class contain the methods to collect the list of instance specifiers.
 */
class InstanceSpecifier {
    new() {
  }

  /**
   * This function generate the hash value.
   * 
   * @param String contain the port name.
   * 
   * @return Hash calculated number
   */
  def static HashCalculate(String s) {
   var h = 0;
   for (var i = 0; i < s.length(); i++)
   { 
    h = 31 * h + s.toCharArray.get(i);
   }
   
   return Math.abs(h.shortValue);
 }
  /**
   * This function is to create Map of instance specifier.
   * 
   * @param CryptoDataCollector contain the collector.
   * 
   * @return Map of instance specifier
   */ 
    def static getInstanceSpecifier(CryptoDataCollector collector) {
    var collectAllInString='''{'''
    var fileSeperate='''/'''

    var cryptoProviderId=0

    val rportStorage = newArrayList 

    val cryptoExecutable = newArrayList 

  /**
   * Collect all Crypto Executables.
   */   
  	for(executable:collector.executables.filter[(rootSwComponentPrototype!==null)
     && rootSwComponentPrototype.applicationType!==null]){
    
        val rportsCryptoProvider=(executable.rootSwComponentPrototype.applicationType as 
            AdaptiveApplicationSwComponentType).ports.filter(RPortPrototype).filter[
                (requiredInterface instanceof CryptoProviderInterface)||(requiredInterface 
                instanceof CryptoKeySlotInterface)
                ||(requiredInterface instanceof CryptoCertificateInterface)
            ].toList

        if(!(rportsCryptoProvider.empty)){
          cryptoExecutable.add(executable.shortName)
        }
    }
     

  /**
   * Collect all Rport mapped to CryptoProviderInterface.
   */ 
    for(processes:collector.process.filter[executable!==null]){
   
        if(cryptoExecutable.contains(processes.executable.shortName)){
            val rports=((processes.executable.rootSwComponentPrototype.applicationType as 
            AdaptiveApplicationSwComponentType).ports.filter(RPortPrototype).filter[requiredInterface
             instanceof CryptoProviderInterface].toList)
            for(sortingCryptoProviderRport:rports){
              rportStorage.add(sortingCryptoProviderRport.shortName)
            }
        }
    }

  /**
   * Calculating Hash value for respective Rport and storing into Map. 
   */ 
    for(mapping:collector.cryptoProviderToPortPrototypeMapping){
    
    var executableName=mapping.portPrototype.contextRootSwComponentPrototype.executable.
    shortName+fileSeperate
    var rootSwComponentPrototypeName=mapping.portPrototype.contextRootSwComponentPrototype.executable.
    rootSwComponentPrototype.shortName+fileSeperate
    var SwComponentName=mapping.portPrototype.contextRootSwComponentPrototype.executable.
    rootSwComponentPrototype.applicationType.shortName+fileSeperate
    var portShortName=mapping.portPrototype.targetPortPrototype.shortName
    var cryptoPorviderSn=mapping.cryptoProvider.shortName
        if(rportStorage.contains(portShortName)){

        var instanceIdentifier=HashCalculate(cryptoPorviderSn)
        var instanceIdentifierToString="SOME/IP:"+instanceIdentifier.toString()
        if(cryptoProviderId==0){
          collectAllInString+='''{"«executableName+rootSwComponentPrototypeName+
          SwComponentName+portShortName»","«instanceIdentifierToString»"}
          '''
          }else{
          collectAllInString+=''',{"«executableName+rootSwComponentPrototypeName+
          SwComponentName+portShortName»","«instanceIdentifierToString»"}
          '''  
          }     
          cryptoProviderId++
        }
    }

     collectAllInString+='''};'''
     println(" Size "+rportStorage.size())
     rportStorage.clear()
     println("String is "+collectAllInString) 
     
     return collectAllInString
  }
}