---
author: aabs
comments: true
date: 2007-01-29 03:37:42+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2007/01/29/c-automatic-properties-semantic-changes-to-help-cope-with-poor-syntax/
slug: c-automatic-properties-semantic-changes-to-help-cope-with-poor-syntax
title: 'C# Automatic Properties: Semantic changes to help cope with poor syntax? '
wordpress_id: 348
tags:
- C#
---

[Steve Eichert](http://steve.emxsoftware.com/LINQ/C+Automatic+Properties) and [Bart de Smet](http://community.bartdesmet.net/blogs/bart/archive/2006/11/08/C_2300_-Automatic-Properties.aspx) both posted last November about a new C# feature: Automatic Properties.


The language enhancement will convert code like this in a class definition:


public string MyProperty { get; set; }



 

Into something like this:


private string myProperty;  
public string MyProperty  
{  
 get{return myProperty;}  
 set{myProperty = value;}  
}


I've got to admit that I view this with a few misgivings. Primarily, I worry about how to interpret MyProperty in an abstract or virtual class. Will there be an implementation generated for an abstract class? Is the code generation skipped if the property is virtual? At first glance, it doesn't seem to enhance to the clarity of the language. Currently { get; set; } has only one meaning – _no implementation provided_! Now we will have to look at the context of the declaration to know if it really means that at all! What will this do for those porting C# 2 code to C# 3?


Some of the commenters on Steve's post suggested adding a new keyword to the language so that you could get code like this:


public property string MyProperty;


or


public readonly property string MyProperty;


It's definitely less ambiguous than the syntax proposed by Anders Hejlsberg. I just wonder why it is necessary at all. What encapsulation is provided by such a feature? What are you getting? A property provides a degree of encapsulation to a field, which is why people declare properties to wrap fields. It allows you to add validation or some other source for the data. I have no objection to the C# team providing as much syntactic sugar as they like, but when they change the semantics of the language, I think they should definitely look before they leap! The only advantage to be gained from it is less typing. Hejlsberg must _really _hate typing! You can't define a field in an interface so the _empty property anti-pattern_ is obligatory in C# 2.0. Why enshrine it in the language though?


Are empty properties another one of those half-understood injunctions that developers pay lip service to? (I lump [configuration](http://aabs.wordpress.com/configuration/) in this category) After all, the only point of a property is if you do something in it other than turn a private variable into a public one. Why not make fields declarable in interfaces? That way, we'd only have to encapsulate them if they weren't stored in the class or if we wanted to do something with them prior to changing state? It wouldn't change the semantics of the language either, and it would be backwards compatible.


If I was obliged to add Automatic Properties to the language I might be tempted to make use of an attribute to annotate a field instead:


[Property("MyProperty")] private string myField;


Attributes are an excellent way to pass on hints to the compiler. They are more versatile than keywords, and wouldn't require syntax changes. Keeping the language definition stable would probably please tool vendors too.
