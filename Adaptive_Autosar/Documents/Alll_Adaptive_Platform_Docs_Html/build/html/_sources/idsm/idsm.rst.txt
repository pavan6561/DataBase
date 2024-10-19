**Intrusion Detection System Manager (IDSM)**
===============================================

Introduction
##############

A number of special challenges apply to automotive IDS. For one, the E/E architecture of vehicles is a distributed system. Many familiar vehicle functions can only be implemented in the interplay of two or more ECUs. This property has effects on a potential onboard IDS architecture. Since no central node has knowledge of all security events on other nodes, the IDS itself must be a distributed system.

A distributed onboard IDS is that it avoids a single point of failure. Detection algorithms – known as “sensors” – are implemented in the ECU software. For one, they detect protocol violations that occur as the result of hacker attacks. If the security sensors on one ECU fail, the rest of the IDS should continue to operate properly with practically no limitations. 

General Architecture of a distributed onboard IDS
######################################################

.. figure:: resources/Architecture_of_a_distributed_onboard_IDS.png
   :class: with-border
   :align: center

   **Architecture of a distributed onboard IDS**

Functionality of IDSM
#########################

Reporting Mode
-----------------

IdsM shall determine the default reporting mode of every reported SEv from the SecXT model parameter SecurityEventContextProps.defaultReportingMode.

Reporting Mode Options:
^^^^^^^^^^^^^^^^^^^^^^^^^^

    - OFF
    - BRIEF
    - DETAILED
    - BRIEF_BYPASSING_FILTERS
    - DETAILED_BYPASSING_FILTERS

Reconfiguration of the reporting mode via diagnostic access is possible.

IdsM standardizes a DID for reading and changing the reporting mode of events during runtime.

API to change the reporting mode at runtime

    - GetReportingMode(SecurityEventDefinition.id)
    - SetReportingMode(SecurityEventDefinition.id, ReportingMode)

Filter Chain
--------------

The IdsM shall support the qualification of SEv by applying a configurable chain of filters to the reported SEv. One configured sequence of filters is called Filter Chain.

Filter chains are configured using the SecXT model element SecurityEventFilterChain.

Filter chain is evaluated after evaluating the reporting mode.

When a SEv is reported, the IdsM shall apply the filter chain that is mapped to the SecurityEventDefinition of the reported SEv via the SecurityEventContextMapping.

Types of Filter 
-------------------

1. MachineState Filter ​

2. Forward-Every-nth Filter​

3. Aggregation Filter​

4. Threshold Filter

IdsM shall evaluate all activated filter in the order MachineState Filter, Forward-Every-nth Filter, Aggregation Filter, Threshold Filter

Machine State Filter & Sampling Filter​
---------------------------------------

IdsM evaluates the current machine state equals one of the states referenced by SecurityEventStateFilter.blacklistedState, then IdsM shall drop the SEv.

IdsM evaluates the sampling filter for a SEv, IdsM shall drop all the SEvs but every n-th per SecurityEventDefinition, where n is defined by SecurityEventOneEveryNFilter.n.

Aggregation Filter
--------------------

All SEv of a given type occurring within a configured time interval are aggregated into one SEv with an additional counter information attached that indicates how often the event occurred in the time interval.

The aggregation filter shall not forward (i.e., to the next filter) any incoming SEv during the aggregation interval.

Aggregation of events can reduce resource consumption for SEvs occurring at a high frequency, while maintaining information on the total number of event occurrences.

Threshold Filter
------------------

**Event Dropping Below Threshold:**

The threshold filter shall drop an SEv of given type if the sum of count parameters of all SEvs of given type that were processed by the threshold filter in the current threshold interval is smaller than the configured parameter SecurityEventThresholdFilter.thresholdNumber.

**Event Forwarding Above Threshold:**

The threshold filter shall forward an SEv of given type if the sum of count parameters of all SEvs of given type that were processed by the threshold filter in the current threshold interval is equal to or greater than the configured parameter SecurityEventThresholdFilter.thresholdNumber.

Timestamp
-----------

Timestamps are optional and can be provided to the IdsM in different ways.​

Timestamps are optional:

    - IdsmInstance.timestampSupport is set to false, IdsM shall not add a timestamp to a QSEv and shall ignore.

Timestamps provided by the stack:

    - If IdsmInstance.timestampSupport is set to true, IdsmInstance.timestampFormat equals "‘AUTOSAR"’, and the ReportEvent() function is called without a timestamp parameter, then Idsm shall add a timestamp from the TimeSync::TimeBaseResource referenced as IdsPlatformInstantiation.timeBase.
    - If IdsmInstance.timestampSupport is set to true and the ReportEvent() function is called with a timestamp parameter, then Idsm shall use this provided timestamp.

