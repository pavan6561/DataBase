# Introduction to Adaptive Platform {#adaptive_platform_introduction}
To understand, why we need an Adaptive Platform, let's start to understand current **In-Vehicle Network Infrastructure**. Current generation ECUs mainly implement functionality that replaces or augments electromechanical systems. Software in those deeply-embedded ECUs, controls electrical output signals based on input signals and information from other ECUs connected to vehicle network. Much of the control software is designed and implemented for the target vehicle and does not change fundamentally during vehicle life-time. 
![Current In-Vehicle Network Infrastructure](images/In-VehicleNetworking.png)
  * Deeply Embedded with real time, safe and secure system
  * Static Network Configuration
  * Static deployment of Application SWCs 
  * Low Computing Power
  * Not scalable, Limited Bandwidth
  * No standardized solution available for fail operational systems and updates over the air

Future vehicle functions/applications such as Autonomous Car, Car-2-X, software updates over the air, or vehicles as part of the IoT, will introduce highly complex and computing resource demanding software into the vehicles and must fulfill strict integrity and security requirements. Such software realizes functions, such as environment perception, behavior planning, integrates the vehicle into external backend and infrastructure systems. The software in the vehicle needs to be changed during the life cycle of the vehicle, due to evolving external systems or improved functionality. However, the generic automotive MCUs of today are not focused on high-performance and are therefore not capable of handling such tasks. A trend towards having a larger, central and more computational heavy core can be seen, making high-performance hardware an interesting option. This compels AUTOSAR to adopt the idea of utilizing either VMs or more powerful hardware. The AUTOSAR Classic Platform (CP) standard addresses the needs of deeply-embedded ECUs, while the needs of ECUs described above cannot be fulfilled well. Therefore, AUTOSAR specifies a second software platform, the AUTOSAR Adaptive Platform (AP). AP provides mainly high-performance computing and communication mechanisms and offers flexible software configuration, e.g. to support software update over-the-air.
![Future Application/Functions](images/FutureVehicle.png)
 
**Comparison between AUTOSAR Classic Platform and Adaptive Platform**
 
Classic Platform                          | Adaptive Platform
----------------------------------------- | ------------------
Based on OSEK OS (Real time Embedded System) and Generally Developed in “C” | Based on POSIX (PSE51 – Minimal Real time Embedded System) and Developed in C++
Signal based communication | Service based communication
Static Network, Task and Application SWC Configuration and Scheduling Strategy. Whole Stack compiled and linked to generate the full target image for the hardware | Thread and not runnable based communication, Flexible Configuration, Dynamic Deployment of new functionalities without the need to generate the full target image for the hardware
Same Address space for all the applications (MPU Support) | Each Application run in it’s own protected address space (MMU Support)
Execution of code directly from ROM | Application is loaded from persistence memory to RAM
Real time (High in the range of micro second), Safety critical (up to ASIL D), Low computing power | Not so real time (in the range of milli second), High Computing Power, Fail Safe Operational System, Highly secured


![Quick Comparison](images/ClassicVsAdaptive.png)


While the communication protocols of the classical platform are based on signal based paradigm that is statically pre-configured before run-time (operation), while the adaptive platform is based service-oriented communication allowing dynamic initiation of communication paths. Similarly, the dynamic scheduling of applications would allow dynamic deployment of application during runtime. Another important characteristic is the Memory Management Unit, through which each process (task) executed by the operating system have its own virtual address and doesn’t recognize the existence of others. This assists the achievement of freedom from interference between applications, even after their deployment during runtime. All this is realized through a POSIX complaint operating system, to allow the compatibility between other applications.

**Types of Adaptive Platform:** 
Machine term has been newly introduced with the AUTOSAR Adaptive Platform. A machine is resembled to virtualized ECU, an entity where software can be deployed to. In this context, one real ECU could run several machines. In the simplest case the term machine is nothing but an ECU.
![Machine View](images/machine_view.png)

> **Existence of Classic Platform:**
> AUTOSAR has no intention of replacing the Classical Platform with the Adaptive Platform, but the aim is to have both platforms coexisting and functioning together on the same network without risking the stability of the existing classical architecture that has been proven over the years. The introduction of the Adaptive Platform intends to complement the automotive specific functionalities and sustain the domain’s fundamental attributes such as reliability, availability, maintainability and safety. 

