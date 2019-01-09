---
author: aabs
comments: true
date: 2009-03-31 05:58:49+00:00
layout: post
link: https://aabs.wordpress.com/2009/03/31/does-this-seem-nice-to-you/
slug: does-this-seem-nice-to-you
title: Does this seem nice to you?
wordpress_id: 625
categories:
- .NET
- humour
- programming
---

After years of recoiling at the sight of code like this, am I supposed now to embrace it in a spirit of reconciliation?

 
    
    <span style="color:blue;">namespace </span>ConsoleApplication1
    {
        <span style="color:blue;">class </span><span style="color:#2b91af;">Program
        </span>{
            <span style="color:blue;">static void </span>Main(<span style="color:blue;">string</span>[] args)
            {
                <span style="color:blue;">dynamic </span>blah = GetTheBlah();
                <span style="color:#2b91af;">Console</span>.WriteLine(blah);
            }
    
            <span style="color:blue;">private static dynamic </span>GetTheBlah()
            {
                <span style="color:blue;">if </span>(<span style="color:#2b91af;">DateTime</span>.Now.Millisecond % 3 == 0)
                    <span style="color:blue;">return </span>0;
                <span style="color:blue;">else
                    return </span><span style="color:#a31515;">"hello world!"</span>;
            }
        }
    }


[](http://11011.net/software/vspaste)



need to wash my hands.
