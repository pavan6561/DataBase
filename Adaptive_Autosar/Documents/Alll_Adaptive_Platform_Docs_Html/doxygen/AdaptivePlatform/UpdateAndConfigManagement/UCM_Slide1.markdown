## Update and Configuration Management (UCM) {#ucm_overview}
## 1. Introduction
<p><b>Update and Configuration Management</b> has the responsibility of installing, updating and removing software on an Adaptive Platform in a safe and secure way while not sacrificing the dynamic nature of the Adaptive Platform.
These updates and changes can be to the AUTOSAR Adaptive Platform itself, including all functional clusters, the underlying POSIX OS and its kernel.
UCM exposes its functionality via <b>ara::com service interfaces</b> and not direct APIs. This ensures the user of UCM does not have to be located on the same ECU.</p> 


<p><b>Responsibilities</b>
- Version reporting of the software present in the AUTOSAR Adaptive Platform.
- Receiving and buffering software updates.
- Checking that enough resources are available to ensure a software update.
- Performing software updates and providing log messages and progress info.
- Validating the outcome of a software update.
- Providing rollback functionality to restore to a known functional state in case of failure. 
</p>

## 2. Limitations
-  UCM is not responsible to initiate the update process.
-  UCM realizes a service interface to achieve this operation.
-  The safe state of the vehicle has to be ensured by the client, before executing a software update procedure on demand.
-  UCM receives a locally available software package for processing.
-  Prior to triggering their processing, the software packages have to be transferred to UCM by using the provided ara::com interface.

## 3. Technical Overview

<TABLE>
<TR>
<TD>
<p>- The UCM functional cluster provides a service interface that exposes its functionality to consistently execute software updates.</p>
<p>- The Diagnostic Client uploads a Software Package by standard UDS services.</p>
<p>- After decompression and verification steps inside the OEM-specific Diagnostic Application, the Software Package is passed to UCM.</p>
<p>- Both OTA and UDS connections are very much OEM specific. For this reason a OEM specific application, like Diagnostic Service Application manages how data is transferred to UCM.</p>
<p>- UCM handles the package management after it has received the data.</p>

</TD>
<TD>
<p><b>Diagnostic Application communicating with UCM and DM. </b></p>
![ ](images/ucm_logicalview.png)</TD></TR>
</TABLE>

## 4. Software Package
<p>The unit for deployment which is the input for UCM is a <b>Software Package</b>. 
<TABLE>
<TR>
<TD>
![ ](images/swpkg.png)
</TD>
<TD>
<b>Content of a Software Package </b>
- Each Software Package addresses a single SoftwareCluster and contains manifests, executables and further data (depending on the role of the SoftwareCluster)
- The manifest include a name and a version. A time stamp will be trailing the Major.Minor.Patch version.
- UCM can create, update or remove any persistency data that is contained in the SoftwareCluster. Rollback of persisted data can be done to persistent data during update process.
- SoftwareCluster can have execution dependencies towards other SoftwareClusters. Execution dependencies are checked  in the end of the update process before activating/switching to the new software version.

</TD>
</TR>
</TABLE>
<p>
Each Software Package will address a single SoftwareCluster. A SoftwareCluster can act in two roles:
<ul>
<li><b>‘Sub’-SoftwareCluster</b> : A SoftwareCluster without diagnostic target address, containing processes, executables and further elements.</li> 
<li><b>‘Root’-SoftwareCluster</b>: A SoftwareCluster with a diagnostic target address that may reference several other <b>‘Sub’-SoftwareClusters</b>, which thus form a logical group.</li>
</ul>
<p>A software package has to be modelled as a so-called SoftwareCluster which describes the content of a software package that has to be uploaded to the AUTOSAR Adaptive Platform.
Software Package is used for the "physical", uploadable software package  that is processed by UCM  whereas the term SoftwareCluster is used for the modeling element.
</p>In the model, the content of a SoftwareCluster will be determined by references to all required model elements. The SoftwareCluster and the related model elements will be put into a manifest that is part of the Software Package. 
</p>


### 4.1 SW Package Manifest Dependency

<p>Installing a SW Package in <b>Dependency Example 1</b> has <i>Application A, Application B and Library A</i>  and in <b>Dependency Example 2</b> 
has <i>Application A, Manifest</i> files as dependencies. So during installation dependencies will be resolved and application list builder will be updated 
with these resolved Dependencies.</p>

