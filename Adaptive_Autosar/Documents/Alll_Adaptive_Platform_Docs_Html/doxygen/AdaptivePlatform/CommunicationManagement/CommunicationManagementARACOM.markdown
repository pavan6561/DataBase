# ARA Com API {#adaptive_communicationmanagement_aracom}

* Existing communication middleware API/technology are listed below:

  * **ROS(Robot Operating System) API**
  
  * **DDS(Data Distribution Service) API**
  
  * **CommonAPI(GENIVI)**
  
  * **DADDY API(Bosch)**

* Decision to come up with a new and AUTOSAR-specific Communication Management API was due to the fact, that not all of the key requirements were met by existing solutions:
  * Need for a Communication Management, which is NOT bound to a concrete network communication protocol. It has to support the SOME/IP protocol along with the flexibility to exchange that.
  * The AUTOSAR service model, which defines services as a collection of provided methods, events and fields are supported naturally/straight forward.
  * The API supports an event-driven and a polling model to get access to communicated data equally . The latter is needed by real-time applications to avoid unnecessary context switches, while the former is much more convenient for applications without real-time requirements.
  * Possibility for seamless integration of end-to-end protection to fulfill ASIL requirements.
  * Support for static (preconfigured) and dynamic (runtime) selection of service instances to communicate with.
  
>  **ara::com only defines the API signatures and its behaviour visible to the application developer. Providing an implementation of those APIs and the underlying middleware transport layer is the responsibility of the AUTOSAR AP vendor.**
  
  
  
# 1. High Level API Structure

## 1.1 Proxy/Skeleton Architecture
* The basic idea of this pattern is, that from a formal service definition two code artifacts are generated:
![Proxy Skeleton Pattern](images/proxy_skeleton_pattern.png)
  * **Service Proxy**: This code is from the perspective of the service consumer, which wants to use a possibly remote service. Service proxy represents this service on code level. 
  
  * **Service Skeleton**: This code is from the perspective of the service implementation, which provides functionalities according to the service definition - Connects the service implementation to the Communication Management transport layer, so that the service implementation can be contacted by distributed service consumers.

  
## 1.2 Datatype Abstractions
* ara::com API introduces specific datatypes, which are used throughout its various interfaces. They can roughly be divided into the following classes:
  * **Pointer types:** for pointers to data transmitted via middleware
  * **Collection types:** for collections of data transmitted via middleware
  * **Types for async operation result management:** ara::com specific versions of C++ std::future and std::promise
  * **Function wrappers:** for various application side callback or handler functions to be called by the middleware
  
## 1.3 Error Notification
* ara::com API uses C++ exceptions to notify the user of the API about any errors occurred.

### 1.3.1 Checked Exceptions
* Checked Exceptions within ara::com API can only occur in the context of a call of a service interface method

### 1.3.2 Unchecked Exceptions
* Unchecked Exceptions within ara::com API can basically occur in the context of any ara::com API call and are fully ara::com implementation specific! A property of unchecked exceptions is, that programs/program artifacts hitting such an exception can typically not recover from it.

# 2. API Elements
* A virtual service (interface) called "RadarService is defined in the below example.

![RadarService Definition](images/RadarService_Definition.png)

* So the example service RadarService provides an event called “BrakeEvent”, which consists of a structure containing a flag and a variable length array of uint8 (as extra payload). A field “UpdateRate”, which is of type uint32 (supports get and set calls) and  methods Adjust, Calibrate and LogCurrentState.

* Method “Adjust” is used to position the radar. It contains a target position as in-parameter and two out-parameters. One to signal the success of the positioning and other to report the final (maybe deviating) effective position. 
The method “Calibrate”  is used to calibrate the radar. It contains a configuration string as in-parameter and returning a success indicator as an out-parameter. "Calibrate" method may raise two different application errors, in case of the calibration failure: “CalibrationFailed” and “InvalidConfigString”. The method “LogCurrentState” is a one way method, which means, no feedback is returned to the caller. It instructs the service RadarService to output its current state into its local log files.


## 2.1 Common Types
### 2.1.1 Optional data elements
* The structure in below Figure has the optional declared elements "current and health". These elements are not mandatorily present. The consuming application has to check whether the optional elements contain a value or not during runtime. 
Optional element contains a may or may not contain a value depends on the providing application. The providing application may or may not set the value for that specific instance. The feature of optional contained elements provide forward and backward compatibility of the service interface as new added record elements can just be ignored by old applications.

    struct BatteryState {
      Voltage_t voltage;
      Temperature_t temperature;
      ara::com::optional<Current_t> current;
      ara::com::optional<Health> health;
    };

### 2.1.2 Instance Identifier
* Instance identifiers are used within ara::com during following scenarios: 
  - On client/proxy side when a specific instance of a service will be searched for.
  - At the server/skeleton side when a specific instance of a service is created.
* At ara::com API level the instance identifier is a technical binding specific identifier. The content of instance identifier are totally technology specific.
* Example:  SOME/IP is using 16 bit unsigned integer, while DDS (DDS-RPC) uses string<256> as service_instance_name.
* Independant of the binding technology, instance identifier complying to a signature given below.
```{.cpp}
  class InstanceIdentifier {
  public:
  static const InstanceIdentifier Any;
  
  explicit InstanceIdentifier(std::string value);
  std::string toString() const;
  bool operator==(const InstanceIdentifier& other) const;
  bool operator<(const InstanceIdentifier& other) const;
  InstanceIdentifier& operator=(const InstanceIdentifier& other);
  
};
```
* An ara:com provided functionality, which translates a logical local name used by the software developer in his realm into the technology/binding specific ara::com::InstanceIdentifier.
* The local name from which the ara::com::InstanceIdentifier is constructed comes basically from AUTOSAR meta-model, describing your software component model.
* The requirement for this local name(abstract identifier) is, that it is unambiguous within an executable. It has basically the form:

```{.cpp}
<context 0>::<context 1>::...::<context N>::<port name>
```
If the unambiguousness is ensured, the integrator/deployer can assign a dedicated technical binding with its specific instance ids to those abstract identifiers via a "manifest file", which is specifically used for a distinct instantiation/execution of the executable.
* This explicitly allows, to start the same executable N times, each time with a different manifest, which maps the same abstract identifier differently.
* The API ara:com provides, to do the translation from the abstract identifier (local name in the software developers realm) to the technical ara::com::InstanceIdentifier, is the following:

