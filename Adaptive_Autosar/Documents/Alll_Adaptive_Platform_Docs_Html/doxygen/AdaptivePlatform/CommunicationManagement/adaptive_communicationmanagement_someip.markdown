# SOME/IP(Scalable service-Oriented MiddlewarE over IP) {#adaptive_communicationmanagement_someip}
SOME/IP is an abbreviation for "Scalable service-Oriented MiddlewarE over IP". This middleware was designed for typical automotive use cases and for being compatible with AUTOSAR. SOME/IP is an automotive/embedded communication protocol which supports remote procedure calls, event notifications and the underlying serialization/wire format.

# Glossary of Terms

| Term            | Description                                                         | 
| :---------------------             | :---------------------------                                        |
| Method                             | A method, procedure, function, or subroutine that is called/invoked.|
| Parameters                         | input, output, or input/output arguments of a method or an event    |
| Remote Procedure Call (RPC)        | A method call from one ECU to another that is transmitted using messages    |
| Request        | a message of the client to the server invoking a method    |
| Response        | a message of the server to the client transporting results of a method invocation    |
| Request/Response communication        | a RPC that consists of request and response    |
| Event        | A uni-directional data transmission that is only invoked on changes or cyclically and is sent from the producer of data to the consumers.    |
| Field        | A field does represent a status and thus has an valid value at all times on which getter, setter and notifier act upon.    |
| Notification Event        | An event message of the notifier of a field.    |
| Getter        | A Request/Response call that allows read access to a field.    |
| Setter        | A Request/Response call that allows write access to a field.    |
| Notifier        | Sends out event message with a new value on change of the value of the field.    |
| Service        | A logical combination of zero or more methods, zero or more events, and zero or more fields.    |
| Service Interface        | the formal specification of the service including its methods, events, and fields    |
| Eventgroup        | A logical grouping of events and notification events of fields inside a service in order to allow subscription    |
| Service Instance   | Implementation of a service, which can exist more than once in the vehicle and more than once on an ECU    |
| Server   | The ECU offering a service instance is called server in the context of this service instance.    |
| Client   | The ECU using the service instance of a server is called client in the context of this service instance.    |
| Fire and Forget  | Requests without response message are called fire&forget.   |
|   Union  | A data structure that dynamically assumes different data types.   |


The three main parts of the SOME/IP specification is as follows:

1. @ref on_wire_format "On-wire format"
2. @ref protocol "Protocol"
3. @ref service_discovery "Service Discovery(SOME/IP-SD)"

# 1. On-wire format {#on_wire_format}

![SOME/IP On wire format](images/cm_someip_intro.png)

* SOME/IP allows applications to communicate.
* Packet formats are automatically determined by the specification of the Service.
* Server offers a Service Instance that implements the Service Interface.
* Client uses the Service Instance using SOME/IP.

# 2. Protocol {#protocol}
## 2.1 SOME/IP Header Format
![SOME/IP Header Format](images/someip_header_format.png)

### 2.1.1 Message ID [32 Bit]
* The Message ID is a 32 Bit identifier that is used to identify the RPC call to a method of an application or to identify an event.
* Message IDs of method calls will be structured in the ID with 2^16 services with 2^15 methods as shown below.
![Structure of Message ID](images/messageID.png)


### 2.1.2 Length [32 Bit]
* Length field contains the length in Byte starting from Request ID/Client ID until the end of the SOME/IP message.

### 2.1.3 Request ID [32 Bit]
* The Request ID allows a provider and subscriber to differentiate multiple parallel uses of the same method, event, getter or setter.
* The Request ID is unique for a provider- and subscriber-combination only.
* When generating a response message, the provider copies the Request ID from the request to the response message.
* Request ID constructed of the Client ID and Session ID as shown below.
![Structure of Request ID](images/requestID.png)
* The Client ID is the unique identifier for the calling client inside the ECU. The Client ID allows an ECU to differentiate calls from multiple clients to the same method.
* The Session ID is a unique identifier that allows to distinguish sequential messages. It also allows to distinguish requests originating from the same sender from each other.
* The Client ID supports being unique in the overall vehicle by having a configurable prefix or fixed value (e.g. the most significant byte of Client ID being the diagnostics address or a configured Client ID for a given application/SW-C).
![Structure of Request ID](images/clientID.png)
* Request/Response methods use session handling with Session IDs. Session ID should be incremented after each call.
* When the Session ID reaches 0xFFFF, it wraps around and starts again with 0x01.
* For request/response methods, a subscriber has to ignore a response if the Session ID of the response does not match the Session ID of the request.
* For notification messages, a receiver ignores the Session ID in case Session Handling is not active.
* For notification messages, a receiver treats the Session ID according to the respective use case.

### 2.1.4 Protocol Version [8 Bit]
* The Protocol Version set to 1.

### 2.1.5 Interface Version [8 Bit]
* Interface Version contains the Major Version of the Service Interface.

### 2.1.6 Message Type [8 Bit]
* The Message Type field is used to differentiate different types of messages and contains the following values as shown in Table.