<TABLE>
<TR><TD>
![Software Package Dependency](images/swpkg_depend1.png)
</TD>
<TD><p><b>Dependency Example 1</b></p>
<p>Software Package contents:</p>
-  Application C, Application D and Library B (provided contents)
-  Manifest "Second"  has dependency of >>
           -  Application A
           -  Application B
           -  Library A
-  Before installation application listbuilder will  check for the installed applications. After checking for dependencies, 
    Manifest "First" containing >>
           -  Application A 
		   -  Application B 
		   -  Library A are installed.
</TD>
</TR>
<TR><TD></TD><TD></TD></TR>
<TR><TD>
![Software Package Dependency](images/swpkg_depend2.png)
</TD>
<TD><p><b>Dependency Example 2</b></p>
<p>Software Package contents:</p>
-  Application E and Library C (provided contents)
-  Manifest "Third"  has dependency of >>
           -  Application A
		   -  Manifest "Second"
				-  Manifest "Second" has dependency of >>
					-  Application A
					-  Application B
					-  Library A
- Installation sequence >>
  - 1)Manifest "Third" contains >> 
           -  Application A 
  - 2)Followed  by Manifest "Second" containing >>
           -  Application A
		   -  Application B
		   -  Library A   
</TD>
</TR>
</TABLE>
		

## 5. Software Package Management 
The UCM update sequence consists of three different phases:

- <b>Software Package transfer</b>:  Phase in which, one or several Software Packages are transferred from the Diagnostic Application Client to the internal buffer of the UCM.
- <b>Software Package processing</b>: Phase in which the UCM performs the operation (kInstall,  kUpdate, kRemove) on the relevant SoftwareCluster (which will be inactivated [not executed] during the operation). 
- <b>Activation</b>: Phase in which the UCM checks the dependencies of the SoftwareClusters that have been involved in the operation, activates them and finally checks that all the SoftwareClusters can be executed properly prior to finishing the update. 


### 5.1 Transferring Software Packages
<p>Each Software Package gets its own state as soon as it is being transferred to UCM.
During this lifecycle, a Software Package is uniquely identified with a <b>TransferId</b> that UCM provides to the client.</p>
![](images/transfer_sw.png)

- UCM provides  support to transfer Software Packages at any time when UCM is running. Transferring is decoupled from the UCM Package Management states. 
- Data transfer starts with method <b>TransferStart(size of pkg)</b>. Number of bytes to be transferred is provided by the client. On successful allocation of resources,  UCM assigns a <b>Transfer id</b> for the Software Package to be transferred. 
- <b>TransferData(id, buffer, block counter)</b> method supports the transmission of the Software Package block-wise.
- <b>TransferExit(Transfer id)</b> is called on a successful transfer of Software Package.
- Handling of multiple data transfers in parallel will be supported by UCM. While a software package is being transferred, if UCM receives a subsequent <b>TransferStart()</b> call targeting another software package, UCM will make sure the UCM buffer size does not exceed. 
- The processing (i.e. unpacking and actually applying changes to the AUTOSAR Adaptive Platform) of Software Packages is described by the state kProcessing .
- <b>DeleteTransfer(id)</b> is  a method that will delete the targeted Software Package and free the resources reserved to store that Software Package. 
- If <b>TransferData() or TransferExit()</b> is called without prior call to <b>TransferStart()</b> UCM will raise the <b>ApplicationError OperationNotPermitted</b>. 


### 5.2 Processing Software Packages 

<p>In contrast to package transmission, only one Software Package can be processed at the same time to ensure consistency of the system.</p>

- A software or package processing can involve any combination of an installation, update or removal of applications, configuration data, calibration data or manifests.
- UCM provides a method <b>ProcessSwPackage(id)</b> to process transferred Software Package. 
- UCM will support processing of several Software Packages by calling method <b>ProcessSwPackage()</b> several times in sequence. 
- <b>GetSwProcessProgress()</b> method provides the percentage of progress of the package.
- UCM will raise the <b>ApplicationError PackageInconsistent</b> in case the package to be processed is not consistent.
- UCM provides a method <b>Cancel(id)</b> to cancel the running package processing. UCM will then immediately abort the current package processing task and undo the required changes.

### 5.3  Status Reporting
<p>In contrast to the transmission, the processing and activation tasks happen in a strict sequential order.The global state of UCM can be queried using the field <i>CurrentStatus</i>.</p>