**Integration of Classic, Adaptive and Non-AUTOSAR ECUs:**
AUTOSAR Adaptive Architecture uses the SOME/IP to integrate with AUTOSAR CP and other Non-AUTOSAR Platforms like GENIVI
![Integration of CP, AP and GENIVI](images/Integration_of_AP.png)

# 1. Technology Drivers
There are two major groups of technology drivers behind. One is Ethernet, and the other is processors.
  * _Ethernet_
      * High bandwidth
      * Communication system is not a limiting aspect any more
      * Switched network
      * Efficient point-to-point communication
      * Efficient transfer of long messages
  * _Processor_
      * Switch from microcontroller to processors with external memory (and maybe filesystems) 
      * Many core processors
      * Parallel computing
      * "Cheap" availability of computing power
      * Special purpose processors
        
# 2. Adaptive Platform Characteristics
  * <i>Language C++</i>
  * _SOA_
  * _Parallel processing_
  * _Leveraging existing standard_
  * _Safety and security_
  * _Planned dynamics_ 
  * _Agile_
  

  ## 2.1 Service Oriented Architecture (SOA)
  
    A service-oriented architecture (SOA) is an approach used to create a software architecture based on the use of services that is exchanged between application software components. It enables the idea of service reuse, no need to start from scratch when upgrades and other modifications are needed; which creates an efficient and flexible way to interconnect systems to perform a specific job increasing the system scalability and re-usability, and simultaneously eases the coexistence of different applications.   
    ![Service Oriented Architecture](images/serviceorientedarchitecture.png)
  
    The exchange of services is based on a Service-Oriented Communication (SOC) paradigm, where “an application is interpreted as a set of data providing (sensors), data processing (application logic), and data consuming (actuators) services. As shown in the above figure, the communication paths follow a producer/consumer or client/server model, where some applications offer their services as producers, while other applications subscribe for services as consumers. This allows the introduction of new services independent of vendors, products, technologies, and without introducing changes in the underlying program, which further support the establishment of communication paths at run-time.

    Only the needed data is subscribed for and reaches the client; unlike the classical way where all the data is broadcasted regardless of the receiver. Moreover, SOME/IP can be implemented on different operating system and even embedded devices without an operating system; serving the intention of providing compatibility, and allowing stronger interactions with off-board systems and non-AUTOSAR applications.
    
    A service consists of a combination of
    * Events
    * Methods
    * Fields

  ## 2.2 Parallel Processing
    The distributed computing is inherently parallel. The SOA, as different applications uses different set of services, shares this characteristic. The advancement or many core processors and heterogeneous computing that offer parallel processing capability offers technological opportunities to harness the compute power to match the inherent parallelism. Thus, the AP possesses the architectural capability to scale its functionality and performance as the manycore-heterogeneous computing technologies advance.
  
  ## 2.3 Leveraging existing standard
    * Reuse existing (non-automotive) standards
    * Ease software development
    * Support automotive use-cases and protocols
    ![Reuse existing standard](images/RefArchitecture.png)

  
  ## 2.4 Safety and Security
    The systems that AP targets often require highest level of safety and security. To cope with the challenge, AP combines architectural, functional, and procedural approaches. The architecture is based on distributed computing based on SOA, which inherently makes each component more independent and free of unintended interferences, dedicated functionalities to assist achieving safety and security.

  ## 2.5  Planned Dynamics
    The AP supports incremental deployment of applications, where resources and communications are managed dynamically to reduce the effort for software development and integration, enabling short iteration cycles. Incremental deployment also supports explorative software development phases. AP allows the system integrator to carefully limit dynamic behavior to reduce the risk of unwanted or adverse effects allowing safety qualification. Dynamic behavior of an application will be limited by constraints stated in the Application Manifest.
    ![Planned Dynamics](images/planneddynamics.png)
    Configuration of communication paths can happen at design-, at startup- or at run-time and is therefore considered either static or dynamic. 
    * **Full static configuration:** service discovery is not needed at all as the server knows all clients and clients know the  server.
    * **No discovery by application code:** the clients know the server but the server does not know the clients. Event subscription is the only dynamic communication pattern in the application.
    * **Full service discovery in the application:** No communication paths are known at configuration time. An API for Service discovery allows the application code to choose the service instance at runtime.
    
    Examples of Planned Dynamics are 
      * Pre-determination of service discovery process
      * Restriction of dynamic memory allocation to startup phase only
      * Fair scheduling policy in addition to priority-based scheduling
      * Fixed allocation of processes to CPU cores
      * Access to pre-existing files in the file-system only
      * Constraints for AP API usage by Applications
      * Execution of authenticated code only

  ## 2.6 Agile
  To support Agile Development process, it is important that underlying architecture e.g. Adaptive Platform is scalable with the possibility of updating the system after its deployment.