| Number | Value                | Description                                                         | 
| :----  | :-----------------   | :----                                                               |
| 0x00   | REQUEST              | A request expecting a response (even void)                          |
| 0x01   | REQUEST_NO_RETURN    | A fire&forget request                                               |
| 0x02   | NOTIFICATION         | A request of a notification/event callback expecting no response    |
| 0x80   | RESPONSE             | The response message                                                |
| 0x81   | ERROR                | The response containing an error                                    |
| 0x20   | TP_REQUEST           | A Transport Protocol request expecting a response (even void)                       |
| 0x21   | TP_REQUEST_NO_RETURN | A Transport Protocol fire&forget request                                            |
| 0x22   | TP_NOTIFICATION      | A Transport Protocol request of a notification/event callback expecting no response |
| 0x23   | TP_RESPONSE          | The Transport Protocol response message                                             |
| 0x24   | TP_ERROR             | The Transport Protocol response containing an error                                 |

* The 3rd highest bit of the Message Type (=0x20) is called as TP-Flag and will be set to 1 to signal that the current SOME/IP message is a segment. The other bits of the Message Type are set as specified in this Section.

### 2.1.7 Return Code [8 Bit]
* The Return Code is used to signal whether a request was successfully processed. For simplification of the header layout, every message transports the field Return Code. The allowed Return Codes for specific message types are shown in Table.

| Message Type       | Allowed Return Codes   |
| :----              | :-----------------     |
| REQUEST            | N/A set to 0x00 (E_OK) |
| REQUEST_NO_RETURN  | N/A set to 0x00 (E_OK) |
| NOTIFICATION       | N/A set to 0x00 (E_OK) |
| RESPONSE           | Please refer 2.2.6.1 section   |
| ERROR              | Please refer 2.2.6.1 section   |

### 2.1.8 Payload [variable size]
* In the payload field the parameters are carried. The serialization of the parameters will be specified in the following section.
* The size of the SOME/IP payload field depends on the transport protocol used. 
* The UDP binding of SOME/IP can only transport SOME/IP messages that fit directly into an IP packet. Use TCP only if very large chunks of data need to be transported (> 1400 Bytes) and no hard latency requirements in the case of errors exists

### 2.1.9 Event, Field and Eventgroup
* Eventgroup is a logical grouping of events and notification events of fields inside a service in order to allow subscription.
* Events and notifications are transported using RPC. Events will be structured as shown in Table
![Structure of Event ID](images/eventID.png)


### 2.1.10 Endianess
* SOME/IP Header will be encoded in network byte order (big endian).
* The byte order of the parameters inside the payload is defined by configuration.

### 2.1.11 Serialization of Data Structures
* The serialization is based on the parameter list defined by the interface specification.
The interface specification defines the exact position of all data structures in the PDU
and has to consider the memory alignment.
* Alignment is used to align the beginning of data by inserting padding elements after
the data in order to ensure that the aligned data starts at certain memory addresses.

#### 2.1.11.1 Basic Datatypes
The following basic datatypes as shown in Table are supported:

| Type    | Description           | Size [bit] |  Remark                                |
| :----   | :-----------------    | :----      |  :-----------                          |
| boolean | TRUE/FALSE value      | 8          |  FALSE (0), TRUE (1)                   |
| uint8   | unsigned Integer      | 8          |                                        |
| uint16  | unsigned Integer      | 16         |                                        |
| uint32  | unsigned Integer      | 32         |                                        |
| sint8   | signed Integer        | 8          |                                        |
| sint16  | signed Integer        | 16         |                                        |
| sint32  | signed Integer        | 32         |                                        |
| float32 | floating point number | 32         | IEEE 754 binary32 (Single Precision)   |
| float64 | floating point number | 64         | IEEE 754 binary64 (Double Precision)   |

#### 2.1.11.2 Structured Datatypes (structs)
* The serialization of a struct will be close to the in-memory layout. This means, only
the parameters will be serialized sequentially into the buffer. Especially for structs it
is important to consider the correct memory alignment.
![Serialization of Structs](images/serialization_of_structs.png)


#### 2.1.11.3 Strings
* All strings always starts with a Byte Order Mark (BOM).
  * BOM : The byte order mark (BOM) is a Unicode character, U+FEFF byte order mark (BOM), whose appearance as a magic number at the start of a text stream can signal several things (eg. endianness) to a program consuming the text
* The BOM will be included in fixed-length-strings as well as dynamic-length strings.
* BOM allows the possibility to detect the used encoding.

#### 2.1.11.4 Arrays (fixed length)
* The length of fixed length arrays is defined by the data type definition. They can be seen as repeated elements.Fixed length arrays are easier for use in very small devices. Dynamic length arrays might need more resources on the ECU using them.

##### 2.1.11.4.1 One-dimensional
* The one-dimensional arrays with fixed length "n" will carry exactly "n" elements of the same type.
![One-dimensional array (fixed length)](images/one_dimension_array_fixedlength.png)


##### 2.1.11.4.2 Multidimensional
* The serialization of multidimensional arrays follows the in-memory layout of multidimensional arrays in the programming language (row-major order) and is shown in Figure below.
![Multidimensional array (fixed length)](images/multi_dimension_array_fixedlength.png)


#### 2.1.11.5 Dynamic Length Arrays
* The layout of arrays with dynamic length will be based on the layout of fixed length arrays.
* An optional length field at the beginning of an array should be used to specify the length of the array in Bytes.


##### 2.1.11.5.1 One-dimensional
![One-dimensional array (dynamic length)](images/one_dimension_array_dynamiclength.png)
* In the one-dimensional array one length field is used, which carries the number of bytes
used for the array.