```{.cpp}
std::vector<InstanceIdentifier> ResolveInstanceIdentifiers(ara::meta::ModelIdentifier modelName);
```
* API does return a list of ara::com::InstanceIdentifier
* AUTOSAR supports, that the integrator may configure multiple technical bindings behind one abstract identifier visible to the software component developer.This feature is called multi-binding.
* Using multi-binding on the skeleton/ server side is a common use case, since it simply allows different clients to use
their preferred binding, when contacting the server.
* On the Contrary using multi-binding on the proxy/client side is a rather exotic one. E.g. it could be used to support some failover approaches (if binding A does not work, fall back on binding B).
* The possible returns for a call of ResolveInstanceIdentifiers() are:
  * empty list: The integrator failed to provide a mapping for the abstract identifier. This most likely is a configuration error.
  * list with one element: The common case. No multi-binding.
  
## 2.2 Proxy Class
* ara::com does standardization of the interface of the generated Proxy class.
>  **Note:The interfaces , Proxy class has to provide are defined by ara::com, a generic (product independent) generator could generate an abstract class or a mock class against which the application developer could implement the service consumer application. This perfectly suits the platform vendor independent development of Adaptive AUTOSAR SWCs.**
* ara::com expects proxy related artifacts inside a namespace "proxy". This namespace is typically included in a namespace hierarchy deduced from the service definition and its context.
```{.cpp}
    class RadarServiceProxy {
      public:
      /* 
       * brief Implementation is platform vendor specific
       * A HandleType must contain the information that is needed to create a
         proxy.
       * This information will be hidden.
       * Since the platform vendor is responsible for creation of handles, the
       * ctor signature is not given as it is not of interest to the user.
       */
      
      class HandleType {
      /*
       * brief Two ServiceHandles are considered equal if they represent the
         same service instance.
       * param other
       * return
       */
      inline bool operator==(const HandleType &other) const;
      const ara::com::InstanceIdentifier &GetInstanceId() const;
      };
      
      /*
       * StartFindService does not need an explicit version parameter as this
         is internally available in ProxyClass
       * That means only compatible services are returned.
       *
       * param handler this handler gets called any time the service
         availability of the services matching the given
       * instance criteria changes. If you use this variant of FindService,
         the Communication Management has to
       * continuously monitor the availability of the services and call the
         handler on any change.
       *
       * param instance which instance of the service type defined by T will
         be searched/found. Wildcards may be given.
       * Default value is wildcard.
       *
       * return a handle for this search/find request, which will be used to 
         stop the availability monitoring and related
       * firing of the given handler. (\see StopFindService())
       **/
      static ara::com::FindServiceHandle StartFindService(
      ara::com::FindServiceHandler<RadarServiceProxy::HandleType> handler,
      ara::com::InstanceIdentifier instance =
      ara::com::InstanceIdentifier::Any);
      
      /*
       * Method to stop finding service request (see above)
       */
      static void StopFindService(ara::com::FindServiceHandle handle);
      
      /*
       * Opposed to StartFindService(handler, instance) this version is a "oneshot"
         find request, which is
       * - synchronous, i.e. it returns after the find has been done and a
        result list of matching service instances is
       * available. (list may be empty, if no matching service instances 
         currently exist)
       * - does reflect the availability at the time of the method call. No
        further (background) checks of availability
       * are done.
       *
       * param instance which instance of the service type defined by T will
         searched/found. Wildcards may be given.
       * Default value is wildcard.
       */
      static ara::com::ServiceHandleContainer<RadarServiceProxy::HandleType>
      FindService(
      ara::com::InstanceIdentifier instance =
      ara::com::InstanceIdentifier::Any);
      
      /*
       * brief The proxy can only be created using a specific handle which
        identifies a service.
       *
       * This handle can be a known value which is defined at deployment or it 
         can be obtained using the ProxyClass::FindService method.
       *
       * param handle The identification of the service the proxy should represent.
       */
      explicit RadarServiceProxy(HandleType &handle);
      
      /*
       * brief Public member for the BrakeEvent
       */
      events::BrakeEvent BrakeEvent;
      
      /*
       * brief Public Field for UpdateRate
       */
      fields::UpdateRate UpdateRate;
      
      /*
       * brief Public member for the Calibrate method
       */
      methods::Calibrate Calibrate;
      
      /*
       * brief Public member for the Adjust method
       */
      methods::Adjust Adjust;
      
      /*
       * brief Public member for the LogCurrentState fire-and-forget method
       */
      methods::LogCurrentState LogCurrentState;
    };
```


### 2.2.1 Constructor and Handle Concept
* As seen in the above code, ara::com prescribes the Proxy class to provide a constructor. This means, that the developer is responsible for creating a proxy instance to communicate with a possibly remote service. The ctor takes a parameter of type RadarServiceProxy::HandleType — an inner class of the generated proxy class.
* After the call to the ctor, there is proxy instance, which allows to communicate with the service. Therefore the handle has to contain the needed addressing information, so that the Communication Management binding implementation is able to contact the service.


### 2.2.2 Finding Services
* The Proxy class provides class (static) methods to find service instances, which are compatible with the Proxy class.
* Since the availability of service instances is dynamic by nature, as they have a life cycle, ara::com provides two different ways to do a ‘FindService’ for convenience:
  * StartFindService is a class method, which starts a continuous ‘FindService’ activity in the background, which notifies the caller via a given callback anytime the availability of instances of the service changes.
  * FindService is a one-off call, which returns available instances at the point in time of the call.
* Both of those methods have the instance parameter in common, which allows to either search for an explicit instance of the service or any instance (which is the default parameter value). The synchronous one-off variant FindService returns a container of handles for the matching service instances, which might also be empty, if no matching service instance is currently available. Opposed to that, the
StartFindService returns a FindServiceHandle, which can be used to stop the ongoing background activity of monitoring service instance availability via call to StopFindService. The first (and specific for this variant) parameter to StartFindService is a user provided handler function with the following signature:

```{.cpp}
using FindServiceHandler = std::function<void(ServiceHandleContainer<T>)>;
```

