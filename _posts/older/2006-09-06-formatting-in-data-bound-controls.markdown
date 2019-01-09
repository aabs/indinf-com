---
author: aabs
comments: true
date: 2006-09-06 03:15:59+00:00
layout: post
link: https://aabs.wordpress.com/2006/09/06/formatting-in-data-bound-controls/
slug: formatting-in-data-bound-controls
title: Formatting in Data Bound Controls
wordpress_id: 262
tags:
- ASP.NET
- programming
---

I recently ran across a problem with data bound controls in ASP.NET, where I set the string formatting specification (by the book) and nothing happened. If you come across this problem you are probably missing the HtmlEncode attribute on your column specification. Without this the braces are probably encoded and thus the formatting string is unrecognisable to the control. Consider adding the HtmlEncode="false" to your binding specifications - it worked for me.
