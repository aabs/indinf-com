---
author: aabs
comments: true
date: 2005-04-24 17:28:00+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2005/04/24/predicates-and-expressions-part-1-3/
slug: predicates-and-expressions-part-1-3
title: Predicates and Expressions - Part 1
wordpress_id: 216
tags:
- DBC
---

The predicates that are used to specify the _in_, _out_ and _inv_ conditions on a component need to follow certain rules. The first and least obvious is that it must follow the syntax and semantic rules of the C# language. I say that it is the least obvious, since it seems to be conventional to invent a new predicate language for these purposes. I can see the point of this, since it would allow a tailor made language to be used, and to extend it beyond the capabilities of a normal C# expression. I intend to start off with the syntax of C# because they will have to end up as C# expressions, and I want to have to intervene in the simplest way possible. I want the intervention to be achievable via regex replacements where possible.

It it essential to make one significant enhancement to the expression language of C#. It needs to be temporal. It needs to be able to make statements about the value of a variable before and after a method has been invoked. In a temporal logic that might be represented by a prime (i.e. x'). That could get rather confusing when we are making comparisons with characters that need to be enclosed by single quotes. At the very least it would be hard to read. Likewise we could represent it as a function which can be made available through a supertype or a utility object. An expression might resemble this: "before(x) == after(x) - 1". The problem is that this pollutes the method namespace so we would have to have something like this in the expressions: "base.before(x)". Which is already looking ugly, and it also forces the user to inherit from a supertype that may wreck their application design.

It seems that we need _pragmas_ - preprocessor directives that can be used as clues to the framework. Since we are using the syntax of C# we can be sure that neither types nor member names will begin with a dollar sign. It seems safe to assume that we can begin pragmas with a dollar sign like so: "$before(x) == $after(x) + 1". It should not be confused with interpreted scripting language though, this is input for a pre-processor that outputs code, which is subsequently compiled. That applies whether we are performing dynamic code generation.

The sharp-eyed of you will notice that in various kinds of predicate the $before(x) and $after(x) are synonymous with **_x_** itself. For example it makes no sense to refer to a $after(x) in a _requires_ predicate. Likewise, there isn't much use for an $after() pragma since we are inserting code into the method of a target object. x will already embody the value in $after(x) so the $after() will just be getting in the way. So, we only need a $before(x) pragma
to take a snapshot of the value of the variable at the beginning of the method, for later comparison with what the variable becomes.

There are some rules that predicates must obey, and we just saw one of them before. In a pre-condition, $before(x) and x are synonymous, and therefore $before() is meaningless, and shouldn't be found there. This sort of rule can be detected and converted into harmless code by the preprocessor. Another hard and fast rule is that predicates must **_never_** have side-effects. That is - the state before testing the predicate should be exactly the same as the state after the test. There are circumstances in which that is untrue. If a predicate detects a bug, it may throw an exception, which may cause the stack to unwind, which will have a side effect. But that is a desired side effect that is part of the semantics of design by contract and error detection, as well as being permissible in a constant method (assuming such a thing were possible in C#). What I mean is _undesired side-effects_ - side effects that are not implicit in the error checking framework, and which are not testable by the predicates themselves.

There is a need to avoid harmful code that is harder to spot. The predicates should be without side-effects. It's easy enough to spot when someone accidentally inserts a "=" instead of a "==", but if they code a helper method such as "IsOkToDoIt(this)" that happens to change a variable as it runs, then the predicates change from being an external test of correctness into another kind of source code. We need to prevent this - we are interested in making statements about the state of the class, and how it changes from moment to moment. We don't need to be concerned with making changes to it, since that is what the component code is for.

There is no way to prevent the predicates from having a side effect. Fullstop! Surprisingly C# doesn't support the ability to define constant methods. Constant methods are a powerful feature in the armoury of the C++ library writer. It makes it possible to assert the fact that a method has no side effects. In C# you are not able to make that kind of assertion. So we will just have to make it known to the DBC-driven programmer that coding predicates that have side-effects is asking for misery.
