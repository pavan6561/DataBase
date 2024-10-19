# State Manager {#adaptive_state_manager}
- State Management is responsible to determine the operation state, based on information received or gathered from other Adaptive Platform Applications or Adaptive Applications.
- State Manager functional cluster determine the overall operation of Adaptive platform.
- It would interact with Execution Manager to change functional group and machine state.
- Adaptive Platform Functional clusters e.g. Diagnostic Manager and, Update and Configuration Manager can request the State Manager to change the state, depending on arbitration State Manager would change the state.
- State Manager would arbitrate multiple request and signal to decide state change and State Manager implementation would be project spcific. 

## Glossary of Terms
|Term                        |   Definition                                                                                                                                                                          |
|----------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
|State Management            |  The element defining modes of operation for AUTOSAR Adaptive Platform. It allows flexible definition of functions which are active on the platform at any given time.                |
|Execution Management        |  Functional Cluster within the Adaptive Platform Foundation.                                                                                                                           |
|Communication Management    |  A Functional Cluster within the Adaptive Platform Foundation.                                                                                                                         |
|Network Management          |  A Functional Cluster within the Adaptive Platform Services. Part of Communication Management.                                                                                        |
|Adaptive Diagnostics        |  A Functional Cluster within the Adaptive Platform Services.                                                                                                                          |
|Update And Configuration Management|  A Functional Cluster within the Adaptive Platform Services.                                                                                                                           |
|Network Handle              |  Network Handles are provided by Network Management. A handle represents a set of (partial) networks.                                                                                 |
|Process                     |  A process is a loaded instance of an Executable to be executed on a Machine.                                                                                                         |
|Function Group              |  A Function Group is a set of coherent Processes, which need to be controlled consistently. Depending on the state of the Function Group, Processes are started or terminated.        |
|Component                   |  Element of a Process. Processes are comprised of one or more SW-entities that provide a particular function or group of  related functions called Component.                         |
|Function Group State        |  The element of State Management that characterizes the current status of a set of (functionally coherent) user-level Applications. The set of Function Groups and their Function Group States is machine specific and are configured in the Machine Manifest.  |
|Machine State               |  The state of Function Group "MachineState" with some predefined states (Startup/Shutdown/Restart).                                                                     |
|Operational State           |  The element of State Management that characterizes the current internal state of the State Management. The Operational State is machine specific and depends on multiple events from somewhere within the system.                                                              |
|Component State             |  The element of State Management that characterizes the current state of Components within an Adaptive Application.  The Component State is Adaptive Application specific and therefore it has to be described in the respective Execution  Manifest. Every Adaptive Application provides at least an "On" and an "Off" State.   |
|Execution Manifest          |  Manifest file to configure execution of an Adaptive Application.                                                                                                                     |
|Machine Manifest            |  Manifest file to configure a Machine.                                                                                                                                                |
## Dependencies of State Management       

- Operating System Interface: 
   - State Management has no direct interface to the Operating System, all OS dependencies are abstracted by the Execution Management and Persistency.

- Execution Manager Interface: 
   - State Management is dependent on Execution Management to control operation state.
   - Execution Manager will fork or terminate an application depending upon the state requested and state defined in the application manifest.

- Persistency: 
   - State Management is dependent on the Persistency functional cluster to access persistent storage.

- Adaptive Diagnostics: 
   - State Manager will evaluate the information received from the Diagnostic Manager about the ongoing Session and decide the operational states based on the evaluation.

- Update And Configuration Management: 
   - State Manager will interact with Update and Configuration Management to check the ongoing update session and decide the operation states.

- Network Management: 
   - Network Management provides multiple NetworkHandle fields which represents a set of (partial) networks. 
   - State Management evaluates this fields to set Function Groups to the corresponding Function Group State defined in Manifest and vice versa.
   - Additionally State Management shall prevent system from shutting down during an update session is ongoing.

