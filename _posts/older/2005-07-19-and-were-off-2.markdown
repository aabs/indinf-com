---
author: aabs
comments: true
date: 2005-07-19 16:04:00+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2005/07/19/and-were-off-2/
slug: and-were-off-2
title: And we're off
wordpress_id: 172
tags:
- DBC
---

I have set up the sourceforge project. You can find it [here](http://sourceforge.net/projects/aabsnorm/). I've also classified all of the work, and split it up into releases.

Here's what will go into release one:
<table ><tbody >
<tr >

<td >Configuration
</td>

<td >Use native .NET configuration
</td>
</tr>
<tr >

<td >Configuration
</td>

<td >Remove existing config assembly
</td>
</tr>
<tr >

<td >Installers
</td>

<td >WIX installers
</td>
</tr>
<tr >

<td >Runtime Control
</td>

<td >Add transactional support from COM+
</td>
</tr>
<tr >

<td >Runtime Control
</td>

<td >Extend reverse engineering to examine SPs and create wrappers for them.
</td>
</tr>
<tr >

<td >Runtime Control
</td>

<td >Configurable ID strategy
</td>
</tr>
<tr >

<td >Runtime Control
</td>

<td >Configurable transaction isolation policy
</td>
</tr>
<tr >

<td >Templates
</td>

<td >Move core templates into resource assembly
</td>
</tr>
<tr >

<td >Testing
</td>

<td >Create a proper test database
</td>
</tr>
<tr >

<td >Runtime Control
</td>

<td >Divide system between runtime and development projects
</td>
</tr>
<tr >

<td >Runtime Control
</td>

<td >Standardise all names to CamelCase
</td>
</tr>
</tbody></table>
I think the highest priority is the configuration rework. Configuration in the previous system was way too complicated. What we need is a very simple, very reliable system that can easily be expanded to accommodate something like the config app block at a later date. As soon as that is done, the key task will be converting it from its current broken state to a working state, and then splitting the system up into runtime and development arms. I will also do some work towards creating WIX installers for the runtime and development systems, including an installer for packaging source releases, that will allow the easy setup of a development environment for new volunteers on the project.

This is of course based on the "_if you build it, they will come_" model of open source development.
