---
author: aabs
comments: true
date: 2005-08-04 20:22:00+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2005/08/04/dbc-in-the-persistence-broker-2/
slug: dbc-in-the-persistence-broker-2
title: Dbc in the persistence broker
wordpress_id: 176
tags:
- DBC
- ORM
---

The work is well under way now, and I have reverse engineering, code generation and configuration pretty much sorted out, with a bit of code generation to make the whole thing compile nicely within an AabsNorm generated nant script that will take care of building the domain model, base objects, interfaces, mock objects etc.

At that point I shall put a bit of effort into making some WIX installers to allow me to quickly set up a development environment. That way the volunteers will come flooding in right?

Anyway, I've been using AabsNorm as a test bed for my last open source project Aabs.Dbc, a design by contract framework in C#. [Here](http://aabsdotnet.blogspot.com/2005/08/dbc-in-use.html) you'll find a sample of how the AabsNorm code will look when it gets released next.
