**Persistency**
===================

The Architecture of Persistency 
#################################

The functional cluster Persistency offers two different mechanisms to access persistent memory:

    - Key-value storage
    - File storage

.. figure:: resources/persistency.png
   :class: with-border
   :align: center

   **Persistency**

The Persistency usage of an Adaptive Application is modeled in the Execution Manifest as part of the AdaptiveApplicationSwComponentTypes of an Executable.

The global functions of ara::per is to receive the identifier (the fully qualified shortName path) of a PortPrototype typed by a PersistencyInterface as an ara::core::InstanceSpecifier input parameter

The manifest contains separate deployment data for each Process that references the Executable. The Process is bound to the deployment data by specialization of the class PersistencyPortPrototypeToDeploymentMapping, which refers to a PortPrototype typed by a PersistencyInterface, a PersistencyDeployment, and the Process.

**Read Only** if the PortPrototype is instantiated as RPortPrototype, or

**Read/Write** if the PortPrototype is instantiated as PRPortPrototype, or

**Write Only** if the PortPrototype is instantiated as PPortPrototype.

Key-Value Storages
###################

Every Key-Value Storage is represented by a PortPrototype typed by a PersistencyKeyValueStorageInterface in the application design for the respective AdaptiveApplicationSwComponentType,and by a PersistencyKeyValueStorage containing deployment information.

Key-Value Pairs can be added and removed at run-time by the Adaptive Application using the Persistency.

A Key-Value Storage with predefined Key-Value Pairs can be deployed with default data during installation or update of an Adaptive Application. This operation is triggered by the UCM module and data is provided by softwarepackage.

The link between application design and deployment information of a Key-Value Storage is represented by PersistencyPortPrototypeToKeyValueStorageMapping, which refers to a PortPrototype typed by a PersistencyKeyValueStorageInterface, the corresponding PersistencyKeyValueStorage, and a Process.

File Storages
##############

Every File Storage is represented by a PortPrototype typed by a PersistencyFileStorageInterface in the application design for the respective AdaptiveApplicationSwComponentType,and by a PersistencyFileStorage containing deployment information.

Every File Storage can hold multiple files

File storage also can be deleted and created during runtime.

The link between application design and deployment information of a File Storageis represented by PersistencyPortPrototypeToFileStorageMapping, which refers to a PortPrototype typed by a PersistencyFileStorageInterface, the corresponding PersistencyFileStorage, and a Process.

Initialization and Shutdown of Persistency
##############################################

During Initialization of Application the Persistency shall read in the manifest information and prepare the access structures to all Key-Value Storages and File Storages that are defined in the manifest.

During Deinitialization od Application the persistency shall ensure that all open files of persistent data shall be persisted and synced with application and closed.

Parallel Access to Persistent Data
####################################

The persistent data is local to one Process. Therefore, Persistency will never share persistent data between two (or more) Processes, even of the same Executable.

If persistent data needs to be accessed by multiple Processes (of the same or different applications), it is the duty of the application designer to provide Service Interfaces for communication.

Security Concepts
###################

The Persistency supports encryption and authentication of data stored in a Key-Value Storage or File Storage.

The signed hash used for the authentication of a storage is likewise verified when opening a storage, and calculated during installation or when saving a Key-Value Storage or File Storage.

In case of a read-only Key-Value Storage or File Storage, encryption is done only once during installation. A signed hash used for authentication of a read-only Key-Value Storage or File Storage (or a key or file therein) is either provided as PersistencyDeploymentToCryptoKeySlotMapping.verificationHash or PersistencyDeploymentElementToCryptoKeySlotMapping. verificationHash in the manifest, or calculated during installation.

Redundancy Concepts
######################

The Persistency functional cluster shall take care of the integrity of the stored data. This can be achieved by calculating CRCs or hash values of the stored data, and by creating redundant copies.

The Persistency cluster shall use the redundant information to detect data corruption in the persistent memory.

The Persistency cluster shall use the redundant information to recover corrupted data if possible. If data is corrupted that cannot be restored using the redundant information, Persistency will fail with kValidationFailed.

