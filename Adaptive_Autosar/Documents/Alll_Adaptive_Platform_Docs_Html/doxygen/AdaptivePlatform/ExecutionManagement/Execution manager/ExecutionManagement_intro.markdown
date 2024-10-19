# Execution Management {#execution_management}
Execution Management is the functional cluster within the Adaptive Platform Foundation that is responsible for platform initialization and the startup and shutdown of Applications. It performs these tasks using information contained within one or more Manifest files such as when and how Executables should be started. The Execution Management functional cluster is part of the Adaptive Platform. 

Execution Management, in common with other Applications is assumed to be a process executed on a POSIX compliant operating system. Execution Management is responsible for initiating execution of the processes in all the Functional Clusters, Adaptive AUTOSAR Services, and Adaptive Applications. The launching order is derived by Execution Management to ensure proper startup of the Adaptive Platform. 

For an example, The Adaptive AUTOSAR Services are provided via mechanisms provided by the Communication Management functional cluster of the Adaptive Platform Foundation. In order to use the Adaptive AUTOSAR Services, the functional clusters in the Foundation must be properly initialized beforehand.

Execution Management is responsible for all aspects of process lifecycle management, including:

* **Machine startup and shutdown**
    – Execution Management is the initial (“boot”) process of the operating system.
* **Required process hierarchy of started services, e.g., init and its child process.**
    – The boot process in this case corresponds to machine init process.
* **Provision of process isolation with each instance of an Executable managed as a single process.**
* **Startup and shutdown of Applications.**
    – Loading Executable based on a defined Execution Dependency.
    – Specific requirements until starting an Executable main function (i.e. entry point)
    - Ensure that startup and shutdown dependencies between Executable instances are respected, if an execution dependency is 
      specified in the Application Manifest of an Executable instance. If no execution dependency is specified between Executable instances, they can be started and stopped in an arbitrary order.
* **Privileges and use of access control**
    – Description and semantics of access control in manifest files
    - Allocate OS resource budgets for Executable and/or groups of Executables
    - Authentication and authorization of Executables at startup of Executable
* **State management**
    – Conditions for the execution of Applications
		 
	
		
# 2. **Technical aspect of execution manager**
## 2.1	**Application**
	*	Applications are developed to resolve a set of coherent functional requirements.
	*	An Application consists of executable software units, additional execution related items (e.g. data or parameter files), and descriptive information used for integration
		and execution (e.g. a formal model description based on the AUTOSAR meta model,	test cases).
	*	Applications can be located on user level above the middleware or can be implementation of functional clusters of the Adaptive Platform.
	*	Applications might use all mechanisms and APIs provided by the operating system	and other functional clusters of the Adaptive Platform, which in general restricts
		portability to other Adaptive Platforms.
	*	All Applications, including Adaptive Applications, are treated the same by Execution Management.

## 2.2	**Adaptive Application**
	*	An Adaptive Application is a specific type of Application that fully complies with the AUTOSAR specification,
		i.e. it is restricted to use APIs standardized by AUTOSAR and needs to follow specific	coding guidelines to allow reallocation between different Adaptive Platforms.
	*	Adaptive Applications are always located above the middleware. To allow portability	and reuse, user level Applications should be Adaptive Applications	whenever technically possible.			
	*	An Adaptive Application is the result of functional development and is the unit of delivery for Machine specific configuration and integration. 
	*	Some contracts and Service Interfaces to interact with other Adaptive Applications need to be agreed on beforehand.
	
## 2.3	**Executable**
	*	An Executable is a software unit which is part of an Application. It has exactly one entry point (main function). An Application can be implemented in one or more Executables.	
	*	Executables which belong to the same Adaptive Application might need to be deployed to different Machines, e.g. to one high performance Machine and one high safety Machine.		
			
|			Process Step 				|                     Software                                         																						| Meta Information																											|		
| ------------------------------------- | --------------------------------------------------------------------------------- 																		| ----------------------------------------------------------------------													|
|		Development and Integration	    | Linked, configured and calibrated binary for deployment onto the target Machine. The binary might contain code which was generated at integration time.  	|  Application Manifest and Service Instance Manifest (not used by Execution Management)				|
|		Deployment and Removal			| Binary installed on the target Machine.         																											|  Processed Manifests, stored in a platform-specific format which is efficiently readable at Machine startup.				|
|		Execution						| Process started as instance of the binary.																												|  The Execution Management uses contents of the Processed Manifests to start up and configure each process individually.	|		
		
	
![Executable lifecycle](images/Executable_lifecycle.png)	

