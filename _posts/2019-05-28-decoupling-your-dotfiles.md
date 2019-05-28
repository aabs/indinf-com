---
layout: post
title: Decoupling your dotfiles with Fish Shell
---

# Decoupling your dotfiles with Fish Shell

Fish shell has a very handy mechanism - a function called `emit` - to decouple
your functions and scripts.  It allows you to publish events without havinbg to
worry too much about how they are to be dealt with.  Lovers of architecture will
see parallels between the mechanism and design paradigms such as message
oriented middleware, ESBs and delegates in languages like C#.

As I continue to flesh out the plugins for fishdots, I've found that there are
often numerous ways to handle a message of a given type.  It came to a head,
when I decided to add a '_transcript_' plugin that would keep a written record
of everything that was going on.

<!--  
To cover:
- event handlers and functions
- the fishdots plugin model
- subcommand pattern
- emitting from the dispatcher
- using JSON and JQ to format messages
- An example: handling tasks via TW and IFTTT/iOS
-->

<!--  
vim: tw=80 wrap sw=2 ts=2
-->
