---
author: aabs
comments: true
date: 2009-09-30 02:39:37+00:00
layout: post
link: https://aabs.wordpress.com/2009/09/30/postsharp-laos-beautiful-aop/
slug: postsharp-laos-beautiful-aop
title: PostSharp Laos - Beautiful AOP.
wordpress_id: 635
categories:
- .NET
- functional programming
- programming
tags:
- .NET
- AOP
- functional programming
- postsharp
- pure functions
---

I've recently been using [PostSharp ](http://msdn.microsoft.com/en-us/library/dd140063.aspx)1.5 ([Laos](http://doc.postsharp.org/1.5/##PostSharp.HxS/UserGuide/Laos/Overview.html)) to implement various features such as logging, tracing, API performance counter recording, and repeatability on the softphone app I've been developing. Previously, we'd been either using hand-rolled code generation systems to augment the APIs with IDisposable-style wrappers, or hand coded the wrappers within the implementation code. The problem was that by the time we'd added all of the above, there were hundreds of lines of code to maintain around the few lines of code that actually provided a business benefit.

Several years ago, when I worked for Avanade, I worked on a very large scale project that used the Avanade Connected Architecture ([ACA.NET](http://www.avanade.com/delivery/acanet/)) - a proprietary competitor for PostSharp. We found [Aspect Oriented Programming](http://en.wikipedia.org/wiki/Aspect-oriented_programming) (AOP) to be a great way to focus on the job at hand and reliably dot all the 'i's and cross all the 't's in a single step at a later date.

ACA.NET, at that time, used a precursor of the [P&P Configuration Application Block](http://msdn.microsoft.com/en-us/library/dd140063.aspx) and performed a form of post build step to create external wrappers that instantiated the aspect call chain prior to invoking your service method. That was a very neat step that could allow configurable specifications of applicable aspects. It allowed us to develop the code in a very naive in-proc way, and then augment the code with top level exception handlers, transactionality etc at the same time that we changed the physical deployment architecture. Since that time, I've missed the lack of such a tool, so it was a pleasure to finally acquaint myself  with PostSharp.

I'd always been intending to introduce PostSharp here, but I'd just never had time to do it. Well, I finally found the time in recent weeks and was able to do that most gratifying thing - remove and simplify code, improve performance and code quality, reduced maintenance costs and increased the ease with I introduce new code [policies](http://en.wikipedia.org/wiki/Policy-based_design) all in a single step. And all without even scratching the surface of what PostSharp is capable of.

Here's a little example of the power of AOP using PostSharp, inspired by [Elevate's memoize extension method](http://elevate.codeplex.com/sourcecontrol/changeset/view/44244?projectName=elevate#690734). We try to distinguish as many of our APIs as possible into Pure and Impure. Those that are impure get database locks, retry handlers etc. Those that are pure _in a [functional sense](http://en.wikipedia.org/wiki/Pure_function)_ can be cached, or [memoized](http://en.wikipedia.org/wiki/Memoize). Those that are not pure in a functional sense are those that while not saving any data still are not one-to-one between arguments and result, sadly that's most of mine (it's a distributed event driven app).

    
    [<span style="color:#2b91af;">Serializable</span>]
    <span style="color:blue;">public class </span><span style="color:#2b91af;">PureAttribute </span>: OnMethodInvocationAspect
    {
        <span style="color:#2b91af;">Dictionary</span><<span style="color:blue;">int</span>, <span style="color:blue;">object</span>> PreviousResults = <span style="color:blue;">new </span><span style="color:#2b91af;">Dictionary</span><<span style="color:blue;">int</span>, <span style="color:blue;">object</span>>();
    
        <span style="color:blue;">public override void </span>OnInvocation(MethodInvocationEventArgs eventArgs)
        {
            <span style="color:blue;">int </span>hashcode = GetArgumentArrayHashcode(eventArgs.Method, eventArgs.GetArgumentArray());
            <span style="color:blue;">if </span>(PreviousResults.ContainsKey(hashcode))
                eventArgs.ReturnValue = PreviousResults[hashcode];
            <span style="color:blue;">else
            </span>{
                eventArgs.Proceed();
                PreviousResults[hashcode] = eventArgs.ReturnValue;
            }
        }
    
        <span style="color:blue;">public int </span>GetArgumentArrayHashcode(<span style="color:#2b91af;">MethodInfo </span>method, <span style="color:blue;">params object</span>[] args)
        {
            <span style="color:#2b91af;">StringBuilder </span>result = <span style="color:blue;">new </span><span style="color:#2b91af;">StringBuilder</span>(method.GetHashCode().ToString());
    
            <span style="color:blue;">foreach </span>(<span style="color:blue;">object </span>item <span style="color:blue;">in </span>args)
                result.Append(item);
            <span style="color:blue;">return </span>result.ToString().GetHashCode();
        }
    }


[](http://11011.net/software/vspaste)

I love what I achieved here, not least for the fact that it took me no more than about 20 lines of code to do it. But that's not the real killer feature, for me. It's the fact that PostSharp Laos has [MulticastAttributes](http://doc.postsharp.org/1.5/##PostSharp.HxS/UserGuide/Laos/Multicasting/Overview.html), that allow me to apply the advice to numerous methods in a single instruction, or even numerous classes or even every method of every class of an assembly. I can specify what to attach the aspects to by using regular expressions, or wildcards. Here's an example that applies an aspect to all public methods in class MyServiceClass.

    
    [<span style="color:blue;">assembly</span>: Pure(
        AspectPriority = 2,
        AttributeTargetAssemblies = <span style="color:#a31515;">"MyAssembly"</span>,
        AttributeTargetTypes = <span style="color:#a31515;">"UiFacade.MyServiceClass"</span>,
        AttributeTargetMemberAttributes = <span style="color:#2b91af;">MulticastAttributes</span>.Public,
        AttributeTargetMembers = <span style="color:#a31515;">"*"</span>)]


[](http://11011.net/software/vspaste)

Here's an example that uses a wildcard to NOT apply the aspect to those methods that end in "Impl".

    
    [<span style="color:blue;">assembly</span>: Pure(
        AspectPriority = 2,
        AttributeTargetAssemblies = <span style="color:#a31515;">"MyAssembly"</span>,
        AttributeTargetTypes = <span style="color:#a31515;">"UiFacade.MyServiceClass"</span>,
        AttributeTargetMemberAttributes = <span style="color:#2b91af;">MulticastAttributes</span>.Public,
        AttributeExclude = <span style="color:blue;">true</span>,
        AttributeTargetMembers = <span style="color:#a31515;">"*Impl"</span>)]


Do you use AOP? What aspects do you use, other than the usual suspects above?