##### 2.1.11.5.2 Multidimensional
![Multidimensional array (dynamic length)](images/multi_dimension_array_dynamiclength.png)
* In multidimensional arrays every sub array of different dimensions will have its own length field.

#### 2.1.11.6 Enumeration
* Enumerations are not considered in SOME/IP. Enumerations are transmitted as unsigned integer datatypes.

#### 2.1.11.7 Bitfield
* Bitfields are transported as unsigned datatypes uint8/uint16/uint32.

#### 2.1.11.8 Union / Variant
* A union (also called variant) is such a parameter that can contain different types of
data. For example, if one defines a union of type uint8 and type uint16, the union will
carry data which are a uint8 or a uint16.

* Which data type will be transmitted in the payload can only be decided during execution.
In this case, however, it is necessary to not only send the data itself but add an
information about the applicable data type as a form of "meta-data" to the transmission.By the means of the attached meta-data the sender can identify the applicable data type of the union and the receiver can accordingly access the data properly.
* A union consists of a length field, type selector and the payload as shown Table below.
![Default Serialization of Unions](images/default_serialization_unions.png)
* The length field defines the size of the data and padding in bytes and does not include the size of the length field and type field.
* The type field specifies the data type of the data.


## 2.2 SOME/IP Protocol specification
This section describes the Remote Procedure Call(RPC), Event Notifications and Error Handling of SOME/IP.

### 2.2.1 Transport Protocol Bindings
* In order to transport SOME/IP messages different transport protocols may be used. SOME/IP currently supports UDP and TCP.
* If a server runs different instances of the same service, messages belonging to different service instances will be mapped to the service instance by the transport protocol port on the server side.
* All Transport Protocol Bindings supports transporting more than one SOME/IP message in a Transport Layer PDU (i.e. UDP packet or TCP segment).

#### 2.2.1.1 UDP Binding
* The UDP binding of SOME/IP will be achieved by transporting SOME/IP messages in UDP packets.
* The header format allows transporting more than one SOME/IP message in a single UDP packet. The SOME/IP implementation identifies the end of a SOME/IP message by means of the SOME/IP length field. Based on the UDP length field, SOME/IP determines if there are additional SOME/IP messages in the UDP packet.

#### 2.2.1.2 TCP Binding
* The TCP binding of SOME/IP is based on the UDP binding. In contrast to the
UDP binding, the TCP binding allows much bigger SOME/IP messages and uses the
robustness features of TCP (coping with loss, reorder, duplication, etc.).
* When the TCP connection is lost, outstanding requests will be handled as timeouts. Since TCP handles reliability, additional
means of reliability are not needed.
* The client and server uses a single TCP connection for all methods, events, and notifications of a service instance. When having more
than one instance of a service a TCP connection per services instance is needed.
* The TCP connection will be opened by the client, when the first method call will be transported or the client tries to receive the first notifications. The client is responsible for re-establishing the TCP connection whenever it fails.
* The TCP connection will be closed by the client, when the TCP connection is not required anymore.
* The TCP connection will be closed by the client, when all Services using the TCP connections are not available anymore (stopped or timed out).
* The server will not stop the TCP connection when stopping all services. Give the client enough time to process the control data to shutdown the TCP connection itself.

#### 2.2.1.3 Multiple Service-Instances
* Service-Instances of the same Service are identified through different Instance IDs. SOME/IP supports multiple Service-Instances reside on different ECUs as well as multiple Service-Instances of one or more Services reside on one single ECU.
* While different Services will be able to share the same port number of the transport layer protocol used, multiple Service-Instances of the same service on one single ECU listens on different ports per Service-Instance.
* A Service Instance can be identified through the combination of the Service ID combined with the socket (i.e. IP-address, transport protocol (UDP/TCP), and port number).

#### 2.2.1.4 Transporting large SOME/IP messages of UDP (SOME/IP-TP)
* The UDP binding of SOME/IP can only transport SOME/IP messages that fit directly into an IP packet. If larger SOME/IP messages need to be transported over UDP (e.g. of 32 KB) the SOME/IP Transport Protocol (SOME/IP-TP) will be used. The SOME/IP message too big to be transported directly with the UDP binding will be called "original" SOME/IP message. The "pieces" of the original SOME/IP message payload transported in SOME/IP-TP messages will be called "segments".
* SOME/IP messages using SOME/IP-TP activates Session Handling (Session ID must be unique for the original message).
* All SOME/IP-TP segments carry the Session ID of the original message; thus, they have all the same Session-ID.
* SOME/IP-TP segments will have the TP-Flag of the Message Type set to 1.
* SOME-IP-TP-Header is as shown in below. SOME/IP-TP segments will have a TP header right after the SOME/IP header (i.e. before the SOME/IP payload) with the following structure (bits from highest to lowest):
  
  **a. Offset [28 bits]**

  **b. Reserved Flag [1 bit]**

  **c. Reserved Flag [1 bit]**

  **d. Reserved Flag [1 bit]**

  **e. More Segments Flag [1 bit]**
  
![SOME/IP TP header](images/someip_tp_header.png)

#### 2.2.1.4.1 Example
* This example describes how an original SOME/IP message of 5880 bytes payload has to be transmitted. The Length field of this original SOME/IP message is set to 8 + 5880 bytes.
![Example: Header of Original SOME/IP message](images/example_header.png)

