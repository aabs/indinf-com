---
author: aabs
comments: true
date: 2007-06-20 13:07:41+00:00
layout: post
link: https://aabs.wordpress.com/2007/06/20/groupjoins-in-linq/
slug: groupjoins-in-linq
title: GroupJoins in LINQ
wordpress_id: 396
tags:
- C#
- LINQ
- programming
- software
---

OWL defines two types of property: DatatypeProperty and ObjectProperty. An object property links instances from two Classes, just like a reference in .NET between two objects. In OWL you define it like this:

<owl:ObjectProperty rdf:ID="isOnAlbum">   
<rdfs:domain rdf:resource="#Track"/>  
<rdfs:range rdf:resource="#Album"/>   
</owl:ObjectProperty>

A DatatypeProperty is similar to a .NET property that stores some kind of primitive type like a string or an int. In OWL it looks like this:

<owl:DatatypeProperty rdf:ID="fileLocation">  
<rdfs:domain rdf:resource="#Track" />   
<rdfs:range rdf:resource="&xsd;string"/>  
</owl:DatatypeProperty>  

The format is very much the same, but the task of querying for primitive types in LINQ and SPARQL is easy compared to performing a one to many query like a SQL Join. So far, I have confined my efforts to DatatypeProperties, and tried not to think about ObjectProperties too much. But the time of reckoning has come - I've not got much else left to do on LinqToRdf except ObjectProperties.

Here's the kind of LINQ join I plan to implement:
    
    [<span style="color:rgb(43,145,175);">TestMethod</span>]
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">void</span> TestJoin()
    {
        <span style="color:rgb(43,145,175);">TestContext</span> db = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">TestContext</span>(CreateSparqlTripleStore());
        var q = from a <span style="color:rgb(0,0,255);">in</span> db.Album
                join t <span style="color:rgb(0,0,255);">in</span> db.Track on a.Name equals t.AlbumName into tracks
                select <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">Album</span>{Name = a.Name, Tracks = tracks};
        <span style="color:rgb(0,0,255);">foreach</span>(var album <span style="color:rgb(0,0,255);">in</span> q){
            <span style="color:rgb(43,145,175);">Console</span>.WriteLine(album.Name);
            <span style="color:rgb(0,0,255);">foreach</span> (<span style="color:rgb(43,145,175);">Track</span> track <span style="color:rgb(0,0,255);">in</span> album.Tracks)
            {
                <span style="color:rgb(43,145,175);">Console</span>.WriteLine(track.Title);
            }
        }
    }




This uses a GroupJoin to let me collect matching tracks and store them in a temporary variable called tracks. I then insert the tracks into the Tracks property on the album I'm newing up in the projection. I need to come up with a SPARQL equivalent syntax, and convert the expression passed for the join into that. SPARQL is a graph based query language, so I am going to be converting my requests into the usual SPARQL triple format, and then using the details from the NewExpression on the query to work out where to put the data when I get it back.




With the non-join queries I have been testing my query provider on, I have observed that for each syntactical component of the query I was passed an Expression tree, representing its contents. With a GroupJoin, you get one, and it contains everything you need to perform the query. My first quandary is over the process of converting this new expression structure into a format that my existing framework can understand. Below is a snapshot of the expression tree created for the join I showed above.




[![GroupJoin Expression contents](http://aabs.files.wordpress.com/2007/06/img.06-20-2007.3-thumb.png)](http://aabs.files.wordpress.com/2007/06/img.06-20-2007.3.png)




There are five parameters in the expression:






  1. The query object on the Album. That's the "_a in db.Album_" part.

  2. The query object on the Track. The "_t in db.Track_" part.

  3. A lambda function from an album to its Name.

  4. A lambda function from a track to its AlbumName.

  5. A projection creating a new Album, and assigning the tracks collected to the Tracks collection on the newly created Album.



Parameters 1 & 2 are LinqToRdf queries that don't need to be parsed and converted. I can't just ask them to render a query for me, since they don't have any information of value to offer me other than the OriginalType that they were created with. They have received no expressions filtering the kind of data that they return, and they'll never have their results enumerated. These query objects are just a kind of clue for the GroupJoin about how to compose the query. They can tell it where the data that it's looking for is to be found.




Here's how I would guess the SPARQL query would look:
    
    SELECT ?Name ?Title ?GenreName <font color="#ff0000"><snip></font>
    WHERE {
        _:a a a:Album .
        _:t a a:Track .
        _:a a:name ?Name.
        _:t a:albumName ?Name .
        OPTIONAL {_:t a: ?Title}
        OPTIONAL {_:t a: ?GenreName}
        <font color="#ff0000"><snip></font>
    }




We can get the names for blank nodes _:a and _:t from the parameter collections of the GroupJoins parameters 3 and 4 respectively. We know that we will be equating ?Name on _:a and ?Name on _:t since those are the lambda functions provided and that's the format of the join. The rest of the properties are included in optional sections so that if they are not present it won't stop the details of the OWL instance coming back. By using
    
        _:a a:name ?Name.
        _:t a:albumName ?Name .
    




We achieve the same as equality, since two things that are equal to the same are equal to each other. That restricts the tracks to those that are part of an album at the same time.




I'm not sure yet what I will do about the projection, since there is an intermediate task that needs to be performed: to insert the temporary variable 'tracks' into the Album object after it has been instantiated. More on that once I've found out more.