Timestamp provided via event reporting interface:​

    - If IdsmInstance.timestampSupport is set to true and the ReportEvent() function is called with a timestamp parameter, then Idsm shall use this provided timestamp.

Timestamp provided via application software:​

    - If IdsmInstance.timestampSupport is set to true, IdsmInstance.timestampFormat does not equal "‘AUTOSAR"’, and the ReportEvent() function is called without a timestamp parameter, then IdsM shall add a timestamp that is provided by a application software through the TimestampProvider callback to the QSEv.

Authenticity
--------------

IdsM can optionally protect the authenticity of transmitted QSEvs using cryptographic signatures.

**Signing QSEv:**

    - If an IdsmSignatureSupportAp is aggregated at the IdsmInstance in the role signatureSupportAp, then IdsM shall attach a cryptographic signature to each QSEv transmitted to the IdsR and to each locally persisted QSEv.

**Primitive and Key:**

    - IdsM shall use the signing algorithm specified in the parameter IdsmSignatureSupportAp.cryptoPrimitive and the key identified by the CryptoKeySlot that is referenced by IdsmSignatureSupportAp in the role keySlot.

Rate & Traffic Limitation
----------------------------

Before sending a QSEv to the IdsR or persisting a QSEv, IdsM shall apply rate and traffic limitation that can lead to dropping the QSEv.

**Rate Limitation:** IdsM shall drop an QSEv from transmission, if its transmission would cause the number of QSEvs transmitted in the current interval, which is specified in IdsmRateLimitation.timeInterval, to exceed the maximum number of transmission configured as IdsmRateLimitation. maxEventsInInterval.

**Traffic Limitation:** IdsM shall drop an QSEv from transmission, if its transmission would cause the number of bytes transmitted in the current interval, which is specified in IdsmTrafficLimitation.timeInterval, to exceed the maximum number of bytes configured as IdsmTrafficLimitation.maxBytesInInterval.

Configuration Methodology
##############################

IdsM Instantiation
--------------------

The definition of the deployment for the Intrusion Detection System Manager (IdsM) is modeled by means of the meta-class IdsmModuleInstantiation.

On each Machine, only one instance of the Intrusion Detection System Manager (modeled by IdsmModuleInstantiation) shall exist.

This instance manages all the reported SEvs created by SWCLs or Function Clusters on this Adaptive Machine.

.. figure:: resources/Specification_of_the_IdsmModuleInstantiation.png
   :class: with-border
   :align: center

   **Specification of the IdsmModuleInstantiation**

Deployment for Security Events
--------------------------------

The security events that are actually reported by a local IdsM are represented by meta-class SecurityEventMapping, referenced by IdsmModuleInstantiation in the role reportableSecurityEvent. 

.. figure:: resources/Modeling_of_between_a_SecurityEventMapping.png
   :class: with-border
   :align: center

   **Modeling of a SecurityEventMapping** 

Security Event Report Interface
----------------------------------

On the AUTOSAR adaptive platform, a dedicated PortInterface for the interaction of application-layer software with the AUTOSAR Intrusion Detection System
Manager is defined.

The name of this sub-class of abstract meta-class PortInterface is SecurityEventReportInterface.

Each RPortPrototype typed by a SecurityEventReportInterface is able to report exactly one security event.

The modeling of the association between a specific security event and the corresponding RPortPrototype typed by a SecurityEventReportInterface is created by means of the SecurityEventReportToSecurityEventDefinitionMapping.

This meta-class maps the RPortPrototype to a SecurityEventDefinition that itself is part of the so-called Security Extract.

.. figure:: resources/Specification_of_the_SecurityEventReportInterface_and_SecurityEventReportToSecurityEventDefinitionMapping.png
   :class: with-border
   :align: center

   **Specification of the SecurityEventReportInterface and SecurityEventReportToSecurityEventDefinitionMapping** 

Obtaining custom Time Stamps for Security Events
---------------------------------------------------

One of the tasks of an IdsM is to obtain a custom time stamp that corresponds to reported security events under certain circumstances (which are not relevant for the sake of discussing the modeling).

Time stamps can be obtained from sub-classes of the abstract base-class TimeBaseResource. Therefore, IdsmModuleInstantiation maintains a reference to
TimeBaseResource.

.. figure:: resources/Creating_an_associsation_between_a_IdsmModuleInstantiation_and_a_TimeBaseResource.png
   :class: with-border
   :align: center

   **Creating an association between a IdsmModuleInstantiation and a TimeBaseResource**
