# Configuration Methodology {#adaptive_configurationmethodology}
The Adaptive Platform is usually not exclusively used within a single AUTOSAR System as the vehicle is also equipped with a number of ECUs developed on the AUTOSAR Classic Platform. The System design for the entire vehicle will therefore cover both ECUs built using Classic Platform(CP) as well as Machines using the Adaptive Platform(AP).

![Functional Architecture to Common Architecture](images/vfb++.png)

* The Function Architecture is composed of a number of function networks. A function network consists of a set of function blocks with their interfaces and corresponding interconnections.

* Function Architecture is one of the input to deduce Common Software Architecture. The Common Software Architecture provides a dedicated view of all software entities and their communication relation within the E/E vehicle system. In this light, the Common Software Architecture comprises both types, AUTOSAR software components of the Classic Platform as well as those entities that form later an Adaptive Application Software deployed to an Adaptive Platform-based machine.

* The communication entry and exit points of components are ports typed by a particular interface definition. In case of the Adaptive Platform, interfaces are expressed as Service Interfaces. The Common Software Architecture can be specified
by means of the System Description.

![Subsystem Architecture](images/system_extract.png)

**System:** Like for the CP methodology, this development domain will cover activities which refine the Common Software Architecture into a system defined by specific ECUs or machines. The overall system which talk about the refinement of the VFB by the definition of a topology of ECUs and networks and the deployment of software components onto ECUs, with the extensions necessary for the Common Software Architecture and the additions to specify machines and the corresponding mapping of machines to ECUs.

# 1. Development Workflow
![Adaptive Methodology Overview](images/AR_Method_DOC_AdaptiveMethodologyOverview.png)


|Task Definition                         | Responsibility |  Task Description |
|:-----------------                      | :-----------   |  :----------------
|Develop a Service Interface Description | OEM            | This activity describes the definition of the service interfaces, aggregating events, methods and fields, including the definition of data types |
|Develop Adaptive Software (Application/Platform) | Tier1/Tier2 | Develop an Adaptive Application with category application-level or platform level. |
|Develop the communication structure by means of MachineDesign | OEM | Define and configure network connections and configure the SD message |
|Create Diagnostic Mapping | OEM | A diagnostic mapping is used to establish relation between adaptive diagnostic management and endpoints in the application software so that SD can connect the corresponding end points correctly | 
|Design Communication between CP and AP  | OEM            | All activities which are necessary to design communication between two platforms |
|Integrate the Software Components       | Tier 1         | The compiled Software and one main function are integrated into one executable | 
|Select OS Distribution                  | Tier 2         | Select and Assemble an Operating System |
|Define and Configure Machine            | Tier 1/2       | This includes the configuration of all machine states, function groups and the available hw resources  and configure Adaptive Autosar platforms|                   
|Create Application Manifest             | Tier 1         | In this activity, processes, startup configuration and execution dependencies for the application will be defined |
|Define and Configure Service Instance   | OEM/Tier 1     | Define the service instances, configure their search or offer criteria and map service instance to prototype |
|Setup initial Machine | Tier 1/2 | Configure and Install OS and other necessary platform modules on the machine |
|Create Software Package | Tier 1 | Creation of Software Package |
|Provide and Manage Software Packages | OEM | Deploy and store software package on a back-end server |

## 1 Software Configuration
### 1.1 Configure Data Types
The specification of data types on the AUTOSAR adaptive platform follows the same pattern as the counterpart on the AUTOSAR classic platform: data types are defined on different levels of abstraction that complement each other.
<br/>
In Adaptive Autosar we focus on two data types.
- ImplementationDatatTypes
- ApplicationDataTypes

#### 1.1.1 ImplementationDataTypes
A subset of the modeling of ImplementationDataTypes that is supported on the AUTOSAR classic platform can directly be used on the AUTOSAR adaptive platform as well

![Example for ImplementationDataType](images/ImplementationDataType.png)

