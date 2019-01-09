---
author: aabs
comments: true
date: 2008-06-09 12:20:08+00:00
layout: post
link: https://aabs.wordpress.com/2008/06/09/linqtordf-v071-and-rdfmetal/
slug: linqtordf-v071-and-rdfmetal
title: LinqToRdf v0.7.1 and RdfMetal
wordpress_id: 539
categories:
- .NET
- programming
- science
- Semantic Web
- SemanticWeb
---

I've just uploaded [version 0.7.1](http://linqtordf.googlecode.com/files/LinqToRdf-0.7.1.msi) of LinqToRdf. This bug fix release corrects an issue I introduced in version 0.7. The issue only seemed to affect some machines and stems from the use of the GAC by the WIX installer (to the best of my knowledge). I've abandoned GAC installation and gone back to the original approach.  

Early indications (Thanks, Hinnerk) indicate that the issue has been successfully resolved. Please let me know if you are still experiencing problems. Thanks to 13sides, Steve Dunlap, Hinnerk Bruegmann, Kevin Richards and [Paul Stovell](http://www.paulstovell.com/blog/) for bringing it to my attention and helping me to overcome the allure of the GAC.  

Kevin also reported that he's hoping to use LinqToRdf on a project involving the Biodiversity Information Standards ([TDWG](http://www.tdwg.org/)). It's always great to hear how people are using the framework. Please drop me a line to let me know how you are using LinqToRdf.  

Kevin might find feature [#13](http://code.google.com/p/linqtordf/issues/detail?id=13&colspec=ID%20Type%20Summary%20Priority) useful. It will be called **_RdfMetal_** in honour of SqlMetal. It will automate the process of working with remotely managed ontologies. RdfMetal will completely lower any barriers to entry in semantic web development. You will (in principle) no longer need to know the formats, protocols and standards of the semantic web in order to consume data in it. 

[![clip_image001[5]](http://aabs.files.wordpress.com/2008/06/clip-image0015-thumb.png)](http://aabs.files.wordpress.com/2008/06/clip-image0015.png)  

Here's an example of the output it generated from DBpedia.org for the FOAF ontology: 

`./RdfMetal.exe -e:http://DBpedia.org/sparql -i -n http://xmlns.com/foaf/0.1/ -o foaf.cs`  

Which produced the following source:
    
    <span style="color:blue;">namespace </span>Some.Namespace
    {
    [<span style="color:blue;">assembly</span>: <span style="color:#2b91af;">Ontology</span>(
        BaseUri = <span style="color:#a31515;">"http://xmlns.com/foaf/0.1/"</span>,
        Name = <span style="color:#a31515;">"MyOntology"</span>,
        Prefix = <span style="color:#a31515;">"MyOntology"</span>,
        UrlOfOntology = <span style="color:#a31515;">"http://xmlns.com/foaf/0.1/"</span>)]
    
    
        <span style="color:blue;">public partial class </span><span style="color:#2b91af;">MyOntologyDataContext </span>: <span style="color:#2b91af;">RdfDataContext
        </span>{
            <span style="color:blue;">public </span>MyOntologyDataContext(<span style="color:#2b91af;">TripleStore </span>store) : <span style="color:blue;">base</span>(store)
            {
            }
            <span style="color:blue;">public </span>MyOntologyDataContext(<span style="color:blue;">string </span>store) : <span style="color:blue;">base</span>(<span style="color:blue;">new </span><span style="color:#2b91af;">TripleStore</span>(store))
            {
            }
    
                    <span style="color:blue;">public </span><span style="color:#2b91af;">IQueryable</span><<span style="color:#2b91af;">Person</span>> Persons
                    {
                        <span style="color:blue;">get
                        </span>{
                            <span style="color:blue;">return </span>ForType<<span style="color:#2b91af;">Person</span>>();
                        }
                    }
    
                    <span style="color:blue;">public </span><span style="color:#2b91af;">IQueryable</span><<span style="color:#2b91af;">Document</span>> Documents
                    {
                        <span style="color:blue;">get
                        </span>{
                            <span style="color:blue;">return </span>ForType<<span style="color:#2b91af;">Document</span>>();
                        }
                    }
    
                    <span style="color:blue;">// ...</span>
    
        }
    
    [<span style="color:#2b91af;">OwlResource</span>(OntologyName=<span style="color:#a31515;">"MyOntology"</span>, RelativeUriReference=<span style="color:#a31515;">"Person"</span>)]
    <span style="color:blue;">public partial class </span><span style="color:#2b91af;">Person
    </span>{
      [<span style="color:#2b91af;">OwlResource</span>(OntologyName = <span style="color:#a31515;">"MyOntology"</span>, RelativeUriReference = <span style="color:#a31515;">"surname"</span>)]
      <span style="color:blue;">public string </span>surname {<span style="color:blue;">get</span>;<span style="color:blue;">set</span>;}
      [<span style="color:#2b91af;">OwlResource</span>(OntologyName = <span style="color:#a31515;">"MyOntology"</span>, RelativeUriReference = <span style="color:#a31515;">"family_name"</span>)]
      <span style="color:blue;">public string </span>family_name {<span style="color:blue;">get</span>;<span style="color:blue;">set</span>;}
      [<span style="color:#2b91af;">OwlResource</span>(OntologyName = <span style="color:#a31515;">"MyOntology"</span>, RelativeUriReference = <span style="color:#a31515;">"geekcode"</span>)]
      <span style="color:blue;">public string </span>geekcode {<span style="color:blue;">get</span>;<span style="color:blue;">set</span>;}
      [<span style="color:#2b91af;">OwlResource</span>(OntologyName = <span style="color:#a31515;">"MyOntology"</span>, RelativeUriReference = <span style="color:#a31515;">"firstName"</span>)]
      <span style="color:blue;">public string </span>firstName {<span style="color:blue;">get</span>;<span style="color:blue;">set</span>;}
      [<span style="color:#2b91af;">OwlResource</span>(OntologyName = <span style="color:#a31515;">"MyOntology"</span>, RelativeUriReference = <span style="color:#a31515;">"plan"</span>)]
      <span style="color:blue;">public string </span>plan {<span style="color:blue;">get</span>;<span style="color:blue;">set</span>;}
      [<span style="color:#2b91af;">OwlResource</span>(OntologyName = <span style="color:#a31515;">"MyOntology"</span>, RelativeUriReference = <span style="color:#a31515;">"knows"</span>)]
      <span style="color:blue;">public </span><span style="color:#2b91af;">Person </span>knows {<span style="color:blue;">get</span>;<span style="color:blue;">set</span>;}
      [<span style="color:#2b91af;">OwlResource</span>(OntologyName = <span style="color:#a31515;">"MyOntology"</span>, RelativeUriReference = <span style="color:#a31515;">"img"</span>)]
      <span style="color:blue;">public </span><span style="color:#2b91af;">Image </span>img {<span style="color:blue;">get</span>;<span style="color:blue;">set</span>;}
      [<span style="color:#2b91af;">OwlResource</span>(OntologyName = <span style="color:#a31515;">"MyOntology"</span>, RelativeUriReference = <span style="color:#a31515;">"myersBriggs"</span>)]
      <span style="color:blue;">// ...</span>
    }
    
    [<span style="color:#2b91af;">OwlResource</span>(OntologyName=<span style="color:#a31515;">"MyOntology"</span>, RelativeUriReference=<span style="color:#a31515;">"Document"</span>)]
    <span style="color:blue;">public partial class </span><span style="color:#2b91af;">Document
    </span>{
      [<span style="color:#2b91af;">OwlResource</span>(OntologyName = <span style="color:#a31515;">"MyOntology"</span>, RelativeUriReference = <span style="color:#a31515;">"primaryTopic"</span>)]
      <span style="color:blue;">public </span>LinqToRdf.<span style="color:#2b91af;">OwlInstanceSupertype </span>primaryTopic {<span style="color:blue;">get</span>;<span style="color:blue;">set</span>;}
      [<span style="color:#2b91af;">OwlResource</span>(OntologyName = <span style="color:#a31515;">"MyOntology"</span>, RelativeUriReference = <span style="color:#a31515;">"topic"</span>)]
      <span style="color:blue;">public </span>LinqToRdf.<span style="color:#2b91af;">OwlInstanceSupertype </span>topic {<span style="color:blue;">get</span>;<span style="color:blue;">set</span>;}
    }
    
    // ...







As you can see, it's still pretty rough, but it allows me to write queries like this:
    
    [<span style="color:#2b91af;">TestMethod</span>]
    <span style="color:blue;">public void </span>TestGetPetesFromDbPedia()
    {
        <span style="color:blue;">var </span>ctx = <span style="color:blue;">new </span><span style="color:#2b91af;">MyOntologyDataContext</span>(<span style="color:#a31515;">"http://DBpedia.org/sparql"</span>);
        <span style="color:blue;">var </span>q = <span style="color:blue;">from </span>p <span style="color:blue;">in </span>ctx.Persons
                <span style="color:blue;">where </span>p.firstName.StartsWith(<span style="color:#a31515;">"Pete"</span>)
                <span style="color:blue;">select </span>p;
        <span style="color:blue;">foreach </span>(<span style="color:#2b91af;">Person </span>person <span style="color:blue;">in </span>q)
        {
            <span style="color:#2b91af;">Debug</span>.WriteLine(person.firstName + <span style="color:#a31515;">" " </span>+ person.family_name);
        }
    }
    

[](http://11011.net/software/vspaste)[](http://11011.net/software/vspaste)


RdfMetal will be added to the v0.8 release of LinqToRdf in the not too distant future. If you have any feature requests, or want to help out, please reply to this or better-still join the [LinqToRdf discussion group](http://groups.google.com/group/linqtordf-discuss) and post there.