#### 2.2.2.1 Auto Update Proxy instance
* Irrespective of the usage of one-off FindService or the StartFindService variant, in both cases a handle identifying the — possibly remote — service instance is received, from which proxy instance is created. 

* In the service based communication universe it is expected, that during the life time of the entire system (e.g. vehicle) service provider and consumer instances start up and go down frequently due to their own life cycle concepts.
To deal with this, there is service discovery infrastructure, where the life cycle of service providers and consumers is monitored in terms of service offerings and service (re)subscriptions. If a service consumer application has instantiated a service proxy instance from a handle returned from some of the Find Service variants, the following sequence might possibly occur:


![Auto Updating of Proxy Instance](images/AutoUpdatingProxyInstance.png)

* Explanation of above figure:
  * **T0**: The service consumer may successfully call a service method of that proxy and GetSubscriptionState() on subscribed events will return
  kSubscribed.
  * **T1**: The service instance goes down, correctly notified via service discovery.
  * **T2**: A call of a service method on that proxy will lead to an exception, since the targeted service instance of the call does not exist anymore. Correspondingly GetSubscriptionState() on any subscribed event will return kSubscriptionPending at this point even if the event has been successfully subscribed (kSubscribed) before.
  * **T3**: The service instance comes up again, notified via service discovery infrastructure. The CM at the proxy side will be notified and will silently update the proxy object instance with a possibly changed transport layer addressing information.
  * **T4**:Consequently service method calls on that proxy instance will succeed again and GetSubscriptionState() on events which the service consumer had subscribed before, will return kSubscribed again.

### 2.2.3 Events
* For each event the remote service provides, the proxy class containing a member of a event specific wrapper class.
* In the example the member has the name BrakeEvent and is of type events::BrakeEvent. 
* Below mentioned method gets generated for each event.

| Method                                                                           | Description                            |
| :----                                                                            | :-----------                          |
| void Subscribe(ara::com::EventCacheUpdatePolicy policy, size_t cacheSize);       |  Used for subscription of events                 |
| ara::com::SubscriptionState GetSubscriptionState() const;                        |  Query current subscription state                                      |
| void Unsubscribe();                                                              |  Unsubscribe from the service                                       |
| void SetReceiveHandler(ara::com::EventReceiveHandler handler);                   |  Setting a receive handler signals the Communication Management implementation to use event style mode.           |
| void UnsetReceiveHandler();                                                      |  Remove handler set by SetReceiveHandler                                      |
| void  SetSubscriptionStateChangeHandler(ara::com:: SubscriptionStateChangeHandler handler); |  Setting a subscription state change handler, which gets called by the Communication Management implementation as soon as the subscription state of this event has hanged.                           |
| void UnsetSubscriptionStateChangeHandler();                                      | Remove handler set by SetSubscriptionStateChangeHandler                                       |
| bool Update(ara::com::FilterFunction<SampleType> filter = {});                    | Fetch data from the Communication Management buffers and apply filter before writing the samples into the cache.   |
| const ara::com::SampleContainer<ara::com::SamplePtr<const SampleType>> & GetCachedSamples() const; | Get the container of the samples in the cache that was updated by the last call of upadate   |
| void Cleanup(); | Explicitly clean the application local cache   |
                                                                    

```{.cpp}
   class BrakeEvent {
      /*
      * brief Shortcut for the events data type.
      */
      using SampleType = RadarObjects;
      
      /*
      * brief The application expects the Communication Management to
        subscribe the event.
      *
      * The Communication Management will try to subscribe and resubscribe
        until \see
      * Unsubscribe() is called explicitly.
      * The error handling will be kept within the Communication Management.
      *
      * The function return immediately. If the user wants to get notified,
        when subscription has succeeded, he needs
      * to register a handler via \see SetSubscriptionStateChangeHandler().
        This handler gets then called after
      * subscription was successful.
      *
      * \param policy Defines the update policy for the application local cache.
      * \param cacheSize Defines the size of the application local cache.
      *
      */
      void Subscribe(ara::com::EventCacheUpdatePolicy policy, size_t cacheSize);
      
      /*
      * brief query current subscription state.
      *
      * \return current state of the subscription.
      */
      ara::com::SubscriptionState GetSubscriptionState() const;
      
      /*
      * brief Unsubscribe from the service.
      */
      void Unsubscribe();
      
      /*
      * Setting a receive handler signals the Communication Management 
        implementation to use event style mode.
      * I.e. the registered handler gets called asynchronously by the
        Communication Management as soon as new event data
      * arrives for that event. If user wants to have strict polling behaviour,
        where you decide when to check for
      * new data via Update() he should NOT register a handler.
      * Handler may be overwritten anytime during runtime.
      *
      * Provided Handler needs not to be re-entrant since the Communication
        Management implementation has to serialize calls
      * to the handler: Handler gets called once by the MW, when new events
        arrived since the last call to Update().
      * When application calls Update() again in the context of the receive
        handler, MW must - in case new events arrived
      * in the meantime - defer next call to receive handler until after the
        previous call to receive handler has been
      * completed.
      *
      **/
      void SetReceiveHandler(ara::com::EventReceiveHandler handler);
      
      /*
      * Remove handler set by SetReceiveHandler()
      **/
      void UnsetReceiveHandler();
      
      /*
      * Setting a subscription state change handler, which gets called by
        the Communication Management
      * implementation as soon as the subscription state of this event has
        changed.
      * Communication Management implementation will serialize calls to the
        registered handler. If multiple
      * changes of the subscription state take place during the runtime of a
        previous call to a handler, the
      * Communication Management aggregates all changes to one call with the
        last/effective state.
      * Handler may be overwritten during runtime.
      *
      **/
      void  SetSubscriptionStateChangeHandler(ara::com:: SubscriptionStateChangeHandler   handler);
      
      /*
      * Remove handler set by SetSubscriptionStateChangeHandler()
      **/
      void UnsetSubscriptionStateChangeHandler();
      
      /*
      * brief Fetch data from the Communication Management buffers and apply
        filter before
      * writing the samples into the cache.
      *
      * \pre BrakeEvent::Subscribe has been called before (and not be
        withdrawn by BrakeEvent::Unsubscribe)
      *
      * \param filter
      * parblock
      * FilterFunction for the samples.
      *
      * This filter will be applied to the deserialized data within the
        context of
      * the update this function should return true if the sample will be
        added to
      * the cache.
      * parblockend
      *
      * \return True if new values received and those values are NOT ALL
        filtered out,
      * else false.
      *
      * \note If precondition is not fulfilled, middleware impl might raise an
        unchecked exception
      * (e.g. NotSubscribed).
      */
      bool Update(ara::com::FilterFunction<SampleType> filter = {});
      /*
      * brief Get the container of the samples in the cache that was updated
        by the last call
      * of \see update.
      *
      * The container and referenced data is expected to be stable until
        update is
      * called again.
      *
      * \return Container of SamplePtr
      */
      const ara::com::SampleContainer<ara::com::SamplePtr<const SampleType>> &
      GetCachedSamples() const;
      /*
      * brief Explicitly clean the application local cache.
      *
      * This should free the references to the data samples which are owned by
        the
      * Communication Management.
      * This method only has an effect if policy in the call to \see Subscribe
        has been
       * set to kNewestN!
       */
       void Cleanup();
    };
```
#### 2.2.3.1 Event Subscription and Cache Semantics
* The mere fact, that there exists a member of the event wrapper class inside the proxy instance does not mean, that the user gets instant access to events raised/sent out by service instance. First ‘subscribe’ for the event, in order to tell the Communication Management, that the application is now interested in receiving events. For that purpose the event wrapper class of ara::com provides below method:


