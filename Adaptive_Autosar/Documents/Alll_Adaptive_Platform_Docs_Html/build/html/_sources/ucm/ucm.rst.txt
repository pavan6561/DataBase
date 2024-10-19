**Update and Configuration Management (UCM)**
================================================

Introduction
##############

**Update and Configuration Management** has the responsibility of installing, updating and removing software on an Adaptive Platform in a safe and secure way while not sacrificing the dynamic nature of the Adaptive Platform. These updates and changes can be to the AUTOSAR Adaptive Platform itself, including all functional clusters, the underlying POSIX OS and its kernel. UCM exposes its functionality via **ara::com service interfaces** and not direct APIs. This ensures the user of UCM does not have to be located on the same ECU.

Responsibilities
#################

    - Version Reporting of the software present in the AUTOSAR Adaptive Platform.
    - Receiving and buffering software updates.
    - Checking that enough resources are available to ensure a software update.
    - Performing software updates and providing log messages and progress info.
    - Validating the outcome of a software update.
    - Providing rollback functionality to restore to a known functional state in case of failure.

Limitations
##############

    - UCM is not responsible to initiate the update process.
    - UCM realizes a service interface to achieve this operation.
    - The safe state of the vehicle must be ensured by the client, before executing a software update procedure on demand.
    - UCM receives a locally available software package for processing.
    - Prior to triggering their processing, the software packages have to be transferred to UCM by using the provided ara::com interface.

Technical Overview
#####################

    - One of the declared goals of AUTOSAR Adaptive Platform is the ability to flexibly update the software and its configuration through over-the-air updates.
    - During the life-cycle of an AUTOSAR Adaptive Platform, UCM is responsible to perform software modifications on the machine and to retain consistency of the whole system.
    - The UCM Functional Cluster provides a service interface to retrieve AUTOSAR Adaptive Platform software information and consistently execute software updates. Since ara::com is used, the client using the UCM service interface can be located on the same AUTOSAR Adaptive Platform, but also remote clients are possible.
    - It is also possible to use standard diagnostic services for downloading and installing software updates for the AUTOSAR Adaptive Platform.

.. figure:: resources/ucm_logicalview.png
   :class: with-border
   :align: center

   **UCM Logical View** 
   
Dependencies to other functional clusters
############################################

The UCM functional cluster expose services to client applications via the ara::com middleware.

.. figure:: resources/UCM_dependencies_to_other_Functional_clusters.png
   :class: with-border
   :align: center

   **UCM dependencies to other Functional clusters** 
   
Interfaces to Adaptive State Management
---------------------------------------

UCM relies on State Management and its provided UpdateRequest Service Interface to perform the necessary Function Group state changes needed to activate the newly installed, updated or removed software.

UCM service over ara::com
-------------------------

The UCM shall provide a service interface over ara::com using methods and fields.

Interfaces to Adaptive Crypto Interface
----------------------------------------

UCM uses Crypto Interface to verify package integrity and authenticity and to decrypt confidential update data.

Interfaces to Identity and Access Management
---------------------------------------------

Identity and Access Management controls the UCM’s Clients access to UCM’s service interface PackageManagement.

Software Package
-----------------

UCM takes Software Package as input to perform software install, updates and removal.

.. figure:: resources/Software_Package.png
   :class: with-border
   :align: center
   
   **Software Package**

**Content of a Software Package:**

    - Each Software Package addresses a single SoftwareCluster and contains manifests, executables and further data (depending on the role of the SoftwareCluster)
    - The manifest includes a name and a version. A time stamp will be trailing the Major.Minor.Patch version.
    - UCM can create, update or remove any persistency data that is contained in the SoftwareCluster. Rollback of persisted data can be done to persistent data during update process.
    - SoftwareCluster can have execution dependencies towards other SoftwareClusters. Execution dependencies are checked in the end of the update process before activating/switching to the new software version.

Each Software Package will address a single SoftwareCluster. A SoftwareCluster can act in two roles:

    - **‘Sub’-SoftwareCluster** : A SoftwareCluster without diagnostic target address, containing processes, executables and further elements.
    - **‘Root’-SoftwareCluster**: A SoftwareCluster with a diagnostic target address that may reference several other **‘Sub’-SoftwareClusters**, which thus form a logical group.

