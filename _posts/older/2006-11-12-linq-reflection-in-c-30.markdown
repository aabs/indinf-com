---
author: aabs
comments: true
date: 2006-11-12 22:33:34+00:00
layout: post
link: https://aabs.wordpress.com/2006/11/13/linq-reflection-in-c-30/
slug: linq-reflection-in-c-30
title: LINQ & Reflection in C# 3.0
wordpress_id: 294
tags:
- C#
- Code Generation
---

I posted an article the other day showing you how to exploit the query
capabilities of LINQ to do reflection over the attributes on a C# class. I want
to show you how to exploit some of the extension methods in System.Query to make
the code even cleaner. I used a method called Members that got all of the
members in order of member type (i.e. fields first, properties next and so on).
The code looked like this:



    
    <span style="color:blue;">
    
    
    <span style="color:blue;">public</span> <span style="color:blue;">static</span> IEnumerable<MemberInfo>Members(<span style="color:blue;">this</span> Type t)
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







I needed to split the queries into each of the types we required in order to
get elements ordered by type. Well System.Query provides a neater way to do this
sort of thing. As luck would have it, I don't care what order I bring the
members back in so long as they are grouped by type and then by name. We can use
the ordering queries in LINQ to do this:



    
    <span style="color:blue;">foreach</span> (MemberInfo mi <span style="color:blue;">in</span> from m <span style="color:blue;">in</span> t.GetMembers() orderby m.GetType().Name, m.Name select m)
    {
        <span style="color:blue;">yield</span> <span style="color:blue;">return</span> mi;
    }




Much cleaner, and it also grabs the constructors which I forgot to add in the
previous post! ;-) The query syntax there is equivalent to the following C# 2.0
syntax:



    
    <span style="color:blue;">foreach</span> (MemberInfo mi <span style="color:blue;">in</span> t.GetMembers().OrderBy(a => a.GetType().Name).ThenBy(b => b.Name))
    {
        <span style="color:blue;">yield</span> <span style="color:blue;">return</span> mi;
    }


I can impose some specific order by using the union method:



    
    <span style="color:blue;">foreach</span> (MemberInfo mi <span style="color:blue;">in</span> t.GetFields().Union<MemberInfo>(t.GetProperties().Union<MemberInfo>(t.GetMethods())))
    {
        <span style="color:blue;">yield</span> <span style="color:blue;">return</span> mi;
    }


We can mix and match these operators since they are all extension
methods on the IEnumerable<T> or IEnumerable types. The methods are
defined in System.Query.Sequence, so you can use the object browser or reflector
to find out what's available.

In the code generation patterns posts I wrote last year, I demonstrated that
we often need to recursively enumerate structural elements in assemblies or
databases. Here is an untyped example of how we can apply these enumerators to
simply dig through the capabilities of the type:



    
    <span style="color:blue;">public</span> <span style="color:blue;">static</span> IEnumerable<<span style="color:blue;">object</span>> ParseType(Type t)
    {
        <span style="color:blue;">foreach</span> (MemberInfo mi <span style="color:blue;">in</span> t.Members())
        {
            <span style="color:blue;">yield</span> <span style="color:blue;">return</span> mi;
            <span style="color:blue;">foreach</span> (<span style="color:blue;">object</span> x <span style="color:blue;">in</span> mi.Attributes<DbcPredicateAttribute>())
            {
                <span style="color:blue;">yield</span> <span style="color:blue;">return</span> x;
            }
        }
    }




You can see that now the code required to enumerate the type is very simple.
But we haven't gained simplicity at the expense of power. We can explore the
object in any way we please.
