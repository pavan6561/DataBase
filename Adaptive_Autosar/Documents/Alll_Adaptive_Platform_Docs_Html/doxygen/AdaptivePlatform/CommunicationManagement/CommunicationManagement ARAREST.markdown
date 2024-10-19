# ARA REST {#adaptive_communicationmanagement_ararest}
##1. What is REST ?
  REpresentational State Transfer (REST) is  a stateless client-server architecture in which the web services are viewed as resources offered by server and can be identified by their URIs. 
  Web service clients that want to use these resources access via globally defined set of remote methods like GET, PUT etc, that describe the action to be performed on the resource.
  It consists of two components REST server which provides access to the resources and a REST client which accesses and modify the REST resources.
  In the REST architecture style, clients and servers exchange representations of resources by using a standardized interface and protocol.REST isn’t protocol specific, but REST works over HTTP.
  The response from server is considered as the representation of the resources. This representation can be generated from one resource or more number of resources.
  It enables web applications that are built on various programming languages to communicate with each other. With the help of Restful services, these web applications can reside on different environments, some could be on Windows, and others could be on Linux.
  ![REST Overview ](images/Rest_Overview.png)

* REST is Representational State Transfer (REST) architectural style.
* Architecture style for networked systems, it is not a protocol nor a standard.
* Relies on other standards like HTTP, JSON, etc.
* Protocol, language and platform independent which makes it very flexible.
* Client-Server architecture just like the web.
* With REST, networked components are a resource you request access to whose implementation details are unclear.


##1.1 Key Properties of REST API
* **Client-Server :** 
  * The client-server architecture is a fairly well-known and widely used architecture. Most network based applications work in this manner. 
  * The architecture consists of a server that acts as a central point where one or many clients may interact. 
  * The idea behind a client-server architecture is that the server does the heavy lifting and orchestration required, whilst the client consumes the rich data and functionality by interacting with the server. 
  * When these computers communicate with each other, there’s a clear need for a common protocol that is understood and supported by both parties. 
  * The commonality required is the message format. By separating the user interface concerns from the data storage concerns, the portability of the user interface across multiple platforms is improved and also improve the scalibilty by simplifying the server components. 
  * The separation also allows the components to evolve independently.
  ![REST Client-Server ](images/architecture-client_server.png)

  
* **Stateless :**
  * A vast majority of web applications make use of a server-side managed session to keep track of a specific client. 
  * These sessions are often used for auth (authentication and authorization), keeping track of context, and storing meta data in memory that may be useful for managing the user’s activity. 
  * This makes scaling difficult as additional technologies and development is required to create session management servers in a clustered environment. 
  * The stateless architecture removes the need for the server to create and hold sessions. 
  * In a stateless architecture,  Communication must be stateless in nature, such that each request from client to server must contain all of the information necessary to understand the request and cannot take advantage of any stored context on the server.
  * The disadvantage is that it may decrease network performance by increasing the repetitive data sent in a series of request.
![REST Stateless ](images/architecture-stateless.png)
* **Cache :**
  *  In order to improve network efficiency, we add cache constraints to form the client-cache-stateless-server.
  *  The cacheable architecture is the concept of cleverly caching data that is used often, and changed infrequently. 
  *  Caching may occur in places like the client browser, and a caching server. 
  *  Caching mechanisms are expected to be smart enough to serve cached data or resources until that piece of data or resource changes for the client requesting it. 
  *  One of the advantages of caching is a reduced load on the network, this translates to reduction of unnecessary requests, lower data usage, and a more optimal performing application.
![REST Cache ](images/architecture-cacheable.png)
  
* **Uniform Interface :**
  *  The concept of a uniform interface includes creating a standard method of interacting with a system. 
  *  The uniform interface architecture also promotes abstraction of the implementation from the interface definition. 
  *  This allows for clients to interact with all services in the same way, using a standard protocol and message format with a well defined set of operations.
  *  The central feature that distinguishes the REST architectural style from other network-based styles is its emphasis on a uniform interface between components
  *  Implementations are decoupled from the services they provide, which encourages independent evolvability. 
  *  The trade-off, though, is that a uniform interface degrades efficiency, since information is transferred in a standardized form rather than one which is specific to an application's needs.
![REST Uniform Interface ](images/architecture-uniform_interface.png)

* **Layered System  :** 
  *  The layered architecture is utilised for a number of different factors. 
  *  A layered approach allows developers to separate concerns and loosely couple components that interact with each other. 
  *  The advantage of this approach when implemented optimally is that layers may change independently without impacting the rest of the application. 
  *  There are also cases where a layered architecture may consist of a layer that is shared across multiple other layers. 
  *   The layered system style allows an architecture to be composed of hierarchical layers by constraining component behavior.
  ![REST Layered system ](images/architecture-layered.png)    
  
  
  