## 2.4	**Process**
	*	A Process is a forked instance of an Executable.
>	**Note: In the initial release it is assumed, that processes are self contained, i.e. that they take care of controlling thread creation and scheduling by calling APIs from within the code.** 
	*	Execution Management only starts and terminates	the processes and while the processes are running, Execution Management only interacts with the processes by using State Management mechanisms.


## 2.5	**Application Manifest**
	*	The Application Manifest consists of parts of the Application design information which is provided by the application developer in an application description, and
		additional machine-specific information which is added at integration time. 
	*	An Application Manifest is created together with a Service Instance Manifest (not used by Execution Management) at integration time and deployed onto a
		Machine together with the Executable it is attached to. 
	*	It describes in a standardized	way the machine-specific configuration of Process properties (startup parameters, resource group assignment, priorities etc.).
	*	Each instance of an Executable binary, i.e. each started process, is individually configurable, with the option to use a different configuration set per Machine state or per Function Group State.

## 2.6	**Machine Manifest**
	*	The Machine Manifest holds all configuration information which cannot be assigned to a specific Executable, i.e. which is not already covered by an Application Manifest or a Service Instance Manifest.
	*	The Machine Manifest is also created at integration time for a specific Machine	and is deployed like Application Manifests whenever its contents change. 
	*	The contents of a Machine Manifest includes the configuration of Machine properties	and features (resources, safety, security, etc.), e.g. configured Machine States and Function Group States, resource groups, access right groups, scheduler	configuration, SOME/IP configuration, memory segmentation.

## 2.7	**Manifest format**
	*	The Application Manifests and the Machine Manifest can be transformed into a platform-specific format (called Processed Manifest), which is efficiently readable at Machine startup. 
	*	The format transformation can be done either off board at integration time or at deployment time, or on the Machine (by Update and Configuration management) at installation time.

## 2.8	**Execution Management Responsibilities**
*	Execution Management is responsible for all aspects of Process execution management. A Process is a loaded instance of an Executable, which is part of an Application.
*	Execution Management is started as part of the Adaptive Platform startup phase and is responsible for starting and terminating Processes.
*	Execution Management determines when, and possibly in which order, to start or stop Processes, i.e. instances of the deployed Executables, based on information in the Machine Manifest and Application Manifests.
*	Execution Management will be solely responsible for initiating execution of Processes.
*	Depending on the Machine State or on a Function Group State, deployed Executables are started during Adaptive Platform startup or later.
*	Execution Management is not responsible for run-time scheduling of Processes since this is the responsibility of the Operating System. 

* 	**Application Lifecycle Management:**
	*	The Execution Management is responsible	for the ordered startup and shutdown of the deployed Applications.
	*	The Execution Management determines when, and possibly in which order, to start or stop the deployed Applications, based on information in the Machine
		Manifest and Application Manifests.
	*	Depending on the Machine State or on a Function Group State, deployed	Applications are started during Adaptive Platform startup or later,
		however, it is not expected that all will begin active work immediately since many Applications will provide services to other Applications and therefore wait
		and “listen” for incoming service requests.
	*	The Execution Management derives an ordering for startup/shutdown within the State Management framework, based on declared Application dependencies.
		The dependencies are described in the Application Manifests.
*	The Execution Management is not responsible for run-time scheduling of Applications since this is the responsibility of the Operating System. However the Execution Management is responsible for initialization configuration of the OS to enable it to perform the necessary run-time scheduling based on information extracted by the Execution Management from the Machine Manifest and Application Manifests.
		
### 2.8.1	**Process Lifecycle Management**
*	**Process States:**
	From the execution stand point, Process States characterize the lifecycle of any Process. 
