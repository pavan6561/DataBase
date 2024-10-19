## Persistency {#peristency_overview}
## 1. Overview
<p><b>Persistency</b> offers mechanisms to applications and other functional clusters of the Adaptive Platform to store information in the non-volatile memory of an Adaptive Machine.</p>
<p>Responsibilities of the Persistency: 
- Persistently stored data is available over boot and ignition cycles
- Loads data which is persistently stored
- Accesses data using an unique identifier
- Reads and writes data from file-like structures
- Encrypts/Decrypts the persistent data
- Error detection and correction of stored data
</p>

### 1.1 Available Storage locations in persistency
 - <b> Key-Value Storage </b> provides a mechanism to store and retrieve multiple Key-Value pairs in one storage location. The keys need to be unique for each Key-Value database and are defined by an application using the methods provided by the Persistency. 
 - <b> File-Proxy Storage </b> A File-Proxy Port allows an application to access a storage location and create one or multiple accessors in it. These accessors again are identified by unique keys in string format.
 
<TABLE>
<TR>
<TD bgcolor = #0xC8BFE7><b>Key-Value Storage </b></TD>
<TD bgcolor = #0xC8BFE7><b>File-Proxy Storage </b></TD>
</TR>
<TR>
<TD>Used to store and retrieve multiple Key-Value pairs in one storage location and key should be unique. Basically holds:
1) Data types defined in SWS_AdaptivePlatformTypes.
2) Simple byte arrays that result from a streaming of complex types in the application. 
3) All Implementation Data Types.</TD>
<TD>Used to support large chunk of data and allows application to access a storage location and create one or multiple accessors in it. These accessors again are identified by unique keys in string format. Example: It is filesystem directory in which an application is allowed to create multiple files (accessors).</TD>
</TR>
</TABLE>

### 1.2 Architectural concepts
<p>Persistent data is always private to one application. This design decision was taken to prevent second communication path beneath the functionality provided by Communication Management. If persistent data needs to be shared between multiple applications, it is the duty of an application developer to provide Service Interfaces for communication.</p>

### 1.3 Security concepts
- The Persistency cluster shall encrypt data before storing it to the persistent memory according to Crypto need.
- The Persistency cluster shall decrypt data data after reading it from the persistent memory according to Crypto need.

### 1.4 Redundancy concepts
The implementer of the Persistency functional cluster shall take care of the error detection and error correction mechanisms which are implementation specific.

<TABLE>
<TR>
<TD bgcolor = #0xC8BFE7><b>Enumeration</b></TD>
<TD bgcolor = #0xC8BFE7><b>Value</b></TD>
<TD bgcolor = #0xC8BFE7><b>PersistencyRedundancyEnum</b></TD>
</TR>
<TR>
<TD>none</TD><TD>1</TD><TD>This value represents the requirement that a piece of data to be stored persistently
shall not end up in a redundant persistent storage facility.</TD>
</TR>
<TR>
<TD>redundant</TD><TD>0</TD><TD>This value represents the requirement that a piece of data to be stored persistently
shall end up in a redundant persistent storage facility.</TD></TR></TABLE>


## 2. Persistency - Persistent data over UCM 

**Update and Configuration Management handles 3 main use cases for handling of Adaptive Application**
- Installation of new software
- Update of already installed software
- Uninstallation of installed software

<p>Persistency needs for UCM:
 - Deployment of persistent data that was defined by an application designer
 - Deployment of persistent data that was defined by an application designer and changed by an integrator
 - Deployment of persistent data that was defined by an integrator
 - Definition of update strategies for persistent data when a new version of an application is installed 
 - Removing persistent data when an application is uninstalled 
</p>


### 2.1 Installation of Key-Value-Databases
- Persistency shall create an entry containing the Persistency Data Element in the Persistency Key-Value Database Interface which is addressed by Port Prototype.
- The created entry in the Key-Value-Database shall have the shortName of the PersistencyDataElement as key. 
- The created entry in the Key-Value-Database shall be of the datatype defined in PersistencyDataElement.
- The value of the created entry in the KVS db shall be taken from the <b>PersistencyDataRequiredComSpec.initValue</b> , if no Persistency Key-Value Pair with the same short Name as the Persistency Data Element exists in the Persistency KV db for the corresponding Adaptive application Mode(w.r.t process defined).
- The value of the created entry in the KV db shall be taken from the <b>PersistencyKeyValuePair.initValue</b>, if a Persistency Key-Value Pair with the same short Name as the Persistency Data Element exists in the database for the corresponding Adaptive application Mode(w.r.t process defined).
- If a Persistency Data Element exists that is neither referenced by a Persistency Data Required ComSpec with a <b>PersistencyDataRequiredComSpec.initValue</b> nor a Persistency Key-Value Pair with the same short Name as the Persistency Data Element in the PersistencyKeyValueDatabase for the corresponding Adaptive application Mode(w.r.t process defined), no entry in the KV db shall be created.
- Incompatible AUTOSAR datatypes are rejected.
- During installation if the key-value pair with same Short name does not exist then persistency will create a new entry.
- Short name of Persistency Key-Value-Pair will be the key.
- The created entry in the KV db shall be of the datatype defined in <b>PersistencyKeyValuePair.valueDataType</b>.
- The created entry in the KV db shall have the <b>PersistencyKeyValuePair.initValue</b> as value.
- If <b>UpdateStatergy</b> is defined as delete then no entry in the KV db shall be created.

