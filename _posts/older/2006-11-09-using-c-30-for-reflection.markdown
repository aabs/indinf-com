---
author: aabs
comments: true
date: 2006-11-09 03:26:02+00:00
excerpt: In this post I show you how to use the attribute metadata system to filter
  types using LINQ and C# 3.0. In past posts I’ve talked about a Design By Contract
  (DBC) system I use to generate pre and post conditions for methods on a class. In
  the post I’ll use Queries, Extension Methods, and iterators to show you how to get
  types for inserting into (something like) a code generation system. This one's long
  on code and short on talk for a change.
layout: post
link: https://aabs.wordpress.com/2006/11/09/using-c-30-for-reflection/
slug: using-c-30-for-reflection
title: Using C# 3.0 For Reflection
wordpress_id: 291
tags:
- C#
- Code Generation
---




C# in Orcas will provide a bunch of really useful tools for those who want to perform tasks involving reflection. The reflection APIs in C# are already excellent but with the new query capabilities provided by LINQ, Reflection will be a real pleasure. There’ve been a few excellent posts on the topic.




In this post I’ll show you how to use the attribute metadata system to filter types using LINQ. In past posts I’ve talked about the Design By Contract (DBC) system I used to generate pre and post conditions for methods on a class. In the post I’ll use Queries, Extension Methods, and iterators to show you how to get types for inserting into the code generation system. Just to recap, here’s how I annotate a class:




    
    [Dbc, Invariant(<span style="color:maroon;">"Prop1 >= 1"</span>)]
    <span style="color:blue;">public</span> <span style="color:blue;">class</span> MyTestClass
    {
        <span style="color:blue;">public</span> MyTestClass()
        {
            Prop1 = <span style="color:maroon;">10</span>;
        }
    
        <span style="color:blue;">int</span> prop1 = <span style="color:maroon;">0</span>;
        [Requires(<span style="color:maroon;">"value != 1561"</span>)]
        <span style="color:blue;">public</span> <span style="color:blue;">int</span> Prop1
        {
            <span style="color:blue;">get</span>
            {
                <span style="color:blue;">return</span> prop1;
            }
            <span style="color:blue;">set</span>
            {
                prop1 = value;
            }
        }
    
        [Requires(<span style="color:maroon;">"arg1 > 10"</span>)]
        [Requires(<span style="color:maroon;">"arg2 < 100"</span>)]
        [Ensures(<span style="color:maroon;">"$after(Prop1) == $before(Prop1) + 1"</span>)]
        <span style="color:blue;">public</span> <span style="color:blue;">void</span> TestMethod(<span style="color:blue;">int</span> arg1, <span style="color:blue;">int</span> arg2, <span style="color:blue;">string</span> arg3)
        {
            Prop1 = Prop1 + <span style="color:maroon;">1</span>;
            Debug.WriteLine(<span style="color:maroon;">"MyTestClass.TestMethod.Prop1 == {0}"</span>, prop1);
        }
    }




The class is annotated by a DbcAttribute plus some optional InvariantAttribute attributes. The members are optionally annotated with InvariantAttribute, RequiresAttribute and EnsuresAttribute. The code generator needs to create proxies for any classes that contain any of these attributes, but by convention I wrap only those classes that are adorned with the DbcAttribute to make life simpler. With queries we can do away with the DbcAttribute, but at the cost of having to do more itieration which will affect performance. This will be an issue if you are using the queries for dynamic proxy generation. If you perform static code generation it’s less of an issue. We first need a query to check whether a class has the DbcAttribute.




    
    <span style="color:blue;">public</span> <span style="color:blue;">static</span> <span style="color:blue;">bool</span> HasAttribute(<span style="color:blue;">this</span> Type t, Type attrType)
    {
        <span style="color:blue;">return</span> t.GetCustomAttributes(attrType, <span style="color:maroon;">true</span>).Count() > <span style="color:maroon;">0</span>;
    }


HasAttribute is an extension method that uses the Count() extension method from System.Query to get the number of elements in the collection returned. It just declares if there is more than one of them. Now we can perform a query to get all classes that have the attribute: 

    
    <span style="color:blue;">private</span> <span style="color:blue;">void</span> Example1()
    {
        IEnumerable<Type> annotated =
    	from t <span style="color:blue;">in</span> GetType().Assembly.GetTypes()
    	where t.HasAttribute(<span style="color:blue;">typeof</span>(DbcAttribute))
    	select t;
        Console.WriteLine(<span style="color:maroon;">"classes: {0}"</span>, annotated.Count());
        Console.ReadKey();
    }


