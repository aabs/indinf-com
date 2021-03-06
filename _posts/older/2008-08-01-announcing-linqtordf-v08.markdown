---
author: aabs
comments: true
date: 2008-08-01 13:04:12+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2008/08/01/announcing-linqtordf-v08/
slug: announcing-linqtordf-v08
title: Announcing LinqToRdf v0.8
wordpress_id: 546
categories:
- .NET
- programming
- Semantic Web
- SemanticWeb
tags:
- .NET
- C#
- LinqToRdf
- RDF
- Semantic Web
- SemanticWeb
- semweb
- web3
- web3.0
---

I'm very pleased to announce the release of version 0.8 of LinqToRdf. This release is significant for a couple of reasons. Firstly, because it provides a preview release of RdfMetal and secondly because it is the first release containing changes contributed by someone other than yours truly. The changes in this instance being provided by Carl Blakeley of [OpenLink Software](http://www.openlinksw.com).

LinqToRdf v0.8 has received a few major chunks of work:



	
  * New installers for both the designer and the whole framework
WIX was proving to be a pain, so I downgraded to the integrated installer generator in Visual Studio.

	
  * A preview release of RdfMetal. I brought this release forward a little, on Carl Blakeley's request, to coincide with a post he's preparing on using OpenLink Virtuoso with LinqToRdf, so RdfMetal is not as fully baked as I'd planned. But it's still worth a look. Expect a minor release in the next few weeks with additional fixes/enhancements.


I'd like to extend a very big **_thank-you_** to Carl for the the work he's done in recent weeks to help extend and improve the mechanisms LinqToRdf uses to represent and traverse relationships. His contributions also include improvements in representing default graphs, and referencing multiple ontologies within a single .NET class. He also provided fixes around the quoting of URIs and some other fixes in the ways LinqToRdf generates SPARQL for default graphs. Carl also provided an interesting example application using OpenLink Virtuoso's hosted version of Musicbrainz that is significantly richer than the test ontology I created for the unit tests and manuals.

I hope that Carl's contributions represent an acknowledgement by OpenLink that not only does LinqToRdf support Virtuoso, but that there is precious little else in the .NET space that stands a chance of attracting developers to the semantic web. .NET is a huge untapped market for semantic web product vendors. LinqToRdf is, right now, the best way to get into semantic web development on .NET.

Look out for blog posts from Carl in the next day or two, about using LinqToRdf with OpenLink [Virtuoso](http://virtuoso.openlinksw.com/).