>	**Note that each process is independent and therefore has its own Process State.**
	*	**Idle Process State:** The Idle Process State will be the Process	state prior to creation of the process and resource allocation.
	*	**Starting Process State:** The Starting Process State will apply when the process has been created and resources have been allocated.
	*	**Running Process State:** The Running Process State will apply to a process after it has been scheduled and it has reported Running State to the Execution Manager.
	*	**Terminating Process State:** 
		*	The Terminating Process State will apply either after an process has received the termination indication
			from the Execution Manager or after it has decided to self-terminate and informed the Execution Manager.
		*	The Terminating and Running Process State indications from Application to Execution Management use the ReportApplicationState API. On entering the kTerminatingProcess State, the Process is expected to save persistent data, free all used Process internal resources and exit.
	*	**Terminated Process State:** 
		*	The Terminated Process State will apply after the process has been terminated and the process resources
			have been freed. For that, Execution Manager will observe the exit status of all processes, with the POSIX waitpid() command. 
		*	From the resource allocation stand point, Terminated state is similar to the Idle state as there is	no process running and no resources are allocated anymore.
		*	From the execution stand point, Terminated state is different from the Idle state since it tells that the process has already been executed and terminated. 
		*	This is relevant for one shot Processes which are supposed to run and terminate on their own as once they have reached their Terminated Process State they are to return to the Idle Process State without any external trigger.
	*	**One-shot Process State:**
		*	After a one-shot Process has terminated, Execution Management shall immediately set its Process State to Idle.
		![Process lifecycle](images/Process_lifecycle.png)
*	**Startup and Shutdown:**
		*	*Ordering:* 
			*	The Execution Manager can derive an ordering for the startup and shutdown of Processes within the State Management framework based on the declared Execution Dependencies. 
			*	An Execution Dependency defines the provider of service(s) required by a Process before that Process can provide its own services. Hence Execution Management ensures the dependent Processes are in the state defined by the Execution Dependency before the Process with the dependency is started.
			*	Execution Dependencies are described in the Application Manifest.
			*	During startup, Execution Management shall respect Execution Dependencies by ensuring that any Processes upon which the Process to be started depends have reached the requested state before starting the Process.
			*	The same Execution Dependencies used to define the startup order are also used to define the shutdown order. However the situation is reversed as Execution Management must ensure that dependent processes are shutdown after the process to ensure that the services required remain available until no longer required.
			*	During shutdown, Execution Management will respect Execution Dependencies by ensuring that any Processes upon which the Process to be shutdown depends are not terminated before shutting down the Process.
		*	*Arguments :*
			*	Execution Manager provides argument passing for a Process contain is gone or more ModeDependentStartupConfig in the role Process.modeDepen-dentStartupConfig. This permits different Processes to be started with different arguments.
			*	At the initiation of startup of a Process, the aggregated StartupOptions of the StartupConfig referenced by the ModeDependentStartupConfig will be passed to the call of the exec-family based POSIX interface to start the Process by the Operating System.
			*	The first argument on the command-line passed by Execution Management is the name of the Executable.

*	**Startup Sequence:**
	*	When the Machine is started, the OS will be initialized first and then Execution Manager is launched as one of the OS’s initial Processes. 
		Other functional clusters and platform-level Applications of the Adaptive Platform Foundation are then launched by Execution Management. 
		After the Adaptive Platform	Foundation is up and running, Execution Management continues to launch userlevel Applications.
	*	The startup order of the platform-level Processes is determined by the Execution Management, based on Machine Manifest
		and Application Manifest information.
	![Startup sequence](images/startup.png)
	
*	**Execution Dependency:**
	*	The Execution Management provides support to the Adaptive Platform for ordered startup and shutdown of Applications. 
	*	This ensures that Applications are started before dependent Applications use the services that they provide and, likewise, that Process are shutdown only when their provided services are no longer required.
	*	The Execution dependencies, are configured in the Application Manifests, which is created at integration time based on information provided by the Application developer.
	*	User-level applications use service discovery mechanisms of the Communication Management and should not depend on startup dependencies. Which Executable instances are running depends on the current Machine State and on the current Function Group States. The integrator must ensure that all service dependencies	are mapped to State Management configuration, i.e. that all dependent Executable instances are running when needed.
	*	In real life, specifying a simple dependency to an Process might not be sufficient	to ensure that the depending service is actually provided. Since some Process will reach a certain Application State to be able to offer their services to other Process,	the dependency information will also refer to Process State of the	Process specified as dependency. With that in mind, the dependency information	may be represented as a pair like: "<Process>.<ApplicationState>".
	*	The following dependency use-cases have been identified:
		*	In case Process B has a simple dependency on Process A, the Running Application State of Process A is specified in the dependency section of Process B’s Application Manifest.
		*	In case Process B depends on One-Shot Process A, the Terminated	Application State of Process A is specified in the dependency section of Process B’s Application Manifest.
	*	Processes are only started by the Execution Manager if they reference a requested Machine State or Function Group State, but not because of configured Execution Dependencies. 
	*	Execution Dependencies are only used to control a startup or terminate sequence at state transitions or at machine startup/shutdown.
		
