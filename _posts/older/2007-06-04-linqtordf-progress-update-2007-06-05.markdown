---
author: aabs
comments: true
date: 2007-06-04 23:57:24+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2007/06/05/linqtordf-progress-update-2007-06-05/
slug: linqtordf-progress-update-2007-06-05
title: LinqToRdf - Progress Update 2007-06-05
wordpress_id: 385
tags:
- C#
- LINQ
- programming
---

I've been given a week by work to try to make some progress on the LINQ to RDF query provider, and I'm glad to say that the query generation phase is now pretty much complete for SPARQL. It's amazing what a difference a full day can make to your progress, compared to trying to get as much done as I can when I'm on the train.

Last week when I blogged, I had the rough outlines of a SPARQL query, but it was missing quite a bit. There were also a few bits that were just plain wrong, such as commas separating SELECT parameters. That's been corrected now. The properties in the GraphPattern are also restricted to those that are mentioned in the FILTER clause, or the projection.

I've also added support for the OrderBy, Take and Skip operators, which correspond to the "ORDER BY", "LIMIT" and "OFFSET" clauses in SPARQL. The unit test I'm working with is looking pretty overweight now:
    
    [<span style="color:rgb(0,128,128);">TestMethod</span>]
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> SparqlQueryWithTheLot()
    {
        <span style="color:rgb(0,0,255);">string</span> urlToRemoteSparqlEndpoint = <span style="color:rgb(128,0,0);">@"http://someUri"</span>;
        <span style="color:rgb(0,128,128);">TripleStore</span> ts = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(0,128,128);">TripleStore</span>();
        ts.EndpointUri = urlToRemoteSparqlEndpoint;
        ts.QueryType = <span style="color:rgb(0,128,128);">QueryType</span>.RemoteSparqlStore;
        <span style="color:rgb(0,128,128);">IRdfQuery</span><<span style="color:rgb(0,128,128);">Track</span>> qry = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(0,128,128);">RDF</span>(ts).ForType<<span style="color:rgb(0,128,128);">Track</span>>();
        var q = (from t <span style="color:rgb(0,0,255);">in</span> qry
            where t.Year == 2006 &&
            t.GenreName == <span style="color:rgb(128,0,0);">"History 5 | Fall 2006 | UC Berkeley"</span>
            orderby t.FileLocation
            select <span style="color:rgb(0,0,255);">new</span> {t.Title, t.FileLocation}).Skip(10).Take(5);
        <span style="color:rgb(0,0,255);">foreach</span>(var track <span style="color:rgb(0,0,255);">in</span> q){
            <span style="color:rgb(0,128,128);">Trace</span>.WriteLine(track.Title + <span style="color:rgb(128,0,0);">": "</span> + track.FileLocation);
        }
    }
    

[](http://11011.net/software/vspaste)


Here's a sample of the query string that gets produced for it:
    
    @prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .
    @prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .
    @prefix xsdt: <http://www.w3.org/2001/XMLSchema#> .
    @prefix fn: <http://www.w3.org/2005/xpath-functions#>  .
    @prefix a: <http://aabs.purl.org/ontologies/2007/04/music#> .
    
    SELECT ?FileLocation ?Title
    WHERE {
    ?t a:year ?Year .
    ?t a:genreName ?GenreName .
    ?t a:fileLocation ?FileLocation .
    ?t a:title ?Title .
    FILTER {
    ((?Year)=(2006^^xsdt:int))&&((?GenreName)=("History 5 | Fall 2006 | UC Berkeley"^^xsdt:string))
    }
    }
    ORDER BY ?FileLocation
    LIMIT 5
    OFFSET 10

[](http://11011.net/software/vspaste)[](http://11011.net/software/vspaste)Which is almost exactly what we want. I'm thinking it's about time to set up some kind of SPARQL server to test the queries for real. We also have to check whether the ObjectDeserialisationSink is capable of deserialising results from a SPARQL query as well as an RSQuary query.