* This original SOME/IP message will now be segmented into 5 consecutive SOME/IP segments. Every payload of these segments carries at most 1392 bytes in this example.
* For these segments, the SOME/IP TP module adds additional TP fields. The Length field of the SOME/IP carries the overall length of the SOME/IP segment including 8 bytes for the Request ID, Protocol Version, Interface Version, Message Type and Return Code. Because of the added TP fields (4 bytes), this Length information is extended by 4 additional SOME/IP TP bytes.
* The following figure provides an overview of the relevant SOME/IP header settings for every SOME/IP segment:
![Example: Overview of relevant SOME/IP TP headers](images/someiptp.png)
* Please note that the value provided within the Offset Field is given in units of 16 bytes, i.e.: The Offset Value of 87 correspond to 1392 bytes Payload.
* The first 4 segments contain 1392 Payload bytes each with "More Segments Flag" set to ’1’:
![Example: Header of the SOME/IP segments](images/someipsegment.png)

* The last segment (i.e. 5) contains the remaining 312 Payload bytes of the original 5880 bytes payload. This last segment is marked with "More Segments flag" set to ’0’.
![Example: Header of the SOME/IP segments](images/someiptplastsegmen.png)


### 2.2.2 Request/Response Communication
* One of the most common communication patterns is the request/response pattern. One communication partner (Client) sends a request message, which is answered by another communication partner (Server).
![](images/request_response.png)

* For the SOME/IP request message the client has to do the following for payload and header:
  * Construct the payload
  * Set the Message ID based on the method the client wants to call
  * Set the Length field to 8 bytes (for the part of the SOME/IP header after the length field) + length of the serialized payload
  * Optionally set the Request ID to a unique number (unique for client only)
  * Set the Protocol Version according
  * Set the Interface Version according to the interface definition
  * Set the Message Type to REQUEST (i.e. 0x00)
  * Set the Return Code to 0x00
  
* The server builds the header of the response based on the header of the client’s request and does in addition:
  * Construct the payload 
  * Take over the Message ID from the corresponding request
  * Set the length to the 8 Bytes + new payload size
  * Take over the Request ID from the corresponding request
  * Set the Message Type to RESPONSE (i.e. 0x80) or ERROR (i.e. 0x81)
  * Set Return Code to the return code of called method or in case of an Error message to a valid error code

### 2.2.3 Fire&Forget Communication
* Requests without response message are called fire&forget.
![](images/fire_forgot.png)
* For the SOME/IP request-no-return message the client has to do the following for payload and header:
  * Construct the payload
  * Set the Message ID based on the method the client wants to call
  * Set the Length field to 8 bytes (for the part of the SOME/IP header after the length field) + length of the serialized payload
  * Optionally set the Request ID to a unique number (unique for client only)
  * Set the Protocol Version according
  * Set the Interface Version according to the interface definition
  * Set the Message Type to REQUEST_NO_RETURN (i.e. 0x01)
  * Set the Return Code to 0x00

### 2.2.4 Notification Events
* Notifications describe a general Publish/Subscribe-Concept. Usually the server publishes a service to which a client subscribes. On certain cases the server will send the client an event, which could be for example an updated value or an event that occurred.
![](images/events.png)
* SOME/IP is used only for transporting the updated value and not for the publishing and subscription mechanisms. These mechanisms are implemented by SOME/IP-SD.
* For the SOME/IP notification message the client has to do the following for payload and header:
  * Construct the payload
  * Set the Message ID based on the event the server wants to send
  * Set the Length field to 8 bytes (for the part of the SOME/IP header after the length field) + length of the serialized payload
  * Set the Client ID to 0x00. In case of active Session Handling the Session ID will be incremented upon each transmission.
  * Set the Protocol Version according
  * Set the Interface Version according to the interface definition
  * Set the Message Type to NOTIFICATION (i.e. 0x02)
  * Set the Return Code to 0x00
  
#### 2.2.4.1 Strategy for sending notifications
* For different use cases different strategies for sending notifications are possible. The following examples are common:

  **a. Cyclic update** — send an updated value in a fixed interval (e.g. every 100 ms for safety relevant messages with Alive)
  
  **b. Update on change** — send an update as soon as a "value" changes (e.g. door open)
  
  **c. Epsilon change** — only send an update when the difference to the last value is greater than a certain epsilon. This concept may be adaptive, i.e. the prediction is based on a history; thus, only when the difference between prediction and current value is greater than epsilon an update is transmitted.

### 2.2.5 Fields
* A field represents a status and has a valid value. The consumers subscribing for the field instantly after subscription get the field value as an initial event.
![](images/field.png)
* A field is a combination of getter, setter and notification event.
* The getter of a field is a request/response call that has an empty payload in the request message and the value of the field in the payload of the response message.
* The setter of a field is a request/response call that has the desired value of the field in the payload of the request message and the value that was set to the field in the payload of the response message.
* The notifier sends an event message that transports the value of the field to the client when the client subscribes to the field.

### 2.2.6 Error Handling
* Error handling can be done in the application or the communication layer below. Therefore SOME/IP supports two different mechanisms explained below:

#### 2.2.6.1 Error Handling Return Codes in the Response Messages of methods
* Return Codes in the Response Messages of methods is used to transport application errors and the response data of a method from the provider to the caller of a method.

![Return Codes](images/return_codes.png)