##2. ARA::REST Architectural Elements
*  ARA::REST is a web standard based architecture and uses HTTP Protocol for data communication with help of POCO library.
*  A resource is accessed by a common interface using HTTP standard methods like GET, PUT, DELETE etc.
*  In ARA::REST architecture, a ARA::REST Server simply provides access to resources Object graph model and the ARA::REST client accesses and presents the resources.
*  Here each resource is identified by URIs/ Global IDs.
*  ARA::REST uses various representations to represent a resource like Text, JSON and XML.
*  ARA::REST is an API framework, which has components like Object Graph, Messages, Router, URI, Client, Server.
*  All transport protocol bindings must map to basic set of components.

![ARA Rest components ](images/ararest-component.png)

![RESTful ](images/REST.png)

*  The following HTTP methods are most commonly used in a REST based architecture.
   *  **GET** − Provides a read only access to a resource.
   *  **PUT** − Used to create a new resource.
   *  **DELETE** − Used to remove a resource.
   *  **POST** − Used to update an existing resource or create a new resource.
   *  **OPTIONS** − Used to get the supported operations on a resource.

*  The ARA::Rest elements are as follows:
    *  **Resources:** 
      *  ARA::REST architecture treats every content as a resource. These resources can be Text Files, Html Pages, Images, Videos or Dynamic Business Data.
      *  ARA::REST Server simply provides access to resources and ARA::REST client accesses and modifies the resources.
      *  Here each resource is identified by URIs/ Global IDs. ARA::REST uses various representations to represent a resource which can be Text, JSON, XML.
      *  The most popular representations of resources are XML and JSON.
      *  A resource in ARA::REST is a similar to an Object in Object Oriented Programming or is like an Entity in a Database.
      *  Once a resource is identified then its representation is to be decided using a standard format so that the server can send the resource in the above said format and client can understand the same format.

    *  **Messages:** 
      *  RESTful make use of HTTP protocols as a medium of communication between client and server.
      *   A client sends a message in form of a HTTP Request and the server responds in the form of an HTTP Response.
      *   This technique is termed as Messaging. These messages contain message data and metadata i.e. information about message itself.
      *   Message are of following types:
        *  **HTTP Request**
          *  An HTTP Request has five major parts −
            -  Verb − Indicates the HTTP methods such as GET, POST, DELETE, PUT, etc.
            -  URI − Uniform Resource Identifier (URI) to identify the resource on the server.
            -  HTTP Version − Indicates the HTTP version. For example, HTTP v1.1.
            -  Request Header − Contains metadata for the HTTP Request message as key-value pairs. For example, client (or browser) type, format supported by the client, format of the message body, cache settings, etc.
            -  Request Body − Message content or Resource representation.
        *  **HTTP Response**
            *  An HTTP Response has four major parts −
                -  Status/Response Code − Indicates the Server status for the requested resource. For example, 404 means resource not found and 200 means response is ok.
                - HTTP Version − Indicates the HTTP version. For example HTTP v1.1.
                - Response Header − Contains metadata for the HTTP Response message as keyvalue pairs. For example, content length, content type, response date, server type, etc.
                - Response Body − Response message content or Resource representation.
      *  Basic request and response messages.
        *  For the request:  
            *  Consists of a line that have the method, the path, and the version of HTTP being used.
            *  An optional header to send extra information, for example the user-agent, the content of the body etc…
            *  An optional body also can be used to send information depending on the method. For example you cant send any with GET but a POST request usually contains a body having the required information specific to a resource.
        *  For a response:
            *  Again the version, also there is a status code stating the result of the request (200 for a success, 404 not found, etc… error codes are grouped by the nature of the error, for example 5xx is about server side errors. 4xx client errors, 2xx successful, 1xx informational) and a status message.
                Also, the header and the body are optional.
          
    *  **Addressing(URI):**  
        *  Addressing refers to locating a resource or multiple resources lying on the server.
        *  Each resource in ARA::REST architecture is identified by its URI (Uniform Resource Identifier). A URI is of the following format −
            *  "<protocol>://<service-name>/<ResourceType>/<ResourceID>"
        *  Purpose of an URI is to locate a resource(s) on the server hosting the web service.
        *  Another important attribute of a request is VERB which identifies the operation to be performed on the resource.
        *  There are 4 parts in a URL:
            *  Protocol: The application-level protocol used by the client and server, e.g., HTTP, FTP, and telnet. 
            *  Hostname: The DNS domain name (e.g., www.nowhere123.com) or IP address (e.g., 192.128.1.2) of the server. 
            *  Port: The TCP port number that the server is listening for incoming requests from the clients. 
            *  Path-and-file-name: The name and location of the requested resource, under the server document base directory.
    *  **Object graph model:**
        *  ara::rest communicates via tree-structured data called object graphs.  
        *  OGM is a syntax tree of a “struct”; shall be able to map struct as well as JSON.
        *  It provides all necessary abstractions to build, traverse and dissect object graphs at C++ level.
        *  It reflects the capabilities of JSON to some extent for easy serialization.
        *  JSON is map–based, which makes it easy to divide a large data set into smaller subsets and send them across the wire on demand.
        *  JSON Graph allows a graph to be modeled as JSON without introducing duplicates. Instead of inserting an entity into the same message multiple times, each entity with a unique identifier is inserted into a single, globally unique location in the JSON Graph object.
        *  The Path to the only location within the JSON Graph object where an entity is stored is referred to as the entity’s Identity Path.
        *  No two entities in an application’s domain model should have the same Identity Path.
        *   So identity can be used in URI to browse through different node to GET the status of node element.
    ![Object Graph Model ](images/ObjectGraph.png)

    
    
