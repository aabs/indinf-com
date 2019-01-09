---
author: aabs
comments: true
date: 2008-05-25 13:34:26+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2008/05/25/announcing-linqtordf-v07/
slug: announcing-linqtordf-v07
title: Announcing LinqToRdf v0.7
wordpress_id: 535
categories:
- Semantic Web
- SemanticWeb
---

I'm proud to say that I've finally uploaded [version 0.7 of LinqToRdf](http://linqtordf.googlecode.com/files/LinqToRdf-0.7.msi). This is by far the best release of [LinqToRdf ](http://code.google.com/p/linqtordf/)(so far), and it now contains full support for relationship navigation out of the box. Relationship support has always been possible (provided you were intimately familiar with various undocumented parts of LINQ:), but with the automated support for instance URIs that came in version 0.6, it's now automatic.

The high points of the release are:

  * LinqToRdf and all its prerequisites are now installed to the Global Assembly Cache (GAC).  
  * The code generator creates EntitySet and EntityRef properties for the parent and child ends of class relationships. This provide a lazily loaded collection and instance management system - the same as is used in LINQ to SQL.  
  * All generated code makes use of partial classes to allow code to be extended cleanly with standard queries. This allows you to marry standard queries with the DataContext to allow easy relationship navigation while the DataContext is in scope.  
  * DataContext classes are automatically generated using a LINQ to SQL idiom that makes the code MUCH more readable. See below for an example of how clean LinqToRdf code looks these days.  
  * New meta-model query operators allow you to query by subject URI and Instance URI. This was to support relationship navigation, but it can be used anywhere. This is the first step in providing reification via LinqToRdf.  
  * Support for commonly used LINQ Standard Query Operators like Count, First, and FirstOrDefault. 

I've been meaning to do all of this for quite a while now, but it took some prompting by Carl Blakeley of OpenLink (back at the beginning of May) to force me to get my act together. My apologies to Carl for taking so long to come up with the goods - I'm sure you used EntitySets in the end, but now you have Instance URIs that you can plug into them to make the code cleaner. Enjoy.

The following is now actively supported by designer-generated code.
    
    <span style="color:blue;">public void </span>Foo()
    {
        <span style="color:blue;">var </span>ctx = <span style="color:blue;">new </span>MusicDataContext(<span style="color:#a31515;">@"Some SPARQL Endpoint"</span>);
        <span style="color:blue;">var </span>track = (<span style="color:blue;">from </span>t <span style="color:blue;">in </span>ctx.Tracks
                     <span style="color:blue;">where </span>t.HasInstanceUri(<span style="color:#a31515;">"Some Instance URI"</span>)
                     <span style="color:blue;">select </span>t).First();
    
        Debug.WriteLine(<span style="color:#a31515;">"Track was " </span>+ track.Title);
        Debug.WriteLine(<span style="color:#a31515;">"Album was " </span>+ track.Album.Name);
    
        <span style="color:blue;">foreach </span>(<span style="color:blue;">var </span>t <span style="color:blue;">in </span>track.Album.Tracks)
        {
            Debug.WriteLine(<span style="color:#a31515;">"Album Track was " </span>+ track.Title);
        }
    }
    




Please download it. Give it a try. Any and all feedback is much appreciated.