### 2.2 Update of Key-Value-Databases
- Same rules of Installation KVS is applicable but updateStrategy parameter should be respected.
- If updateStrategy is 'overwrite', then entry to the KV db shall be created or overwritten with the new Key.
- If updateStrategy is 'keepExisting', no changes to the existing key value pair shall be done in the db if key already exists, else a new entry shall be created.
- If updateStrategy is delete, then entry to the KV db shall be deleted if key already exists, and no entry for the KV db shall be created.
- If the final updateStrategy  is 'keepExisting', all Key- Value-Pairs in the Key-Value-Database that are not explicitly modeled as PersistencyDataElement or PersistencyKeyValuePair shall be kept.
- If the final updateStrategy is 'delete', all Key-Value-Pairs in the Key-Value-Database that are not explicitly modeled as PersistencyDataElement or PersistencyKeyValuePair 
shall be deleted.

#### 2.2.1 Key-Value-Databases - Update
<TABLE>
<TR>
<TD bgcolor = #0xC8BFE7><b>UpdateStategy Value
</b></TD>
<TD bgcolor = #0xC8BFE7><b>Description</b></TD>
</TR>
<TR>
<TD>OVERWRITE</TD><TD>Persistency will create a new entry or overwrite an existing Key-Value-Pair with the same key.
</TD>
</TR>
<TR>
<TD>KEEP-EXISTING</TD><TD>Persistency will keep existing Key-Value-Pair with the same key shall be kept in the Key-Value-Database, if doesn’t exist then new entry is created.</TD>
</TR>
<TR>
<TD>DELETE</TD><TD>Persistency will not add new entry into the DB, if already exists with same key then delete.</TD>
</TR>
</TABLE>


### 2.3 Uninstallation of Key-Value-Databases
- When deleting an Adaptive Application, Persistency shall delete all Key-Value-Databases used by this Adaptive Application from the Adaptive Machine. 


### 2.4 Installation of File-Proxies
- While installing a new Adaptive Application, for every PersistencyFileProxy in a PersistencyFileProxyInterface addressed by PortPrototype, Persistency shall create an entry in the File-Proxy.
- The created entry in the File-Proxy shall have the <b>PersistencyFileProxy.fileName</b> as key.
- The content of the created entry in the File-Proxy shall be taken from a file in the Software Package addressed by <b>PersistencyFileProxy.contentUri</b> if no PersistencyFile with the same shortName as the Persistency-FileProxy exists in the PersistencyFileArray that is mapped to the aggregating PortPrototype typed by the PersistencyFileProxyInterface with a PersistencyPortPrototypeToFileArrayMapping in the context of the Process of this Adaptive Application.
- The content of the created entry in the File-Proxy shall be taken from a file in the Software Package addressed by <b>PersistencyFile.contentUri</b>, if a PersistencyFile with the same shortName as the PersistencyFileProxy exists in the PersistencyFileArray that is mapped to the aggregating PortPrototype typed by the PersistencyFileProxyInterface with a Persistency-PortPrototypeToFileArrayMapping in the context of the Process of this Adaptive Application.
- When installing a new Adaptive Application, for every PersistencyFile in a PersistencyFileArray that is mapped to a PersistencyFileProxyInterface using a PersistencyPortPrototypeToFileArrayMapping, Persistency shall create an entry in this File-Proxy if no PersistencyFileProxy in the PortPrototype typed by the PersistencyFileProxyInterface exists with the same shortName as the PersistencyFile.
- The created entry in the File-Proxy shall have the <b>PersistencyFile.fileName</b> as key.
- The content of the created entry in the File-Proxy shall be taken from a file in the Software Package addressed by the <b>PersistencyFile.contentUri</b>.
- Persistency shall reject any configuration in which a PersistencyFileProxy and a PersistencyFile with the same fileName but different
shortNames exist that are mapped by a PersistencyPortPrototypeToFileArrayMapping referring to the PortPrototype typed by the PersistencyFileProxyInterface and the PersistencyFileArray.
- If the final updateStrategy of an entry to be created is delete, no entry in the File-Proxy shall be created.

### 2.5 Update of File-Proxies
- Same rules of File-Proxies is applicable as installation, but updateStrategy parameter should be respected.
- If updateStrategy is 'overwrite', the entry to the File-Proxy shall be created and shall overwrite an existing entry with the same key.
- If updateStrategy is 'keepExisting', an existing entry with the same key shall be kept in the File-Proxy. If no entry with the same key exists, the entry to the File-Proxy shall be created.
- If updateStrategy is 'delete', an existing entry with the same key shall be deleted and no entry to the File-Proxy shall be created.
- If the final updateStrategy  is 'keepExisting', all entries in the File-Proxy that are not explicitly modeled as PersistencyFileProxy or PersistencyFile shall be kept.
- If the final updateStrategy  is 'delete', all entries in the File-Proxy that are not explicitly modeled as PersistencyFileProxy or PersistencyFile shall be deleted.

#### 2.5.1 File-Proxies - Update
<TABLE>
<TR>
<TD bgcolor = #0xC8BFE7><b>UpdateStategy Value
</b></TD>
<TD bgcolor = #0xC8BFE7><b>Description</b></TD>
</TR>
<TR>
<TD>OVERWRITE</TD><TD>Persistency will either create a new entry or overwrite an existing File-Proxy with the same key.
</TD>
</TR>
<TR>
<TD>KEEP-EXISTING</TD><TD>Persistency will keep existing File-Proxy with the same key shall be kept in the File-Proxy, if doesn’t exist then new entry is created.</TD>
</TR>
<TR>
<TD>DELETE</TD><TD>Persistency will not add new entry into the DB, if already exists with same key then delete.</TD>
</TR>
</TABLE>

### 2.6 Uninstallation of File-Proxies
- When deleting an Adaptive Application, Persistency shall delete all File-Proxies used by this Adaptive Application from the Adaptive Machine.