This is now every class that needs to have code generated for it. Now we can enumerate the collection of members of the classes.



    
    <span style="color:blue;">private</span> <span style="color:blue;">static</span> <span style="color:blue;">void</span> Example2()
    {
        Type T = <span style="color:blue;">typeof</span>(MyTestClass);
        <span style="color:blue;">int</span> count = T.GetCustomAttributes(<span style="color:blue;">typeof</span>(DbcAttribute), <span style="color:maroon;">true</span>).Count();
        IEnumerable<Type> annotated = from t <span style="color:blue;">in</span> <span style="color:blue;">typeof</span>(Program).Assembly.GetTypes()
                                     where t.HasAttribute(<span style="color:blue;">typeof</span>(DbcAttribute))
                                      select t;
        Console.WriteLine(<span style="color:maroon;">"classes: {0}"</span>, annotated.Count());
        Console.ReadKey();
        <span style="color:blue;">foreach</span> (MemberInfo mi <span style="color:blue;">in</span> GetAllMembers(T))
        {
            Console.WriteLine(<span style="color:maroon;">"member: {0}"</span>,mi.Name);
            DisplayAttributes<InvariantAttribute>(mi);
            DisplayAttributes<RequiresAttribute>(mi);
            DisplayAttributes<EnsuresAttribute>(mi);
        }
        Console.ReadKey();
    }
    
    <span style="color:blue;">private</span> <span style="color:blue;">static</span> <span style="color:blue;">void</span> DisplayAttributes<AttrType>(MemberInfo mi) where AttrType : DbcPredicateAttribute
    {
        <span style="color:blue;">foreach</span> (AttrType ra <span style="color:blue;">in</span> GetAllAttributes<AttrType>(mi))
        {
            Console.WriteLine(<span style="color:maroon;">"tAttribute: {0} ({1})"</span>, ra.Predicate, <span style="color:blue;">typeof</span>(AttrType).Name);
        }
    }
    
    <span style="color:blue;">private</span> <span style="color:blue;">static</span> IEnumerable<MemberInfo> GetAllMembers(Type t)
    {
        <span style="color:blue;">foreach</span> (FieldInfo fi <span style="color:blue;">in</span> t.GetFields())
            <span style="color:blue;">yield</span> <span style="color:blue;">return</span> fi;
        <span style="color:blue;">foreach</span> (PropertyInfo pi <span style="color:blue;">in</span> t.GetProperties())
            <span style="color:blue;">yield</span> <span style="color:blue;">return</span> pi;
        <span style="color:blue;">foreach</span> (MethodInfo mi <span style="color:blue;">in</span> t.GetMethods())
            <span style="color:blue;">yield</span> <span style="color:blue;">return</span> mi;
        <span style="color:blue;">foreach</span> (EventInfo ei <span style="color:blue;">in</span> t.GetEvents())
            <span style="color:blue;">yield</span> <span style="color:blue;">return</span> ei;
    }
    
    <span style="color:blue;">private</span> <span style="color:blue;">static</span> IEnumerable<AttrType> GetAllAttributes<AttrType>(MemberInfo mi) where AttrType : Attribute
    {
        <span style="color:blue;">foreach</span> (AttrType ra <span style="color:blue;">in</span> mi.GetCustomAttributes(<span style="color:blue;">typeof</span>(AttrType), <span style="color:maroon;">true</span>)
                               .Where(t => t.GetType().Equals(<span style="color:blue;">typeof</span>(AttrType))))
        {
            <span style="color:blue;">yield</span> <span style="color:blue;">return</span> ra <span style="color:blue;">as</span> AttrType;
        }
    }