#### 2.2.6.2 Explicit Error Messages
* Explicit Error Messages are used to transport application errors and the response data or generic SOME/IP errors from the provider to the
caller of a method.
* If more detailed error information need to be transmitted, the payload of the Error Message (Message Type 0x81) will be filled with error specific data, e.g. an exception string. Error Messages will be sent instead of Response Messages.
* This can be used to handle all different application errors that might occur in the server. In addition, problems with the communication medium or intermediate components (e.g. switches) may occur, which have to be handled e.g. by means of reliable transport.
* For more flexible error handling, SOME/IP allows a different message layout specific for Error Messages instead of using the message layout of Response Messages.
The recommended layout for the exception message is the following:
  a. Union of specific exceptions. At least a generic exception without fields needs to exist.
  b. Dynamic Length String for exception description.
* The union gives the flexibility to add new exceptions in the future in a typesafe manner. The string is used to transport human readable exception descriptions to ease testing and debugging.

#### 2.2.6.3 Error Processing Overview
Error handling is based on the message type received (e.g. only methods can be answered with a return code) and will be checked in a defined order shown below.

![Message Validation and Error Handling in SOME/IP](images/error_handling.png)

# 3. Service Discovery(SOME/IP-SD) {#service_discovery}
* SOME/IP-SD is used to
  * Locate service instances.
  * Detect if service instances are running.
  * Implement the Publish/Subscribe handling.

* SOME/IP-SD depends on SOME/IP. SOME/IP itself supports both TCP and UDP communications but SOME/IP SD uses SOME/IP only over UDP

![SOME/IP-SD Dependencies to other protocol layers](images/dependancy_someipsd.png)

## 3.1 SOME/IP-SD Message Format
![SOME/IP-SD Header Format](images/someipsd_header.png)
We can divide the above mention figure into two parts.

### 3.1.1 SOME/IP header
| Field()                  | Description(Default Value)           
| :--------------        | :-------------------------    | 
| Service-ID(16 bits)    | 0xFFFF      | 
| Method-ID(16 bits)     | 0x8100      | 
| length(32 bits)        | length is measured in bytes and starts with the first byte after the length field and ends with the last byte of the SOME/IP-SD message.      | 
| Client-ID(16 bits)                 | Set to 0x0000      | 
| Session-ID (16 Bits)                  | The Session-ID (SOME/IP header) will be incremented for every SOME/IP-SD message sent.The Session-ID (SOME/IP header) starts with 1 and be 1 even after wrapping. | 
| Protocol- Version (8 Bits)                 | 0x01        | 
| Interface- Version (8 Bits)                 | 0x01       | 
| Message Type (8 bits)                | 0x02 (Notification) | 
| Return Code(8 bits)                | Return Code(8 bits) | 


### 3.1.2 SOME/IP-SD Header
![SOME/IP-SD Example PDU](images/someip_sd_pdu.png)
* The SOME/IP-SD Header starts with an 8 Bit field called Flags as shown in the Figure below.
![Flags in SOME/IP-SD](images/someipsd_flags.png)
* The Reboot Flag of the SOME/IP-SD Header is set to 1 for all messages after reboot until the Session-ID in the SOME/IP-Header wraps around and thus starts with 1 again. After this wrap around the Reboot Flag is set to 0.

* The information for the reboot flag and the Session ID will be kept for multicast, unicast and every sender-receiver relation (i.e. source address and destination address) separately.
* This means there will be separate counters for sending and receiving.
* **Sending**
    * There will be a counter for multicast.
    * There will be a separate counter for each peer for unicast.
    
* **Receiving**
    * There will be a counter for each peer for multicast.
    * There will be a counter for each peer for unicast.

* The Unicast Flag of the SOME/IP-SD Header is set to Unicast (that means 1) for all SD Messages since this means that receiving using unicast is supported.
* The third flag of the SOME/IP-SD Flags (third highest order bit) will be called Explicit Initial Data Control Flag and means that the ECU supports explicit initial data control.
* After the SOME/IP-SD Header the Entries Array follows. The entries will be processed exactly in the order they arrive.
* After the Entries Array in the SOME/IP-SD Header an Option Array will follow.

#### 3.1.2.1 Entry Format
* The service discovery supports multiple entries that are combined in one service discovery message. The entries are used to synchronize the state of services instances and the Publish/Subscribe handling.
* Two types of entries exist: A Service Entry Type for Services and an Eventgroup Entry Type for Eventgroups.

##### 3.1.2.1.1 Service Entry Type for Services
![SOME/IP-SD Service Entry Type](images/someip_sd_serviceentry.png)

* Type Field [uint8]: encodes FindService (0x00) and OfferService (0x01).
* Index First Option Run [uint8]: Index of this runs first option in the option array.
* Index Second Option Run [uint8]: Index of this runs second option in the option array.
* Number of Options 1 [uint4]: Describes the number of options the first option run uses.
* Number of Options 2 [uint4]: Describes the number of options the second option run uses.
* Service-ID [uint16]: Describes the Service ID of the Service or Service-Instance this entry is concerned with.
* Instance ID [uint16]: Describes the Service Instance ID of the Service Instance this entry is concerned with or is set to 0xFFFF if all service instances of a service are meant.
* Major Version [uint8]: Encodes the major version of the service (instance).
* TTL [uint24]: Describes the lifetime of the entry in seconds.
* Minor Version [uint32]: Encodes the minor version of the service.

