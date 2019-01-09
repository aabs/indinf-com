---
author: aabs
comments: true
date: 2007-09-11 04:55:12+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2007/09/11/continue-if-not-null-operator-yes-please/
slug: continue-if-not-null-operator-yes-please
title: Continue if not null operator? Yes please!
wordpress_id: 424
tags:
- C#
---

[Olmo](http://forums.microsoft.com/MSDN/User/Profile.aspx?UserID=695544&SiteID=1) made a very worthwhile [suggestion ](http://forums.microsoft.com/MSDN/ShowPost.aspx?PostID=2033762&SiteID=1)on the LINQ forums recently. His suggestion was for a new operator to be added to the C# language to allow us to do away with the following kind of pesky construct:




    
    
    
    
     [sourcecode language='csharp']string x;
    if(a != null &amp;&amp; a.Address != null &amp;&amp; a.Address.FirstLine != null)
        x = a.Address.FirstLine;[/sourcecode]



instead he suggested a new **?.** operator so that we could produce something like this:

    
    
    
    
     [sourcecode language='csharp']x = a?.Address?.FirstLine;[/sourcecode]



and at the first sign of failure it would just yield a null or the equivalent. I like this suggestion, it is elegant, and provides a similar meaning to the coalescing operator.  The C# team have been adding lots of these little bits of syntactic sugar recently, so why not add another that will save us lots of thrown exceptions or grisly if statements?