## State Manager Architecture: 
![State Manager Architecture](Images/state-manager-architecture.png)
  - State Manager communicate with execution manager for state change through API provided by execution manager i.e GetState(), SetState() which uses POSIX FIFO.
  - It report its internal state i.e kRunning , kTerminating back to Execution Manager through exec API.
  - Platform Health Manager interact with State Manager through service interface, in case State Manager does not respond back Platform Health Manager will also have an interface with Execution Manager to change state for recovery action. 
  - It also has service interface defined for other application to interact with State Manager to  notify and request for functional group state change as well as for component state.
  
## State Management Responsibility:
  - State Management is the functional cluster responsible to communicate with the Execution Manager, determine the current operational state and request for transition to different operational state.
  - It ensures that the application are executed when it is actually needed and it also provide necessary resources granting them full control.
  - It is the central point where all events are received, further operational state transition will be decided after the evaluation is done based on event type, event priority and application identifier.
  - If an operational state change is triggered then State Manager will request the Execution Manager for a state change.
  - Operation state change can be triggered by multiple application which are as follows:
     • Platform Health Management to trigger error recovery, e.g. to activate fall back Functionality.
     • Adaptive Diagnostics, to switch the system into diagnostic states.
     • Update and Configuration Management to switch the system into states where software or configuration can be updated.
     • Network Management to coordinate required functionality and network state.
     • Authorized applications, e.g. a vehicle State Manager which might be located in a different machine or on a different ECU.
  
  - Autosar Application may trigger the State Manager events through ara-com interface where the State Manager will be subscribing.
  - It also provide interface to the Application to request information about the current state through ara-com.
  - State Manager has critical functionality, access is secured through the Identity Access Manager. 
  - It is monitored and supervised by Platform health Manager which also has direct access to Execution Manager to change operational state when State Manager stops responding.
  - Operational state is the internal state of State Manager upon which decision will be made for next state change.


## Machine State:
  - Machine state is specific type of function group state which refer to the global state of the machine.
  - State Manager will request the Execution Manager to change these state based on vehicle-wide event and  priority evaluation.
  - Whenever State Manager requests Execution Manager for a state change, Execution Manager will terminate the application belonging to the current state and fork new application.
  - Machine state define the set of process to be forked by execution manager.
  - Each Application would have an application manifest, which defines the machine state in which the application is allowed to execute.
  - Adaptive Autosar Platform will have 3 mandatory machine state i.e Startup, Restart and Shutdown along with project specific machine state.
  - Startup will be the default machine state in Execution Manager and State Manager will control the operational flow henceforth.
  - Startup Machine State:
    - Execution Manager will be controlled by State Manager hence it should not be able to change the machine state on its own.
    - Hence State Manager should run in each and every machine state or the Adaptive Platform will not be able to transit to any other machine state.
    - Startup will be the default machine state in Execution Manager and State Manager should also belong to Startup Machine state so that it can control the operational flow.
  - Shutdown State/Restart State:
    - State Management will be configured to run in Shutdown/Restart.
    - State transition is not a trivial system change and it can fail for a number of reasons. When ever this happens State Management has to be alive, to report an error and wait for further instructions.

## State Transition
    ![Machine State Transition](Images/state-change-sequence.png)
      - Execution Manager is initiated after boot up, it will fork all application belonging to startup phase.
      - State Manager has Startup state defined in its execution manifest.
      - After State Manager is initialized it will decide the state change henceforth and request Execution Manager for state change.
      - Executiion Manager on request from State Manager will initiate state change, first it will send SIGTERM signal to all application an indication for all application to terminate itself.
      - If the application doesnot terminate within the timeout specified in its execution mnifest execution terminate the application and confirm state change.
    ![State Transition to Driving State](Images/state-manager-machine.png)


## Functional Group State
  - Machine state mechanism is not flexible enough to control a group of application which are functionally coherent, in particular if they have to be started and terminated with interleaving life-cycle.
  - Hence additional  function group and functional group state can be configured.
  - During diagnostic and error recover it is necessary to handle such application using functional group state.
  - Machine state controls machine life-cycle and processes of platform level application while function group state control process functionally coherent user application.
  - State Manager will obtain available functional group state and machine state from machine manifest.
  - Processes refers in their execution manifest the state in which they want to execute, state can be functional group state and machine state.
  - Application is provided with communication interface with the State Manager to request functional group state change.
  - If there are multiple request from different application, State Manager would arbitrate based on priority and other factor before requesting Execution Manager for state change.
    ![Function Group State Transition](Images/RequestStateMultiple_Requests.jpg)