##### 3.1.2.1.2 Eventgroup Entry Type for Eventgroups
![SOME/IP-SD Eventgroup Entry Type](images/someip_sd_eventgroupentry.png)
* Type Field [uint8]: encodes Subscribe (0x06), and SubscribeAck (0x07).
* Index First Option Run [uint8]: Index of this runs first option in the option array.
* Index Second Option Run [uint8]: Index of this runs second option in the option array.
* Number of Options 1 [uint4]: Describes the number of options the first option run uses.
* Number of Options 2 [uint4]: Describes the number of options the second option run uses.
* Service-ID [uint16]: Describes the Service ID of the Service or Service Instance this entry is concerned with.
* Instance ID [uint16]: Describes the Service Instance ID of the Service Instance this entry is concerned with or is set to 0xFFFF if all service instances of a service are meant.
* Major Version [uint8]: Encodes the major version of the service instance this eventgroup is part of.
* TTL [uint24]: Descibes the lifetime of the entry in seconds.
* Reserved [uint8]: Set to 0x00.
* Initial Data Requested Flag [1 bit] (I Flag): Set to 1, if initial data will sent by Server
* Reserved2 [uint3]: Set to 0x0.
* Counter [uint4]: Is used to differentiate identical Subscribe Eventgroups of the same subscriber. Set to 0x0 if not used.
* Eventgroup ID [uint16]: Transports the ID of an Eventgroup.

##### 3.1.2.1.3 Referencing Options from Entries
* Index First Option Run: Index into array of options for first option run. Index 0 means first of SOME/IP-SD packet.
* Index Second Option Run: Index into array of options for second option run. Index 0 means first of SOME/IP-SD packet.
* Number of Options 1: Length of first option run. Length 0 means no option in option run.
* Number of Options 2: Length of second option run. Length 0 means no option in option run.
* Two different option runs exist: First Option Run and Second Option Run. Two different types of options are expected:
options common between multiple SOME/IP-SD entries and options different for each SOME/IP-SD entry. Supporting to different options runs is the most efficient way to support these two types of options, while keeping the wire format highly efficient.
* Each option run will reference the first option and the number of options for this run.
* If the number of options is set to zero, the option run is considered empty.
* For empty runs the Index (i.e. Index First Option Run and/or Index Second Option Run) will set to zero.

#### 3.1.2.2 Options Format
* Options are used to transport additional information to the entries. This includes for instance the information how a service instance is reachable (IP-Address, Transport Protocol, Port Number).
* In order to identify the option type every option starts with:
  * Length [uint16]: Specifies the length of the option in Bytes.
  * Type [uint8]: Specifying the type of the option.
  
* The length field covers all bytes of the option except the length field and type field.

##### 3.1.2.2.1 Configuration Option
* The configuration option is used to transport arbitrary configuration strings. This allows to encode additional information like the name of a service or its configuration.
![SOME/IP-SD Configuration Option](images/SOMEIPSDConfigurationOption.png)


##### 3.1.2.2.2 Load Balancing Option
* The Load Balancing option is used to prioritize different instances of a service, so that a client chooses the service instance based on these settings. This option will be attached to Offer Service entries.
* The Load Balancing Option carry a Priority and Weight, which is used for load balancing different service instances.
![SOME/IP-SD Load Balancing Option](images/SOMEIPSDLoadBalancingOption.png)


##### 3.1.2.2.3 IPv4 Endpoint Option
* The IPv4 Endpoint Option is used by a SOME/IP-SD instance to signal the relevant endpoint(s).
* Endpoints include the local IP address, the transport layer protocol (e.g. UDP or TCP), and the port number of the sender.
* These ports are used for the events and notification events as well.
![SOME/IP-SD IPv4 Endpoint Option](images/SOMEIPSDIPv4EndpointOption.png)
* This option is used by the server for its OfferService entry and by client for Subscribe Eventgroup entry.

##### 3.1.2.2.4 IPv6 Endpoint Option
* The IPv6 Endpoint Option is used by a SOME/IP-SD instance to signal the relevant endpoint(s).
* Endpoints include the local IP address, the transport layer protocol (e.g. UDP or TCP), and the port number of the sender.
* ports are used for the events and notification events as well.
* This option is used by the server for its OfferService entry and by client for Subscribe Eventgroup entry.
![SOME/IP-SD IPv6 Endpoint Option](images/SOMEIPSDIPv6EndpointOption.png)


##### 3.1.2.2.5 IPv4 Multicast Option
* The IPv4 Multicast Option is used by the server to announce the IPv4 multicast address, the transport layer protocol (ISO/OSI layer 4), and the port number the multicast events and multicast notification events are sent to.
* As transport layer protocol currently only UDP is supported.
* The IPv4 Multicast Option is referenced by Subscribe Eventgroup Ack entries.
* The server reference the IPv4 Multicast Option, which encodes the IPv4 Multicast Address and Port Number the server will send multicast events and notification events to.
![SOME/IP-SD IPv4 Multicast Option](images/SOMEIPSDIPv4MulticastOption.png)


##### 3.1.2.2.6 IPv6 Multicast Option
* The IPv6 Multicast Option is used by the server to announce the IPv6 multicast address, the layer 4 protocol, and the port number the multicast events and multicast notifications events are sent to.
* For the transport layer protocol (ISO/OSI layer 4) currently only UDP is supported.
* The IPv6 Multicast Option, referenced by Subscribe Eventgroup Ack messages.
* The server references the IPv6 Multicast Option, which encodes the IPv6 Multicast Address and Port Number the server will send multicast events and notification events to.
![SOME/IP-SD IPv6 Multicast Option](images/SOMEIPSDIPv6MulticastOption.png)