## 2.9	**State Management**
*	State Management provides a mechanism to define the state of the operation for an Adaptive Platform. The Application Manifest allows definition in which states the Process instances have to run . State Management grants full control over the set of Applications to be executed and ensures	that Process are only executed (and hence resources allocated) when actually needed.
*	Four different states are relevant for Execution Management:
	*	Application State
	*	Process State
		*	Process States are managed by an Execution Management internal state machine.
	*	Machine State
	*	Functional group state.

### 2.9.1	**Application State:**
![Application states](images/application_state.png)
*	The Application State characterizes the internal lifecycle of any instance of an Process. The states are defined by the ApplicationState enumeration.
*	**Application State Running:** 
	*	Execution Management shall consider Process initialization complete when the state kRunning is reported.
>	**Please note that Service Discovery can introduce non-deterministic delays and thus is advised to be done after reporting kRunning state thus the Process may not have completed all its initialization when the kRunningstate is reported.**
*	**Initiation of Process termination:** 
	*	Execution Management shall initiate termination by sending the SIGTERM signal to a Process. 
	*	Note  that  from  the  perspective  of Execution Management,  requirement only requests the initiation of the steps necessary for termination.  
	*	On receipt of SIGTERM, a Process acknowledges the request (by reporting the new state to Execution Management using the Application-Client:ReportApplicationStateinterface) and then commences the actual termination.
*	**Acknowledgement of termination request:** 
	*	On reception of SIGTERM, the Process will acknowledge the state change request by reporting kTerminating to Execution Management. 
*	**Initiation of Process self-termination:** 
	*	A Process will initiate self-termination by reporting the kTerminating state to Execution Management.
	*	During the Terminating state, the Process is expected to free internally used resources. The Process indicates completion of the Terminating state by simply exiting (with an appropriate exit code). Execution Management as the parent process can detect termination of the child process and take the appropriate platform-specific actions.


### 2.9.2	**Machine State**
*	Requesting and reaching a Machine State is, besides using Function Group States, one way to define the current set of running Process. It is significantly influenced by vehicle-wide events and modes.
*	Each Application can declare in its Application Manifest in which Machine States it has to be running.
*	There are several mandatory machine states specified in this document that have to be present on each machine. 
*	Additional Machine States can be defined on a machine specific basis and are therefore not standardized.
*	Machine States: 
	*	A ModeDeclaration for each required Machine State has to be defined in the Machine Manifest. 
	*	The Execution Manager will obtain the Machine States from the Machine Manifest. 
	*	The API	specification will use the shortName for identification of the Machine State. 
	*	The Machine States are determined and requested by the State Manager. 
* 	Following are the default Machine state:
*	**Startup**
![Startup Sequence](images/startip_sequence.png)
	*	The Startup Machine State will be the first state to be active after the startup of Execution Manager. Therefore, a ModeDeclaration for the Startup has to be defined in the Machine Manifest.
	*	The following behavior apply for the Startup Machine State:
		*	All Processes of platform-level Applications configured for Startup will be started. 
		*	Process configured for Startup are based on the reference from the Process to the ModeDependentStartupConfig in the role Process.modeDependentStartupConfig with the instanceRef to the ModeDeclaration in the role ModeDependentStartupConfig.machineMode	 that belongs to the Startup Machine State.
		*	The Execution Manager will wait for all started Process until their Application State Running is reported.
		*	If that is the case, the Execution Manager will notify the State Manager that the Startup Machine State is ready to be changed.
		*	The Execution Manager will not change the Machine State by itself until a new state is requested by the State Manager.
