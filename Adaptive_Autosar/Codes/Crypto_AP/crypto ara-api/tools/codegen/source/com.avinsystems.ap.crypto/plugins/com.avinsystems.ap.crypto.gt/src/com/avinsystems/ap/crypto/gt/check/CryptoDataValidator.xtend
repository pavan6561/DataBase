/*---------------------------------------------------------------------------------------------------------------------
 * Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved
 * Reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 *--------------------------------------------------------------------------------------------------------------------*/
package com.avinsystems.ap.crypto.gt.check

import com.avinsystems.ap.crypto.gt.collector.CryptoDataCollector
import com.avinsystems.ap.gt.core.AbstractValidator
import org.eclipse.sphinx.emf.check.Check
import autosar40.adaptiveplatform.applicationdesign.applicationstructure.AdaptiveApplicationSwComponentType
import autosar40.swcomponent.components.RPortPrototype
import autosar40.adaptiveplatform.applicationdesign.cryptodesign.CryptoProviderInterface
import autosar40.adaptiveplatform.applicationdesign.cryptodesign.CryptoKeySlotInterface
import autosar40.adaptiveplatform.applicationdesign.cryptodesign.CryptoCertificateInterface

/**
 * This class extends an abstract class of validator.
 */
class CryptoDataValidator extends AbstractValidator {

  /**
   * This data member stores the object of CryptoDataCollector.
   */
  CryptoDataCollector cryptoDataCollector

  /* (non-Javadoc)
   * @see AbstractValidator#init().
   */
  override void init() {
    this.cryptoDataCollector = dataCollector as CryptoDataCollector
  }
  
  /**
   * checkCryptoProviderToPrototypeMapping throws error when 
   * CryptoProviderToPortPrototypeMapping is not configured.
   */
  @Check(constraint="CryptoProviderToPrototypeMapping")
  def checkCryptoProviderToPrototypeMapping() {
    if(!(cryptoDataCollector.cryptoProviderToPortPrototypeMapping.empty)){
      for(cryptoProvider:cryptoDataCollector.cryptoProviderToPortPrototypeMapping){
        if(cryptoProvider.getPortPrototype.targetPortPrototype.shortName===null){
          issue(cryptoProvider, SHORT_NAME, cryptoProvider.shortName)
        } 
      }
    }else{
      issue("CryptoProviderToPortPrototypeMapping", SHORT_NAME)
    }
  }
  
  /**
   * Executables throws error when Executable  is not configured.
   */
  @Check(constraint="RootSwComponentPrototype")
  def checkProcessToPortMapping() {
  
      val cryptoExecutable = newArrayList 
      for(executable:cryptoDataCollector.executables.filter[(rootSwComponentPrototype!==null)
      && rootSwComponentPrototype.applicationType!==null]){
    
            val rportsCryptoProvider=(executable.rootSwComponentPrototype.applicationType as 
            AdaptiveApplicationSwComponentType).ports.filter(RPortPrototype).filter[
              (requiredInterface instanceof CryptoProviderInterface)||
              (requiredInterface instanceof CryptoKeySlotInterface)
              ||(requiredInterface instanceof CryptoCertificateInterface)
            ].toList

            if(!(rportsCryptoProvider.empty)){
              cryptoExecutable.add(executable.shortName)
            }
      }
     
      for(cryptoProcess:cryptoDataCollector.process.filter[executable !==null]){
      	if(cryptoExecutable.contains(cryptoProcess.executable.shortName)){
            if(cryptoProcess.executable===null){
            issue(cryptoProcess, SHORT_NAME, cryptoProcess.executable)
            }else if(cryptoProcess.executable.rootSwComponentPrototype===null){
            issue(cryptoProcess, SHORT_NAME, cryptoProcess.executable.rootSwComponentPrototype)
            }else if(cryptoProcess.executable.rootSwComponentPrototype.applicationType===null){
            issue(cryptoProcess, SHORT_NAME, cryptoProcess.executable.rootSwComponentPrototype.
            applicationType)         
            }
        }
      }
  }

  /**
   * Every Crypto provider instance must and should be mapped.
   */
  @Check(constraint="CryptoProviderInstanceToPortPrototypeMapping")
  def checkCryptoProviderInstanceMapping() {

    val cryptoProvider = newArrayList
      
    for(cryptoProviderName:cryptoDataCollector.cryptoPovider){
        cryptoProvider.add(cryptoProviderName.shortName)
    }
  
    for(cryptoPortPrototypeMapping:cryptoDataCollector.cryptoProviderToPortPrototypeMapping){

        if(!(cryptoProvider.contains(cryptoPortPrototypeMapping.cryptoProvider.shortName))){
          issue(cryptoPortPrototypeMapping, SHORT_NAME, cryptoPortPrototypeMapping.cryptoProvider.shortName)
        }  
      }
 
  } 
  
  /**
   * Every Crypto provider instance must and should be mapped.
   */
  @Check(constraint="Process")
  def checkEachProcess() {
      for(cryptoProcess:cryptoDataCollector.process){
        if(cryptoProcess.executable===null){
          issue(cryptoProcess, SHORT_NAME, cryptoProcess.executable)
        }else if(cryptoProcess.executable.rootSwComponentPrototype===null){
          issue(cryptoProcess, SHORT_NAME, cryptoProcess.executable.rootSwComponentPrototype)
        }else if(cryptoProcess.executable.rootSwComponentPrototype.applicationType===null){
       	  issue(cryptoProcess, SHORT_NAME, cryptoProcess.executable.rootSwComponentPrototype.applicationType)
        }
      }
   } 
             
  /**
   * UUID should be configured.
   */
  @Check(constraint="UUID")
  def checkUuid() {
  for(cryptoProvider:cryptoDataCollector.cryptoPovider){
    for(keyslots:cryptoProvider.keySlots){
      if(keyslots.uuid.empty){
        issue(keyslots, SHORT_NAME, keyslots.shortName)
      }
    }
  }
 }
}