##### 3.1.2.2.7 IPv4 SD Endpoint Option
* The IPv4 SD Endpoint Option is used to transport the endpoint (i.e. IP-Address and Port) of the senders SD implementation.
* This is used to identify the SOME/IP-SD Instance even in cases in which the IP-Address and/or Port Number cannot be used.
* A use case would be a proxy service discovery on one ECU which handles the multicast traffic for another ECU.
![SOME/IP-SD IPv4 SD Endpoint Option](images/SOMEIPSDIPv4SDEndpointOption.png)


##### 3.1.2.2.8 IPv6 SD Endpoint Option
* The Ipv6 SD Endpoint Option is used to transport the endpoint (i.e. IP-Address and Port) of the senders SD implementation.
* This is used to identify the SOME/IP-SD Instance even in cases in which the IP-Address and/or Port Number cannot be used.
* A use case would be a proxy service discovery on one ECU which handles the multicast traffic for another ECU.
![SOME/IP-SD IPv6 SD Endpoint Option](images/SOMEIPSDIPv6SDEndpointOption.png)



* Example:Below Figure shows an example with the different Endpoint and a Multicast Option:
* The Server offers the Service Instance on Server UDP-Endpoint SU and Server TCP-Endpoint ST
* The Client sends a Subscribe Eventgroup entry with Client UDP-Endpoint CU (unicast) and a Client TCP-Endpoint CT.
![Publish/Subscribe Example for Endpoint Options and the usage of ports](images/Endpoints.png)


### 3.1.3 Service Entries
#### 3.1.3.1 Find Service Entry
* The Find Service entry type is used for finding service instances and will only be sent if the current state of a service is unknown.(no current Service Offer was received and is still valid).
* Find Service entries sets the entry fields in the following way:
| Field  | Description(Default Value)                                          | 
| :----  | :----                                                               |
| Type    | 0x00 (FindService)                         |
| Service ID   | Set to the Service ID of the service                                                 |
| Instance ID   | 0xFFFF(all service instances) or Instance ID of a specific service instance     |
| Major Version   | 0xFF                                                |
| Minor Version   | 0xFFFFFFFF(services with any version will be returned) or set to a value different to 0xFFFF FFFF, services with this specific minor version will be returned only                                  |
| TTL   | Set to the lifetime of the Find Service entry. After this lifetime the Find Service entry is considered not existing. If TTL is set to 0xFFFFFF, the Find Service entry is considered valid until the next reboot. TTL will not be set to 0x000000 since this is considered to be the Stop Find Service Entry.                      |

* Find the wireshark output for FindService service below.
* Consider IP address 192.168.7.4 for consumer application, 192.168.7.2 for producer application and 224.244.224.245 as a multicast address.

![Wireshark output for FindService()](images/FindService_Wireshark.png)

#### 3.1.3.2 Offer Service Entry
* The Offer Service entry type is used to offer a service to other communication partners.
* Offer Service entries sets the entry fields in the following way:
| Field  | Description(Default Value)                                          | 
| :----  | :----                                                               |
| Type    | 0x01 (OfferService)                         |
| Service ID   | Set to the Service ID of the service instance offered.                                                 |
| Instance ID   | Set to the Instance ID of the service instance that is offered     |
| Major Version   | Set to the Major Version of the service instance that is offered.                                                |
| Minor Version   | Set to the Minor Version of the service instance that is offered.                                  |
| TTL   | Set to the lifetime of the Offer Service entry. After this lifetime the Offer Service entry is considered not existing. If TTL is set to 0xFFFFFF, the Offer Service entry will be considered valid until the next reboot. TTL will not be set to 0x000000 since this is considered to be the Stop Offer Service Entry.                      |

* Find the wireshark output for OfferService service below.
* Consider IP address 192.168.7.4 for consumer application, 192.168.7.2 for producer application and 224.244.224.245 as a multicast address.

![Wireshark output for OfferService()](images/OfferService_Wireshark.png)

### 3.1.4 Eventgroup Entry
#### 3.1.4.1 Subscribe Eventgroup Entry
* The Subscribe Eventgroup entry type is used to subscribe to an eventgroup.
| Field  | Description(Default Value)                                          | 
| :----  | :----                                                               |
| Type    | 0x06 (SubscribeEventgroup)                         |
| Service ID   | set to the Service ID of the service instance that includes the eventgroup subscribed to. |
| Instance ID   | set to the Instance ID of the service instance that includes the eventgroup subscribed to.     |
| Major Version   | set to the Major Version of the service instance of the eventgroup subscribed to.         |
| Eventgroup ID   | set to the Eventgroup ID of the eventgroup subscribed to.                      |
| Minor Version   | set to the Minor Version of the service instance of the eventgroup subscribed to.           |
| TTL   | If set to 0xFFFFFF, the Subscribe Eventgroup entry considered as valid until the next reboot. TTL will not be set to 0x000000 since this is considered to be the Stop       |
| Reserved   | set to 0x00 |
| Initial Data Requested Flag   | set to 1, if the client sends the first subscribe in sequence to trigger the sending of initial events. Set to 0 otherwise |
| Reserved2   | set to three 0 bits |
| Counter   | used to differentiate between parallel subscribes to the same   eventgroup of the same service (only difference in endpoint). If not used, set to 0x0.|
![Wireshark output for SubscribeEventgroup()](images/SubscribeEG_Wireshark.png)


