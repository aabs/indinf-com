---
author: aabs
comments: true
date: 2006-11-28 01:41:01+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2006/11/28/automated-parallelization-of-sequential-systems/
slug: automated-parallelization-of-sequential-systems
title: Automated parallelization of Sequential Systems
wordpress_id: 314
tags:
- C#
- Computer Science
- LINQ
---

I recently came across the [Dryad](http://research.microsoft.com/research/sv/dryad/) project at MS Research. It is concerned with the development of automated systems to deploy sequential systems across parallel platforms. This is an interesting and topical issue at the moment because the advent of multi-core processors poses problems for developers. In C#, at the very least, there is no inherent parallelization of the language. Developers have to explicitly code a system with threading or [grid computing](http://gridenvy.wordpress.com/) in mind. Coding in such a way is generally pretty hard, and fraught with peril to the unwary programmer, so any system that can ease the work required is likely to be finding its way into the .NET platform in the near future.

My impression from Dryad is that to make use of the multi-core processors we may have to change the way we design systems to allow automated parallelization. The Dryad project talks about a multileveled description of a system that incorporates declarations of the dataflow of the system as well as primitive operations to be performed on them. It seems to be targeted more at the massively computationally intensive operations traditionally performed in giant vector processors. What does it mean to those of us who develop distributed applications on web farms? I'm not sure, but I think that the trend will be the hot topic of the next few years.

At Tech Ed Sydney 2006, this issue was [highlighted](http://callvirt.net/files/DEV318.zip) by [Joel Pobar](http://callvirt.net/blog/) – he pointed out that the declarative elements of technologies like [LINQ](http://msdn.microsoft.com/data/ref/linq/default.aspx?pull=/library/en-us/dndotnet/html/linqprojectovw.asp) and [F#](http://research.microsoft.com/projects/ilx/fsharp.aspx) provide a [window of opportunity ](http://www.bluebytesoftware.com/blog/PermaLink,guid,81ca9c00-b43e-4860-b96b-4fd2bd735c9f.aspx)to allow the runtime to incorporate structures that will make the life of systems like Dryad easier. An interesting trend is the gradual productisation of automated analysis and proof systems. The coolest thing I've seen lately is the [Spec#](http://research.microsoft.com/SpecSharp/) project, again from MS Research which augments the C# IDE with various kinds of static analysis and automated theorem provers. For a while, I was a researcher on the [RAPIER](http://www.it-innovation.soton.ac.uk/rapier/summary.htm) at the Parallel Application Centre at the University Of Southampton, UK. The project sought to integrate design, development, simulation and theorem proof tools for the development of real-time systems. I can't wait till such tools become the conventional static error checkers of IDEs like Visual Studio. It is discouraging that such tools are still not in the mainstream after 10 years of progress during which time software systems have become exponentially more complex.
