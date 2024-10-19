/*---------------------------------------------------------------------------------------------------------------------
 * Copyright (C) AVIN SYSTEMS PVT LTD 2021 - All Rights Reserved
 * Reproduction, distribution and utilization of this document as well as the communication of its contents to
 * others without explicit authorization is prohibited. Offenders will be held liable for the payment of damages.
 *--------------------------------------------------------------------------------------------------------------------*/
package com.avinsystems.ap.crypto.gt.generator

import com.avinsystems.ap.gt.core.AbstractCppGenerator
import java.util.List

class CryptoCppGenerator extends AbstractCppGenerator {
  
  /**
   * Variable of type string and used to store file name.
   */
   val String fileName

  /**
   * Variable of type string and used to store file content.
   */
   val String fileContent

  /**
   * List of type string and used to store file purpose.
   */
   val List<String> filePurpose

  /**
   * List of type string and used to store output path.
   */
   val List<String> outputPath
  
  /**
   * Instantiate the PhmCppGenerator and initialize the variables.
   * 
   * @param fileName - String
   * @param filePurpose - List<String>
   * @param outputPath - List<String>
   * @param fileContent - String
   */
   new(String fileName, List<String> filePurpose, List<String> outputPath, String fileContent) {
    this.fileName = fileName
    this.filePurpose = filePurpose
    this.outputPath = outputPath
    this.fileContent = fileContent
  }
      
  /* (non-Javadoc)
   * @see com.avinsystems.ap.gt.core.AbstractCppGenerator#getFileContent().
   */
  override getFileContent() {
    return fileContent
  }
  
  /* (non-Javadoc)
   * @see com.avinsystems.ap.gt.core.AbstractCppGenerator#getFileName().
   */
  override getFileName() {
    return fileName
  }
  
  /* (non-Javadoc)
   * @see com.avinsystems.ap.gt.core.AbstractCppGenerator#getFilePurpose().
   */
  override getFilePurpose() {
    return filePurpose
  }
  
  /* (non-Javadoc)
   * @see com.avinsystems.ap.gt.core.AbstractCppGenerator#getOutputPath().
   */
  override getOutputPath() {
    return outputPath
  }
  
  /* (non-Javadoc)
   * @see com.avinsystems.ap.gt.core.AbstractCppGenerator#init().
   */
  override init() {
    /* Nothing to init */
  }
  
}