```{.cpp}
  /*
   * brief The application expects the Communication Management to
      subscribe the event.
   *
   * ....
   *
   * \param policy Defines the update policy for the application local
     cache.
   * \param cacheSize Defines the size of the application local cache.
   *
   */
    void Subscribe(ara::com::EventCacheUpdatePolicy policy, size_t cacheSize);
```
* This method expects two parameters, policy and cacheSize.


##### 2.2.3.1.1 policy
* With this parameter you control how the cache is updated if you call the Update method. Currently ara::com support two policies:
* **kLastN:** With this policy new available events are put in the cache by each call of Update. If they do not fit in the cache, older entries (oldest first) are displaced. With this policy the following applies: If the cache has been filled with a certain amount of events, the amount can only remain constant (if no new event has arrived) or get bigger with upcoming Update calls (if new events have been arrived).

* **kNewestN:** With this policy in each update the cache gets cleared and then filled with the newest arrived events. Even if NO event has arrived since the last call to Update, the cache gets cleared/emptied.

##### 2.2.3.1.2 cacheSize
* With calling the method, you not only tell the Communication Management, that you now are interested in receiving
event updates, but you are at the same time setting up a local cache for those events bound to the event wrapper instance with the given cacheSize.
* The idea behind this is very simple and possible uses are:
  * **stability**
  * **interpolation/averaging between a number of events**
  * **rate adoption**
* Stability is realized with this concept by having an explicit method (see below) to update event data inside the cache. So this local cache decouples the event-wrapper instance from the Communication Management buffers into which the service instance may send its event updates! Updates of the cache status/content is only done explicitly by the user by calling Update.

#### 2.2.3.2 Monitoring Event Subscription
* The call to the Subscribe method is asynchronous by nature. This means that at the point in time Subscribe returns, it is just the indication, that the Communication Management has accepted the order to care for subscription.
* If the user needs to monitor the subscription state, he has two possibilities:
  * **Polling via GetSubscriptionState**
  * **Registering a handler, which gets called, when the subscription state changes**

##### 2.2.3.2.1 Polling via GetSubscriptionState
* If the user — after having called Subscribe — wants to get feedback about the success of the subscription, he might call:
```{.cpp}
    /*
     * brief query current subscription state.
     *
     * \return current state of the subscription.
     */
    ara::com::SubscriptionState GetSubscriptionState() const;
```

##### 2.2.3.2.1 Registering a handler, which gets called, when the subscription state changes
* User register a handler function, for subscription state changes.
```{.cpp}
    /*
     * Setting a subscription state change handler, which gets called by
       the Communication Management implementation as soon
     * as the subscription state of this event has changed.
     * Handler may be overwritten during runtime.
     *
     **/
     void SetSubscriptionStateChangeHandler(ara::com::SubscriptionStateChangeHandler handler);
```
* Anytime the subscription state changes, the Communication Management implementation calls the registered handler.

#### 2.2.3.3 Event-Driven vs Polling-Based access
* **Polling-Based access** 
  * An explicit interaction needed with ara::com API by calling Update to fill the local event wrapper specific cache:
  ```{.cpp}
      /*
      * brief Fetch data from the Communication Management buffers and apply filter before
      * writing the samples into the cache.
      *
      * \pre BrakeEvent::Subscribe has been called before (and not be withdrawn by BrakeEvent::Unsubscribe)
      * \param filter
      *  parblock
      * FilterFunction for the samples.
      *
      * This filter will be applied to the deserialized data within the context of
      * the update this function should return true if the sample is added to
      * the cache.
      * parblockend
      *
      * \return True if new values received and those values are NOT ALL filtered out,
      * else false.
      *
      * \note If precondition is not fulfilled, middleware impl might raise an unchecked exception
      * (e.g. NotSubscribed).
      */
      bool Update(ara::com::FilterFunction<SampleType> filter = {});
  ```
  
  * The method takes an optional parameter of a user defined filter function.
  ```{.cpp}
  using FilterFunction = std::function<bool(const S& sample)>;
  ```
  * the user provided filter function gets an event (sample) and has to return true after checking, if he wants the sample to be put into the local cache.
  * During Update(), the Communication Management calls the filter function for each event, which has arrived since the last call to Update().
  * Update returns true in case new events have been placed into the local cache by the call, false otherwise.
  * After filling event specific local cache with event-data via Update() user typically want access those events. This is done with the following API:
  ```{.cpp}
      /*
       * brief Get the container of the samples in the cache that was updated by the last call
       * of \see update.
       *
       * The container and referenced data is expected to be stable until update is
       * called again.
       *
       * \return Container of SamplePtr
       */
       const ara::com::SampleContainer<ara::com::SamplePtr<const SampleType>> &GetCachedSamples() const;
  ```   
  
  * User can call this method as many times as you want — as long as user do not call Update() in between, the returned collection will always be the same/stable, even if the service side has send out several new events.
    
