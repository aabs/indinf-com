---
author: aabs
comments: true
date: 2007-04-16 13:22:19+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2007/04/16/using-rdf-and-c-to-create-an-mp3-manager-part-1/
slug: using-rdf-and-c-to-create-an-mp3-manager-part-1
title: Using RDF and C# to create an MP3 Manager - Part 1
wordpress_id: 374
tags:
- C#
- Computer Science
---

[digg=http://www.digg.com/programming/Using_RDF_and_C_to_create_an_MP3_Manager_Part_1]This article follows on from the previous [post](http://aabs.wordpress.com/2007/04/13/a-simple-semantic-web-application-in-c/) about semantic web applications in C#. I'll be using the SemWeb framework again, but this time I chose to demonstrate the capabilities of RDF by producing a simple MP3 file manager. I haven't completed it yet, and I'll be working on that over the next few days to show you just how easy RDF/OWL is to work with in C# these days.

The program is pretty simple – I was inspired to write it by the [RDF-izers](http://simile.mit.edu/wiki/RDFizers) web site, where you can find conversion tools to producing RDF from a variety of different data sources. While I was playing with LINQ I produced a simple file tagging system – I simply scanned the file system extracting as much metadata as I could from the files that I found, adding them to a tag database that I kept in SQL server. Well this isn't much different. I just extract ID3 metadata tags from the MP3 files I find and store them in Track objects. I then wrote a simple conversion system to extract RDF URIs from the objects I'd created for insertion into an in-memory triple store. All-up it took about 3-4 hours including finding a suitable API for ID3 reading. I won't show (unless demanded to) the code for the test harness or for iterating the file system. Instead I'll show you the code I wrote for persisting objects to an RDF store.

First up, we have the Track class. I've removed the vanilla implementation of the properties for the sake of brevity.

`[OntologyBaseUri("file:///C:/dev/prototypes/semantic-web/src/Mp3ToRdf/")]
[OwlClass("Track", true)]
public class Track : OwlInstanceSupertype
{
[OwlProperty("title", true)]
public string Title /* ... */
[OwlProperty("artistName", true)]
public string ArtistName /* ... */
[OwlProperty("albumName", true)]
public string AlbumName /* ... */
[OwlProperty("year", true)]
public string Year /* ... */
[OwlProperty("genreName", true)]
public string GenreName /* ... */
[OwlProperty("comment", true)]
public string Comment /* ... */
[OwlProperty("fileLocation", true)]
public string FileLocation /* ... */
`
`    private string title;
private string artistName;
private string albumName;
private string year;
private string genreName;
private string comment;
private string fileLocation;
`
`    public Track(TagHandler th, string fileLocation)
{
this.fileLocation = fileLocation;
title = th.Track;
artistName = th.Artist;
albumName = th.Album;
year = th.Year;
genreName = th.Genere;
comment = th.Comment;
}
}
`

Nothing of note here except for the presence of a few all-important attributes which are used to give the persistence engine clues about how to generate URIs for the class, its properties and their values. Obviously this is a rudimentary implementation, so we don't have lots of extra information about XSD types and versions etc. But for the sake of this illustration I'm sure you get the idea, that we can do for RDF pretty much what LINQ to SQL does for relational databases.

The attribute classes are also very simple:

`[AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct | AttributeTargets.Property)]
public class OwlResourceSupertypeAttribute : Attribute
{
public string Uri
{
get { return uri; }
}
private readonly string uri;
public bool IsRelativeUri
{
get { return isRelativeUri; }
}
private readonly bool isRelativeUri;
public OwlResourceSupertypeAttribute(string uri)
: this(uri, false){}
public OwlResourceSupertypeAttribute(string uri, bool isRelativeUri)
{
this.uri = uri;
this.isRelativeUri = isRelativeUri;
}
}
`
`[AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct | AttributeTargets.Property)]
public class OwlClassAttribute : OwlResourceSupertypeAttribute
{
public OwlClassAttribute(string uri)
: base(uri, false){}
public OwlClassAttribute(string uri, bool isRelativeUri)
: base(uri, isRelativeUri){}
}
`
`[AttributeUsage(AttributeTargets.Property)]
public class OwlPropertyAttribute : OwlResourceSupertypeAttribute
{
public OwlPropertyAttribute(string uri)
: base(uri, false){}
public OwlPropertyAttribute(string uri, bool isRelativeUri)
: base(uri, isRelativeUri){}
}
`
`[AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct)]
public class OntologyBaseUriAttribute : Attribute
{
public string BaseUri
{
get { return baseUri; }
}
private string baseUri;
public OntologyBaseUriAttribute(string baseUri)
{
this.baseUri = baseUri;
}
}
`

OwlResourceSupertypeAttribute is the supertype of any attribute that can be related to a resource in an ontology – that is anything that has a URI. As such it has a Uri property, and in addition it has an isRelativeUri property which indicates whether the URI is relative to a base URI defined elsewhere. Although I haven't implemented my solution that way yet, this is intended to allow the resources to reference a base namespace definition in the triple store or in an RDF file. The OwlClassAttribute extends the OwlResourceSupertype restricting its usage to classes or structs. You use this (or the parent type if you want) to indicate the OWL class URI that the type will be persisted to. So for the Track class we have an OWL class of "Track". In an ontology that Track will be relative to some URI, which I have defined using the OntologyBaseUriAttribute. That attribute defines the URI of the ontology the Class and Property URIs are relative to in this example (i.e. "file:///C:/dev/prototypes/semantic-web/src/Mp3ToRdf/").

For each of the properties of the class Track I have defined another sublass of OwlResourceSupertype called OwlPropertyAttribute that is restricted solely to Property members. Another simplification that I have introduced is that I am not distinguishing between ObjectProperties and DatatypeProperties, which OWL does. That would not be hard to add, and I'm sure I'll have to over the next few days.

So, I have now annotated my class to tell the persistence engine how to produce statements that I can add to the triple store. These annotations can be read by the engine and used to construct appropriate URIs for statements. We still need a way to construct instances in the ontology. I've done that in a very simple way – I just keep a counter in the scanner, and I create an instance URI out of the Class Uri by adding the counter to the end. So the first instance will be "file:///C:/dev/prototypes/semantic-web/src/Mp3ToRdf/Track_1" and so on. This is simple, but would need to be improved upon for any serious applications.

Next I need to reflect over an instance of class Track to get a set of statements that I can add to the triple store. For this I have exploited the extension method feature of C# 3.5 (May CTP) which allows me to write code like this:
`foreach (Track t in GetAllTracks(txtFrom.Text))
{
t.InstanceUri = GenTrackName(t);
store.Add(t);
}
`
The triple store is called store, GetAllTracks is an iterator that filters the files under the directory indicated by whatever is in txtFrom.Text. GenTrackName creates the instance URI for the track instance. I could have used a more sophisticated scheme using hashes from the track location or somesuch, but I was in a rush ;-). The code the the persistence engine is easy as well:

`public static class MemoryStoreExtensions
{
public static void Add(this MemoryStore ms, OwlInstanceSupertype oc)
{
Debug.WriteLine(oc.ToString());
Type t = oc.GetType();
PropertyInfo[] pia = t.GetProperties();
foreach (PropertyInfo pi in pia)
{
if(IsPersistentProperty(pi))
{
AddPropertyToStore(oc, pi, ms);
}
}
}
private static bool IsPersistentProperty(PropertyInfo pi)
{
return pi.GetCustomAttributes(typeof (OwlPropertyAttribute), true).Length > 0;
}
private static void AddPropertyToStore(OwlInstanceSupertype track, PropertyInfo pi, MemoryStore ms)
{
Add(track.InstanceUri, track.GetPropertyUri(pi.Name), pi.GetValue(track, null).ToString(), ms);
}
public static void Add(string s, string p, string o, MemoryStore ms)
{
if(!Empty(s) && !Empty(p) && !Empty(o))
ms.Add(new Statement(new Entity(s), new Entity(p), new Literal(o)));
}
private static bool Empty(string s)
{
return (s == null || s.Length == 0);
}
}
`
Add is the extension method which iterates the properties on the class OwlInstanceSupertype. OwlInstanceSupertype is a supertype of all classes that can be persisted to the store. As you can see, it gets all properties and checks each on to see whether it has the OwlPropertyAttribute. If it does, then it gets persisted using AddPropertyToStore. AddPropertyToStore creates URIs for the subject (the track instance in the store), the predicate (the object property on class Track) and the object property (which is a string literal containing the value of the property). That statement gets added by the private Add method, which just mirrors the Add API on the MemoryStore itself.

And that's it. Almost. The quick and dirty ontology I defined for the music tracks looks like this:

`@prefix rdf:   .
@prefix daml:  .
@prefix log:  .
@prefix rdfs:  .
@prefix owl:   .
@prefix xsdt: .
@prefix :  .`

`:ProducerOfMusic a owl:Class.
:SellerOfMusic a owl:Class.
:NamedThing a owl:Class.
:TemporalThing a owl:Class.
:Person a owl:Class;
owl:subClassOf :NamedThing.
:Musician owl:subClassOf :ProducerOfMusic, :Person.
:Band a :ProducerOfMusic.
:Studio a :SellerOfMusic, :NamedThing.
:Label = :Studio.
:Music a owl:Class.
:Album a :NamedThing.
:Track a :NamedThing.
:Song a :NamedThing.
:Mp3File a owl:Class.
:Genre a :NamedThing.
:Style = :Genre.
:title
rdfs:domain :Track
rdfs:range  xsdt:string.
:artistName
rdfs:domain :Track
rdfs:range  xsdt:string.
:albumName
rdfs:domain :Track
rdfs:range  xsdt:string.
:year
rdfs:domain :Album
rdfs:range  xsdt:integer.
:genreName
rdfs:domain :Track
rdfs:range  xsdt:string.
:comment
rdfs:domain :Track
rdfs:range  xsdt:string.
:isTrackOn
rdfs:domain :Track
rdfs:range  :Album.
:fileLocation
rdfs:domain :Track
rdfs:range  xsdt:string.
`

When I run it over my podcasts, the output persisted to N3 looks like this:



<file:///C:/dev/prototypes/semantic-web/src/Mp3ToRdf/Track_1> <file:///C:/dev/prototypes/semantic-web/src/Mp3ToRdf/title> "History 5 | Fall 2006 | UC Berkeley" ; <file:///C:/dev/prototypes/semantic-web/src/Mp3ToRdf/artistName> "Thomas Laqueur" ;
<file:///C:/dev/prototypes/semantic-web/src/Mp3ToRdf/albumName> "History 5 | Fall 2006 | UC Berkeley" ;
<file:///C:/dev/prototypes/semantic-web/src/Mp3ToRdf/year> "2006" ;
<file:///C:/dev/prototypes/semantic-web/src/Mp3ToRdf/genreName> "History 5 | Fall 2006 | UC Berkeley" ;
<file:///C:/dev/prototypes/semantic-web/src/Mp3ToRdf/comment> " (C) Copyright 2006, UC Regents" ;
<file:///C:/dev/prototypes/semantic-web/src/Mp3ToRdf/fileLocation> "C:\\Users\\andrew.matthews\\Music\\hist5_20060829.mp3" .


You can see how the URIs have been constructed from the base URI, and the properties are all attached to instance Track_1. Next on the list will probably be a bit of cleaning up to use a prefix rather than this longhand URI notation, then I'll show you how to query the store to slice and dice your music collections every which way.