*	**Shutdown**
	*	The Shutdown Machine State will be active after the Shutdown Machine State is requested by the State Manager. Therefore, a ModeDeclaration for the Shutdown has to be defined in the Machine Manifest. 
	*	The following behavior apply for the Shutdown Machine State:
		*	All Processes, including the platform-level Applications, that have a Process State different than Idle or Terminated will be shutdown.
		*	When Process State of all Process is Idle or Terminated, all Process configured for Shutdown will be started. 
		*	Process configured for Shutdown are based on the reference from the Processes to the ModeDependentStartupConfig in the role Process.modeDe-pendentStartupConfig with the instanceRef to the ModeDeclaration in the role ModeDependentStartupConfig.machineMode that belongs to the Shutdown Machine State.
	*	Shutdown of the Operating System:
		*	There will be at least one Process consisting of at least one Process that has a ModeDependentStartupConfig in the role Process.modeDependentStartupConfig with the instanceRef to the ModeDeclaration in the role ModeDependentStartupConfig.machineMode that belongs to the Shutdown Machine State. 
		*	This Application will contain the actual mechanism(s) to initiate shutdown of the Operating System.
*	**Restart**
	*	The Restart Machine State will be active after the Restart Machine State is requested by the State Manager. Therefore, a ModeDeclaration for the Restart has to be defined in the Machine Manifest.
	*	The following behavior applies for the Restart Machine State:
		*	All Process, including the platform-level Applications, that have a Process State different than Idle or Terminated will be shutdown.
		*	When Process State of all Process is Idle or Terminated, all Process configured for Restart will be started. 
		*	Process configured for Restart are based on the reference from the Process to the ModeDependentStartupConfig in the role Process.modeDependentStartupConfig with the instanceRef to the ModeDeclaration in the role ModeDependentStartupConfig.machineMode that belongs to the Restart Machine State.
	*	Restart of the Operating System: 
		*	There will be at least one Application consisting of at least one Process that has a ModeDependentStartupConfig in the role Process.modeDependentStartupConfig with the instanceRef to the ModeDeclaration in the role ModeDependentStartupConfig.machineMode that belongs to the Restart Machine State. 
		*	This Application will contain the actual mechanism(s) to initiate restart of the Operating System.
				
## 2.10 **State Management Architecture**
*	State Management is the functional cluster which is responsible for determining the current set of active Machine State and Function Group States, and for initiating State transitions by requesting them from Execution Management. 
*	Execution Management performs the State transitions and controls the actual set of running Processes, depending on the current States.
*	State Management is the central point where new Machine States and Function Group States can be requested and where the requests are arbitrated, including coordination of contradicting requests from different sources. Additional data and events might need to be considered for arbitration.
*	The State change requests can be issued by:
	*	Platform Health Management to trigger error recovery, e.g. to activate fallback functionality
	*	Diagnostics, to switch the system into diagnostic states.
	*	Update and Configuration Management to switch the system into states where software or configuration can be updated.
	*	Network Management to coordinate required functionality and network state.
	*	authorized applications, e.g. a vehicle state manager which might be located in a different machine or on a different ECU State Change requests can be issued by other.
*	Since State Management functionality is critical, access from other Functional Clusters or Process must be secured, e.g. by IAM (Identity and Access Management).
*	State Management is monitored and supervised by Platform Health Management.
*	State Management provides interfaces to request information about current states.
		
### 2.10.1 **State Interaction**
	![State Interaction](images/interaction_between_states.png)
*	One can see the state transitions of a Function Group and the Process and Application States of one Process which references one state of this Function Group, ignoring possible delays and dependencies if several Processes were involved. 
*	The interaction is identical if the Process references a Machine State instead of a Function Group State.
	
### 2.10.2 **State Change**
	![State Change](images/state_transition.png)
*	State Management can request to change one or several Function Group States and/or the Machine State from Execution Management by passing pairs of "Function Group" "requested State" as parameters, with Machine State being treated like any Function Group State.	
*	A state change request by State Management will lead to immediate state transitions and hereof a state change to the requested Machine State and/or Function Group States.
*	State Management can request multiple Machine State and Function Group State changes sequentially by issuing several individual state change requests, or atomically within the same state change request, which leads to multiple coherent state changes. 
*	However, the following restriction applies to avoid undefined behaviour while the state transitions are performed by Execution Management:	

*	**Deny State Change Request:**
	*	Execution Management will deny state change requests, that are received before all previously requested Machine State and/or Function Group State transitions are completed. If a request is denied, Execution Management will return an error code to the requester of the state transition.
*	**State Transition Timeout:** 
	*	If a timeout is detected when stopping or starting Processes at a state transition, Execution Management will return an error code to the requester of the state changes.
	*	This implies that the state change request blocks until the state transitions are completed or until an error is detected.
*	**State Change Failed:**
	*	Execution Management will return an error code to the requester of the state changes when other or unspecified errors occur at a state transition.