* **Event-Driven**
* If user explicitly want the Communication Management to notify application thereby issuing asynchronous context switches to application process by below API.

```{.cpp}
void SetReceiveHandler(ara::com::EventReceiveHandler handler);
```
* This API allows user to register a user defined callback, which the Communication Management has to call in case new event data is available since the last call to Update().

#### 2.2.3.4 Buffering Strategies
* The following figure sketches a simple deployment, where we have a service providing an event, for which two different local adaptive SWCs have subscribed through their respective ara::com proxies/event wrappers.
* Both proxies have a local event cache. This is the cache, which gets filled via Update().
* Service implementation sends its event data to a Communication Management buffer, which is apparently outside the process space of the service implementation

![Event Buffering Approaches](images/EventBufferingApproaches.png)
* This buffer is owned by kernel or it is realized as a shared memory between communicating proxies and skeleton or owned by a separate binding implementation specific ‘demon’ process.
  * **Kernel Space**
  * **Shared Memory**
  * **IPC-Demon Process**

### 2.2.4 Methods
* For each method the remote service provides, the proxy class contains a member of a method specific wrapper class.
* In our example, we have two methods and the corresponding members have the name Calibrate (of type methods::Calibrate) and Adjust (of type methods::Adjust).

![Adjust Method Class](images/AdjustMethodClass.png)
* The return value of a call to a service method, which is not a “one-way method” is an ara::com::Future, where the template parameter
is of the type of the inner struct, which aggregates all OUT-parameters of the method.

#### 2.2.4.1 One-Way or Fire-and-Forget Methods
* ara::com supports a special flavor of a method, which we call “one-way” or “fire-and-forget”.
* Technically this is a method with only IN-params — no OUT-params and no raising of errors allowed.
* The client/caller therefore gets no feedback at all, whether the server/callee has processed a “one-way” call or not.

```{.cpp}
    class LogCurrentState {
      public:
      /*
      * brief Operation will call the method.
      *
      * Using the operator the call will be made by the Communication Management.
      * It is a one-way method, so no feedback (return value/out-parameter) is given.
      */
      void operator()();
    };
```
#### 2.2.4.2 Event-Driven vs Polling access to method results

##### 2.2.4.2.1 Event-Driven
* Event-Driven means, that the caller of the method (the application with the proxy instance) gets notified by the Communication Management implementation as soon as the method call result has arrived.
* Event driven method call can be achieve with following ways:
  * **1.future.get()**
  * **2.wait(), wait_for() or wait_until()**
  * **3.Registering a callback method via then()**
  
##### 2.2.4.2.2 Polling access
* It is used in RT(real time) application.
* ara::com::Future::is_ready() is used to poll the method whether the method call has been finished.
* After ara::com::Future::is_ready() has returned true, it is guaranteed that the next call to ara::com::Future::get() will NOT block, but immediately return either the valid value or throw an exception in case of error

#### 2.2.4.3 Canceling Method Result
* There may be cases, where you already have called a service method via the ()- operator, which returned you an ara::com::Future, but you are not interested in the result anymore. 
* It could even be the case, that you already have registered a callback via ara::com::Future::then() for it. Instead of just let things go and ‘ignore’ the callback, you should tell the Communication Management explicitly. 
* This might free resources and avoid unnecessary processing load on the binding implementation level.
* Telling that you are not interested in the method call result anymore is simply done by letting the ara::com::Future go out of scope, so that its destructor gets called. 
* Call of the dtor of the ara::com::Future is a signal to the binding implementation, that any registered callback for this future should not be called anymore, reserved/allocated memory for the method call result might be freed and event waiting mechanisms for the method result should be stopped.

#### 2.2.4.4 Checked Exception Handling
* Unchecked Exceptions might be thrown by any ara::com API call, whereas Checked Exceptions might be thrown only in the context of non “one-way/fire-and-forget” service interface method calls.
* There are two types of Checked Exceptions, which might be thrown in the course of a service method call on the proxy side:
* ServiceNotAvailableException:This is a direct subclass of std::exception and will be thrown by the middleware implementation, when it detects during processing of a method call, that the providing service instance has already stopped offering the service.

* Subclasses of ara::com::ApplicationErrorException:which are defined on meta-model level for a specific service interface method as ApplicationError and which are created at the application level service provider (skeleton) side and transported to the caller (proxy) side.

* If one of those Checked Exception types happens, it will always be thrown in the context of the ara::com::Future::Get() call.
* Calibrate method may raise the two application errors CalibrationFailed and InvalidConfigString, which get transformed to subclasses
of ara::com::ApplicationErrorException on ara::com API level:
```{.cpp}
  using namespace ara::com;
  using namespace com::mycompany::division::radarservice;
  
  Future<Calibrate::Output> calibrateFuture;
  
  int main() {
  // some code to acquire handle
  // ...
  RadarServiceProxy service(handle);
  calibrateFuture = service.Calibrate(myConfigString);
  
  /* ....
  Now we access the future (with potentially blocking) Get()
  and directly handle the 3 different checked exceptions, which
  might be raised. */
  
  try {
  calibrateFuture.Get();
  } catch (const ServiceNotAvailableException& e) {
  // react on service shut down ...
  cout << "RadarService not available anymore" << endl;
  ...
  } catch (const CalibrationFailed& e) {
  // react on Application Error ’CalibrationFailed’ ...
  cout << "RadarService calibration failed because: " << e.getFailureText() << endl;
  ...
  } catch (const InvalidConfigString& e) {
  // react on Application Error ’InvalidConfigString’ ...
  cout << "Oops, we called RadarService.Calibrate with invalid config string: " << e.getInvalidConfig() << endl;
  cout << "Current (valid) config is: " << e.getCurrentValidConfig() << endl;
  ...
  }
  
  // go on doing something ...
  return 0;
 }
 
```


### 2.2.5 Fields
* Conceptually a field has — unlike an event — a certain value at any time. That result in the following additions compared to an event:
  * if a subscribtion to a field has been done, “immediately” initial values are sent back to the subscriber in an event-like notification pattern.
  * the current field value can be queried via a call to a Get() method or could be updated via a Set() method.
