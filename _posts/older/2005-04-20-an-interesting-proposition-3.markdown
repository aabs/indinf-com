---
author: aabs
comments: true
date: 2005-04-20 03:04:00+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2005/04/20/an-interesting-proposition-3/
slug: an-interesting-proposition-3
title: An interesting proposition
wordpress_id: 215
tags:
- Code Generation
- DBC
---

As you will recall from the previous post, I have been wondering about how to implement the functionality for dynamic proxies. Well I saw [this ](http://www.theserverside.net/articles/showarticle.tss?id=AspectOrientingNET)great article, by Viji Sarathy, on the web, and think that this might be the way to go.

It uses Context Bound Objects to ensure the interception of all calls by the CLR without any explicit framework setup by the user. We can guarantee that the contracts get followed, whether the user tries to evade them or not.

The only misgiving I have is over the performance characteristics of the thing. Any thoughts?