![ ](images/pkgmgr_status.png)
- <i>kIdle</i> will be the default state. Once <b>ProcessSwPackage()</b> is performed successfully, this state will only be entered if a Clean-up has been performed successfully. 
- <i>kBusy</i> state will be set only if <b>ProcessSwPackage()</b> has been called. This is possible, if <i>CurrentStatus -> kIdle or kReady</i>.
- Busy state will be exited 
    - when calling of  <b>ProcessSwPackage() or RevertProcessedSwPackages()</b> has finished.
    - when the processing of the package is interrupted by calling <b>Cancel()</b>. 
    <p>State reported by CurrentStatus :
	- <i>kCleaning-up</i> - in case of a <b>RevertProcessedSwPackages()</b> call. 
	- <i>kReady</i>       - in case of a <b>ProcessSwPackage()</b> completion or in case of a <b>Cancel()</b> call.</p> 
- Ready state will be reported
    -  after the package processing is finished successfully. 
    -  <b>Cancel()</b> has been performed.
    -  when <b>Activate()</b> fails due to an <b>ApplicationError MissingDependencies</b>.
- Activating state of Package Management <i>kActivating</i> will be set 
    - when <b>Activate</b> is called. 
  <p>After <b>Activate</b> was performed successfully (all dependencies are satisfied), the system  has to be restarted in case a A/B partition is used. 
     In case A/B partition is not used, all affected Function Groups or the platform could be restarted. Immediately after the processed Software Cluster has been restarted, 
	 a system check has to be performed in order to make sure the system is able to start up as expected. </p>
- Activated state of Package Management <i>kActivated</i> state gives the client controlling the update process a chance to perform verification test.
  If the system check was successful, the client can decide either to Rollback to the previous processed working software, or to perform <b>Finish</b> so that the changes of processed software become permanent.
- By calling <b>Finish()</b> a clean-up is initiated. In case of A/B partition, a swap between the partitions happens and the newly inactive partition becomes a copy of the newly active partition. 
  On a successful <b>Finish()</b> (including cleanup), <i>CurrentStatus</i> changes to <i>kIdle</i>.
- Calling <b>Rollback()</b> sets UCM into <i>kRollingBack</i> state where original software version is made executable and to <b>kRolledBack</b> state when the original software is activated by the State Management.
  UCM will switch the state into <b>kCleaning-up</b> state when <b>Finish() or RevertProcessedSwPackages()</b> is called. 
  kIdle will be set when Finish() is called and the clean-up has been successfully performed. 


### 5.4 Activation and Rollback 
#### 5.4.1 Activation
- The installed SoftwareCluster does not express whether a SoftwareCluster is currently executed or not.
- When <b>Activate()</b> is called, UCM will check if dependencies for the SoftwareClusters are all fulfilled. Unfulfilled dependencies will raise the <b>ApplicationError MissingDependencies</b>.
- The activation method could either lead to a full system reset or restart of Function Groups impacted by the Software Package. 
- The UCM does not trigger the restart of processed software. This needs to be performed by the client application. This is due to the fact that such restart might need to be synchronized between several Platforms/ECUs.
- UCM will switch from Activating state to Activated state when the State Management FunctionGroupState field notifies that the updated software is executed successfully. 

![ ](images/act_proc.png)

#### 5.4.2 Rollback 
- UCM will provide a method <b>Rollback()</b> to recover from an activation that went wrong. Rollback can be called in the case of A/B partitions or if UCM uses some other solution to maintain backups of updated or removed Software Packages.
- During Rolling-Back UCM will disable the changes done by the software update. 
- While Rolling-Back, UCM can forcefully shutdown the newly processed software (i.e the one that needs to be the Rolled-back).

#### 5.4.3 Boot options
- UCM will manage which version of software is available for the Execution Management to launch. 
- During the <i>kActivating</i> state UCM modifies the Boot options so that in the next restart of the updated software the new versions will be executed. 
- In the <i>kRollingBack</i> state UCM modifies the Boot options so that in the next restart of the updated software the original versions will be executed.


### 5.5 Software Cluster Lifecycle
<TABLE>
<TR>
<TD>
![ ](images/swcluster.png)
</TD>
<TD><p>The <b>state machine</b> in the figure describes the states of a SoftwareCluster :</p>
<p>- The new SoftwareCluster starts its lifecycle with state <i>kAdded</i>.</p>
<p>- After finishing update process with method Finish, it is in state <i>kPresent</i>.</p>
<p>- In another update process, by processing a Software Package with new data for the SoftwareCluster, it changes to <i>kUpdated</i>.</p>
<p>- Returns to <i>kPresent</i> once update process has finished. If a Software Package is processed and it involves the deletion of an existing SoftwareCluster the state changes to <i>kRemoved</i>.</p>
</TD>
</TR>
</TABLE>