* Field concept is roughly an aggregation of an event with correlated get()/set() methods.
  * call Get() or Set() methods of the field with exactly the same mechanism as regular methods
  * access field update notifications in the form of events/event data, which are sent by the service instance our proxy is connected to with     exactly the same mechanism as regular events 
  
![UpdateRate Field Class](images/UpdateRateFieldClass.png)

## 2.3 Skeleton Class
* The Skeleton class is generated from the service interface description of the AUTOSAR meta model.
* ara::com does standardize the interface of the generated Skeleton class.
* The toolchain of an AP product vendor will generate a Skeleton implementation class exactly implementing this interface.
* The generated Skeleton class is an abstract class. It cannot be instantiated directly, because it does not contain implementations of the service methods, which the service should provide.
* Therefore the service implementer has to subclass the skeleton and provide the service method implementation within the subclass.
![RadarService Skeleton](images/RadarServiceskeleton.png)


### 2.3.1 Instantiation
* Skeleton class from which the service implementer has to subclass his service implementation provides a ctor with a parameter of type ara::com::InstanceIdentifier.
* This identifier has to be unique — creating two instances in a way that they would exist at the same time with the same instance identifier will raise an exception.
* If a new instance will be created with the same identifier, the existing instance needs to be destroyed before.

### 2.3.2 Offering Service instance
* The skeleton provides the method OfferService(). As application developer for the service provider side — have instantiated your custom service implementation class and initialized/set up your instance to a state, where it is now able to serve requests (method calls) and provide events to subscribing consumers, you will call this OfferService() method on your instance.
* If you decide at a certain point (maybe due to some state changes), that you do not want to provide the service anymore, you call StopOfferService() on your instance.

### 2.3.3 Polling and event-driven processing modes 
* The behavior of a skeleton instance is controlled by the second parameter of Skeleton class ctor, which is of type ara::com::MethodCallProcessingMode.

```{.cpp}
    /*
    * Request processing modes for the service implementation side
    * (skeleton).
    *
    * \note Should be provided by platform vendor exactly like this.
    */
    enum class MethodCallProcessingMode { kPoll, kEvent, kEventSingleThread };
```
* That means the processing mode is set for the entire service instance (i.e. all its provided methods are affected) and is fix for the whole lifetime of the skeleton instance. The default value in the ctor is set to kEvent.

#### 2.3.3.1 Polling Mode 
* If you set it to kPoll, the Communication Management implementation will not call any of the provided service methods asynchronously! If you want to process the next (assume that there is a queue behind the scenes, where incoming service method calls are stored) pending service-call, you have to call the following method on your service instance:

```{.cpp}
    /*
    * This fetches the next call from the Communication Management and executes it.
    * Only available in polling mode. In event mode it throws an exception.
    */
    ara::com::Future<bool> ProcessNextMethodCall();
```
* A simple use case for a typical RT application could be:
  * RT application gets scheduled.
  * it calls ProcessNextMethodCall and registers a callback with ara::com::Future::then()
  * the callback is invoked after the service method called by the midleware corresponding to the outstanding request has finished.
  * in the callback the RT application decides, if there is enough time left for serving a subsequent service method. If so, it calls another ProcessNextMethodCall.

#### 2.3.3.2 Event-Driven Mode
* If you set the processing mode to kEvent or kEventSingleThread, the Communication Management implementation will dispatch events asynchronously to the service method implementations at the time the service call from the service consumer comes in.
  *  **kEvent:**
    * Communication Management implementation may call the service method implementations concurrently.
    * If at the same point in time one call to method Calibrate and two calls to method Adjust arrive from different service consumers, the Communication Management implementation is allowed to take three threads from its internal thread-pool and do those three calls for the two service methods concurrently.
  
  * **kEventSingleThread:**
    * kEventSingleThread assures, that on the service instance only one service method at a time will be called by the Communication Management    implementation.
    * That means, Communication Management implementation has to queue incoming service method call events for the same service instance and dispatch them one after the other.
 
### 2.3.4 Methods
* Service methods on the skeleton side are abstract methods, which have to be overwritten by the service implementation sub-classing the skeleton.

```{.cpp}
    /*
    * For all output and non-void return parameters
    * an enclosing struct is generated, which contains
    * non-void return value and/or out parameters.
    */
    struct AdjustOutput {
    bool success;
    Position effective_position;
    };
    
    virtual ara::com::Future<AdjustOutput> Adjust(
    const Position& position) = 0;
```
* The returned ara::com::Future contains a structure as template parameter, which aggregates all the OUT-parameters of the service call.

#### 2.3.4.1 Raising Application Errors
* Whenever on the implementation side of a service method, an ApplicationError according to the interface description is detected, the Checked Exception representing this ApplicationError simply has to be stored into the Promise, from which the Future is returned to the caller:

```{.cpp}
    using namespace ara::com;
    using namespace com::mycompany::division::radarservice;
    
    /* our implementation of RadarService */
    class RadarServiceImpl : public RadarServiceSkeleton {
    
    public:
    Future<CalibrateOutput> Calibrate(const std::string& configuration)
    {
    Promise<CalibrateOutput> promise;
    auto future = promise.get_future();
    
    /* we check the given configuration arg */
    if (!checkConfigString(configuration)) {
    /* given arg is invalid: */
    InvalidConfigString myError(configuration, this->curValidConfig_));
    promise.set_exception(std::make_exception_ptr(myError));
    } else {
    ...
    }
    
    /* we return a future with a potentially set exception */
    return future;
    }
    
    private:
    bool checkConfigString(const std::string& config);
    std::string curValidConfig_;
    }
```

* In this example, the implementation of ‘Calibrate’ detects, that the given configuration string argument is invalid and sets the corresponding exception to the Promise.

### 2.3.5 Events
* On the skeleton side the service implementation is in charge of notifying about occurrence of an event.
* The event wrapper class on the skeleton/event provider side looks obviously different than on the proxy/event consumer side.

