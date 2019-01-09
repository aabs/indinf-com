---
author: aabs
comments: true
date: 2007-05-31 06:11:45+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2007/05/31/linqtordf-sparql-query-generation/
slug: linqtordf-sparql-query-generation
title: LinqToRdf - SPARQL Query Generation
wordpress_id: 384
tags:
- C#
- LINQ
- programming
---

Progress with the [LINQ to RDF](http://code.google.com/p/linqtordf/) Query Provider is continuing apace. I have been pretty tightly focused for the last few days, so I haven't ad time to post an update. I've lately been working on a SPARQL query, which will allow me a much richer palette to play with. Here's the current version of the LINQ query I'm using to test the RdfSparqlQuery class.
    
    [<span style="color:rgb(0,128,128);">TestMethod</span>]
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> SparqlQuery()
    {
        <span style="color:rgb(0,0,255);">string</span> urlToRemoteSparqlEndpoint = <span style="color:rgb(128,0,0);">@<a href="http://someUri">http://someUri</a></span>;
        <span style="color:rgb(0,128,128);">TripleStore</span> ts = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(0,128,128);">TripleStore</span>();
        ts.EndpointUri = urlToRemoteSparqlEndpoint;
        ts.QueryType = <span style="color:rgb(0,128,128);">QueryType</span>.RemoteSparqlStore;
        <span style="color:rgb(0,128,128);">IRdfQuery</span><<span style="color:rgb(0,128,128);">Track</span>> qry = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(0,128,128);">RDF</span>(ts).ForType<<span style="color:rgb(0,128,128);">Track</span>>();
        var q = from t <span style="color:rgb(0,0,255);">in</span> qry
            where t.Year == 2006 &&
            t.GenreName == <span style="color:rgb(128,0,0);">"History 5 | Fall 2006 | UC Berkeley"</span>
            select <span style="color:rgb(0,0,255);">new</span> {t.Title, t.FileLocation};
        <span style="color:rgb(0,0,255);">foreach</span>(var track <span style="color:rgb(0,0,255);">in</span> q){
            <span style="color:rgb(0,128,128);">Trace</span>.WriteLine(track.Title + <span style="color:rgb(128,0,0);">": "</span> + track.FileLocation);
        }
    }
    




And here's what it's producing as output:




@prefix a: <http://aabs.purl.org/ontologies/2007/04/music#> . 


SELECT ?Title, ?FileLocation   
WHERE {  
?Track <http://aabs.purl.org/ontologies/2007/04/music#title> ?Title .  
?Track <http://aabs.purl.org/ontologies/2007/04/music#artistName> ?ArtistName .  
?Track <http://aabs.purl.org/ontologies/2007/04/music#albumName> ?AlbumName .  
?Track <http://aabs.purl.org/ontologies/2007/04/music#year> ?Year .  
?Track <http://aabs.purl.org/ontologies/2007/04/music#genreName> ?GenreName .  
?Track <http://aabs.purl.org/ontologies/2007/04/music#comment> ?Comment .  
?Track <http://aabs.purl.org/ontologies/2007/04/music#fileLocation> ?FileLocation .  
?Track <http://aabs.purl.org/ontologies/2007/04/music#rating> ?Rating .  
FILTER {  
((t.Year)=(2006^^xsdt:int))&&((t.GenreName)=("History 5 | Fall 2006 | UC Berkeley"^^xsdt:string))  
}  
}




as you can see, the query is fairly close. I just have to:






  * Convert the property names in the filters to use the free variable name chosen for the object of a property assertion.


  * Restrict the variables enumerated in the graph definition to just those required for the projection (if there is a projection)


  * Use the UNION operator to allow disjunctions (OrElse expression types)


  * Add the XSDT (XML Schema Datatypes) namespace to the prefixes


  * Use standard SPARQL variable syntax for the ?Track



I also need to find a _fast _back-end SPARQL enabled triple store that I can start to run these queries against. A month or two ago I did a [.NET conversion of Jena](http://aabs.wordpress.com/2007/03/23/converting-jena-to-net/) using IKVM. I may end up going back to that. Any other suggestions would be welcome.




If you want to have a play with it, or take a look at how to produce a LINQ Query provider, then you can use Subversion to get the code from Google. Use the command below:




svn checkout **_http_**://linqtordf.googlecode.com/svn/trunk/ linqtordf