### 5.6 Version Reporting

<p>- UCM provides a method <b>GetSwClusterInfo()</b> to provide the identifiers and versions of the SoftwareClusters that are in state <i>kActivated</i>.</p> 
<p>- UCM provides a method <b>GetSwClusterChangeInfo()</b> to provide the identifiers and versions of the SoftwareCluster that are in state <i>kAdded, kUpdated or kRemoved</i>.</p>


## 6. Dependencies to other Functional Clusters
    1) Interfaces to Adaptive State Management
		- Few applications can conflict with the update process and might need to be stopped during the update process. This will be achieved by putting the machine to a safe Machine State, for example <b>Update State</b>, 
		  or by activating a combination of suitable Function Groups and its states. It is the responsibility of the platform integrator to define this state or Function Groups. 
          The application accessing the UCM, should make sure that the platform is switched to this state, before starting an update. UCM uses State Management interface field parameter <b>FunctionGroupState</b> 
		  to monitor the restart of the updated software.

    2) UCM service over ara::com
        - UCM provides a service interface over ara::com using methods and fields.
		
    3) Interfaces to Adaptive Crypto Interface
		- UCM uses Crypto Interface for Adaptive Platform  to verify package integrity and authenticity and to decrypt confidential update data.

    4) Interfaces to Identity and Access Management
		- Communication Management,uses Identity and Access Management to validate the authorization of requests made to UCM’s service interface PackageManagement.

## 7. Service Interfaces

<p> Service Interface PackageManagement - Field</p>
<TABLE>
<TR><TD bgcolor = #0xC8BFE7>Field</TD><TD bgcolor = #0xC8BFE7>Description</TD></TR>
<TR><TD>CurrentStatus</TD><TD>The current status of UCM.</TD></TR>
</TABLE>

<p>Service Interface PackageManagement - Methods</p>
<TABLE>
<TR><TD bgcolor = #0xC8BFE7>Method</TD><TD bgcolor = #0xC8BFE7>Description</TD></TR>
<TR>
<TD>GetSwClusterInfo()</TD><TD>Retrieve a list of SoftwareClusters.</TD>
</TR>

<TR>
<TD>GetSwClusterChangeInfo()</TD><TD>This method returns a list pending changes to the set of SoftwareClusters on the adaptive platform. The returned list includes all SoftwareClusters that are to be added, updated or removed.</TD>
</TR>

<TR>
<TD>GetSwPackages</TD><TD>This method returns a list of all Software Packages that have been successfully  transferred and are ready to be installed.</TD>
</TR>

<TR>
<TD>TransferStart(buffer size)</TD><TD>Start the transfer of a Software Package. The size of the Software Package to be transferred to UCM must be provided. UCM will generate a Transfer ID for subsequent calls to TransferData, TransferExit, ProcessSwPackage.</TD>
</TR>

<TR>
<TD>TransferData(transfer id, data, block counter)</TD><TD>Block-wise transfer of a Software Package to UCM.</TD>
</TR>

<TR>
<TD>TransferExit(transfer id)</TD><TD>Finish the transfer of a Software Package to UCM.</TD>
</TR>

<TR>
<TD>DeleteTransfer(transfer id)</TD><TD>Delete a transferred Software Package.</TD>
</TR>

<TR>
<TD>ProcessSwPackage(transfer id)</TD><TD>Process a transferred Software Package.</TD>
</TR>

<TR>
<TD>RevertProcessedSwPackages()</TD><TD>Reverts the changes done by processing (ProcessSwPackage) of one or several software packages.</TD>
</TR>

<TR>
<TD>GetSwProcessProgress(transfer Id)</TD><TD>Get the progress (0 - 100%) of the currently processed Software Package.</TD>
</TR>

<TR>
<TD>Cancel(transfer Id)</TD><TD>This method aborts an ongoing processing of a Software Package.</TD>
</TR>

<TR>
<TD>Rollback()</TD><TD>Rollback the system to the state before the packages were processed.</TD>
</TR>

<TR>
<TD>Activate(transfer id)</TD><TD>This method activates the processed software components.</TD>
</TR>

<TR>
<TD>Finish()</TD><TD>Finish the processing for the current set of processed Software Packages. Cleanup all data of the processing including the sources of the Software Packages.</TD>
</TR>

</TABLE>
		