# 3 Adaptive Platform Architecture
The AUTOSAR Adaptive architecture organizes the software of the AUTOSAR Adaptive foundation as functional clusters. These clusters offer common functionality as services to the applications. The Adaptive Application (AA) run on top of ARA, AUTOSAR Runtime for Adaptive applications. ARA consists of application interfaces provided by Functional Clusters, which belong to either Adaptive Platform Foundation or Adaptive Platform Services. Adaptive Platform Foundation provides fundamental functionalities of AP, and Adaptive Platform Services provide platform standard services of AP. The interface of Functional Clusters, either they are those of Adaptive Platform Foundation or Adaptive Platform Services, are indifferent from AA point of view. 
![Adaptive Platform Architecture](images/apd_architecture.png)
## 3.1 Adaptive Platform Foundation
  Adaptive Platform Foundation consists of following functional clusters:
  * **Communication Management** 
  * **ARA Core Types** 
  * **Execution Management** 
  * **RESTful Communication**
  * **Time Synchronization**
  * **Persistency**
  * **LogAndTrace** 
  * **Platform Health Management**
  * **Identify Access Management**
  * **Crypto** 
  * **Operating System with POSIX Interface (PSE 51)**

### 3.1.1 Communication Management
The Communication Management (CM) for AUTOSAR Adaptive is a functional cluster and is part of "AUTOSAR Runtime for Adaptive Applications" - ARA. It is responsible for the construction and supervision of communication paths between applications, both local and remote. The CM provides the infrastructure that enables communication between Adaptive AUTOSAR Applications within one machine and with software entities on other machines, e.g. other Adaptive AUTOSAR applications or Classic AUTOSAR SWCs. All communication paths can be established at design- , start-up- or run-time. 
![Communication Management Architecture](images/cm_architecture.png)

The Communication management of AUTOSAR Adaptive can be logically divided into the following sub-parts:
* Language binding
* End-to-end communication protection
* Communication / Network binding
* Communication Management software

The Communication Management provides standardized means how a defined service is presented to the application implementer (upper layer, Language Binding) as well as the respective representation of the service’s data on the network (lower layer, Network Binding). This assures portability of source code and compatibility of compiled services across different implementations of the platform.

Service-Oriented Communication (SoC) is the main communication pattern for Adaptive AUTOSAR Applications. It allows establishing communication paths both at design-time and run-time. Hence, it can be used to build up both static communication with known numbers of participants and dynamic communication with unknown number of participants.  

The service class is the central element of the Service-Oriented Communication pattern applied in Adaptive AUTOSAR. It represents the service by collecting the methods and events which are provided or requested by the applications implementing the concrete service functionality.

Regarding interaction between AAs, PSE51 do not include IPC (Inter-Process-Communication), so there is no direct interface to interact between AAs. The Communication Management (CM) is the only explicit interface. CM also provides Service Oriented Communication for both intra-machine and inter-machine, which are transparent to applications. CM handles routing of Service requests/replies regardless of the topological deployment of Service and client applications.

> **Note: The Communication Management software using Service-Oriented Communication will not achieve hard real time requirements, as the implementation will behave like a virtual ethernet including latencies of communication. This behavior must be respected with the design of the overall ECU and SW system.**

### 3.1.2 Execution Management
Execution Management is responsible for all aspects of system execution management including platform initialization and startup / shutdown of Applications. Execution Management works in conjunction with the Operating System to perform run-time scheduling of Applications. 
![Execution Management Architecture](images/emo_architecture.png)

When the Machine is started, the OS will be initialized first and then Execution Management is launched as one of the OS’s initial processes. Other functional clusters and platform-level Applications of the Adaptive Platform Foundation are then launched by Execution Management. After the Adaptive Platform Foundation is up and running, Execution Management continues launching Adaptive Applications. The startup order of the platform-level Applications and the Adaptive Applications are determined by the Execution Management, based on Machine Manifest and Application Manifest information.
![Startup Sequence](images/emo_startup_seq.png)