A software package has to be modelled as a so-called SoftwareCluster which describes the content of a software package that has to be uploaded to the AUTOSAR Adaptive Platform. Software Package is used for the "physical", uploadable software package that is processed by UCM whereas the term SoftwareCluster is used for the modelling element.

In the model, the content of a SoftwareCluster will be determined by references to all required model elements. The SoftwareCluster and the related model elements will be put into a manifest that is part of the Software Package.

Software Package Management
############################

The UCM update sequence consists three different phases:

    - **Software Package transfer**: Phase in which, one or several Software Packages are transferred from the Diagnostic Application Client to the internal buffer of the UCM.
    - **Software Package processing**: Phase in which the UCM performs the operation (kInstall, kUpdate, kRemove) on the relevant SoftwareCluster (which will be inactivated [not executed] during the operation).
    - **Activation**: Phase in which the UCM checks the dependencies of the SoftwareClusters that have been involved in the operation, activates them and finally checks that all the SoftwareClusters can be executed properly prior to finishing the update.

Transferring Software Packages
--------------------------------
Transferring of Software package uses three phases - Initiation of a data transfer, the data transfer and completion of the data transfer.
    - **Data Transfer Initiation**:

    Initiation of Data transfer carry with the method TransferStart and takes total number of bytes of package to be transferred as input and returns TransferId on successful allocation of resource or returns application error on failure. Main purpose of TransferStart method is to prepare for the data transfer by allocating memory for storing software package.

    - **Data Transfer**:

    Actual data transfer is transferred using TransferData method. TransferData method is used for Block-wise transfer of a Software Package to UCM and takes TransferId, Data and BlockCounter. Main purpose of TransferData method is to transfer software package and perform few error checks eg. Authentication check.

    - **Data Transfer Completion**:

    TransferExit method is used for finishing the transfer of a Software Package to UCM. It extracts software package and performs software package validation checks. This method takes TransferId as input.

UCM provides DeleteTransfer method to delete transferred software package if software package is not required or transferring/processing of software package fails.

Lifecycle of Software Package is described in below figure:

.. figure:: resources/Transfer_Data.png
   :class: with-border
   :align: center

   **Transfer Data**

Processing Software Packages 
---------------------------------

Processing of Software Packages can involve any combination of an installation, update or removal of applications, configuration data, calibration data or manifests.

    - UCM provides a method ProcessSwPackage to process transferred Software Package and takes TransferId as input to identify which transferred software package to be processed.
    - UCM provides few supporting methods GetSwProcessProgress, Cancel and RevertProcessedSwPackage.
    - GetSwProcessProgress method is used to query the progress of executing the ProcessSwPackage method call for provided TransferId.
    - Cancel method is used to cancel the ongoing Software Package processing.
    - RevertProcessedSwPackage method is used to revert all changes done during processing of Software Package.
  
Activation, Rollback and Finish  
----------------------------------

    - The installed Software Cluster after completion of Software Package processing does not express whether a Software Cluster is currently executed or not. Hence UCM uses Activate method to activate the software cluster by performing dependency checks and system reset/application reset or directly activates software cluster depends on the software cluster configuration.
    - UCM provides a method Rollback to recover from an Activation that went wrong.
    - UCM provides a method Finish to commit all the changes and clean up all temporary data of the processed Software Packages.

Status Reporting
#################

Once Software Packages are transferred to UCM, they are ready to be processed to finally apply changes to the AUTOSAR Adaptive Platform.

In contrast to the transmission, the processing and activation tasks must happen in a strict sequential order. To achieve this UCM maintains different states and it is queried by Field called CurrentStatus.

The state machine for CurrentStatus is shown in below figure:   

.. figure:: resources/Package_Manager_Status.png
   :class: with-border
   :align: center

   **Package Manager Status**
   
Software Cluster lifecycle
###############################

Software cluster lifecycle is described in below figure:

.. figure:: resources/swcluster.png
   :class: with-border
   :align: center

   **State Machine for a SoftwareCluster**

- New Software Cluster starts its lifecycle with state kAdded. Software Cluster goes to kAdded state once Software Package is processed.
- Software Cluster goes to kPresent state on successful completion of install or update.
- For an update of existing Software Cluster, it goes to kUpdated on successful processing of a software package with new data.
- For a removal of existing Software Cluster, it goes to kRemoved on successful processing of a software package with new data.
- On reverting of processed software package Software Cluster goes back to kPresent state from kUpdated/kRemoved.

Version Reporting
###################

