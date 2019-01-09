---
author: aabs
comments: true
date: 2007-12-31 01:14:49+00:00
layout: post
link: https://aabs.wordpress.com/2007/12/31/the-ambient-context-design-pattern-in-net/
slug: the-ambient-context-design-pattern-in-net
title: The Ambient Context Design Pattern in .NET
wordpress_id: 479
categories:
- .NET
- programming
- WCF
---

For a piece of agent related work I'm doing at the moment I am making heavy use of multi-threaded development. I'm developing a little special purpose Agent Framework to manage some data that I maintain. As part of that work, I need to have an ambient context object to hold details about the currently active agent and the tasks that it is performing. This is a common [pattern](http://en.wikipedia.org/wiki/Design_Patterns) that we see used throughout the .NET framework. They're a powerful mechanism to keep useful data around, to define scopes and to provide cross-cutting capabilities. They provide functionality and a non-intrusive management mechanism without having to clutter the components that need them with additional parameters or static variables. In effect they are a form of controlled global variable that exists to maintain scoped information.

Since I haven't seen this pattern documented in any detail elsewhere, I thought I might make a first attempt to describe it in pattern language terms. in what follows, I'll try to stick to the Gang of Four ([GoF94](http://www.awprofessional.com/title/0201633612)) format wherever possible, but I may make a few digressions for the sake of drawing parallels with comparable facilities in the framework (.NET 3.5). I'll also highlight when one of the characteristics I describe is not a universal feature of a context class, but is commonly enough used to be worth a mention.


<!-- more -->
 

## Pattern Name and Classification

Ambient Context

## Intent

Provide a place to store scope or context related information or functionality that automatically follows the flow of execution between execution scopes or domains.

## Also Known As

Scope, Context Object

## Motivation (Forces)

You have a problem that demands the use of scoping of execution blocks. You also need to supply execution policy information to those blocks, and a means to pass other information and functionality that is automatically available in sub-scopes. In addition you don't want to add these facilities as parameters to every method signature that you work with. Some of the code that exists in your sub-scopes may not be under your control, or may be third party code - this would prevent you from passing information to other sub-systems that need the contextual information you are attempting to supply. You want a standardised system that will make such information available without having to recourse to costly shared data systems like a database.

## Applicability

This pattern applies in many area that deal with runtime execution scopes such as security, thread management, or call marshalling. If you wish to allow information and control to flow across code boundaries then you either have to employ something equivalent to a context object, or augment every API with parameters that carry this information for you.

## Structure

A context object for a scope is typically created, and managed by a singleton or static manager class or method. Frequently, the context object is a per-thread (or execution scope) singleton that contains several read-only properties supplying information for the scope. In addition, the context object may provide an area for storage of information that can be allowed to flow downstream to other scopes. If nesting is allowed in the scope of the context, then stacks are frequently employed to allow unwinding of the context on exit from a scope.

## Participants

The participants of this pattern include a static or singleton manager class, the context object, and the entities within the execution scope.

## Collaboration

The manager class will provide a means to initiate a new scope. In the process it will instantiate a new context object which it will assign to the new scope. Prior to assignment to the scope the context object will be initialised with appropriate values for its read-only properties. Frequently these will be taken from the ambient values of an enclosing or current context. Once the ambient context is applied it will remain in effect until the scope is left. In some cases this may be due to an error state (such as an exception) in which case the context (and any effects that it might allow on the system state) are unwound and control is returned to the enclosing scope. 

## Consequences

The consequences to the system of making use of an ambient context object will be dependent on the problem domain. One constant is that the need for a proliferation of parameters from client call signatures is reduced.

## Implementation

Typically the context object is stored in a **_Thread Relative Static Field, _**access to which is controlled by the Manager class. Access to that can be achieved through the use of static property objects. 

A simple implementation is shown below. It does not make use of thread static variables to achieve its effect. instead, it makes use of a static Stack class of contexts call scopeStack. being private, this stack is entirely under the control of the context object itself. Obviously there are other ways that a manager class could be made able to manage the creation and disposal of the context objects.
    
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">MyNestedContext</span> : <span style="color:rgb(43,145,175);">IDisposable
    </span>{
        <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(43,145,175);">Stack</span><<span style="color:rgb(43,145,175);">MyNestedContext</span>> scopeStack = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">Stack</span><<span style="color:rgb(43,145,175);">MyNestedContext</span>>();
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">string</span> Id { <span style="color:rgb(0,0,255);">get</span>; <span style="color:rgb(0,0,255);">set</span>; }
        <span style="color:rgb(0,0,255);">public</span> MyNestedContext(<span style="color:rgb(0,0,255);">string</span> id)
        {
            Id = id;
            scopeStack.Push(<span style="color:rgb(0,0,255);">this</span>);
        }
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(43,145,175);">MyNestedContext</span> Current
        {
            <span style="color:rgb(0,0,255);">get
    </span>        {
                <span style="color:rgb(0,0,255);">if</span> (scopeStack.Count == 0)
                {
                    <span style="color:rgb(0,0,255);">return</span> <span style="color:rgb(0,0,255);">null</span>;
                }
                <span style="color:rgb(0,0,255);">return</span> scopeStack.Peek();
            }
        }
    
    <span style="color:rgb(0,0,255);">    #region</span> IDisposable Members
    
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> Dispose()
        {
            <span style="color:rgb(0,0,255);">if</span> (ShouldUnwindScope())
                UnwindScope();
            scopeStack.Pop();
        }
    
    <span style="color:rgb(0,0,255);">    #endregion
    
    </span>    <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">void</span> UnwindScope()
        {
            <span style="color:rgb(0,128,0);">// ...
    </span>    }
    
        <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">bool</span> ShouldUnwindScope()
        {
            <span style="color:rgb(0,0,255);">bool</span> result = <span style="color:rgb(0,0,255);">true</span>;
            <span style="color:rgb(0,128,0);">//...
    </span>        <span style="color:rgb(0,0,255);">return</span> result;
        }
    }
    
    
    <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">Program
    </span>{
        <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">void</span> Main(<span style="color:rgb(0,0,255);">string</span>[] args)
        {
            Test1();
            <span style="color:rgb(43,145,175);">Console</span>.ReadKey();
        }
    
        <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">void</span> Test1()
        {
            <span style="color:rgb(43,145,175);">Console</span>.WriteLine(<span style="color:rgb(163,21,21);">"Current Context is {0}"</span>, <span style="color:rgb(43,145,175);">MyNestedContext</span>.Current != <span style="color:rgb(0,0,255);">null</span> ? <span style="color:rgb(43,145,175);">MyNestedContext</span>.Current.Id : <span style="color:rgb(163,21,21);">"null"</span>);
            <span style="color:rgb(0,0,255);">using</span> (<span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">MyNestedContext</span>(<span style="color:rgb(163,21,21);">"outer scope"</span>))
            {
                <span style="color:rgb(43,145,175);">Console</span>.WriteLine(<span style="color:rgb(163,21,21);">"Current Context is {0}"</span>, <span style="color:rgb(43,145,175);">MyNestedContext</span>.Current != <span style="color:rgb(0,0,255);">null</span> ? <span style="color:rgb(43,145,175);">MyNestedContext</span>.Current.Id : <span style="color:rgb(163,21,21);">"null"</span>);
                <span style="color:rgb(0,0,255);">using</span> (<span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">MyNestedContext</span>(<span style="color:rgb(163,21,21);">"inner scope"</span>))
                {
                    <span style="color:rgb(43,145,175);">Console</span>.WriteLine(<span style="color:rgb(163,21,21);">"Current Context is {0}"</span>, <span style="color:rgb(43,145,175);">MyNestedContext</span>.Current != <span style="color:rgb(0,0,255);">null</span> ? <span style="color:rgb(43,145,175);">MyNestedContext</span>.Current.Id : <span style="color:rgb(163,21,21);">"null"</span>);
                }
                <span style="color:rgb(43,145,175);">Console</span>.WriteLine(<span style="color:rgb(163,21,21);">"Current Context is {0}"</span>, <span style="color:rgb(43,145,175);">MyNestedContext</span>.Current != <span style="color:rgb(0,0,255);">null</span> ? <span style="color:rgb(43,145,175);">MyNestedContext</span>.Current.Id : <span style="color:rgb(163,21,21);">"null"</span>);
            }
            <span style="color:rgb(43,145,175);">Console</span>.WriteLine(<span style="color:rgb(163,21,21);">"Current Context is {0}"</span>, <span style="color:rgb(43,145,175);">MyNestedContext</span>.Current != <span style="color:rgb(0,0,255);">null</span> ? <span style="color:rgb(43,145,175);">MyNestedContext</span>.Current.Id : <span style="color:rgb(163,21,21);">"null"</span>);
        }
    }
    

[](http://11011.net/software/vspaste)


This implementation produces the desired result:




<blockquote>

> 
> Current Context is null  
Current Context is outer scope  
Current Context is inner scope  
Current Context is outer scope  
Current Context is null
> 
> </blockquote>




While this will work in a single-threaded environment its flaw is that the same context stack is shared between all threads. This will probably for example not be appropriate for a service oriented application (such as might be based on WCF) may have multiple unrelated threads going on at a time. the following code (within a context manager class) can be used to create a new thread with the same context as was current in the creating thread.
    
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">void</span> Run(<span style="color:rgb(43,145,175);">ParameterizedThreadStart</span> pts, <span style="color:rgb(43,145,175);">Object</span> obj, <span style="color:rgb(0,0,255);">string</span> threadName)
    {
        <span style="color:rgb(0,128,0);">// get the current context
    </span>    <span style="color:rgb(43,145,175);">Context</span> c = CurrentContext;
        <span style="color:rgb(0,128,0);">// create a wrapper delegate to set up the context
    </span>    <span style="color:rgb(43,145,175);">ParameterizedThreadStart</span> pts2 = (<span style="color:rgb(43,145,175);">Object</span> arg) =>
        {
            <span style="color:rgb(0,128,0);">// extract the package of context, worker func and params
    </span>        <span style="color:rgb(43,145,175);">Tuple</span><<span style="color:rgb(43,145,175);">ParameterizedThreadStart</span>, <span style="color:rgb(43,145,175);">Context</span>, <span style="color:rgb(43,145,175);">Object</span>> t = (<span style="color:rgb(43,145,175);">Tuple</span><<span style="color:rgb(43,145,175);">ParameterizedThreadStart</span>, <span style="color:rgb(43,145,175);">Context</span>, <span style="color:rgb(43,145,175);">Object</span>>)arg;
            <span style="color:rgb(0,128,0);">// set up the context
    </span>        <span style="color:rgb(43,145,175);">ContextManager</span>.StartNewContext(t.Second);
            <span style="color:rgb(0,128,0);">// run the worker
    </span>        t.First(t.Third);
        };
        <span style="color:rgb(0,128,0);">// package up the worker, current context and args
    </span>    <span style="color:rgb(43,145,175);">Tuple</span><<span style="color:rgb(43,145,175);">ParameterizedThreadStart</span>, <span style="color:rgb(43,145,175);">Context</span>, <span style="color:rgb(43,145,175);">Object</span>> x = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">Tuple</span><<span style="color:rgb(43,145,175);">ParameterizedThreadStart</span>, <span style="color:rgb(43,145,175);">Context</span>, <span style="color:rgb(43,145,175);">Object</span>>(pts, c, obj);
        <span style="color:rgb(0,128,0);">// create and run a thread using the wrapper.
    </span>    <span style="color:rgb(43,145,175);">Thread</span> thread = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">Thread</span>(pts2);
        <span style="color:rgb(0,0,255);">if</span> (!<span style="color:rgb(0,0,255);">string</span>.IsNullOrEmpty(threadName))
        {
            thread.Name = threadName;
        }
        thread.Start(x);
    }
    

[](http://11011.net/software/vspaste)


We would also need to achieve the same effect if we were making cross process calls. In WCF, for example, this might be achieved through the use of a custom header that carries the new scope through to the new process. The implementation for that might resemble something like the following:
    
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">EndpointBehaviorAddUserSessionId</span> : <span style="color:rgb(43,145,175);">IEndpointBehavior
    </span>{
    <span style="color:rgb(0,0,255);">    #region</span> IEndpointBehavior Members
    
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> AddBindingParameters(
            <span style="color:rgb(43,145,175);">ServiceEndpoint</span> endpoint,
            <span style="color:rgb(43,145,175);">BindingParameterCollection</span> bindingParameters) { }
    
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> ApplyClientBehavior(
            <span style="color:rgb(43,145,175);">ServiceEndpoint</span> endpoint,
            <span style="color:rgb(43,145,175);">ClientRuntime</span> clientRuntime)
        {
            clientRuntime.MessageInspectors.Add(<span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">MessageInspectorAddCurrentContext</span>());
        }
    
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> ApplyDispatchBehavior(
            <span style="color:rgb(43,145,175);">ServiceEndpoint</span> endpoint,
            <span style="color:rgb(43,145,175);">EndpointDispatcher</span> endpointDispatcher) { }
    
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> Validate(<span style="color:rgb(43,145,175);">ServiceEndpoint</span> endpoint) { }
    
    <span style="color:rgb(0,0,255);">    #endregion
    </span>}
    
    
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">MessageInspectorAddCurrentContext</span>: <span style="color:rgb(43,145,175);">IClientMessageInspector
    </span>{
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> AfterReceiveReply(<span style="color:rgb(0,0,255);">ref</span> <span style="color:rgb(43,145,175);">Message</span> reply, <span style="color:rgb(0,0,255);">object</span> correlationState)
        {
        }
    
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">object</span> BeforeSendRequest(<span style="color:rgb(0,0,255);">ref</span> <span style="color:rgb(43,145,175);">Message</span> request, <span style="color:rgb(43,145,175);">IClientChannel</span> channel)
        {
            AddCurrentContext(<span style="color:rgb(0,0,255);">ref</span> request);
            <span style="color:rgb(0,0,255);">return</span> <span style="color:rgb(0,0,255);">null</span>;
        }
    
        <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">void</span> AddCurrentContext(<span style="color:rgb(0,0,255);">ref</span> <span style="color:rgb(43,145,175);">Message</span> request)
        {
            <span style="color:rgb(0,0,255);">if</span> (<span style="color:rgb(43,145,175);">MyNestedContext</span>.Current != <span style="color:rgb(0,0,255);">null</span>)
            {
                <span style="color:rgb(0,0,255);">string</span> name = <span style="color:rgb(163,21,21);">"MyNestedContext-Context"</span>;
                <span style="color:rgb(0,0,255);">string</span> ns = <span style="color:rgb(163,21,21);">"urn:<some guid>"</span>;
                request.Headers.Add(
                    <span style="color:rgb(43,145,175);">MessageHeader</span>.CreateHeader(name, ns, <span style="color:rgb(43,145,175);">MyNestedContext</span>.Current));
            }
        }
    }
    




Which would then be used like so:
    
    <span style="color:rgb(0,0,255);">internal</span> <span style="color:rgb(0,0,255);">static</span> T CreateProxy<T>(<span style="color:rgb(0,0,255);">string</span> configName) <span style="color:rgb(0,0,255);">where</span> T : <span style="color:rgb(0,0,255);">class
    </span>{
        <span style="color:rgb(43,145,175);">ChannelFactory</span><T> factory = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">ChannelFactory</span><T>(configName);
        factory.Endpoint.Behaviors.Add(<span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">EndpointBehaviorAddCurrentContext</span>());
        <span style="color:rgb(0,0,255);">return</span> factory.CreateChannel();
    }
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">static</span> IMyClientObject CreateCallManager()
    {
        <span style="color:rgb(0,0,255);">return</span> CreateProxy<IMyClientObject>(<span style="color:rgb(163,21,21);">"tcpMyClient"</span>);
    }
    

[](http://11011.net/software/vspaste)


Allowing the context at the top of the stack to flow to new new domain. When the new domain call progresses it may push further context objects onto the stack




## Sample Code




the following sample demonstrates the implementations described above in action. the first example shows the single-threaded case, where no specific support is required to maintain and protect the context in a thread-safe way:
    
    <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">void</span> Test1()
    {
        DisplayScopeDetails();
        <span style="color:rgb(0,0,255);">using</span> (<span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">Context</span>(<span style="color:rgb(163,21,21);">"outer scope"</span>))
        {
            DisplayScopeDetails();
            <span style="color:rgb(0,0,255);">using</span> (<span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">Context</span>(<span style="color:rgb(163,21,21);">"inner scope"</span>))
            {
                DisplayScopeDetails();
            }
            DisplayScopeDetails();
        }
        DisplayScopeDetails();
    }




Which produces the expected output.




<blockquote>

> 
> Thread: unknown thread Context: null  
Thread: unknown thread Context: outer scope  
Thread: unknown thread Context: inner scope  
Thread: unknown thread Context: outer scope  
Thread: unknown thread Context: null
> 
> </blockquote>




The next example demonstrates the cross threaded support at work:
    
    <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">void</span> Test2()
    {
        DisplayScopeDetails(<span style="color:rgb(163,21,21);">"start"</span>);
        <span style="color:rgb(0,0,255);">using</span> (<span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">Context</span>(<span style="color:rgb(163,21,21);">"outer scope"</span>))
        {
            DisplayScopeDetails();
            <span style="color:rgb(0,0,255);">using</span> (<span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">Context</span>(<span style="color:rgb(163,21,21);">"inner scope"</span>))
            {
                DisplayScopeDetails(<span style="color:rgb(163,21,21);">"begin"</span>);
                <span style="color:rgb(43,145,175);">ContextManager</span>.Run(WorkerFunction, <span style="color:rgb(0,0,255);">null</span>, <span style="color:rgb(163,21,21);">"new thread"</span>);
                <span style="color:rgb(43,145,175);">Thread</span>.Sleep(20);
                DisplayScopeDetails(<span style="color:rgb(163,21,21);">"end"</span>);
            }
            DisplayScopeDetails();
        }
        DisplayScopeDetails(<span style="color:rgb(163,21,21);">"end"</span>);
    }
    
    
    <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">void</span> WorkerFunction(<span style="color:rgb(0,0,255);">object</span> o)
    {
        DisplayScopeDetails(<span style="color:rgb(163,21,21);">"In Worker Function"</span>);
        <span style="color:rgb(0,0,255);">using</span> (<span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">Context</span>(<span style="color:rgb(163,21,21);">"inner inner scope"</span>))
        {
            DisplayScopeDetails();
        }
        DisplayScopeDetails(<span style="color:rgb(163,21,21);">"Leaving Worker Function"</span>);
    }
    

[](http://11011.net/software/vspaste)[](http://11011.net/software/vspaste)


which this time produces the following 




<blockquote>

> 
> Thread: unknown thread Context: null (start)  
Thread: unknown thread Context: outer scope  
Thread: unknown thread Context: inner scope (begin)  
Thread: new thread Context: inner scope (In Worker Function)  
Thread: new thread Context: inner inner scope  
Thread: new thread Context: inner scope (Leaving Worker Function)  
Thread: unknown thread Context: inner scope (end)  
Thread: unknown thread Context: outer scope  
Thread: unknown thread Context: null (end)
> 
> </blockquote>




[](http://11011.net/software/vspaste)


This demonstrates a situation where a new scope stack has to be created on a new thread, and then is allowed to grow before being unwound and discontinued. After 20ms the old thread is continued and it proceeds to unwind its own scope stack before completion.




## Known Uses




There are numerous uses of this pattern in cross domain communications libraries, multi-threading libraries and in server environments where thread pools handle numerous incoming requests. Examples include the core .NET framework classes listed below:






  * System.Globalization.CultureInfo


  * System.ActivationContext


  * System.Threading.ExecutionContext 


  * System.Threading.SynchronizationContext


  * System.Transactions.TransactionScope


  * System.ServiceModel.OperationContext


  * System.Web.HttpContext


  * System.Security.SecurityContext