#### 3.1.4.2 Subscribe Eventgroup Acknowledgement (Subscribe Eventgroup Ack) Entry
* The Subscribe Eventgroup Acknowledgment entry type is used to indicate that Subscribe Eventgroup entry was accepted.
* Type: 0x07 (SubscribeEventgroupAck).
* Service ID, Instance ID, Major Version, Eventgroup ID, Counter, and Reserved are same value as in the Subscribe that is being answered.
![Wireshark output for SubscribeEventgroupAck()](images/SubscribeEgAck_Wireshark.png)


### 3.1.5 Service Discovery Communication Behavior
* SOME/IP Service Discovery reduces the number of Service Discovery messages by packing entries together whenever possible.

#### 3.1.5.1 Startup Behavior
* For each Service Instance or Eventgroup the Service Discovery have at least these three phases in regard to sending entries:
  * Initial Wait Phase
  * Repetition Phase
  * Main Phase

* The service discovery enters the InitialWait Phase for a client service instance when the link on the interface needed for this service instance is up and the client service is requested by the application.
* The service discovery enters the Initial Wait Phase for a server service instance when the link on the interface needed for this service instance is up and the server service is available.
* The Service Discovery waits based on the INITIAL_DELAY after entering the Initial Wait Phase and before sending the first messages for the Service Instance.
* INITIAL_DELAY defined as a minimum and a maximum delay.

```{.xml}
    <INITIAL-DELAY-MAX-VALUE>0.1</INITIAL-DELAY-MAX-VALUE>
    <INITIAL-DELAY-MIN-VALUE>0.01</INITIAL-DELAY-MIN-VALUE>
```
* The wait time will be determined by choosing a random value between the minimum and maximum of INITIAL_DELAY.
* After sending the first message the Repetition Phase of this Service Instance/these Service Instances is entered.
* The Service Discovery waits in the Repetitions Phase based on REPETITIONS_BASE_DELAY.
* After each message sent in the Repetition Phase the delay is doubled.
* The Service Discovery sends out only up to REPETITIONS_MAX entries during the Repetition Phase.

```{.xml}
    <INITIAL-REPETITIONS-BASE-DELAY>0.2</INITIAL-REPETITIONS-BASE-DELAY>
    <INITIAL-REPETITIONS-MAX>3</INITIAL-REPETITIONS-MAX>
```
* After the Repetition Phase the Main Phase is being entered for a Service Instance.
* After entering the Main Phase, the provider waits 1*CYCLIC_OFFER_DELAY before sending the first offer entry message.
* In the Main Phase Offer Messages is sent cyclically if a CYCLIC_OFFER_DELAY is configured.
* After a message for a specific Service Instance the Service Discovery waits for 1*CYCLIC_OFFER_DELAY before sending the next message
for this Service Instance.
```{.xml}
    <OFFER-CYCLIC-DELAY>2.0</OFFER-CYCLIC-DELAY>
```
* Example:
* A. Initial Wait Phase:
  * 1. Wait for random_delay in Range(INITIAL_DELAY_MIN, _MAX) 
  * 2. Send message (Find Service and Offer Service entries)
* B. Repetition Phase (REPETITIONS_BASE_DELAY=100ms, REPETITIONS_MAX=2):
  * 1. Wait 2^0 * 100ms
  * Send message (Find Service and Offer Service entries)
  * Wait 2^1 * 100ms
  * Send message (Find Service and Offer Service entries)
* C. Main Phase (as long message is active and CYCLIC_OFFER_DELAY is defined):
  * Wait CYCLIC_OFFER_DELAY
  * Send message (Offer Service entries)
  
#### 3.1.5.2 Server Answer Behavior
* The Service Discovery delay answers to entries that were received in multicast SOME/IP-SD messages using the configuration item REQUEST_RESPONSE_DELAY.
* The REQUEST_RESPONSE_DELAY will not apply if unicast messages are answered with unicast messages.
* REQUEST_RESPONSE_DELAY will be specified by a minimum and a maximum.
* The actual delay is randomly chosen between minimum and maximum of REQUEST_RESPONSE_DELAY.
```{.xml}
    <REQUEST-RESPONSE-DELAY>
      <MAX-VALUE>1.5</MAX-VALUE>
      <MIN-VALUE>1.5</MIN-VALUE>
    </REQUEST-RESPONSE-DELAY>
```

#### 3.1.5.3 Shutdown Behavior
* When a server service instance of an ECU is in the Repetition and Main Phase and is being stopped, a Stop Offer Service entry will be sent out.
* When a server sends out a Stop Offer Service entry all subscriptions for this service instance get deleted on the server side.\
* When the whole ECUs is being shut down Stop Offer Service entries will be sent out for all service entries and Stop Subscribe Eventgroup entries for Eventgroups.
* TTL(Time to live) : Describes the lifetime of the entry in seconds

```{.xml}
     <SERVICE-OFFER-TIME-TO-LIVE>10</SERVICE-OFFER-TIME-TO-LIVE>
```
### 3.1.6 Generation of vsomeip.json
* In case of adaptive platform, vsomeip.json will get generated from system_manifest.arxml file by using jsongen.py scripts.

![Generated Output(vsomeip.json)](images/jsongen.png)
