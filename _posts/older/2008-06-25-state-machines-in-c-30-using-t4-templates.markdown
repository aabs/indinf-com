---
author: aabs
comments: true
date: 2008-06-25 14:07:14+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2008/06/26/state-machines-in-c-30-using-t4-templates/
slug: state-machines-in-c-30-using-t4-templates
title: State Machines in C# 3.0 using T4 Templates
wordpress_id: 544
categories:
- .NET
- functional programming
- programming
tags:
- .NET
- C#
- DFT
- FSA
- lambda
- NDFA
- partial classes
- partial methods
- T4
- VS2008
---

**UPDATE**: The original code for this post, that used to be available via a link on this page, is no longer available. I'm afraid that if you want to try this one out, you'll have to piece it together using the snippets contained in this post. Sorry for the inconvenience - blame it on ISP churn.



* * *



Some time back I [wrote](http://aabs.wordpress.com/2007/01/16/342/) about techniques for implementing non-deterministic finite automata (NDFAs) using some of the new features of C# 3.0. Recently I've had a need to revisit that work to provide a client with a means to generate a bunch of really complex state machines in a lightweight, extensible and easily understood model. VS 2008 and C# 3.0 are pretty much the perfect platform for the job - they combine partial classes and methods, lambda functions and T4 templates making it a total walk in the park. This post will look at the prototype system I put together. This is a very code intensive post - sorry about that, but it's late and apparently my eyes are very red, puffy and panda like.

State machines are the core of many applications - yet we often find people hand coding them with nested switch statements and grizzly mixtures of state control and business logic. It's a nightmare scenario making code completely unmaintainable for anything but the most trivial applications.

The key objective for a dedicated application framework that manages a state machine is to provide a clean way to break out the code that manages the state machine from the code that implements the activities performed as part of the state machine. C# 3.0 has a nice solution for this - partial types and methods.


### Partial types and methods


A partial type is a type whose definition is not confined to a single code module - it can have multiple modules. Some of those can be written by you, others can be written by a code generator. Here's an example of a partial class definition:

    
    <span style="color:blue;">public partial class </span><span style="color:#2b91af;">MyPartialClass</span>{}


By by declaring the class to be partial, you say that other files may contain parts of the class definition. the point of this kind of structure is that you might have piece of code that you want to write by hand, and others that you want to have driven from a code generator, stuff that gets overwritten every time the generator runs. If your code got erased every time you ran the generator, you'd get bored very quickly. You need a way to chop out the bits that don't change. Typically, these will be framework or infrastructure stuff.

Partial classes can also have partial methods. Partial methods allow you to define a method signature in case someone wants to define it in another part of the partial class. This might seem pointless, but wait and see - it's nice. Here's how you declare a partial method:

    
    <span style="color:green;">// the code generated part </span><span style="color:blue;">public partial class </span><span style="color:#2b91af;">MyPartialClass </span>{
        <span style="color:blue;">partial void </span>DoIt(<span style="color:blue;">int </span>x);
    }


You can then implement it in another file like so:

    
    <span style="color:green;">// the hand-written part </span><span style="color:blue;">partial class </span><span style="color:#2b91af;">MyPartialClass </span>{
        <span style="color:blue;">partial void </span>DoIt(<span style="color:blue;">int </span>x)
        {
            <span style="color:blue;">throw new </span><span style="color:#2b91af;">NotImplementedException</span>();
        }
    }