## State Management and Components state
  - Transition from one operational state to another usually require termination of application and forking new application.
  - When execution manager need to change the internal behaviour of process it needs to unload the process from memory and reload the executable, to avoid such overhead component state is used .
  - Component state is basically state inside a process where process can have one or multiple component i.e threads.
  ![Component State Startup](Images/ComponentState-StartUp.jpg)
  - Late-wakeup is an use-case of component state, where process can have ON or OFF state and threads corresponding to each state.
  - whenever shutting down is invoked process can execute OFF state threads, while shutting down new wake-up reason is found.
  - Instead of terminating the process and again loading the process, Component state i.e ON and OFF can be used.
  - OFF state would perform all the persistence data and ON state would resume the activity without terminating the process hence avoiding the overhead of unloading process from memory.

  
  ![Component state transition (late-wakeup)](Images/Late-wake-up-Exec.jpg)

## Component state Handling
  - State management will calculate component state from current operational state and other requirement and send state to registered process.
  - Process need to register with State Manager via API interface to receive component state ,when components do not require component state it can unregister from State Manager.
  - Registration done by calling the constructor of component-client and un-registration  is done by calling destructor.
  - Component are allowed to temporarily avoid component state change due ongoing critical execution. component has to send confirmation that it has received the state change request.
  - After certain timeout the State Manager will again request for component state change, this retry count and timeout values are configured in the execution manifest.
  - When maximum retries are over State Manager will force the process to transit into a component state.
  - Components must be able to perform a transition from any Component State into any Component State that they have defined.  
    ![Function group state transition](Images/RegisterForComponentStateSM.jpg)
    

## Interaction with Diagnostic Manager
  - Adaptive Diagnostics is responsible for diagnosis, configuration and resetting of Function Groups. During any diagnostic request is executed it is necessary to prevent system from shutting down.
  - State Management shall not shutdown the system during an active diagnostic session. Therefore State Management has to register to Adaptive Diagnostics to receive information about active diagnostic session.
  - Adaptive Diagnostic have several reset type i.e hardReset, softReset and keyOffOnReset to carry out functionalities of diagnostic.
  - State Manager would translate this reset and send equivalent component state to the process.
  - A hardReset could be interpreted as Hardware reset while softReset could be interpreted as loading the software configuration and keyOFFOnReset could mean restarting the functional group state.
  - State Manager also has functionality to persist the reset cause while resetting the Machine state.
  - State Manager will also read the last reset cause when spawned by execution manager and set the persist data to default value.
  
## Interaction with the Update and Configuration Management
  - State Manager has to register with the update and Configuration Management to receive information about the ongoing update and It will not shutdown the system while the update is going on.
  - Update and verify state should be available to enable the update and Configuration Management to fulfill its functionality.
  - Update and Configuration Management would request for the corresponding state from State Manager.
  - When a platform software is updated which needs a Machine reset State Manager should have means to reset the machine.
  - If an application software is updated which doesn't need system reset the State Manager should have meant to trigger execution manager to re-parse the manifest update the application list.

## Interaction with Network Management
  - State Management interacts with Network Management via a service interface.
  - Network Management provides multiple instances of NetworkHandles, where each represents a set of (partial) networks.
  - The NetworkHandles are defined in the Machine Manifest and are there assigned to a Function Group State.
  - Whenever network is activated or deactivated due outside request corresponding handle is set which is notified to State Manager which in turn would change the corresponding functional group state.
  - Similarly State Manager would change the network handle when functional group is changed and the association between functional group and network handle is available in Machine Manifest.
  - It might be needed that a Function Group stays longer in its Function Group State when the causing (partial) network set has been switched off 
  network is longer available than the causing Function Group has been switched to Function Group State ’Off’. This is called ’afterrun’. The corresponding timeoutvalue has to be configured in Machine Manifest.
  