**Redundancy Types:**

    - CRC
    - HASH
    - MoutN

Installation and Update of Persistent Data
##############################################

persistent data needs to be handled alongside the application. The Adaptive Application may trigger this handling explicitly by calling ara::per::UpdatePersistency during the verification phase that follows the installation or update, or rely on the Persistency cluster to do this implicitly when persistent data is accessed (ara::per::OpenKeyValueStorage/ara::per::-OpenFileStorage). In both cases, the Persistency cluster will compare the stored manifest version against the current manifest version, and perform the required action.

Persistency compareds the executable version and software version during update and installation of persistent data.

After an update of the Adaptive Application or the manifest, the Persistency cluster will create a backup of the persistent data, and then update the existing persistent data using one of the following strategies:

Existing persistent data is kept unchanged (keepExisting).
Existing persistent data is replaced (overwrite).
Existing persistent data is removed (delete). New persistent data is added (keepExisting and overwrite).

Resource Management Concepts
##############################

The Persistency cluster supports configuration of both an upper and a lower limit for the resources used by a Key-Value Storage or a File Storage.

The lower limit may already be defined by the application developer using PersistencyInterface.minimumSustainedSize.

During deployment, the integrator may update the lower limit using PersistencyDeployment.minimumSustainedSize and add an upper limit using Persistency-Deployment.maximumAllowedSize.

Configuration Methodology
############################

Persistency Configuration:
---------------------------

Deployment of Persistent Key-Value Storage:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. figure:: resources/Deployment_of_Persistent_Key-Value_Storage.png
   :class: with-border
   :align: center

   **Deployment of Persistent Key-Value Storage**

- The modeling of PersistencyKeyValuePair aggregated in the role PersistencyKeyValueStorage, keyValuePair is optional.

- PersistencyPortPrototype-ToKeyValueStorageMapping has the ability to map a specific PortPrototype referenced in the role portPrototype to a PersistencyKeyValueStorage referenced in the role keyValueStorage.

- The reference PersistencyPortPrototypeToKeyValueStorageMapping portPrototype shall only be used for a PortPrototype typed by a PersistencyKeyValueStorageInterface.

- If a PersistencyKeyValueStorage contains PersistencyKeyValuePairs that do not correspond to any dataElements of the PersistencyKeyValueStorageInterface
  that is mapped (indirectly) via PersistencyPortPrototypeToKeyValueStorageMapping then those keyValuePairs are created within the PersistencyKeyValueStorage.

- Initial value for PersistencyKeyValuePair dIt ispossible to define an initial value for a given PersistencyKeyValuePair by means of the aggregation of ValueSpecification in the role initValue.

- PersistencyKeyValuePair.valueDataType shall match to AbstractImplementationDataType for the corresponding PersistencyDataElement.

- Each PersistencyKeyValueStorage shall only be referenced by at most one PersistencyPort-PrototypeToKeyValueStorageMapping.

Deployment of File Storage:
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. figure:: resources/Deployment_of_Persistent_File_Storage.png
   :class: with-border
   :align: center

   **Deployment of Persistent File Storage**

- Port-Prototype typed by a PersistencyFileStorageInterface actually builds an abstraction for an entire directory of files.

- Here PersistencyPortPrototypeToFileStorageMapping creates a mapping between a PortPrototype referenced in the role portPrototype to a PersistencyFileStorage referenced in the role fileStorage under consideration of a Process referenced in the role process.

- The usage of PersistencyFileStorage.file allows for the explicit modeling of elements of the PersistencyFileStorage.

- The creation of this aggregation is optional. It can be used to define the update strategy and/or initial content of selected files.

- The reference Persistency-PortPrototypeToFileStorageMapping portPrototype shall only be used for a PortPrototype typed by a PersistencyFileStorageInterface.

- Within the scope of any given PersistencyFileStorage, the value of all file.fileName shall be unique.

- Each PersistencyFileStorage shall only be referenced by at most one PersistencyPortPrototypeToFileStorageMapping.