```{.cpp}
    class BrakeEvent {
    public:
    
    /*
    * Shortcut for the events data type.
    */
    using SampleType = RadarObjects;
    
    void Send(const SampleType &data);
    
    ara::com::SampleAllocateePtr<SampleType> Allocate();
     /*
     * After sending data you loose ownership and can’t access
     * the data through the SampleAllocateePtr anymore.
     * Implementation of SampleAllocateePtr will be with the
     * semantics of std::unique_ptr (see types.h)
     **/
     void Send(ara::com::SampleAllocateePtr<SampleType> data);
    
     };
```
* ara::com  provides two different variants of a ‘Send’ method, which is used to send out new event data.
1. The event data has been allocated somewhere by the service application developer and is given via reference to the binding implementation of Send(). After the call to send returns, the data might be removed/altered on the caller side. The binding implementation will make a copy in the call.
2. The second variant of ‘Send’ also has a parameter named ‘data’, but this is now of a different type ara::com::SampleAllocateePtr<SampleType>. ara::com introduced here, will behave like a std::unique_ptr<T>.Only one party can hold the pointer - if the owner wants to give it away, he has to explicitly do it via std::move.
3. ara::com::SampleAllocateePtr<SampleType> Allocate();
  * The event wrapper class provides us here with a method to allocate memory for one sample of event data.
  * It returns a smart pointer ara::com::SampleAllocateePtr<SampleType>, which points to the allocated memory, where we then can write an event data sample to.
  
### 2.3.6 Fields
* On the skeleton side the service implementation is in charge of
  * updating and notifying about changes of the value of a field.
  * serving incoming Get() calls.
  * serving incoming Set() calls.
* The field wrapper class on the skeleton/field provider side looks obviously different than on the proxy/field consumer side.
![UpdateRate Class](images/UpdateRateClass.png)


#### 2.3.6.1 Registering Getters
* The RegisterGetHandler method provides the possibility to register a method implementation by the service implementer, which gets then called by the binding implementation on an incoming Get() call from any proxy instance.
* The RegisterGetHandler method in the generated skeleton does only exist in case availability of “field getter” has been configured for the field in the configuration.

#### 2.3.6.1 Registering Setters
* RegisterSetHandler API has to be called by the service implementer in case field has been configured with setter support.

# 3 Service Discovery Implementation Strategies
* As the service discovery functionality is basically defined at the API level with the methods for FindService, OfferService and StopOfferService.
* When an AP node (more concretely an AP SWC) offers a service over the network or requires a service from another network node, then service discovery/service registry obviously takes place over the wire.
* The protocol for service discovery over the wire needs to be completely specified by the used communication protocol.
* if an ara::com application wants to communicate with another ara::com application on the same node within the AP of the same vendor there has to be a local variant of a service discovery available.

## 3.1 Central vs Distributed approach

### 3.1.1 Central approach
* This approach have one central entity(a demon process), which:
  * maintains a registry of all service instances together with their location information
  * serves all FindService, OfferService and StopOfferService requests from local ara::com applications, thereby either updating the registry (OfferService, StopOfferService) or querying the registry ( FindService)
  * serves all SOME/IP SD messages from the network either updating its registry (SOME/IP Offer Service received) or querying the registry (SOME/IP Find Service received)
![Centralized discovery approach](images/Centralizeddiscoveryapproach.png)


  
### 3.1.2 Distributed approach
* This approach would be, to distribute the service registry information (availability and location information) among the ara::com applications within the node.So for the node local communication use case no prominent discovery demon would be needed.
![Distributed discovery approach](images/Distributeddiscoveryapproach.png)



## 3.2 Multi-Binding implications
* Multi-Binding describes the solution to support setups, where the technical transport/connection between different instances of a certain proxy class/skeleton class are different.
  * proxy class uses different transport/IPC to communicate with different skeleton instances. Reason: Different service instances support different transport mechanisms because of deployment decisions.
  * symmetrically it may also be the case, that different proxy instances for the same skeleton instance uses different transport/IPC to communicate with this instance: The skeleton instance supports multiple transport mechanisms to get contacted.
  
### 3.2.1 Simple Multi-Binding use case
* The following figure depicts an obvious and/or rather simple case. In this example, which only deals with node local (inside one AP product/ECU) communication between service consumers (proxy) and service providers (skeleton), there are two instances of the same proxy class on the service consumer side.
* Service consumer application has triggered a ‘FindService’ first, which returned two handles for two different service instances of the searched service type.
* The service consumer application has instantiated a proxy instance for each of those handles.
* Now in this example the instance 1 of the service is located inside the same adaptive application (same process/address space) as the service consumer (proxy instance 1), while the service instance 2 is located in a different adaptive application (different process/ address space).
![Simple MultiBinding intra example](images/SimpleMultiBindingintraAPexample.png)



### 3.2.2 Local/Network Multi-Binding use case
* After we have seen a special variant of Multi-Binding in the preceding section, we now look at a variant, which can also be considered as being a real-world case.
* The instance 2 of the service is located on a different ECU attached to the same Ethernet network as our ECU with the AP product, where
the service consumer (with its proxies for instance 1 and 2) resides.
* As the standard protocol on Ethernet for AP is SOME/IP, it is expected, that the communication between both ECUs is based on SOME/IP.
* For our concrete example this means, that proxy 1 talks to service 1 via unix domain sockets (which might be optimized for process local communication to direct method calls), while the proxy 2 talks to service 2 via network sockets in a SOME/IP compliant message format.
![Multi-Binding local and network example](images/Multi-Bindinglocalandnetworkexample.png)



### 3.2.3 Typical SOME/IP Multi-Binding use case
* It is highly unlikely that an adaptive SWC (i.e. the language and network binding which runs in its context) opens socket connections
itself to communicate with a remote service.
* SOME/IP was explicitly designed to use as few ports as possible.
* The reason for that requirement comes from low power/low resources embedded ECUs:
* Managing a huge amount of IP sockets in parallel means huge costs in terms of memory (and runtime) resources.
* AUTOSAR CP siblings which will be main communication partner in an inside vehicle network demand this approach, which is uncommon, compared to
non-automotive IT usage pattern for ports.
* The entire SOME/IP traffic of an ECU / network endpoint is routed through one IP port!
* That means SOME/IP messages originating from/dispatched to many different local applications (service providers or service consumers) are (de)multiplexed to/from one socket connection.
* POSIX compatible OS with POSIX socket API, multiplexing SOME/IP communication of many applications to/from one port means the introduction of a separate/central (demon) process, which manages the corresponding port.
* The task of this process is to bridge between SOME/IP network communication and local communication and vice versa.
![SOME/IP Bridge](images/SOMEIPBridge.png)


