# Introduction {#adaptive_communicationmanagement_introduction}

The AUTOSAR Adaptive architecture organizes the software of the AUTOSAR Adaptive foundation as functional clusters. These clusters offer common functionality as services to the applications. The Communication Management (CM) for AUTOSAR Adaptive is such a functional cluster and also part of "AUTOSAR Runtime for Adaptive Applications" - ARA. It is responsible for the construction and supervision of communication paths between applications, both local and remote. The CM provides the infrastructure that enables communication between Adaptive AUTOSAR Applications within one machine and with software entities on other machines, e.g. other Adaptive AUTOSAR applications or Classic AUTOSAR SWCs. All communication paths can be established at design- , start-up- or run-time.

![Communication Management Functional Cluster](images/cm_architecture.png)

## 1. Architectural concepts
The Communication management of AUTOSAR Adaptive can be logically divided into
the following sub-parts:
1. Language binding
2. End-to-end communication protection
3. Communication / Network binding
4. Communication Management software

## 2. Design of the ARA API

* The (service) proxy is the representative of the possibly remote (i.e. other
  process, other core, other node) service. It is an instance of a C++ class
  local to the application/client, which uses the service.
  
* The (service) skeleton is the connection of the user provided service implementation
  to the middleware transport infrastructure. Service implementation
  is sub-classing the (service) skeleton.
  
* Beside proxies/skeletons, there might exist a so-called "Runtime" (singleton)
  class to provide some essentials to manage proxies and skeletons.
  
* Client/Server Communication uses concepts introduced by C++11 language, e.g.
  std::future, std::promise, to fully support method calls between different contexts.
    Future and Promise are the two separate sides of an asynchronous operation.
    * std::promise is used by the "producer/writer" of the asynchronous operation.
    * std::future is used by the "consumer/reader" of the asynchronous operation.
    * The reason it is separated into these two separate "interfaces" is to hide the "write/set" functionality from the "consumer/reader"

### 2.1 Communication paradigms

Service-Oriented Communication (SoC) is the main communication pattern for Adaptive
AUTOSAR Applications. It allows establishing communication paths both at
design- and run-time, so it can be used to build up both static communication with
known numbers of participants and dynamic communication with unknown number of
participants.

![Service-Oriented Communication](images/cm_SOC.png)

Service Discovery decides whether external and/or internal service-oriented communication
is established. The discovery strategy allows either returning a specific
service instance or all available instances providing the requested service at the time
of the request, no matter if they are available locally or remote.