##3. Use cases: Door ECU
*  Each Door has a similar but varying set of controls and actors.
*  Door ECU can be expressed as RESTful service.
*  Resource doorStatus always has one element (“leaf”).
*  Resource windowStatus has 0..1 elements (e.g., manual window).
*  Resource controls elements for windows and mirrors.
*   Door ECU becomes the ARA::REST server with Driver Door and Rear Door are two nodes each of them having different elements like doorstate, windowstatus and control etc.
  | Driver Door:        |  Rear Door:          |
  |---------------------------|-------------------------------|
  | Open/Close State      |  Open/Close State      |  
  | Lock/Unlock State      |  Lock/Unlock State      |
  | Handle pulled State    |  Handle pulled State      |
  | Electric Window      |  Electric Window        |
  | Control for four windows  |  Control for four windows  |

*   Restful Service model  
  |  Driver Door:                            |  Rear Door:                        |
  |-----------------------------------------------------------------------|--------------------------------------------------------  |
  |  /doorFL/doorState/leaf/open ={true, false}              |  /doorRR/doorState/leaf/open ={true, false}        |
  |  /doorFL/doorState/leaf/locked ={true, false}            |  /doorRR/doorState/leaf/locked = {true, false}      |
  |  /doorFL/doorState/leaf/handlePulled= {true, false}          |  /doorRR/doorState/leaf/handlePulled = {true, false}    |
  |  /doorFL/windowStatus/window/opened = [0%…100%]            |  /doorRR/windowStatus/window/opened = [0%…100%]      |
  |  /doorFL/controls/window[1-4] ={fullUp, up, keep, down, fullDown}  |  /doorRR/controls/4 ={fullUp, up, keep, down, fullDown}  | 
*  Any web browser or application can become a client and with proper authorization can access the server database (object graph model) and use these RESTful service model in URI to (GET, DELETE, POST , LIST) status of each node element as required .
*  URI to access Doorstate/leaf will be - http://example.com/get//doorFL/doorState/leaf/locked, this link will give you the status of the leaf whether it open or closed.

##4. POCO Library
*  REST works over http protocol and ARA::REST uses POCO libraries to implement the HTTP protocol.
*  The POCO C++ Libraries are a collection of open source C++ class libraries that simplify and accelerate the development of network-centric, portable applications in C++.
*  POCO consists of four core libraries, and a number of add-on libraries. The core libraries are Foundation, XML, Util and Net. Two of the add-on libraries are NetSSL, providing SSL support for the network classes in the Net library, and Data.
*  POCO's Net library makes it easy to write network-based applications. 
*  No matter whether your application simply needs to send data over a plain TCP socket, or whether your application needs a full-fledged built-in HTTP server.
*  At the lowest level, the Net library contains socket classes, supporting TCP stream and server sockets, UDP sockets, multicast sockets, ICMP and raw sockets. 
*  If your application needs secure sockets, these are available in the NetSSL library, implemented using OpenSSL 
![POCO library compon=nent ](images/POCO_component.png)   
*    The five core component libraries cover the following areas that can be found in the basic edition:
        *    **Foundation:**    
            *    Platform abstraction – Eases porting issues of fundamental types, etc.
            *    Memory management – Resource Acquisition Is Initialization (RAII), auto_ptr, reference counting garbage collection, etc.
            *    String utilities.
            *    Error handling – Extended exception classes.
            *    streams – Encoding and decoding and filtering support.
            *    threads – Concurrent processing support.
            *    Date and time – Date and time support including formatting and high precision timers
            *    File system – Abstracted file system support.
            *    Logging – Application and system logging, filtering and logging strategies.
            *    Processes – Interprocess communication and memory sharing.
            *    shared libraries – Dynamic library support.
            *    Notifications – Abstracted notification support.
            *    Events – Abstracted event support and strategies.
            *    Crypt – Random number generation, Digests and encoding/decoding engines.
            *    Text – Unicode encoding support.
            *    Regular expressions – Based on Perl compatible regular expression.
            *    URI – Universal Resource Identifier support.
            *    UUID – Universally Unique Identifiers support and generators.
            *    Cache support – Abstracted strategy support.
        *    **Net:**
            *    sockets – Abstracted low level network support.
            *    Reactor pattern – Popular network abstraction pattern support.
            *    MIME messages – Encoding support.
            *    HTTP – High level HTTP support for client and servers.
            *    FTP – High level FTP support.
            *    Mail – POP3, SMTP stream based support.
            *    HTML – Form support.
        *    **XML:**
            *    Document Object Model (DOM)
            *    XML writer
        *    **JSON:**
            *    APIs for reading and writing JSON
        *    **Util:**
            *    Configuration files
            *    Command line options
            *    Tools and server application framework
        