This is all a little abstract, right now, so let's see how we can use this to implement a state machine framework. First we need a way to define a state machine. I'm going to use a simple XML file for this:

    
    <span style="color:blue;"><?</span><span style="color:#a31515;">xml </span><span style="color:red;">version</span><span style="color:blue;">=</span>"<span style="color:blue;">1.0</span>" <span style="color:red;">encoding</span><span style="color:blue;">=</span>"<span style="color:blue;">utf-8</span>" <span style="color:blue;">?> <</span><span style="color:#a31515;">StateModels</span><span style="color:blue;">> <</span><span style="color:#a31515;">StateModel </span><span style="color:red;">ID</span><span style="color:blue;">=</span>"<span style="color:blue;">My</span>" <span style="color:red;">start</span><span style="color:blue;">=</span>"<span style="color:blue;">defcon1</span>"<span style="color:blue;">> <</span><span style="color:#a31515;">States</span><span style="color:blue;">> <</span><span style="color:#a31515;">State </span><span style="color:red;">ID</span><span style="color:blue;">=</span>"<span style="color:blue;">defcon1</span>" <span style="color:red;">name</span><span style="color:blue;">=</span>"<span style="color:blue;">defcon1</span>"<span style="color:blue;">/> <</span><span style="color:#a31515;">State </span><span style="color:red;">ID</span><span style="color:blue;">=</span>"<span style="color:blue;">defcon2</span>" <span style="color:red;">name</span><span style="color:blue;">=</span>"<span style="color:blue;">defcon2</span>"<span style="color:blue;">/> <</span><span style="color:#a31515;">State </span><span style="color:red;">ID</span><span style="color:blue;">=</span>"<span style="color:blue;">defcon3</span>" <span style="color:red;">name</span><span style="color:blue;">=</span>"<span style="color:blue;">defcon3</span>"<span style="color:blue;">/> </</span><span style="color:#a31515;">States</span><span style="color:blue;">> <</span><span style="color:#a31515;">Inputs</span><span style="color:blue;">> <</span><span style="color:#a31515;">Input </span><span style="color:red;">ID</span><span style="color:blue;">=</span>"<span style="color:blue;">diplomaticIncident</span>" <span style="color:red;">name</span><span style="color:blue;">=</span>"<span style="color:blue;">diplomaticIncident</span>"<span style="color:blue;">/> <</span><span style="color:#a31515;">Input </span><span style="color:red;">ID</span><span style="color:blue;">=</span>"<span style="color:blue;">assassination</span>" <span style="color:red;">name</span><span style="color:blue;">=</span>"<span style="color:blue;">assassination</span>"<span style="color:blue;">/> <</span><span style="color:#a31515;">Input </span><span style="color:red;">ID</span><span style="color:blue;">=</span>"<span style="color:blue;">coup</span>" <span style="color:red;">name</span><span style="color:blue;">=</span>"<span style="color:blue;">coup</span>"<span style="color:blue;">/> </</span><span style="color:#a31515;">Inputs</span><span style="color:blue;">> <</span><span style="color:#a31515;">Transitions</span><span style="color:blue;">> <</span><span style="color:#a31515;">Transition </span><span style="color:red;">from</span><span style="color:blue;">=</span>"<span style="color:blue;">defcon1</span>" <span style="color:red;">to</span><span style="color:blue;">=</span>"<span style="color:blue;">defcon2</span>" <span style="color:red;">on</span><span style="color:blue;">=</span>"<span style="color:blue;">diplomaticIncident</span>"<span style="color:blue;">/> <</span><span style="color:#a31515;">Transition </span><span style="color:red;">from</span><span style="color:blue;">=</span>"<span style="color:blue;">defcon2</span>" <span style="color:red;">to</span><span style="color:blue;">=</span>"<span style="color:blue;">defcon3</span>" <span style="color:red;">on</span><span style="color:blue;">=</span>"<span style="color:blue;">assassination</span>"<span style="color:blue;">/> <</span><span style="color:#a31515;">Transition </span><span style="color:red;">from</span><span style="color:blue;">=</span>"<span style="color:blue;">defcon3</span>" <span style="color:red;">to</span><span style="color:blue;">=</span>"<span style="color:blue;">defcon1</span>" <span style="color:red;">on</span><span style="color:blue;">=</span>"<span style="color:blue;">coup</span>"<span style="color:blue;">/> </</span><span style="color:#a31515;">Transitions</span><span style="color:blue;">> </</span><span style="color:#a31515;">StateModel</span><span style="color:blue;">> </</span><span style="color:#a31515;">StateModels</span><span style="color:blue;">></span>


Here we have a really simple state machine with three states (defcon1, defcon2 and defcon3) as well as three kinds of input (diplomaticIncident, assassination and coup). Please excuse the militarism - I just finished watching a season of 24, so I'm all hyped up. This simple model also defines three transitions. it creates a model like this:



Microsoft released the Text Template Transformation Toolkit (T4) system with Visual Studio 2008. This toolkit has been part of GAT and DSL tools in the past, but this is the first time that it has been available by default in VS. It allows an ASP.NET syntax for defining templates. Here's a snippet from the T4 template that generates the state machine:

    
    <#@ template language="C#" #>
    <#@ assembly name="System.Xml.dll" #>
    <#@ import namespace="System.Xml" #>
    
    <#
        XmlDocument doc = new XmlDocument();
        doc.Load(@"TestHarness\model.xml");
        XmlElement xnModel = (XmlElement)doc.SelectSingleNode("/StateModels/StateModel");
        string ns = xnModel.GetAttribute("ID");
        XmlNodeList states = xnModel.SelectNodes("descendant::State");
        XmlNodeList inputs = xnModel.SelectNodes("descendant::Input");
        XmlNodeList trns = xnModel.SelectNodes("descendant::Transition");
        #>
    using System;
    using System.Collections.Generic;
    
    namespace <#=ns#> {
    public enum <#=ns#>States : int{
    <#
    string sep = "";
    foreach(XmlElement s in states)
        {
        Write(sep + s.GetAttribute("ID"));
        WriteLine(@"// " + s.GetAttribute("name"));
        sep = ",";
        }
    
    #>
    } // end enum <#=ns#>States
    
    public enum <#=ns#>Inputs : int{
    <#
    sep = "";
    foreach(XmlElement s in inputs)
        {
        Write(sep + s.GetAttribute("ID"));
        WriteLine(@"// " + s.GetAttribute("name"));
        sep = ",";
        }
    
    #>
    } // end enum <#=ns#>States
    
    public partial class <#=ns#>StateModel{
    
            public <#=ns#>StateModel()
            {
                SetupStates();
                SetupTransitions();
                SetStartState();
            }
    ...


