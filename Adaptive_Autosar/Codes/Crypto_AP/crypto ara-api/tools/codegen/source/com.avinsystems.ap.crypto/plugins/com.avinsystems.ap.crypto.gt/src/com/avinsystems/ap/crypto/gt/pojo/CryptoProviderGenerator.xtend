/*---------------------------------------------------------------------------------------------------------------------
 * Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved
 * Reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 *--------------------------------------------------------------------------------------------------------------------*/
package com.avinsystems.ap.crypto.gt.pojo

import com.avinsystems.ap.crypto.gt.collector.CryptoDataCollector
import com.avinsystems.ap.crypto.gt.generator.CryptoCppGenerator
import com.avinsystems.ap.gt.core.AbstractCppGenerator
import java.util.ArrayList
import java.util.List

class CryptoProviderGenerator {
 /**
  * CryptoCollector is the object of CryptoDataCollector.
  */
  CryptoDataCollector cryptoCollector
  
 /**
  * Used to store the data of header files.
  */
  val List<AbstractCppGenerator> generatedDataList = newArrayList

  /**
   * Instantiate the CryptoProviderGenerator class and initialize the CryptoDataCollector variable.
   *
   * @param collector is the object of CryptoDataCollector.
   */
  new(CryptoDataCollector collector) {
    this.cryptoCollector = collector
  }

  /**
   * generateHeaderFiles invokes the generateHeaderFile with related process configured to instance specifier for all
   * CryptoProviderGeneratorInterface.
   *
   * @return list of header files for crypto provider generator interface.
   *
   */
  def generateHeaderFiles() {
    val fileName = "crypto_provider_port_mapping.cpp"
    val interfaceName = "crypto_provider"
    val filePurpose = #[
      '''To Generate the Header file for «interfaceName» '''
    ]
    val outputPath = new ArrayList<String>
    outputPath.addAll("gen", "ara", "crypto", interfaceName)
    val String code = '''

      #include <cstdint>
      #include <ara/core/map.h>
      #include <ara/core/string.h>

      namespace ara
      {
      namespace crypto
      { 
       
      ara::core::Map<ara::core::String,ara::core::String> const 
      CryptoProviderInstanceSpecifierMapping«InstanceSpecifier.getInstanceSpecifier(cryptoCollector)»
      
      ara::core::String GetCryptoProviderInstanceIdentifier(ara::core::String inputStr)
      {
        ara::core::String instanceIdentifier;  
        auto instanceIDIterator = CryptoProviderInstanceSpecifierMapping.find(inputStr);
        if ((instanceIDIterator) != (CryptoProviderInstanceSpecifierMapping.end()))
        {
          // Fetch instanceIdentifier.
          instanceIdentifier = instanceIDIterator->second;
        }
        return instanceIdentifier;
      }
      
      
      } // End of namespace for crypto
      } // End of namespace for ara
    '''
    generatedDataList.add(new CryptoCppGenerator(fileName,filePurpose,outputPath,code))
    return generatedDataList
  }
    
}