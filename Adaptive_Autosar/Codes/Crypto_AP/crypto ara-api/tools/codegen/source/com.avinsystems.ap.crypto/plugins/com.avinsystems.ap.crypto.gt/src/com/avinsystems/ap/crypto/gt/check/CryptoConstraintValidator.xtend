/*---------------------------------------------------------------------------------------------------------------------
 * Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved
 * Reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 *--------------------------------------------------------------------------------------------------------------------*/
package com.avinsystems.ap.crypto.gt.check

import autosar40.adaptiveplatform.applicationdesign.cryptodesign.CryptoProviderInterface
import autosar40.swcomponent.components.RPortPrototype
import com.avinsystems.ap.crypto.gt.collector.CryptoDataCollector
import com.avinsystems.ap.gt.core.AbstractManifestGenerator
import com.avinsystems.ap.gt.core.AbstractValidator
import org.eclipse.sphinx.emf.check.Check

/**
 * This class is used to check invalid configuration.
 */
class CryptoConstraintValidator extends AbstractValidator {

  /**
   * This data member stores the object of CryptoDataCollector.
   */
  CryptoDataCollector cryptoDataCollector

  /* (non-Javadoc)
   * @see AbstractManifestGenerator#init().
   */
  override void init() {
    this.cryptoDataCollector = dataCollector as CryptoDataCollector
  }

  /**
   * CryptoProviderRport throws error when the Crypto provider to port prototype mapping 
   * configured with invalid context RPortPrototype.
   */
  @Check(constraint="CryptoProviderRport")
  def checkCryptoProviderRport() {
    for (cyptoProviderMapping : cryptoDataCollector.cryptoProviderToPortPrototypeMapping) {
      val storeObj=cyptoProviderMapping.portPrototype?.targetPortPrototype as RPortPrototype
      if(!(storeObj?.requiredInterface instanceof CryptoProviderInterface)){
        issue(cyptoProviderMapping, SHORT_NAME, cyptoProviderMapping.shortName)
      }
    }
  }

}