#### 1.1.2 ApplicationDataTypes
ApplicationDataType defines a data type from the application point of view. Especially it should be used whenever something "physical" is at stake.<br/>
An ApplicationDataType represents a set of values as seen in the application model,such as measurement units. It does not consider implementation details such as bit-size, endianess, etc.

![Example for ApplicationDataTypes](images/ApplicationDataType_Example.png)

### 1.2 Configure a Service Interface  
This represents the ability to define a PortInterface that consists of a heterogeneous collection of methods, events and fields.
Service interfaces can consist of events, methods and fields and are the basis for the generation of header files for a software component.
![Service Interface  & Workflow](images/ServiceinterfaceDescription.png)

### Example Service Interface
![Example Service Interface ](images/ServiceInterface.png)

### 1.3 Configure Service Interface Deployment
The different meta-class specializations of **ServiceInterfaceDeployment** define a binding of a ServiceInterface to a middleware transport layer.<br/>
In ServiceInterfaceDeployment Configure following Attributes <br/>
- **ServiceMethodDeployment** <br/>
   The ServiceMethodDeployment meta-class provides the ability to define middleware transport layer specific configuration settings relevant for a method that is defined in the
   context of a ServiceInterface. <br/>
- **ServiceEventDeployment** <br/>
   The ServiceEventDeployment meta-class provides the ability to define middleware transport layer specific configuration settings relevant for an event that is defined in
   the context of a ServiceInterface. <br/>
- **ServiceFieldDeployment** <br/>
   The Service-FieldDeployment meta-class provides the ability to define middleware transport layer specific configuration settings relevant for a field that is defined in the context
   of a ServiceInterface. <br/>
 
![Example Service Interface Deployment](images/SomeipserviceInterfaceDeployment.png)

### 1.4 Configure Provided SomeIp Service Instance
The ProvidedSomeipServiceInstance defines the serviceInstanceId for the Service Instance of the SomeipServiceInterfaceDeployment that is referenced with the serviceInterface reference.<br/>
It means that the Server on which the ProvidedSomeipServiceInstance is deployed offers the Service Instance over SOME/IP with the serviceInstanceId and serviceInterfaceId.

![Example Provided SomeIp Service Instance ](images/ProvidedSomeIpServiceInterface_Example.png)

### 1.5 Configure Required SomeIp Service Instance
The RequiredSomeipServiceInstance defines the required-ServiceInstanceId of a SomeipServiceInterfaceDeployment that the client searches.

![Example Required SomeIp Service Instance ](images/RequiredSomeipServiceInstance_Example.png)

### 1.6 Configure Application Manifest
The purpose of the application manifest is to provide information that is needed for the actual deployment of an application (formally modeled as an SwComponentType) onto the AUTOSAR adaptive platform.
- To instantiate the same application software several times on the same machine
- To deploy the application software to several machines and instantiate the application software per machine

#### 1.6.1 Configure AdaptiveApplicationSwComponent
This meta-class represents the ability to support the formal modeling of application. software on the AUTOSAR adaptive platform. Consequently, it shall only be used on the AUTOSAR adaptive platform.
<br/>
In sw Component Configure Following elements <br/>
- R Port Prototype <br/>
Provide Required SomeipServiceInterface Reference for Service
- P Port Prototype <br/>
Provide Provided SomeipServiceInterface Reference for Service

![Example AdaptiveApplicationSwComponent](images/AdaptiveApplicationSwComponent_Example.png)

#### 1.6.2 Configure Executable
This meta-class represents an executable program.<br/>
It should have reference to AdaptiveApplicationSwComponent which needs to be execute

![Example Executable](images/Executable_Example.png)

#### 1.6.3 Configure Processes
This meta-class provides information required to execute the referenced executable.

![Example Process](images/Process_Example.png)

### 1.7 Map Service Instance To Port Prototype
Configure Which service instance should use which port.
ServiceInstanceToPortPrototypeMapping is used to assign an AdaptivePlatformServiceInstance to a PortPrototype of a
SwComponentType. This allows to define how specific PortPrototypes of a Software Component are represented in the middleware in terms of the service configuration

