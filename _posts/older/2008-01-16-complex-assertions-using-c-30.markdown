---
author: aabs
comments: true
date: 2008-01-16 03:44:35+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2008/01/16/complex-assertions-using-c-30/
slug: complex-assertions-using-c-30
title: Complex Assertions using C# 3.0
wordpress_id: 487
categories:
- SemanticWeb
---

Recently I [attempted](http://aabs.wordpress.com/2007/10/20/lambda-functions-for-design-by-contract/) to implement a declarative predicate checking system to allow design by contract (DBC) within C# 3.0. I was not successful due to a limitation in the kind of parameters one can pass to an Attribute constructor in .NET (no lambdas). I thought I'd just follow that up with a simpler model based on extension methods.
    
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">Predicates
    </span>{
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">void</span> Assert<T>(<span style="color:rgb(0,0,255);">this</span> T obj, <span style="color:rgb(43,145,175);">Func</span><T, <span style="color:rgb(0,0,255);">bool</span>> pred)
        {
            <span style="color:rgb(0,0,255);">if</span> (!pred(obj))
                <span style="color:rgb(0,0,255);">throw</span> <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">ApplicationException</span>();
        }
    }




This simple extension method can be attached to any object allowing Ensures and Requires like this.
    
    <span style="color:rgb(0,0,255);">int</span> MyIntProp{<span style="color:rgb(0,0,255);">get</span>;<span style="color:rgb(0,0,255);">set</span>;}
    
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> MyMethod()
    {
        <span style="color:rgb(0,0,255);">this</span>.Assert(x => x.MyIntProp < 10);
        MyIntProp += 10;
        <span style="color:rgb(0,0,255);">this</span>.Assert(x => x.MyIntProp >= 10);
    }
    




[](http://11011.net/software/vspaste)This is a nice clear implementation that is good for validation. But I think that I can extend it further by exploiting serialization of snapshots within a scope to allow before/after analysis within the scope. Here's what I want to be able to write:
    
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> MyBetterMethod()
    {
        <span style="color:rgb(0,0,255);">this</span>.Require(x => x.MyIntProp < 10);
        MyIntProp += 10;
        <span style="color:rgb(0,0,255);">this</span>.Ensure(x => x.MyIntProp == x.before().MyIntProp + 10);
    }




Well, my recent writings about the Ambient Context pattern might give you a clue about how I would manage the scope. The first thing I need to be able to do is store a snapshot of the object before it gets tested by the Require. I chose an IDisposable object so that I can clean up after myself without the danger of having the serialized guts of objects lying around everywhere.
    
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">PredicateScope</span> : <span style="color:rgb(43,145,175);">IDisposable
    </span>{
        [<span style="color:rgb(43,145,175);">ThreadStatic</span>]
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(43,145,175);">Stack</span><<span style="color:rgb(43,145,175);">PredicateScope</span>> Scopes = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">Stack</span><<span style="color:rgb(43,145,175);">PredicateScope</span>>();
        <span style="color:rgb(0,0,255);">internal</span> <span style="color:rgb(0,0,255);">readonly</span> <span style="color:rgb(43,145,175);">Dictionary</span><<span style="color:rgb(0,0,255);">object</span>, <span style="color:rgb(0,0,255);">string</span>> Snapshots = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">Dictionary</span><<span style="color:rgb(0,0,255);">object</span>, <span style="color:rgb(0,0,255);">string</span>>();
        <span style="color:rgb(0,0,255);">internal</span> <span style="color:rgb(0,0,255);">readonly</span> <span style="color:rgb(43,145,175);">Dictionary</span><<span style="color:rgb(0,0,255);">object</span>, <span style="color:rgb(0,0,255);">object</span>> DeserializedSnapshots = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">Dictionary</span><<span style="color:rgb(0,0,255);">object</span>, <span style="color:rgb(0,0,255);">object</span>>();
    
        <span style="color:rgb(0,0,255);">public</span> PredicateScope(<span style="color:rgb(0,0,255);">params</span> <span style="color:rgb(0,0,255);">object</span>[] objects)
        {
            <span style="color:rgb(0,0,255);">foreach</span> (<span style="color:rgb(0,0,255);">object</span> obj <span style="color:rgb(0,0,255);">in</span> objects)
            {
                Snapshots.Add(obj, CreateSnapShot(obj));
            }
            Scopes.Push(<span style="color:rgb(0,0,255);">this</span>);
        }
        <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">string</span> CreateSnapShot(<span style="color:rgb(0,0,255);">object</span> obj)
        {
    
    
            <span style="color:rgb(43,145,175);">XmlSerializer</span> serializer = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">XmlSerializer</span>(obj.GetType());
            <span style="color:rgb(43,145,175);">StringWriter</span> sr = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">StringWriter</span>();
            serializer.Serialize(sr, obj);
            <span style="color:rgb(0,0,255);">return</span> sr.ToString();
    

[](http://11011.net/software/vspaste)
    
        }
    
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> Dispose()
        {
            Snapshots.Clear();
            Scopes.Pop();
        }
    }




You just pass the scope object whatever objects you intend to test later on. It takes snapshots of the objects and stores them away for later reference. It also maintains a stack, so it can be nested. Strictly speaking this is unnecessary, but I figure it might come in handy later on.




My Assertion methods are pretty much the same, but they're now augmented by a "before" extension method that will get a snapshot keyed to the object it's extending, and return that instead.
    
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">Predicates
    </span>{
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">void</span> Require<T>(<span style="color:rgb(0,0,255);">this</span> T obj, <span style="color:rgb(43,145,175);">Func</span><T, <span style="color:rgb(0,0,255);">bool</span>> pred)
        {
            <span style="color:rgb(0,0,255);">if</span> (!pred(obj))
                <span style="color:rgb(0,0,255);">throw</span> <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">ApplicationException</span>();
        }
    
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">void</span> Ensure<T>(<span style="color:rgb(0,0,255);">this</span> T obj, <span style="color:rgb(43,145,175);">Func</span><T, <span style="color:rgb(0,0,255);">bool</span>> pred)
        {
            <span style="color:rgb(0,0,255);">if</span> (!pred(obj))
                <span style="color:rgb(0,0,255);">throw</span> <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">ApplicationException</span>();
        }
    
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">static</span> T before<T>(<span style="color:rgb(0,0,255);">this</span> T obj) <span style="color:rgb(0,0,255);">where</span> T : <span style="color:rgb(0,0,255);">class
    </span>    {
            <span style="color:rgb(0,0,255);">if</span> (obj == <span style="color:rgb(0,0,255);">null</span>)
                <span style="color:rgb(0,0,255);">throw</span> <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">ArgumentNullException</span>(<span style="color:rgb(163,21,21);">"obj cannot be null"</span>);
    
            <span style="color:rgb(43,145,175);">PredicateScope</span> ctx = <span style="color:rgb(43,145,175);">PredicateScope</span>.Scopes.Peek();
            <span style="color:rgb(0,0,255);">if</span> (ctx == <span style="color:rgb(0,0,255);">null</span>) <span style="color:rgb(0,0,255);">return</span> <span style="color:rgb(0,0,255);">default</span>(T);
    
            <span style="color:rgb(0,0,255);">if</span> (ctx.DeserializedSnapshots.ContainsKey(obj))
                <span style="color:rgb(0,0,255);">return</span> ctx.DeserializedSnapshots[obj] <span style="color:rgb(0,0,255);">as</span> T;
    
    
            <span style="color:rgb(0,0,255);">string</span> serializedObject = ctx.Snapshots[obj];
            <span style="color:rgb(43,145,175);">XmlSerializer</span> ser = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">XmlSerializer</span>(<span style="color:rgb(0,0,255);">typeof</span>(T));
            <span style="color:rgb(43,145,175);">XmlReader</span> reader = <span style="color:rgb(43,145,175);">XmlReader</span>.Create(<span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">StringReader</span>(serializedObject));
            <span style="color:rgb(0,0,255);">object</span> result = ser.Deserialize(reader);
            ctx.DeserializedSnapshots[obj] = result;
            <span style="color:rgb(0,0,255);">return</span> result <span style="color:rgb(0,0,255);">as</span> T;

[](http://11011.net/software/vspaste)[](http://11011.net/software/vspaste)
    
        }
    }




The before method gets the snapshot out of the scope, and returns that. You can then use it in your assertions in exactly the same way as the original object.
    
    [<span style="color:rgb(43,145,175);">TestFixture</span>, <span style="color:rgb(43,145,175);">DataContract</span>]
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">MyClass
    </span>{
        [<span style="color:rgb(43,145,175);">DataMember</span>]
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">int</span> MyInt { <span style="color:rgb(0,0,255);">get</span>; <span style="color:rgb(0,0,255);">set</span>; }
        [<span style="color:rgb(43,145,175);">Test</span>]
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> MyMethod()
        {
            <span style="color:rgb(0,0,255);">using</span> (<span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">PredicateScope</span>(<span style="color:rgb(0,0,255);">this</span>))
            {
                <span style="color:rgb(0,0,255);">this</span>.Require(x => x.MyInt < 10);
                MyInt += 10;
                <span style="color:rgb(0,0,255);">this</span>.Ensure(x => MyInt == x.before().MyInt + 10);
            }
        }
    }




Obviously, for production use you'd have to ensure this stuff didn't get run by using ConditionalAttribute. It would affect performance. But for debugging it can be a godsend.