Execution Management is responsible for all aspects of Adaptive Platform execution management and Application execution management including:

* Machine State Management
* Start-up and shutdown of Applications, including platform-level Applications and Adaptive Applications. 
* The Execution Management is the initial (“boot”) process of the operating system and is responsible for Machine start-up.
* The Execution Management enforces process isolation with each Executable managed as a single process.
* Privileges and use of access control

> **Note: The Execution Management is not responsible for run-time scheduling of Applications since this is the responsibility of the Operating System. However, the Execution Management is responsible for initialization / configuration of the OS to enable it to perform the necessary run-time scheduling based on information extracted by the Execution Management from the Machine Manifest and Application Manifests.**

### 3.1.3 Persistency
Persistency offers mechanisms to applications and other functional clusters of the Adaptive Platform to store information in the non-volatile memory of an Adaptive Machine. The data is available over boot and ignition cycles. Persistency offers standard interfaces to access the non-volatile memory.
The Persistency APIs take storage location identifiers as parameters from the application to address different storage locations.
The available storage locations fall into two categories. Every application may use a combination of these storage types.
* **Key-Value Storage**
* **File-Proxy Storage**

> **Note: Persistent data is always private to one application. There is no mechanism available to share data between different applications using the Persistency**

### 3.1.4 Operating System with POSIX Interface (PSE 51)
The Operating System is responsible for run-time resource management (including time) for all Applications on the Adaptive Platform. It offers the foundation for dynamic behavior of the software applications. It manages the scheduling of
processes and events, the data exchange and synchronization between different processes and provides features for monitoring and error handling. The AP Operating System is required to provide multi-process POSIX OS capability. Each AA is implemented as an independent process, with its own logical memory space and name space. 

GNU/Linux is an operating system that uses a combination of GNU software and Linux as its kernel, hence named "GNU/Linux". GNU/Linux is used to refer to the whole operating system and the term Linux would be used to refer to the kernel itself. Linux -the kernel- is the most important part of the operating system, as it manages access to the hardware resources required by the programs of the operating system run as User Applications. It operates in what is called “the kernel space”; interacting with these applications through its system call interface

On the other hand, GNU/Linux consists of many programs other than the kernel, which may include for example the graphical user interfaces, compilers, specific libraries and many other services. However, without the kernel the operating system can’t provide these services
![Linux OS](images/gnu_linux.png)

> **Note: Adaptive Platform does not specify a new Operating System for highly performant processors. Rather, it defines an execution context and Operating System Interface (OSI) for use by Adaptive Applications.**

**POSIX (PSE51)**:Minimal Real-Time System (PSE51) offers functions for 
1. basic synchronized I/O, 
2. high-resolution timer, 
3. signals,
4. semaphores, 
5. shared memory and threads. 

![POSIX PSE51 Profile](images/posix_pse51.png)

Scheduling: The operating system provides multi-threading and multi-process support. The standard scheduling policies are SCHED_FIFO and SCHED_RR, which are defined by the POSIX standard.

> **Note: As the envisioned application software components for Adaptive Platform will not require to fork new processes themselves, and only need limited direct access to files, the PSE51 profile is thought to be sufficient.**

### 3.1.5 Platform Health Management
Health Monitoring is required by [5, ISO 26262] under the terms control flow monitoring, external monitoring facility, watchdog, logical monitoring, temporal monitoring, program sequence monitoring.

The Platform Health Management supervises the execution of software. It offers the following supervision functionalities:
* **Alive supervision** : Alive Supervision checks that a Supervised Entity is not running too frequently and not too rarely
* **Deadline supervision** : Deadline supervision checks that step in a Supervised Entity are executed in a time that is within the configured minimum and maximum limits.
* **Logical supervision** : Logical supervision checks that the control flow during execution matches the designed control flow.
* **Health Channel Supervision** : Health channel supervision provides the possibility to hook external supervision results (like RAM test, voltage monitoring, …) to the Platform Health Management

## 3.2 Adaptive Platform Services
* **Update and Configuration Management**
* **Signal to Service Mapping**
* **State Management**
* **Diagnostic**
* **Network Management**