![Example Service Instance To Port Prototype](images/ServiceInstanceToPortPrototype_Example.png)

## 2. Hardware Configuration

### 2.1 Configure EtherNet Cluster
It is the main element to describe the topological connection of communicating ECUs.
A cluster describes the ensemble of ECUs, which are linked by a communication medium of arbitrary topology (bus, star, ring, ...). The nodes within the cluster share the same communication protocol, which may be event-triggered, time-triggered or a
combination of both. <br/>
Configure the Following elements In Ethernet Cluster.<br/>
1)EtherNet Physical Channel<br/>
  i) Network Endpoints
     - Ipv4 Configuration
	 - Ipv6 Configuration 
	 
  ii) Communication Connector Ref Conditional <br/>
     Provide Reference To The Machine Design Communication Connectors.

![Example for Ethernet Cluster Configuration](images/EtherNetCluster_Example.png)

### 2.2 Configure Machine Manifest
The Machine meta-class defines the entity on which one Adaptive AUTOSAR Software Stack is running with an operating system. The Machine may be physical or virtual.
In AUTOSAR adaptive the element Machine is an entity which already represents a specific ECU Implementation with dedicated configurations for e.g. Processors,machineModeMachines, functionGroups. The Machine is a model entity which
is not in the focus of communication designers and should not be used during system design.<br/>
The Machine Manifest focuses on the following aspects:
- Configuration of the network connection and defining the basic credentials for the network technology (e.g. for Ethernet this involves setting of a static IP address or the definition of DHCP)
- Configuration of the service discovery technology (e.g. for SOME/IP this involves the definition of the IP port and IP multicast address to be used)
- Definition of the used machine states
- Definition of the used function groups
- Configuration of the adaptive platform functional cluster implementations (e.g. the operating system provides a list of OS users with specific rights)
- Configuration of the Crypto platform Module
- Configuration of Platform Health Management
- Configuration of Time Synchronization
- Documentation of available hardware resources (e.g. how much RAM is available; how many processor cores are available)

#### 2.2.1 Configure Machine Design 
The MachineDesign has been introduced in order to allow the communication system designer to define a placeholder for an adaptive ECU in the scope of the System (the MachineDesign corresponds to the EcuInstance of AUTOSAR classic)
-This meta-class represents the ability to define requirements on a Machine in the context of designing a system
![Example for MachineDesign](images/MachineDesign_Example.png)
#### 2.2.1.1 Configure network Connection
One of the most prominent information defined in the context of the MachineDesign is the network connectivity. Since the AUTOSAR adaptive platform focuses on the usage of Ethernet for communication, this boils down to the specification of IP addresses.
- Configure Communication connector for Ethernet cluster and provide Network endpoint for using ip address.
- Configure Service discovery with Some ip service discovery port 

#### 2.2.2 Configure Machine
Machine that represents an Adaptive Autosar Software Stack. Provide reference to MachineDesign.

![Example for Machine](images/Machine_Example.png)

## 3 Map Hardware and Software Configuration
### 3.1 Service Instance to Machine Mapping
This  allows to map ServiceInstances to a CommunicationConnector of a Machine.


![Example for ServiceInstanceToMachinemapping](images/SomeipServiceInstanceToMachineMapping_Example.png)

## 4 Configure Execution Manifest
The purpose of the execution manifest is to provide information that is needed for the actual deployment of an application (formally modeled as an SwComponentType) onto
the AUTOSAR adaptive platform.
![Class Diagram For Execution Manifest](images/Execution_Manifest.png)


### 4.1 Configure Startup
The configuration of startup behavior is an essential part of the execution manifest.


### 4.2 Configure Mode-dependent Startup Configuration
The purpose of meta-class ModeDependentStartupConfig is to qualify the startup configuration represented by meta-class StartupConfig for specific
ModeDeclarations.
Each ModeDependentStartupConfig of a Process shall reference at least one ModeDeclaration in the role
functionGroupMode or in the role machineMode