# 4 ara::com and AUTOSAR meta-model relationship
## 4.1 Service Interface
* Service Interface defines everything signaturewise of an ara::com proxy or skeleton.
* The ServiceInterface describes the methods, fields and the methods a service interface consists of and how the signatures of those elements
(arguments and data types) look like.
* ara::com proxy and skeleton classes get generated from ServiceInterface.

## 4.2 Software Component
* The idea of a software component is to describe a reusable part of software with well defined interfaces.
* AUTOSAR manifest specification defines a model element SoftwareComponentType, which is an abstract element with several concrete subtypes, of which the subtype AdaptiveApplicationSwComponentType is the most important one for Adaptive Application software developers.
![meta-model to Implementation](images/meta-modeltoImplementation.png)
* SoftwareComponentTypes A and B from the example in the upper part (meta-model level) a concrete implementation exists on implementation level (lower part in the figure).
* The realization/implementation of R-Port of SoftwareComponentType A is based on an instance of ara::com proxy class on implementation level, while the P-Port implementation of SoftwareComponentType B is using an instance of ara::com skeleton class.
* Proxy and skeleton class are generated from the service interface definition ServiceInterface, which is referenced by the corresponding ports.
* Such a code fragment, which realizes a SoftwareComponentType can obviously be re-used. On C++ implementation level an implementation of an AdaptiveApplicationSwComponentType typically boils down to one or several C++ classes. So re-use simply means instantiating this class/those classes in different contexts multiple times.
![Multiple Instantiation in Implementation Contexts](images/MultipleInstantiation.png)

* The figure above shows an arbitrary example, where the implementations of A and B are instantiated in different contexts. On the lower left side there is an Executable 1, which directly uses two instances of As impl and one instance of Bs impl.
* Opposed to that, the right side shows an Executable 2, which "directly" (i.e. on its top most level) uses one instance of Bs impl and an instance of a composite software component, which itself "in its body" again instantiates one instance of As and Bs impl.
* This natural implementation concept of composing software components from other components to a bigger/composite artefact is fully reflected in the AUTOSAR meta-model in the form of a CompositionSwComponentType, which itself is a SoftwareComponentType and allows arbitrary recursive nesting/compositing of software components.

## 4.3 Adaptive Application/Executables and Processes
* Deployable software units within AP are so called Adaptive Applications (the corresponding meta-model element is AdaptiveAutosarApplication).
* Such an Adaptive Application consists of 1..n executeables.
![Instantiation of Addaptive Applications in Deployment](images/InstantiationoAddaptiveApplicationsinDeployment.png)

* Abstract identifiers relate in a certain way to the corresponding port in the model of the SoftwareComponentType.
* Instance IDs obviously have to be assigned to objects, which finally have a distinct identity in an deployment.
![Example of the possible structure of an Executable](images/Executable.png)


## 4.4 Usage of meta-model identifiers within ara::com based application code
* The abstract identifiers relate in a certain way to the corresponding port in the model of the SoftwareComponentType.
* Port name of the model alone isn’t sufficient to clearly identify it in its final instantiation, where the same component implementation might be instantiated multiple times in the code and then eventually started multiple times in different processes.
* Instance IDs obviously have to be assigned to objects, which finally have a distinct identity in an deployment.
![InstanceIds in Deployment](images/InstanceIdsDeployment.png)

* The figure above shows, within Executable 2 there are three instantiations of SoftwareComponentType B implementation in different contexts (nesting levels).All instances do provide a specific instance of SI RadarService.
* The integrator, who applies the Service Instance Manifest for Process 2 has to do the technical mapping on ara::com level. I.e. he has to decide,
which technical transport binding is to be used in each of the B instantiations and subsequently also, which technical transport binding specific instance ID.
* In our example, the integrator wants to provide the SI RadarService via SOME/IP binding and an SOME/IP specific instance ID "1" in the context of the B instantiation, which is nested inside the composite component on the right side, while he decides to provide the SI RadarService via local IPC (Unix domain socket) binding and a Unix domain socket specific instance ID "/tmp/Radar/3" and "/tmp/Radar/4" in the context of the B instantiations on the left side, which are not nested.
* Within the Service Instance Manifest, which allows to specify the mapping of port instantiations within a Process to technical bindings and their concrete instance IDs, the sole usage of the port name from the model isn’t sufficient to differentiate.
* To get unique identifiers within an executable (and therefore a process), the nature of nested instantiation and re-use of SoftwareComponentTypes
has to be considered. Every time a SoftwareComponentType gets instantiated, its instantiation gets a unique name within its instantiation context.
* In our example this means:
  * B instantiations on top level get unique names on their level: "B_Inst_1" and "B_Inst_2"
  * B instantiation within the Composite Component Type gets unique name on this level: "B_Inst_1"
  * Composite Component instantiation on top level gets unique name on its level: "Comp_Inst_1"
  * From the perspective of the executable/process, we therefore have unique identifiers for all instances of B:
    - "B_Inst_1"
    - "B_Inst_2"
    - "Comp_Inst_1::B_Inst_1"

# 5. Serialization
* Serialization is the process of transforming certain data structures into a standardized format for exchange between a sender and a (possibly different) receiver.
* When putting data on the wire and reading it back, receiver have to follow exact, agreed-on rules to be able to correctly interpret the data on the receiver side.
* For Adaptive Platform, the loading of executables during runtime, which have been built independently at different times and have been uploaded to an AP ECU at different times, is definitely a supported use case. The chance, that compiler settings for different ara::com applications were different regarding alignment decisions is consequently high. Therefore an AP product (more concrete its IPC binding implementation) has to use/support serialization of exchanged event/field/method data.

## 5.1 Zero-Copy implications
* One thing which typically is at the top of the list of performance optimizations in IPC/middleware implementations is the avoidance of unnecessary copies between sender and the receiver of data.
* When we talk about AP, where we have architectural expectations like applications running in separate processes providing memory protection, the typical communication approach needs at least ONE copy of the data from source address space to target address space.
* Highly optimizing middleware/IPC implementations could even get rid of this single copy step by setting up shared memory regions between
communicating ara::com components(sender and receiver).