*	**State Change Successful:**
		*	When Execution Management succeeds with the requested state transitions, a success code will be returned to the requester of the state changes.
		
## 2.11 **Deterministic Execution**

### 2.11.1 **Determinism**
*	In real-time systems, deterministic execution often means, that a calculation of a given set of input data always produces a consistent output within a bounded time, i.e. the behavior is reproducible.
*	In the context of Execution Management, the term “calculation” can apply to execution of a thread, a Process, or a group of Processes. The calculation can be event-driven or cyclic; i.e. time-driven.
*	There are multiple elements in determinism and here we distinguish them as follows:
	*	**Time Determinism:** The output of the calculation is always produced before a given deadline (a point in time).
	*	**Data Determinism:** Given the same input and internal state, the calculation always produces the same output.
	*	**Full Determinism:** Combination of Time and Data Determinism as defined above.
*	Deterministic behavior is important for safety-critical systems, which may not be allowed to deviate from the specified behavior at all. 
*	Whether Time Determinism, or in addition Data Determinism is necessary to provide the required functionality depends on the system and on the safety goals.
*	Expected use cases of the Adaptive Platform where such determinism is required include:
	*	Software Lockstep: To execute ASIL C/D 	 with high computing performance demands, specific measures, such as software lockstep are required, due to high transient hardware error rates of high performance microprocessors. Software lockstep is a technique where the calculation is done redundantly through two different execution paths and the results are compared. To make the redundant calculations comparable, software lockstep requires a fully deterministic calculation.
	*	Reuse of verified software: The deterministic subsystem shows the same behavior on different platforms which satisfy the performance and resource needs of the subsystem, regardless of other differences in each environment, such as existence of unrelated applications. Examples include the different development and simulation platforms. Due to reproducible functional behavior, many results of testing, configuration and calibration of the subsystem are valid in each environment where the subsystem is deployed on and don’t need to be repeated.

### 2.11.1.1 **Time Determinism**
*	Each time a calculation is started, its results are guaranteed to be available before a specified deadline. To achieve this, sufficient and guaranteed computing resources (processor time, memory, service response times etc.) must be assigned to the software entities that perform the calculation.
*	Non-deterministic “best-effort” Processes can request guaranteed minimum resources for basic functionality, and additionally can have maximum resources specified for monitoring. However, if Time Determinism is requested, the resources must be guaranteed at any time, i.e. minimum and maximum resources are identical.
*	If the assumptions for deterministic execution are violated, e.g. due to a deadline miss, this must be treated as an error and recovery actions must be initiated. In nondeterministic “best-effort” subsystems such deadline violations or other deviations from normal behavior sometimes can be tolerated and mitigated without dedicated error management.
*	Fully-Deterministic behavior additionally requires Data Determinism, however in many cases Time Determinism is sufficient.

### 2.11.1.2 **Data Determinism**
*	For Data Determinism, each time a calculation is started, its results only depend on the input data. For a specific sequence of input data, the results always need to be exactly the same, assuming the same initial internal state.
*	A common approach to verify Data Determinism in a safety context is the use of lockstep mechanisms, where execution is done simultaneously through two different paths and the result is compared to verify consistency. Hardware lockstep means that the hardware has specific equipment to make this double-/multi-execution transparent. Software lockstep is another technique that allows providing a similar property without requiring the use of dedicated hardware.
*	Depending on the Safety Level, as well as the Safety Concept employed, software lockstep may involve executing multiple times the same software, in parallel or sequentially, but may also involve running multiple separate implementations of the same algorithm.

### 2.11.1.3 **Full Determinism**
*	For Full Determinism, each time a calculation is started, its results are available before a specified deadline and only depend on the input data, i.e. both Time and Data Determinism must be guaranteed.
*	Currently, only Full Deterministic behavior of one Process is specified. Determinism of a cluster of Processes on one or even several machines needs extensions of the Communication Management, which have not been specified yet. 
*	Non-deterministic behavior may arise from different reasons; for example insufficient computing resources, uncoordinated access of data, potentially by multiple threads running on multiple processor cores. The order in which the threads access such data will affect the result, which makes it non-deterministic (“race condition”).
*	A fully deterministic calculation must be designed, implemented and integrated in a way such that it is independent of processor load, sporadic unrelated events, race conditions, etc.