Now we have iterators for all classes that are adorned with a specific attribute, and likewise for all members of those classes. Each member can have specific attributes iterated as well. That’s nice, but we really need a clean interface to provide iterators over a class.




    
    <span style="color:blue;">public</span> <span style="color:blue;">static</span> <span style="color:blue;">class</span> DbcClassIterator
    {
        <span style="color:blue;">public</span> <span style="color:blue;">static</span> IEnumerable<Type> DbcTypes(<span style="color:blue;">this</span> Assembly asm)
        {
            <span style="color:blue;">return</span> from t <span style="color:blue;">in</span> asm.GetTypes()
                   where t.HasAttribute(<span style="color:blue;">typeof</span>(DbcAttribute))
                   select t;
        }
    
        <span style="color:blue;">public</span> <span style="color:blue;">static</span> IEnumerable<MemberInfo> Members(<span style="color:blue;">this</span> Type t)
        {
            <span style="color:blue;">foreach</span> (FieldInfo fi <span style="color:blue;">in</span> t.GetFields())
                <span style="color:blue;">yield</span> <span style="color:blue;">return</span> fi;
            <span style="color:blue;">foreach</span> (PropertyInfo pi <span style="color:blue;">in</span> t.GetProperties())
                <span style="color:blue;">yield</span> <span style="color:blue;">return</span> pi;
            <span style="color:blue;">foreach</span> (MethodInfo mi <span style="color:blue;">in</span> t.GetMethods())
                <span style="color:blue;">yield</span> <span style="color:blue;">return</span> mi;
            <span style="color:blue;">foreach</span> (EventInfo ei <span style="color:blue;">in</span> t.GetEvents())
                <span style="color:blue;">yield</span> <span style="color:blue;">return</span> ei;
        }
    
        <span style="color:blue;">public</span> <span style="color:blue;">static</span> IEnumerable<AttrType> Attributes<AttrType>(<span style="color:blue;">this</span> MemberInfo mi)
            where AttrType : Attribute
        {
            <span style="color:blue;">foreach</span> (AttrType ra <span style="color:blue;">in</span> mi.GetCustomAttributes(<span style="color:blue;">typeof</span>(AttrType), <span style="color:maroon;">true</span>)
                          .Where(t => t.GetType().Equals(<span style="color:blue;">typeof</span>(AttrType))))
            {
                <span style="color:blue;">yield</span> <span style="color:blue;">return</span> ra <span style="color:blue;">as</span> AttrType;
            }
        }
    
        <span style="color:blue;">public</span> <span style="color:blue;">static</span> IEnumerable<AttrType> Attributes<AttrType>(<span style="color:blue;">this</span> IEnumerable<MemberInfo> emi)
            where AttrType : Attribute
        {
            <span style="color:blue;">foreach</span> (MemberInfo mi <span style="color:blue;">in</span> emi)
            {
                <span style="color:blue;">foreach</span> (AttrType ra <span style="color:blue;">in</span> mi.GetCustomAttributes(<span style="color:blue;">typeof</span>(AttrType), <span style="color:maroon;">true</span>)
                        .Where(t => t.GetType().Equals(<span style="color:blue;">typeof</span>(AttrType))))
                {
                    <span style="color:blue;">yield</span> <span style="color:blue;">return</span> ra <span style="color:blue;">as</span> AttrType;
                }
            }
        }
    }


This class makes heavy use of Extension methods to allow the comfortable treatment of framework classes for our reflection tasks. Now we can iterate over the collections in various ways. 

    
    <span style="color:blue;">private</span> <span style="color:blue;">static</span> <span style="color:blue;">void</span> Example3()
    {
        Type t = <span style="color:blue;">typeof</span>(MyTestClass);
        <span style="color:blue;">foreach</span> (Type type <span style="color:blue;">in</span> t.Assembly.DbcTypes())
        {
            <span style="color:green;">// just get every requires attribute in the type
    </span>        <span style="color:blue;">foreach</span> (DbcPredicateAttribute pa <span style="color:blue;">in</span> type.Members().Attributes<RequiresAttribute>())
            {
                Console.WriteLine(<span style="color:maroon;">"predicate: {0}"</span>, pa.Predicate);
            }
            Console.ReadKey();
            <span style="color:green;">// get all members and then get all attributes in order.
    </span>        <span style="color:blue;">foreach</span> (MemberInfo mi <span style="color:blue;">in</span> type.Members())
            {
                Console.WriteLine(<span style="color:maroon;">"member: {0}"</span>, mi.Name);
                mi.DisplayAttributes<InvariantAttribute>();
                mi.DisplayAttributes<RequiresAttribute>();
                mi.DisplayAttributes<EnsuresAttribute>();
            }
        }
        Console.ReadKey();
    }


It’s really pretty neat. It doesn’t do anything we couldn’t do before but it does it in a way that makes it seem as though the framework classes provide all of the features we need from them out of the box. I like that, it makes code much more readable.
