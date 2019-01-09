---
author: aabs
comments: true
date: 2007-06-05 00:04:01+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2007/06/05/linqtordf-designing-a-query-provider/
slug: linqtordf-designing-a-query-provider
title: LinqToRdf - Designing a Query Provider
wordpress_id: 386
tags:
- C#
- Computer Science
- LINQ
- programming
---

When I started implementing the SPARQL support in LINQ to RDF, I decided that I needed to implement as much of the [standard query operators](http://download.microsoft.com/download/5/8/6/5868081c-68aa-40de-9a45-a3803d8134b8/standard_query_operators.doc) as possible. SPARQL is a very rich query language that bears a passing syntactical resemblance to SQL. It didn't seem unreasonable to expect most of the operators of LINQ to SQL to be usable with SPARQL. With this post I thought I'd pass on a few design notes that I have gathered during the work to date on the SPARQL query provider.

The different components of a LINQ query get converted into successive calls to your query class. My class is called SparqlQuery<T>. If you had a query like this:
    
    [<span style="color:rgb(0,128,128);">TestMethod</span>]
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> SparqlQueryOrdered()
    {
        <span style="color:rgb(0,0,255);">string</span> urlToRemoteSparqlEndpoint = <span style="color:rgb(128,0,0);">@"http://someUri"</span>;
        <span style="color:rgb(0,128,128);">TripleStore</span> ts = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(0,128,128);">TripleStore</span>();
        ts.EndpointUri = urlToRemoteSparqlEndpoint;
        ts.QueryType = <span style="color:rgb(0,128,128);">QueryType</span>.RemoteSparqlStore;
        <span style="color:rgb(0,128,128);">IRdfQuery</span><<span style="color:rgb(0,128,128);">Track</span>> qry = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(0,128,128);">RDF</span>(ts).ForType<<span style="color:rgb(0,128,128);">Track</span>>();
        var q = from t <span style="color:rgb(0,0,255);">in</span> qry
            where t.Year == 2006 &&
            t.GenreName == <span style="color:rgb(128,0,0);">"History 5 | Fall 2006 | UC Berkeley"</span>
            orderby t.FileLocation
            select <span style="color:rgb(0,0,255);">new</span> {t.Title, t.FileLocation};
        <span style="color:rgb(0,0,255);">foreach</span>(var track <span style="color:rgb(0,0,255);">in</span> q){
            <span style="color:rgb(0,128,128);">Trace</span>.WriteLine(track.Title + <span style="color:rgb(128,0,0);">": "</span> + track.FileLocation);
        }
    }
    




This would roughly equate to the following code, using the extension method syntax:
    
    [<span style="color:rgb(0,128,128);">TestMethod</span>]
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> SparqlQueryOrdered()
    {
        <span style="color:rgb(0,128,128);">ParameterExpression</span> t;
        <span style="color:rgb(0,0,255);">string</span> urlToRemoteSparqlEndpoint = <span style="color:rgb(128,0,0);"><a href="http://someUri">http://someUri</a></span>;
        <span style="color:rgb(0,128,128);">TripleStore</span> ts = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(0,128,128);">TripleStore</span>();
        ts.EndpointUri = urlToRemoteSparqlEndpoint;
        ts.QueryType = <span style="color:rgb(0,128,128);">QueryType</span>.RemoteSparqlStore;
        var q = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(0,128,128);">RDF</span>(ts).ForType<<span style="color:rgb(0,128,128);">Track</span>>()
            .<font color="#ff0000"><strong>Where</strong></font><<span style="color:rgb(0,128,128);">Track</span>>(<span style="color:rgb(0,128,0);">/*create expression tree*/</span>)
            .<strong><font color="#ff0000">OrderBy</font></strong><<span style="color:rgb(0,128,128);">Track</span>, <span style="color:rgb(0,0,255);">string</span>>(<span style="color:rgb(0,128,0);">/*create  expression tree*/</span>)
            .<font color="#ff0000"><strong>Select</strong></font>(<span style="color:rgb(0,128,0);">/*create  expression tree*/</span>;
        <span style="color:rgb(0,0,255);">foreach</span> (var track <span style="color:rgb(0,0,255);">in</span> q)
        {
            <span style="color:rgb(0,128,128);">Trace</span>.WriteLine(track.Title + <span style="color:rgb(128,0,0);">": "</span> + track.FileLocation);
        }
    }
    




The bold red method calls are the succession of calls to the query's CreateQuery method. That might not be immediately obvious from looking at the code. In fact it's downright unobvious! There's compiler magic going on in this, that you don't see. Anyway, what happens is that you end up getting a succession of calls into your IQueryable<T>.CreateQuery() method. And that's what we are mostly concerned about when creating a query provider.




The last I blogged about the CreateQuery method I gave a method with a switch statement that identified the origin of the call (i.e. Where, OrderBy, Select or whatever) and dispatched the call off to be immediately processed. I now realise that that is not the best way to do it. If I try to create my SPARQL query in one pass, I will not have much of a chance to perform optimization or disambiguation. If I generate the projection before I know what fields were important, I would probably end up having to get everything back and filter on receipt of all the data. I think Bart De Smet was faced with that problem with LINQ to LDAP (LDAP doesn't support projections) so he had to get everything back. SPARQL does support projections, and that means I can't generate the SPARQL query string till after I know what to get back from the Select call.




My solution to this is to keep all the calls into the CreateQuery in a Hashtable so that I can use them all together in the call to GetEnumerator. That gives me the chance to do any amount of analysis on the expression trees I've got stored, before I convert them into a SPARQL query. The CreateQuery method now looks like this:
    
    <span style="color:rgb(0,0,255);">protected</span> <span style="color:rgb(0,128,128);">Dictionary</span><<span style="color:rgb(0,0,255);">string</span>, <span style="color:rgb(0,128,128);"></span><span style="color:rgb(0,128,128);">MethodCallExpression</span>> expressions;
    
    
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,128,128);">IQueryable</span><S> CreateQuery<S>(<span style="color:rgb(0,128,128);">Expression</span> expression)
    {
        <span style="color:rgb(0,128,128);">SparqlQuery</span><S> newQuery = CloneQueryForNewType<S>();
    
        <span style="color:rgb(0,128,128);">MethodCallExpression</span> call = expression <span style="color:rgb(0,0,255);">as</span> <span style="color:rgb(0,128,128);">MethodCallExpression</span>;
        <span style="color:rgb(0,0,255);">if</span> (call != <span style="color:rgb(0,0,255);">null</span>)
        {
            Expressions[call.Method.Name] = call;
        }
        <span style="color:rgb(0,0,255);">return</span> newQuery;
    }
    

[](http://11011.net/software/vspaste)


This approach helps because it makes it much simpler to start adding the other query operators.




I also been doing a fair bit of tidying up as I go along. My GetEnumerator method now reflects the major grammatical components of the SPARQL spec for SELECT queries.
    
    <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">void</span> CreateQuery(<span style="color:rgb(0,128,128);">StringBuilder</span> sb)
    {
        <span style="color:rgb(0,0,255);">if</span>(Expressions.ContainsKey(<span style="color:rgb(128,0,0);">"Where"</span>))
        {
            <span style="color:rgb(0,128,0);">// first parse the where expression to get the list of parameters to/from the query.
    </span>        <span style="color:rgb(0,128,128);">StringBuilder</span> sbTmp = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(0,128,128);">StringBuilder</span>();
            ParseQuery(Expressions[<span style="color:rgb(128,0,0);">"Where"</span>].Parameters[1], sbTmp);
            <span style="color:rgb(0,128,0);">//sbTmp now contains the FILTER clause so save it somewhere useful.
    </span>        Query = sbTmp.ToString();
            <span style="color:rgb(0,128,0);">// now store the parameters where they can be used later on.
    </span>        queryGraphParameters.AddAll(Parser.Parameters);
        }
        CreateProlog(sb);
        CreateDataSetClause(sb);
        CreateProjection(sb);
        CreateWhereClause(sb);
        CreateSolutionModifier(sb);
    }
    




[](http://11011.net/software/vspaste)The If clause checks whether the query had a where clause. If it did, it parses it, creating the FILTER expression, and in the process gathering some valuable information about what members from T were referenced in the where clause. This information is useful for other tasks, so it gets done in advance of creating the Where clause.
