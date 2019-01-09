---
author: aabs
comments: true
date: 2008-01-18 06:00:48+00:00
layout: post
link: https://aabs.wordpress.com/2008/01/18/c-by-contract-using-expression-trees/
slug: c-by-contract-using-expression-trees
title: C# by Contract - Using Expression Trees
wordpress_id: 488
categories:
- SemanticWeb
---

[Last time](http://aabs.wordpress.com/2008/01/16/complex-assertions-using-c-30/) I created a simple, but powerful, little [design by contract](http://en.wikipedia.org/wiki/Design_by_contract) library in C# 3.0. It took hardly any lines of code, and covered a broad range of possible usage scenarios. See [here](http://archive.eiffel.com/doc/manuals/technology/contract/), for more on DBC. One thing that bothered me was the fact that if something failed a check, it wouldn't tell what went wrong. I had a little free time today, so I thought I'd fix that. I wanted the exceptions it threw to have the text of the check we were performing. Developers need to see exactly what test failed. Generic "_assertion failed_" error messages are useless. 

In my previous [efforts](http://aabs.wordpress.com/2005/08/04/dbc-in-use-3/) at .NET DBC, I used strings to pass around the predicate body that I wished to evaluate. That allowed me to have a copy of the text handy, but with lots of drawbacks. In those frameworks, I created a new class that derived from the one being tested, but with the predicates inserted at the beginning and end of the methods they were attached to. That allowed me to do things like this:
    
    <strong>[Requires(<span style="color:rgb(163,21,21);">"arg1 > 10"</span>)]
    [Requires(<span style="color:rgb(163,21,21);">"arg2 < 100"</span>)]
    [Ensures(<span style="color:rgb(163,21,21);">"$after(Prop1) == $before(Prop1) + 1"</span>)]</strong>
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> TestMethod(<span style="color:rgb(0,0,255);">int</span> arg1, <span style="color:rgb(0,0,255);">int</span> arg2, <span style="color:rgb(0,0,255);">string</span> arg3)
    {
        Prop1 = Prop1 + 1;
        System.Diagnostics.Debug.WriteLine(<span style="color:rgb(163,21,21);">"MyTestClass.TestMethod.Prop1 == "</span> + prop1.ToString());
    }




This let me to test private fields and properties, but on the other hand it stopped me from testing sealed classes. There's trade-offs no matter what you do unless you control the compiler, as is the case of [Spec#](http://en.wikipedia.org/wiki/Spec_sharp), [Eiffel](http://en.wikipedia.org/wiki/Eiffel_%28programming_language%29) or [D](http://en.wikipedia.org/wiki/D_%28programming_language%29#Example_3). The attribute based approach is not dissimilar to [Spec#](http://research.microsoft.com/specsharp/), where a contract specification is part of the method signature rather than in the body of the method.
    
    <span style="color:rgb(0,0,255);">int</span> BinarySearch(<span style="color:rgb(0,0,255);">object</span>[ ]! a, <span style="color:rgb(0,0,255);">object</span> o, <span style="color:rgb(0,0,255);">int</span> lo, <span style="color:rgb(0,0,255);">int</span> hi)
        <strong>requires 0 <= lo && lo <= hi && hi <= a.Length;</strong>
    { . . . }




[](http://11011.net/software/vspaste)


The difference is that Spec# uses syntactical enhancements, whereas I used Attributes. As I mentioned in [another post](http://aabs.wordpress.com/2007/10/20/lambda-functions-for-design-by-contract/), you can't use lambda functions in Attributes, nor could you use Expression trees based on lambda functions because the attribute itself cannot be generic. Another major drawback of the textual approach shown earlier is that it isn't type-safe. You could type any old garbage into that string, and you'd never know till run-time. You don't get intellisense either. We need a better way. 




[Expression trees](http://weblogs.asp.net/scottgu/archive/2007/04/08/new-orcas-language-feature-lambda-expressions.aspx) are great for what we want, they _are_ strongly typed, they _can _be assigned from lambda functions and they _needn't be compiled _until they're needed. Another cool thing is that the changes needed to use lambda expressions are trivial. Here's the Require extension method I showed you last time. It uses Expressions now.
    
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">void</span> Require<T>(<span style="color:rgb(0,0,255);">this</span> T obj, <strong><span style="color:rgb(43,145,175);">Expression</span><<span style="color:rgb(43,145,175);">Func</span><T, <span style="color:rgb(0,0,255);">bool</span>>></strong> pred)
    {
        <strong><span style="color:rgb(0,0,255);">var</span> x = pred.Compile();</strong>
        <span style="color:rgb(0,0,255);">if</span> (!x(obj))
            <span style="color:rgb(0,0,255);">throw</span> <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">ContractException</span>(pred.ToString());
    }




[](http://11011.net/software/vspaste)All I had to do was convert the Func<T, bool> into an Expression<Func<T, bool>>. The compiler, seeing the method signature, knows that it needs to do some background conversion. It doesn't convert and pre-compile the lambda function that you pass into the extension method. Instead it first converts it to an expression tree. The Expression<Func<T, bool>> has a Compile method that will convert it to an anonymous method, which we call just before invoking it. You may be wondering why we would bother?




Because Expression<Func<T, bool>> also overrides ToString() giving the source code of the lambda function that it was created from. That's so cool! Now I can pass the code I was trying to run into the exception class if the code fails!. Here's the kind of output you get if the check fails.




<blockquote>

> 
> TestCase 'Tests.TestPredicates.MyFailingMethod'  
failed: Contracts.ContractException : **x => (value(Tests.TestPredicates).MyInt = x.before().MyInt**)
> 
> </blockquote>




That's more readable than a plain old 'ApplicationException', don't you think? The predicates needn't be one-liners either; you can have very complex predicates in this system too. Here's an example from another project I'm working on. The use of scopes is more like the DBC implementation in the [D Programming Language](http://en.wikipedia.org/wiki/D_%28programming_language%29#Example_3). 
    
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">bool</span> Insert(<span style="color:rgb(0,0,255);">string</span> location, <span style="color:rgb(0,0,255);">string</span> xmlFragment)
    {
    <strong>    <span style="color:rgb(0,0,255);">this</span>.Require(x => !<span style="color:rgb(0,0,255);">string</span>.IsNullOrEmpty(location));
        <span style="color:rgb(0,0,255);">this</span>.Require(x => !<span style="color:rgb(0,0,255);">string</span>.IsNullOrEmpty(xmlFragment));</strong>
    
        <span style="color:rgb(43,145,175);">XDocument</span> fragDoc = <span style="color:rgb(43,145,175);">XDocument</span>.Parse(xmlFragment);
        <span style="color:rgb(0,0,255);">object</span> tmpInsertPoint = LobDocument.XPathEvaluate(location);
        <span style="color:rgb(0,0,255);">bool</span> result = <span style="color:rgb(0,0,255);">false</span>;
    
        <strong><span style="color:rgb(0,0,255);">using</span> (<span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">PredicateScope</span>(<span style="color:rgb(0,0,255);">this</span>, fragDoc, tmpInsertPoint))
        {
            <span style="color:rgb(0,0,255);">this</span>.Ensure(x => tmpInsertPoint != <span style="color:rgb(0,0,255);">null</span>);</strong>
    
            <span style="color:rgb(0,0,255);">if</span> (tmpInsertPoint != <span style="color:rgb(0,0,255);">null</span>)
            {
                <span style="color:rgb(0,0,255);">if</span> (tmpInsertPoint <span style="color:rgb(0,0,255);">is</span> <span style="color:rgb(43,145,175);">XElement</span>)
                {
                    <span style="color:rgb(43,145,175);">XElement</span> insertPoint = tmpInsertPoint <span style="color:rgb(0,0,255);">as</span> <span style="color:rgb(43,145,175);">XElement</span>;
                    insertPoint.Add(fragDoc);
                    result = <span style="color:rgb(0,0,255);">true</span>;
                }
            }
    
    <strong>        <span style="color:rgb(0,0,255);">this</span>.Ensure(x =>
                {
                    <span style="color:rgb(43,145,175);">XElement</span> originalInsertPoint = tmpInsertPoint.before() <span style="color:rgb(0,0,255);">as</span> <span style="color:rgb(43,145,175);">XElement</span>;
                    <span style="color:rgb(43,145,175);">XElement</span> currentInsertPoint = tmpInsertPoint <span style="color:rgb(0,0,255);">as</span> <span style="color:rgb(43,145,175);">XElement</span>;
                    <span style="color:rgb(0,0,255);">int</span> countbefore = originalInsertPoint.Elements(fragDoc.Root.Name).Count();
                    <span style="color:rgb(0,0,255);">int</span> countafter = currentInsertPoint.Elements(fragDoc.Root.Name).Count();
                    <span style="color:rgb(0,0,255);">return</span> countafter == (countbefore + 1);
                });</strong>
        }
        <span style="color:rgb(0,0,255);">return</span> result;
    }
    

[](http://11011.net/software/vspaste)


This is a fairly advanced use of lambdas and expression trees, but it certainly doesn't plumb the depths of what we could do. Those of you who've read some of the stuff I did in '06 and '07 on the [internals of LINQ](http://aabs.wordpress.com/linq/) will remember that expression trees will be storing references to all the properties and other parameters of the lambda function. That means we can add them to the ContractException. We can also show what values they were before and after the operation. Perhaps next time I'll explore what can be done with all that extra data we've now got. 




Till then, enjoy!
