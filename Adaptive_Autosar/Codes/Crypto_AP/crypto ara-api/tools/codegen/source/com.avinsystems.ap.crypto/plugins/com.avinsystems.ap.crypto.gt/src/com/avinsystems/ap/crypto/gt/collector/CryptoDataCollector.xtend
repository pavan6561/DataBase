/*---------------------------------------------------------------------------------------------------------------------
 * Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved
 * Reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 *--------------------------------------------------------------------------------------------------------------------*/
package com.avinsystems.ap.crypto.gt.collector

import autosar40.adaptiveplatform.applicationdesign.applicationstructure.Executable
import autosar40.adaptiveplatform.platformmoduledeployment.cryptodeployment.CryptoCertificate
import autosar40.adaptiveplatform.platformmoduledeployment.cryptodeployment.
CryptoCertificateToCryptoKeySlotMapping
import autosar40.adaptiveplatform.platformmoduledeployment.cryptodeployment.
CryptoProvider
import autosar40.adaptiveplatform.platformmoduledeployment.cryptodeployment.
CryptoProviderToPortPrototypeMapping
import com.avinsystems.ap.crypto.gt.generator.CryptoManifestGenerator
import com.avinsystems.ap.crypto.gt.pojo.CryptoProviderGenerator
import com.avinsystems.ap.gt.core.AbstractDataCollector
import com.avinsystems.ap.gt.core.Collect
import java.util.ArrayList
import java.util.List
import org.eclipse.xtend.lib.annotations.Accessors
import autosar40.adaptiveplatform.applicationdesign.applicationstructure.AdaptiveApplicationSwComponentType

class CryptoDataCollector extends AbstractDataCollector {
  /**
   * Used to store list of CryptoCertificateToCryptoKeySlotMapping.
   */  
  @Accessors(PUBLIC_GETTER)
  List<CryptoCertificateToCryptoKeySlotMapping> cryptoCertificateToKeySlotMapping = new ArrayList
 
  /**
   * Used to store list of CryptoProviderToPortPrototypeMapping.
   */  
  @Accessors(PUBLIC_GETTER)
  List<CryptoProviderToPortPrototypeMapping> cryptoProviderToPortPrototypeMapping = new ArrayList

  /**
   * Used to store list of CryptoCertificate.
   */  
  @Accessors(PUBLIC_GETTER)
  List<CryptoCertificate> cryptoCertificate = new ArrayList

  /**
   * Used to store list of CryptoProvider.
   */  
  @Accessors(PUBLIC_GETTER)
  List<CryptoProvider> cryptoPovider = new ArrayList
  
  /**
   * Used to store list of Executable.
   */
  @Accessors(PUBLIC_GETTER)
  List<Executable> executables = new ArrayList
  
  /**
   * Used to store list of Process.
   */  
  @Accessors(PUBLIC_GETTER) 
  List<autosar40.adaptiveplatform.executionmanifest.Process> process= new ArrayList

  /**
   * Used to store list of AdaptiveApplicationSwComponentType.
   */  
  @Accessors(PUBLIC_GETTER)   
  List<AdaptiveApplicationSwComponentType> swComponent=new ArrayList
 
  /**
   * This method collects all the data to their respective lists.
   */
  @Collect
  def CryptoDataCollector() {
    this.cryptoPovider = resourceSet.allContents.filter(CryptoProvider).toList 
    this.cryptoProviderToPortPrototypeMapping = resourceSet.allContents.
    filter(CryptoProviderToPortPrototypeMapping).toList 
    this.executables= resourceSet.allContents.filter(Executable).toList
    this.process= resourceSet.allContents.filter(autosar40.adaptiveplatform.executionmanifest.Process).toList
    this.swComponent=resourceSet.allContents.filter(AdaptiveApplicationSwComponentType).toList
  }

  /* (non-Javadoc)
   * @see AbstractDataCollector#preCodeGenInit().
   */
  override preCodeGenInit() { 
    val cryptoProviderInterfaceData = new CryptoProviderGenerator(this)
    dynamicCppGeneratorList.addAll(cryptoProviderInterfaceData.generateHeaderFiles()) 
    
     
    var cryptoMnifest = new CryptoManifestGenerator
    cryptoMnifest.dataCollector=this;
    cryptoMnifest.init
    dynamicManifestGeneratorList.add(cryptoMnifest)
  }
}