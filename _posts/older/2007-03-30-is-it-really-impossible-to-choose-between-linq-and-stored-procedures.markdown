---
author: aabs
comments: true
date: 2007-03-30 10:58:44+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2007/03/30/is-it-really-impossible-to-choose-between-linq-and-stored-procedures/
slug: is-it-really-impossible-to-choose-between-linq-and-stored-procedures
title: Is it really impossible to choose between LINQ and Stored Procedures?
wordpress_id: 366
tags:
- Computer Science
- databases
- LINQ
- ORM
- philosophy
- programming
- science
- Work
---




<blockquote>For the mathematician there is no [_Ignorabimus_](http://en.wikipedia.org/wiki/Ignorabimus), and, in my opinion, not at all for natural science either. ... The true reason why [no one] has succeeded in finding an unsolvable problem is, in my opinion, that there is no unsolvable problem. In contrast to the foolish Ignoramibus, our credo avers:
We must know,
We shall know.</blockquote>


It's that time of the month again – when all of the evangelically inclined [maven](http://en.wikipedia.org/wiki/Maven)s of Readify gather round to have the _traditional debate_. Despite the fact that they've had similar debates for years, they tend to tackle the arguments with gusto, trying to find a new angle of attack from which to sally forth in defence of their staunchly defended position. You may (assuming you never read the title of the post :) be wondering what is it that could inspire such fanatical and unswerving devotion? What is it that could polarise an otherwise completely rational group of individuals into opposing poles that consider the other completely mad?

What is this Lilliputian debate? I'm sure you didn't need to ask, considering it is symptomatic of the gaping wound in the side of modern software engineering. This flaw in software engineering is the elephant in the room that nobody talks about (although they talk an awful lot about the lack of space).

The traditional debate is, of course:


<blockquote>What's the point of a database?</blockquote>


And I'm sure that there's a lot I could say on the topic (there sure was yesterday) but the debate put me in a thoughtful mood. The elephant in the room, the gaping wound in the side of software engineering is just as simply stated:


<blockquote>How do we prove that a design is optimal?</blockquote>


That is the real reason we spend so much of our time rehearsing these architectural arguments, trying to win over the other side. Nobody gets evangelical about something they _just know_ - they only evangelise about things they are _not_ sure about. Most people don't proclaim to the world that the sun will rise tomorrow. But like me, you may well devote a lot of bandwidth to the idea that the object domain is paramount, not the relational. As an object oriented specialist that is my central creed and highest article of faith. The traditional debate goes on because we just don't have _proof _on either side. Both sides have thoroughly convincing arguments, and there is no decision procedure to choose between them.

So why don't we just resolve it once and for all? The computer science and software engineering fraternity is probably the single largest focussed accumulation of IQ points gathered in the history of mankind. They all focus intensively on issues just like this. Surely it is not beyond them to answer the simple question of whether we should put our business logic into stored procedures or use an ORM product to dynamically generate SQL statements. My initial thought was "_We Must Know, We Will Know_" or words to that effect. There is nothing that can't be solved given enough resolve and intelligence. If we have a will to do so, we could probably settle on a definitive way to describe an architecture so that we can decide what is best for a given problem.

Those of you who followed the link at the top of the post will have found references there to David Hilbert, and that should have given you enough of a clue to know that there's a chance that my initial sentiment was probably a pipe dream. If you are still in the dark, I'm referring to Hilbert's [Entscheidungsproblem](http://en.wikipedia.org/wiki/Entscheidungsproblem) (or the _Decision Problem_ in English) and I [beg](http://aabs.wordpress.com/2005/09/08/books-that-i-have-loved/) you to read Douglas Hofstadter's magnificent _Gödel, Escher, Bach – An eternal golden braid_. This book is at the top of my all-time favourites list, and among the million interesting topics it covers, the decision problem is central.


### The Decision Problem - a quick detour


One thing you'll notice about the Entscheidungsproblem and Turing's [_Halting Problem_](http://en.wikipedia.org/wiki/Halting_problem) is that they are equivalent. They seem to be asking about different things, but at a deeper level the problems are the same. The decision problem asks whether there is a mechanical procedure to determine the truth of any mathematical statement. At the turn of the century they might have imagined some procedure that cranked through every derivation of the axioms of mathematical logic till it found a proof of the statement returning true. The problem with that brute-force approach is that mathematics allows a continual complexification and simplification of statements – it is _non-monotonic_. The implication is that just because you have applied every combination of the construction rules on all of the axioms up to a given length you can't know whether there are new statements of the same length that could be found by the repeated application of growth and shrinkage rules that aren't already in your list. That means that even though you may think you have a definitive list of all the true statements of a given length you may be wrong, so you can never give a false, only continue till you either find a concrete proof or disproof.

Because of these non-monotonic derivation rules, you will never be sure that no answer from your procedure means an answer of **false**. You will always have to wait and see. This is the equivalence between the Entscheidungsproblem and Alan Turing's Halting Problem. If you knew your procedure would not halt, then you would just short-circuit the decision process and immediately answer **false**. If you knew that the procedure _would_ halt, then you would just let it run and produce whatever true/false answer it came up with – either way, you would have a decision procedure. Unfortunately it's not that easy, because the halting decision procedure has no overview of the whole of mathematics either, and can't give an answer to the halting question. Ergo there is no decision procedure either. Besides, Kurt Gödel proved that there were undecidable problems, and the quest for a decision procedure was doomed to fail. he showed that even if you came up with a more sophisticated procedure than the brute force attack, you would still never get a decision procedure for all mathematics.


### The Architectural Decision Problem


What has this got to do with deciding on the relative merits of two software designs? Is the issue of deciding between two designs also equivalent to the decision problem? Is it a constraint optimisation problem? You could enumerate the critical factors, assign a rank to them and then sum the scores for each design? That is exactly what I did in one of my recent [posts](http://aabs.wordpress.com/2007/01/23/the-great-domain-model-debate-%e2%80%93-solved/) entitled "_The great Domain Model Debate – Solved!_" Of course the '_Solved!_' part was partly tongue-in-cheek – I just provided a decision procedure for readers to distinguish between the competing designs of domain models.

One of the criticisms levelled at my offering for this problem was that my weights and scores were too subjective. I maintained that although my heuristic was flawed, it held the key to solving these design issues because there was the hope that there _are _objective measures of the importance of design criteria for each design, and it was possible to quantify the efficacy of each approach. But I'm beginning to wonder whether that's really true. Let's consider the domain model approach for a moment to see how we could quantify those figures.

Imagine that we could enumerate all of the criteria that pertained to the problem. Each represents an aspect of the value that architects place in a good design. In my previous post I considered such things as complexity, density of data storage, performance, maintainability etc. Obviously each of these figures varies in just how subjective it is. Complexity is a measure of how easy it is to understand. One programmer may be totally at home with a design whereas another may be confused. But there [are ](http://en.wikipedia.org/wiki/Algorithmic_complexity)measures of complexity that _are _objective that we could use. We could use that as an indicator of maintainability – the more complex a design is, the harder it would be to maintain.

This complexity measure would be more fundamental than any mere subjective measure, and would be tightly correlated with the subjective measure. Algorithmic complexity would be directly related to the degree of confusion a given developer would experience when first exposed to the design. Complexity affects our ability to remember the details of the design (as it is employed in a given context) and also our ability to mentally visualise the design and its uses. When we give a subjective measure of something like complexity, it may be due to the fact that we are looking at it from the wrong level. Yes, there is a subjective difference, but that is because of an objective difference that we are responding to.

It's even possible to prove that such variables exist, so long as we are willing to agree that a subjective dislike that is completely whimsical is not worth incorporating into an assessment of a design's worth. I'm thinking of such knee-jerk reactions like '_we never use that design here_' or '_I don't want to use it because I heard it was no good_'. Such opinions whilst strongly felt are of no value, because they don't pertain to the design per-se but rather to a free-standing psychological state in the person who has them. The design could still be optimal, but that wouldn't stop them from having that opinion. Confusion on the other hand has its origin in some aspect of the design, and thus should be factored in.

For each subjective criterion that we currently use to judge a design, there must be a set of objective criteria that cause it. If there are none, then we can discount it - it contributes nothing to an objective decision procedure – it is just a prejudice. If there are objective criteria, then we can substitute all occurrences of the subjective criterion in the decision procedure with the set of objective criteria. If we continue this process, we will eventually be left with nothing but objective criteria. At that point are we in a position to choose between two designs?


### Judging a good design


It still remains to be seen whether we can enumerate all of the objective criteria that account for our experiences with a design, and its performance in production. It also remains for us to work out ways to measure them, and weigh their relative importance over other criteria. We are still in danger of slipping into a world of subjective opinions over what is most important. We should be able to apply some rigour because we're aiming at a stationary target. Every design is produced to fulfil a set of requirements. Provided those requirements are fulfilled we can assess the design solely in terms of the objective criteria. We can filter out all of the designs that are incapable of meeting the requirements – all the designs that are left are guaranteed to do the job, but some will be better than others. If that requires that we formally specify our designs and requirements then (for the purposes of this argument) so be it. All that matters is that we are able to guarantee that all remaining designs are fit to be used. All that distinguishes them are performance and other quality criteria that can be objectively measured.

Standard practice in software engineering is to reduce a problem to its component parts, and attempt to then compose the system from those components in a way that fulfils the requirements for the system. Clearly there are internal structures to a system, and those structures cannot necessarily be weighed in isolation. There is a context in which parts of a design make sense, and they can only be judged within that context. Often we judge our design patterns as though they were isolated systems on their own. That's why people sometimes decide to use design patterns before they have even worked out if they are [appropriate](http://aabs.wordpress.com/2007/03/08/singleton-%e2%80%93-the-most-overused-pattern/). The traditional debate is one where we judge the efficacy of a certain type of data access approach in isolation of the system it's to be used in. I've seen salesmen for major software companies do the same thing – their marks have decided they are going to use the product before they've worked out why they will do so. I wonder whether the components of our architectural decision procedure can be composed in the same way that our components are.

In the context that they're to be used, will all systems have a monotonic effect on the quality of a system?  Could we represent the quality of our system as a sum of scores of the various sub-designs in the system like this? (Q1 + Q2 + ... + Qn) That would assume that the quality of the system is the sum of the quality of its parts which seems a bit naive to me – some systems will work well in combination, others will limit the damage of their neighbours and some will exacerbate problems that would have lain dormant in their absence. How are we to represent the calculus of software quality? Perhaps the answer lies in the architecture itself? If you were to measure the quality of each unique path through the system, then you would have a way to measure the quality of that path as though it was a sequence of operations with no choices or loops involved. You could then sum the quality of each of these paths weighted in favour of frequency of usage. That would eliminate all subjective bias and the impact of all sub designs would be proportional to the centrality of its role within the system as a whole. In most systems data access plays a part in pretty much all paths through a system, hence the disproportionate emphasis we place on it in the traditional debates.


### Scientific Software Design?


Can we work out what these criteria are? If we could measure every aspect of the system (data that gets created, stored, communicated, the complexity of that data etc) then we have the physical side of the picture – what we still lack is all of those thorny subjective measures that matter. Remember though that these are the subjective measures that _can _be converted into objective measures. Each of those measures can thus be added to the mix. What's left? All of the criteria that we don't know to ask about, and all of the physical measurements that we don't know how to make, or don't even know we should make. That's the elephant in the room. You don't know what you don't know. And if you did, then it would pretty immediately fall to some kind of scientific enquiry. But will we be in the same situation as science and mathematics was at the dawn of the 20th Century? Will we, like Lord Kelvin, declare that everything of substance about software architecture is known and all the future holds for us is the task of filling in the gaps?

Are these unknown criteria like the unknown paths through a mathematical derivation? Are they the fatal flaw that unhinges any attempt to assess the quality of a design, or are they the features that turns software engineering into a weird amalgam of mathematics, physics and psychology? There will never be any way for us to unequivocally say that we have found all of the criteria that truly determine the quality of a design. Any criteria that we can think of we can objectify – but it's the ones we can't or don't think of that will undermine our confidence in a design and doom us to traditional debates. Here's a new way to state Hilbert's 10th Problem:


<blockquote>Is there a way to fully enumerate all of the criteria that determine the quality of a software design?</blockquote>


Or to put it another way


<blockquote>Will we know when we know enough to distinguish good designs from bad?</blockquote>


The spirit of the enlightenment is fading. That much is certain. The resurgence of religiosity in all parts of the world is a backward step. It pulls us away from that pioneering spirit that Kant called a maturing of the human spirit. Maturity is no longer needing authority figures to tell us what to think. He was talking about the grand program to roll back the stifling power of the church. In software design we still cling to the idea that there are authority figures that are infallible. When they proclaim a design as sound, then we use it without further analysis. Design patterns are our scriptures, and traditional wisdom the ultimate authority by which we judge our designs. I want to see the day when scientific method is routinely brought to bear on software designs. Only then will we have reached the state of maturity where we can judge each design on its objective merits. I wonder what the Readify Tech List will be like then?