### 3.2.1 Update and Configuration Management
One of the declared goals of Adaptive AUTOSAR is the ability to flexibly update the software and its configuration through over-the-air updates. To support changes in the software on an Adaptive Platform the Update and Configuration Manager(UCM) provides an Adaptive Platform service that handles software update requests.
![Overview of Software Package](images/ucm_architecture.png)

UCM is responsible for updating, installing, removing and keeping a record of the software on an Adaptive Platform. Its role is similar to known package management systems like dpkg or YUM in Linux, with additional functionality to ensure a safe and secure way to update or modify software on the Adaptive Platform.

### 3.2.2 State Management
State Management is the Functional Cluster which is responsible for defining the current set of Machine State and Function Group States, and for initiating State transitions by requesting them from Execution Management. Execution Management performs the State transitions and controls the actual set of running Processes, depending on the current States.

State Management is the central point where new Machine States and the Function Group States can be requested and where the requests are arbitrated, including coordination of contradicting requests from different sources. Additional data and events might need to be considered for arbitration.

The State change requests can be issued by:
* Platform Health Management to trigger error recovery, e.g. to activate fallback functionality
* Diagnostics, to switch the system into diagnostic states
* Update and Config Management to switch the system into states where software or configuration can be updated
* Network Management to coordinate required functionality and network state
* authorized applications, e.g. a vehicle state manager which might be located in a different machine or on a different ECU

State Change requests can be issued by other Functional Clusters via ara::com service interfaces.
Since State Management functionality is critical, access from other Functional Clusters or Applications must be secured, e.g. by IAM (Identity and Access Management). State Management is monitored and supervised by Platform Health Management.
State Management provides interfaces to request information about current states.
![Interaction Between States](images/interaction_between_states.png)

Four different states are relevant for Execution Management:
* **Machine State:** Machine States are mainly used to control machine life cycle (startup/shutdown/restart), platform level processes and other infrastructure. There are several mandatory Machine states that must be present on each machine. Additional machine specific Machine States can be defined in the Machine Manifest.
* **Function Group State:** Function Group States are mainly used to individually start and stop groups of functionally coherent user level Application processes. They can be configured in the Machine Manifest.
* **Process State:** Process States are used for Application Life cycle Management and are implemented by an Execution Management internal state machine.
* **Application State:** The Application State characterizes the internal life cycle of any instance of an Application Executable, i.e. process. Each process must report Application State changes to Execution Management.

# 4 Configuration Methodology
In contrast to the AUTOSAR Classic Platform, instances of Adaptive Applications, for example, are executed within the context of processes, entities managed by the operating system. If permitted by the configuration of the operating system, processes may be started, executed or stopped, at any time during the life cycle of a machine. As a consequence, the way of configuration (by the means of Manifests) or when and how software packages are deployed (e.g., by software updates over-the-air) clearly differ from the concepts of the AUTOSAR Classic Platform.

A Manifest represents a piece of AUTOSAR model description that is created to support the configuration of an AUTOSAR AP product and which is uploaded to the AUTOSAR AP product, potentially in combination with other artifacts (like binary files) that contain executable code to which the Manifest applies.
![Development workflow](images/configuration_methodology.png)

Development Work flow is mainly divided into four parts:

* **Application Design:** This kind of description specifies all design-related aspects e.g. data types, service interfaces, persistency interfaces etc.. that apply to the creation of application software for the AUTOSAR AP. It is not necessarily required to be deployed to the adaptive platform machine, but the application design aids the definition of the deployment of application software in the Application Manifest and Service Instance Manifest.

* **Application Manifest:** This kind of Manifest is used to specify the deployment-related information of applications running on the AUTOSAR AP e.g how the application instance shall be started with startup options and access roles. An Application Manifest is bundled with the actual executable code to support the integration of the executable code onto the machine.

* **Service Instance Manifest:** This kind of Manifest is used to specify how service-oriented communication is configured in terms of the requirements of the underlying transport protocols. A Service Instance Manifest is bundled with the actual executable code that implements the respective usage of service-oriented communication.

* **Machine Manifest:** This kind of Manifest is supposed to describe deployment-related content that applies to the configuration of just the underlying machine (i.e. without any applications running on the machine) e.g. Configuration of the service discovery technology, Definition of the used machine states, Definition of the used function groups that runs an AUTOSAR AP. A Machine Manifest is bundled with the software taken to establish an instance of the AUTOSAR AP.