## 2.11.2 **Redundant Deterministic Execution**

*	Future systems need high computing performance in combination with high ASIL safety goals. 
*	Here are some additional rationales behind it:
	*	Safety goals for Highly Automated Driving (HAD) systems can be up to ASIL D.
	*	High Performance Computing (HPC) demands can only be met by non automotive-grade, e.g. consumer electronics (CE), microprocessors, which have high transient hardware error rates compared to automotive-grade microcontrollers. Most likely no such microprocessor is available for ASIL above B, at least for the parts relevant to the design.
	*	To deal with high error rates, ASIL C/D HAD applications require specific measures, in particular software lockstep, where execution is done redundantly through two different paths and the result is compared to detect errors.
	*	To make these redundant calculations comparable, software lockstep requires a fully deterministic calculation which must be designed, implemented and integrated in a way such that it is independent of processor load caused by other functions and calculations, sporadic unrelated events, race conditions, deviating random numbers etc, i.e. for the same input and initial conditions it always produces the same result within a given time.
	*	To meet HPC demands, highly predictable and reliable multi-threading must be supported. 
*	Two redundant Processes, which run in an internal cycle, get in each cycle the same input data via regular interfaces of the Communication Management and produce (in the absence of errors) the same results, due to full deterministic execution.

## 2.12	**Resource limitation**
*	Despite the correct behavior of a particular Adaptive Application in the system, it is important to ensure any potentially incorrect behavior, as well as any unforeseen interactions cannot cause interference in unrelated parts of the system.
*	it is important to consider that Execution Management is only responsible for the correct configuration of the Machine. However, enforcing the associated restrictions is usually done by either the Operating System or another Application like the Persistency service.  

### 2.12.1	**Resource configuration**
*	This section provides an overview on resource assignment to Processes, resources like:
	*	RAM (e.g. for code, data, thread stacks, heap)
	*	CPU time
*	In general, we need to distinguish between two resource demand values:
	*	Minimum resources, which need to be guaranteed so the process can reach its Running state and perform its basic functionality.
	*	Maximum resources, which might be temporarily needed and will not be exceeded at any time, otherwise it will throw an error.
*	Execution Management will configure the maximum amount of RAM available globally for all Processes belonging to each ResourceGroup when defined in the configuration, before loading a Process from this ResourceGroup. If a ResourceGroup does not have a configured RAM limit, then the Processes are only bound by their implicit memory limit.
*	Execution Management will configure the maximum amount of CPU time available globally for all Processes belonging to each ResourceGroup when defined in the configuration, before loading a Process from this ResourceGroup. If ResourceGroup does not have a configured CPU usage limit, then the Processes are only bound by their implicit CPU usage limit (priority, scheduling scheme...).

### 2.12.2	**Resource Monitoring**
*	The resources which are actually used by a Process should be controlled at any given time. 
*	For the entire system, the monitoring part of this activity is fulfilled by the Operating System.
*	Depending on system requirements and safety goals, an appropriate Operating System has to be chosen and configured accordingly, in combination with other monitoring mechanisms (e.g. for execution deadlines) which are provided by Platform Health Management.
*	Resource monitoring can serve several purposes, e.g.
	*	Detection of misbehavior of the monitored Process to initiate appropriate recovery actions, like Process restart or state change, to maintain the provided functionality and guarantee functional safety.
	*	Protection of other parts of the system by isolating the erroneous Processes from unaffected ones to avoid resource shortage.
*	For Processes which are attempting to exceed their configured maximum resource needs, one of the following alternatives:
	*	The resource limit violation or deadline miss is considered a failure and recovery actions may need to be initiated. Therefore the specific violation gets reported to the Platform Health Management, which then starts recovery actions which have been configured beforehand. This will be the standard option for deterministic subsystems.
	*	For Processes without hard deadlines, resource violations sometimes can be mitigated without dedicated error recovery actions, e.g. by interrupting execution and continue at a later point in time.
	*	If the OS provides a way to limit resource consumption of a Process or a group of Processes by design, explicit external monitoring is usually not necessary and often not even possible. Instead, the limitation mechanisms make sure that resource availability for other parts of the system is not affected by failures within the enclosed Processes. When such by-design limitation is used, monitoring mechanisms may still be used for the benefit of the platform, but are not required. Self-monitoring and out-of-process monitoring is currently out-of-scope in Adaptive Platform.