Naturally, there's a lot in the template, but we'll get to that later. First we need a representation of a state. You'll see from the template that an enum get's generated called <#=ns#>States. Here's what it looks like for the defcon model.

    
    <span style="color:blue;">public enum </span><span style="color:#2b91af;">MyStates </span>: <span style="color:blue;">int </span>{
    defcon1<span style="color:green;">// defcon1 </span>,defcon2<span style="color:green;">// defcon2 </span>,defcon3<span style="color:green;">// defcon3 </span>} <span style="color:green;">// end enum MyStates </span>


This is still a bit too bare for my liking. I can't attach an event model to these states, so here's a class that can carry around one of these values:

    
    <span style="color:blue;">public class </span><span style="color:#2b91af;">State </span>{
        <span style="color:blue;">public int </span>Identifier { <span style="color:blue;">get</span>; <span style="color:blue;">set</span>; }
    <span style="color:blue;">public delegate void </span><span style="color:#2b91af;">OnEntryEventHandler</span>(<span style="color:blue;">object </span>sender, <span style="color:#2b91af;">OnEntryEventArgs </span>e);
        <span style="color:green;">// ...</span><span style="color:blue;">public event </span><span style="color:#2b91af;">OnEntryEventHandler </span>OnEntryEvent;
        <span style="color:green;">// ...</span>}


There's a lot left out of this, but the point is that as well as storing an identifier for a state, it has events for both entry into and exit from the state. This can be used by the event framework of the state machine to provide hooks for your custom state transition and entry code. The same model is used for transitions:

    
    <span style="color:blue;">public class </span><span style="color:#2b91af;">StateTransition </span>{
        <span style="color:blue;">public </span><span style="color:#2b91af;">State </span>FromState { <span style="color:blue;">get</span>; <span style="color:blue;">set</span>; }
        <span style="color:blue;">public </span><span style="color:#2b91af;">State </span>ToState { <span style="color:blue;">get</span>; <span style="color:blue;">set</span>; }
    <span style="color:blue;">public event </span><span style="color:#2b91af;">OnStateTransitioningEventHandler </span>OnStateTransitioningEvent;
    <span style="color:blue;">public event </span><span style="color:#2b91af;">OnStateTransitionedEventHandler </span>OnStateTransitionedEvent;
    }


Here's the list of inputs that can trigger a transition between states:

    
    <span style="color:blue;">public enum </span><span style="color:#2b91af;">MyInputs </span>: <span style="color:blue;">int </span>{
    diplomaticIncident<span style="color:green;">// diplomaticIncident </span>,assassination<span style="color:green;">// assassination </span>,coup<span style="color:green;">// coup </span>} <span style="color:green;">// end enum MyStates</span>


The template helps to define storage for the states and transitions of the model:

    
    public static Dictionary<<#= ns#>States, State> states                = new Dictionary<<#= ns#>States, State>();
    public static Dictionary<string, StateTransition> arcs                = new Dictionary<string, StateTransition>();
    public State CurrentState { get; set; }


which for the model earlier, will yield the following:

    
    <span style="color:blue;">public static </span><span style="color:#2b91af;">Dictionary</span><<span style="color:#2b91af;">MyStates</span>, <span style="color:#2b91af;">State</span>> states = <span style="color:blue;">new </span><span style="color:#2b91af;">Dictionary</span><<span style="color:#2b91af;">MyStates</span>, <span style="color:#2b91af;">State</span>>();
    <span style="color:blue;">public static </span><span style="color:#2b91af;">Dictionary</span><<span style="color:blue;">string</span>, <span style="color:#2b91af;">StateTransition</span>> arcs = <span style="color:blue;">new </span><span style="color:#2b91af;">Dictionary</span><<span style="color:blue;">string</span>, <span style="color:#2b91af;">StateTransition</span>>();
    <span style="color:blue;">public </span><span style="color:#2b91af;">State </span>CurrentState { <span style="color:blue;">get</span>; <span style="color:blue;">set</span>; }


Now we can create entries in these tables for the transitions in the model:

    
    private void SetStartState()
    {
        CurrentState = states[<#= ns#>States.<#=xnModel.GetAttribute("start")#>];
    }
    
    private void SetupStates()
    {
    <#
    foreach(XmlElement s in states)
        {
        WriteLine("states[" + ns + "States."+s.GetAttribute("ID")+"] =               new State { Identifier = (int)"+ns+"States."+s.GetAttribute("ID")+" };");
        WriteLine("states[" + ns + "States."+s.GetAttribute("ID")+"].OnEntryEvent               += (x, y) => OnEnter_"+s.GetAttribute("ID")+"();");
        WriteLine("states[" + ns + "States."+s.GetAttribute("ID")+"].OnExitEvent               += (x, y) => OnLeave_"+s.GetAttribute("ID")+"(); ;");
        }
    #>
    }
    private void SetupTransitions()
    {
    <#
    foreach(XmlElement s in trns)
        {
        #>
        arcs["<#=s.GetAttribute("from")#>_<#=s.GetAttribute("on")#>"] = new StateTransition
        {
            FromState = states[<#= ns#>States.<#=s.GetAttribute("from")#>],
            ToState = states[<#= ns#>States.<#=s.GetAttribute("to")#>]
        };
        arcs["<#=s.GetAttribute("from")#>_<#=s.GetAttribute("on")#>"].OnStateTransitioningEvent              += (x,y)=>MovingFrom_<#=s.GetAttribute("from")#>_To_<#=s.GetAttribute("to")#>;
        arcs["<#=s.GetAttribute("from")#>_<#=s.GetAttribute("on")#>"].OnStateTransitionedEvent              += (x,y)=>MovedFrom_<#=s.GetAttribute("from")#>_To_<#=s.GetAttribute("to")#>;
        <#
        }
        #>
    }


which is where the fun starts. First notice that we create a new state for each state in the model and attach a lambda to the entry and exit events of each state. For our model that would look like this:

    
    <span style="color:blue;">private void </span>SetupStates()
    {
        states[<span style="color:#2b91af;">MyStates</span>.defcon1] = <span style="color:blue;">new </span><span style="color:#2b91af;">State </span>{Identifier = (<span style="color:blue;">int</span>) <span style="color:#2b91af;">MyStates</span>.defcon1};
        states[<span style="color:#2b91af;">MyStates</span>.defcon1].OnEntryEvent += (x, y) => OnEnter_defcon1();
        states[<span style="color:#2b91af;">MyStates</span>.defcon1].OnExitEvent += (x, y) => OnLeave_defcon1();
    
        states[<span style="color:#2b91af;">MyStates</span>.defcon2] = <span style="color:blue;">new </span><span style="color:#2b91af;">State </span>{Identifier = (<span style="color:blue;">int</span>) <span style="color:#2b91af;">MyStates</span>.defcon2};
        states[<span style="color:#2b91af;">MyStates</span>.defcon2].OnEntryEvent += (x, y) => OnEnter_defcon2();
        states[<span style="color:#2b91af;">MyStates</span>.defcon2].OnExitEvent += (x, y) => OnLeave_defcon2();
    
        states[<span style="color:#2b91af;">MyStates</span>.defcon3] = <span style="color:blue;">new </span><span style="color:#2b91af;">State </span>{Identifier = (<span style="color:blue;">int</span>) <span style="color:#2b91af;">MyStates</span>.defcon3};
        states[<span style="color:#2b91af;">MyStates</span>.defcon3].OnEntryEvent += (x, y) => OnEnter_defcon3();
        states[<span style="color:#2b91af;">MyStates</span>.defcon3].OnExitEvent += (x, y) => OnLeave_defcon3();
    }


For the Transitions the same sort of code gets generated, except that we have some simple work to generate a string key for a specific <state, input> pair. Here's what comes out:

    
    <span style="color:blue;">private void </span>SetupTransitions()
    {
        arcs[<span style="color:#a31515;">"defcon1_diplomaticIncident"</span>] = <span style="color:blue;">new </span><span style="color:#2b91af;">StateTransition </span>{
                     FromState = states[<span style="color:#2b91af;">MyStates</span>.defcon1],
                     ToState = states[<span style="color:#2b91af;">MyStates</span>.defcon2]
                 };
        arcs[<span style="color:#a31515;">"defcon1_diplomaticIncident"</span>].OnStateTransitioningEvent                  += (x, y) => MovingFrom_defcon1_To_defcon2;
        arcs[<span style="color:#a31515;">"defcon1_diplomaticIncident"</span>].OnStateTransitionedEvent                 += (x, y) => MovedFrom_defcon1_To_defcon2;
        arcs[<span style="color:#a31515;">"defcon2_assassination"</span>] = <span style="color:blue;">new </span><span style="color:#2b91af;">StateTransition </span>{
                     FromState = states[<span style="color:#2b91af;">MyStates</span>.defcon2],
                     ToState = states[<span style="color:#2b91af;">MyStates</span>.defcon3]
                };
        arcs[<span style="color:#a31515;">"defcon2_assassination"</span>].OnStateTransitioningEvent                += (x, y) => MovingFrom_defcon2_To_defcon3;
        arcs[<span style="color:#a31515;">"defcon2_assassination"</span>].OnStateTransitionedEvent                += (x, y) => MovedFrom_defcon2_To_defcon3;
        arcs[<span style="color:#a31515;">"defcon3_coup"</span>] = <span style="color:blue;">new </span><span style="color:#2b91af;">StateTransition </span>{
                     FromState = states[<span style="color:#2b91af;">MyStates</span>.defcon3],
                     ToState = states[<span style="color:#2b91af;">MyStates</span>.defcon1]
               };
        arcs[<span style="color:#a31515;">"defcon3_coup"</span>].OnStateTransitioningEvent                += (x, y) => MovingFrom_defcon3_To_defcon1;
        arcs[<span style="color:#a31515;">"defcon3_coup"</span>].OnStateTransitionedEvent                += (x, y) => MovedFrom_defcon3_To_defcon1;
    }


You can see that for each state and transition event I'm adding lambdas that invoke methods that are also being code generated. these are the partial methods described earlier. Here's the generator:

    
    foreach(XmlElement s in states)
        {
        WriteLine("partial void OnLeave_"+s.GetAttribute("ID")+"();");
        WriteLine("partial void OnEnter_"+s.GetAttribute("ID")+"();");
        }
    foreach(XmlElement s in trns)
        {
        WriteLine("partial void MovingFrom_"+s.GetAttribute("from")+"_To_"+s.GetAttribute("to")+"();");
        WriteLine("partial void MovedFrom_"+s.GetAttribute("from")+"_To_"+s.GetAttribute("to")+"();");
        }


Which gives us:

    
    <span style="color:blue;">partial void </span>OnLeave_defcon1();
    <span style="color:blue;">partial void </span>OnEnter_defcon1();
    <span style="color:blue;">partial void </span>OnLeave_defcon2();
    <span style="color:blue;">partial void </span>OnEnter_defcon2();
    <span style="color:blue;">partial void </span>OnLeave_defcon3();
    <span style="color:blue;">partial void </span>OnEnter_defcon3();
    <span style="color:blue;">partial void </span>MovingFrom_defcon1_To_defcon2();
    <span style="color:blue;">partial void </span>MovedFrom_defcon1_To_defcon2();
    <span style="color:blue;">partial void </span>MovingFrom_defcon2_To_defcon3();
    <span style="color:blue;">partial void </span>MovedFrom_defcon2_To_defcon3();
    <span style="color:blue;">partial void </span>MovingFrom_defcon3_To_defcon1();
    <span style="color:blue;">partial void </span>MovedFrom_defcon3_To_defcon1();


The C# 3.0 spec states that if you don't choose to implement one of these partial methods then the effect is similar to attaching a ConditionalAttribute to it - it gets taken out and no trace is left of it ever having been declared. That's nice, because for some state models you may not want to do anything other than make the transition.

We now have a working state machine with masses of extensibility points that we can use as we see fit. Say we decided to implement a few of these methods like so:

    
    <span style="color:blue;">public partial class </span><span style="color:#2b91af;">MyStateModel </span>{
        <span style="color:blue;">partial void </span>OnEnter_defcon1()
        {
            <span style="color:#2b91af;">Debug</span>.WriteLine(<span style="color:#a31515;">"Going Into defcon1."</span>);
        }
        <span style="color:blue;">partial void </span>OnEnter_defcon2()
        {
            <span style="color:#2b91af;">Debug</span>.WriteLine(<span style="color:#a31515;">"Going Into defcon2."</span>);
        }
        <span style="color:blue;">partial void </span>OnEnter_defcon3()
        {
            <span style="color:#2b91af;">Debug</span>.WriteLine(<span style="color:#a31515;">"Going Into defcon3."</span>);
        }
    }


Here's how you'd invoke it:

    
    <span style="color:#2b91af;">MyStateModel </span>dfa = <span style="color:blue;">new </span><span style="color:#2b91af;">MyStateModel</span>();
    dfa.ProcessInput((<span style="color:blue;">int</span>) <span style="color:#2b91af;">MyInputs</span>.diplomaticIncident);
    dfa.ProcessInput((<span style="color:blue;">int</span>) <span style="color:#2b91af;">MyInputs</span>.assassination);
    dfa.ProcessInput((<span style="color:blue;">int</span>) <span style="color:#2b91af;">MyInputs</span>.coup);


And here's what you'd get:

    
    Going Into defcon2.
    Going Into defcon3.
    Going Into defcon1.


There's a lot you can do to improve the model I've presented (like passing context info into the event handlers, and allowing some of the event handlers to veto state transitions). But I hope that it shows how the partials support in conjunction with T4 templates makes light work of this perennial problem. This could easily save you from writing thousands of lines of tedious and error prone boiler plate code. That for me is a complete no-brainer.

What I like about this model is the ease with which I was able to get code generation. I just added a file with extension '.tt' to VS 2008 and it immediately started generating C# from it. All I needed to do at that point was load up my XML file and feed it into the template. I like the fact that the system is lightweight. There is not a mass of framework that takes over the state management, it's infinitely extensible, and it allows a very quick turnaround time on state model changes.

What do you think? How would you tackle this problem?