UCM is responsible for reporting software information that are present on platform, reporting changes are done to software and provides general information of software cluster.

    - UCM provides a method GetSwClusterInfo to provide the identifiers and versions of the SoftwareClusters that are in state kPresent.
    - UCM provides a method GetSwClusterChangeInfo to provide the identifiers and versions of the SoftwareCluster that are in state kAdded, kUpdated or kRemoved.
    - UCM provides a method GetSwClusterDescription to return the version, type approval, license and release notes of the SoftwareCluster that are in state kPresent.


History
#########
UCM provides a method GetHistory to retrieve all actions that have been performed by UCM when exiting kVerifying state from a specific time window input parameter.

UCM Master
###########

UCM Master objective is to provide a standard Adaptive Autosar solution to safely and securely update a complete vehicle Over The Air or by a Diagnostic Tester.

UCM Master provides services for updating the software and its configuration in a vehicle. UCM Master is coordinating an update campaign within the vehicle.

Following are the functionalities of UCM Master 

    - Interact with backend to identify Software Clusters that could be updated, installed or removed.
    - Interact with backend to authenticate Vehicle Package.
    - Interact with backend to confirm dependencies between Software Clusters within vehicle before starting campaign.
    - Interact with backend to inform Backend of needed Software Packages, receives them and dispatch them to targeted ECUs.
    - Interact with Vehicle State Manager to inform which safety conditions that must be applied according to Vehicle Package.
    - Interact with Vehicle State Manager to share the computed vehicle state to other Applications or Functional Clusters involved in the update campaign.
    - Interact with Vehicle Driver about update campaign to provide campaign state to trigger interaction with Human during update campaign.
    - Interact with Vehicle Driver to get vehicle modification approval or consent from Human when configured in Vehicle Package.
    - Provide information of installed software in vehicle.
    - Provide information of update campaigns history.
    - Recovery in case of failure.

All these actions are what is called a campaign which UCM Master is coordinating. The UCM of the machines in the same network of a UCM Master is the target of a campaign, are referred to as UCM subordinates.

The OTA Client establishes a communication between Backend and UCM Master so that they can exchange information of the installed Software Clusters in the vehicle and the Software Clusters available in the Backend. This communication could be triggered by OTA Client and UCM Master to request the updates in case of newly available Software Clusters (pull case) or by Backend to push, for instance, an important security update to a fleet of vehicles (push case). The computation to find new Software Clusters versions and resolution of dependencies between Software Clusters can be either done at UCM Master or Backend.

Status reporting
-------------------

UCM Master supports a mechanism to provide the state of an update campaign typically to OTA Client, Vehicle Driver Application and Vehicle State Manager.

Campaign State Machine is described in below figure:

.. figure:: resources/State_Machine.png
   :class: with-border
   :align: center

   **State Machine**  

Campaign State Machine for OTA Client (TransferState field) is described in below figure:

.. figure:: resources/TransferState_field.png
   :class: with-border
   :align: center

   **TransferState field**
   
Software Cluster
#################

Modeling of SoftwareCluster is described below:

.. figure:: resources/SoftwareCluster.png
   :class: with-border
   :align: center

   **Software Cluster** 

Software Package 
##################

The existence of the SoftwareCluster by itself is not sufficient for installation. Actually, the SoftwareCluster gets wrapped into a so-called SoftwarePackage that comes with an own manifest format that is at least partly standardized.

The difference between the semantics of a SoftwareCluster and the semantics of SoftwarePackage is that a SoftwareCluster focuses on the structure of the software itself while the SoftwarePackage is created to handle the logistics aspect of the software installation.

Modelling of Software Package is described in below figure:

.. figure:: resources/Modeling_of_Software_Package.png
   :class: with-border
   :align: center

   **Modeling_of_Software_Package** 

Conceptual relation of Software Package and Software Cluster is described in below figure:   

.. figure:: resources/SoftwarePackage&SoftwareCluster.png
   :class: with-border
   :align: center

   **Software Package & Software Cluster**

Vehicle Package
################

The ability to handle Software Packages is the prerequisite for an important further step: The execution of an update campaign that applies for the whole vehicle. The basis for the update campaign is the definition of meta-class VehiclePackage.

Modeling of VehiclePackage is described in below figure:

.. figure:: resources/Modeling_of_VehiclePackage.png
   :class: with-border
   :align: center

   **Modeling of Vehicle Package**