### 2.12.3 **Application-level Resource configuration**

### 2.12.3.1 **CPU Usage**
*	CPU usage is represented in a Process by its threads. Generally speaking, Operating Systems use some properties of each thread’s configuration to determine when to run it, and additionally constrain a group of threads to not use more than a defined amount of CPU time. Because threads may be created at runtime, only the first thread can be configured by Execution Management.

### 2.12.3.2 **Core Affinity**
*	Execution Management will configure the Core affinity of the Process initial thread restricting it to a sub-set of cores in the system. 
*	Depending on the capabilities of the Operating System the sub-set could be a single core. 
*	If the Operating System does not support binding to specific cores then the only supported sub-set is the entire set of cores.

### 2.12.3.3 **Scheduling Policy**
*	Currently available POSIX-compliant Operating Systems offer the scheduling policies required by POSIX, and in most cases additional, but different and incompatible scheduling strategies. This means for now, the required scheduling properties need to be configured individually, depending on the chosen OS.
*	Moreover, scheduling strategy is defined per thread and the POSIX standard allows for modifying the scheduling policy at runtime for a given thread, using pthread_setschedparam(). It is therefore not currently possible for the Adaptive Platform to enforce a particular scheduling strategy for an entire Process, but only for its first thread.
*	While scheduling policies are not a sufficient method to guarantee Full Determinism, they contribute to improve it. While the aim is to limit CPU time for a Process, scheduling policies apply to threads.
> **Note that while Execution Management will ensure the proper configuration for the first thread (that calls the main() function), it is the responsibility of the Process itself to properly configure secondary threads.**

### 2.12.3.4 **Resource Management**
*	In general, for deterministic behavior the required computing time is guaranteed and violations are treated as error, while best-effort subsystems are more robust and might be able to mitigate sporadic violations, e.g. by continuing the calculation at the next activation, or by providing a result of lesser quality. This means, if time (e.g. deadline or runtime budget) monitoring is in place, the reaction on deviations is different for deterministic and best-effort subsystems.
*	In fact, it may not even be necessary to monitor best-effort subsystems, since they by definition are doing only a function that may not succeed. This leads to an architecture where monitoring is a voluntary, configured property.
*	The remaining critical property however is to guarantee that a particular process or set of Processes cannot adversely affect the behavior of other Processes.
*	To guarantee Full Determinism for the entire system, it is important to ensure Freedom from Interference, which the ResourceGroup contribute to ensure.
*	Execution Management will configure the Process according to its ResourceGroup membership.

### 2.12.3.5 **Memory Budget and Monitoring**
*	To render a function, a Process requires the availability of some amount of memory for its usage (mainly code, data, heap, thread stacks). Over the course of its execution however, not all of this memory is required at all times, such that an OS can take advantage of this property to make these ranges of memory available on-demand, and provide them to other Processes when the memory is no longer used. 
*	While this has clear advantages in terms of system flexibility as well as memory efficiency, it is also in the way of both Time Determinism and Full Determinism: when a range of memory that was previously unused must now be made available, the OS may have to execute some amounts of potentially-unbounded activities to make this memory available. Often, the reverse may also be happening, removing previously available (but unused) memory from the Process under scope, to make it available to other Processes. This is detrimental to an overall system determinism.
*	Execution Management will ensure that the entire memory range that deterministic Processes may be using is available at the start and for the whole duration of the respective Process execution.
*	In order to provide sufficient memory at the beginning of the execution of a Process, some properties may need to be defined for each Process.
*	**Maximum heap:**
	*	Execution Management will configure the Maximum heap usage for the Process.
	*	Heap memory is used for dynamic memory allocation inside a Process e.g. through malloc()/free() and new/delete.	
*	**Maximum system memory usage:**
	*	Execution Management will configure the Maximum system memory usage of the Process. 
	*	System memory can be used to create extra resources like file handles or semaphores, as well as creating new threads.
*	**Process pre-mapping:**
	*	Execution Management will premap a Process if required by the corresponding Application Manifest.
	*	Fully pre-mapping a Process ensures that code and data execution is not going to be delayed at its first execution by demand-loading. This helps providing Time Determinism during system startup and first execution phases, but also helps with safety where code handling error cases can be preloaded and made guaranteed to be available. In addition, pre-mapping avoids late issues where filesystem may be corrupted and part of the Process may not be loadable anymore.