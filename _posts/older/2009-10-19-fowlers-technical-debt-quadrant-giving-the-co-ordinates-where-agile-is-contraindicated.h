---
layout: post
title: Fowler's Technical Debt Quadrant - Giving the co-ordinates where Agile is contraindicated.
date: 2009-10-19 08:39:32.000000000 +11:00
type: post
published: true
status: publish
categories:
- programming
tags:
- agile
- design
- martin fowler
- methodology
- technical debt
meta:
  _edit_last: '154469'
author:
  login: aabs
  email: matthews.andrew@gmail.com
  display_name: Andrew Matthews
  first_name: Andrew
  last_name: Matthews
---
<p>Martin Fowler's bliki has a very good post on what he calls the '<a href="http://martinfowler.com/bliki/TechnicalDebtQuadrant.html">technical debt quadrant</a>'. This post succinctly sums up the difference between those who produce poor designs in ways that are contrary to their best interests, and those who do so knowingly and reluctantly.</p>
<p>In the past I've noted that many of those who profess to being agile are really just defaulters in the bank of technical debt. They confuse incurring inadvertent reckless technical debt with being Agile. The <a href="http://en.wikipedia.org/wiki/YAGNI">YAGNI</a> (You Ain't Gonna Need It) maxim is an example of something that, when used prudently and knowingly, is a sensible (temporary) acceptance of short term technical debt. In the hands of those who recklessly and inadvertently incur technical debt, it becomes a waiver for all sorts of abuses of good software development practice. It's not uncommon in small cash-strapped start-ups to hear people invoke the YAGNI maxim as they contemplate key architectural issues, confusing cost-cutting with architectural parsimony. I think it's safe to say that those who live in the dark waters of the bottom left of the quadrant need the formality of a more 'regulated'  process to support them as they grope their way into the light.</p>
<p>Does formality have to mean expensive? I don't think so. I think in most cases it's a matter of perception - people assume that 'formality' implies back-breaking bureaucracy. It needn't, it just means creating a framework that helps people to stop, take a breath and look around them. Without that, people often lose their way and drive headlong into crippling technical debt. I've seen large companies that consciously avoided using up-front design subsequently paying the cost of that debt for years afterwards in maintenance. One notable example would regularly burn through its OpEx budget within the first 6 months of each year - they had a product that was so badly designed, inconsistent and hard to maintain that they lost all forward momentum, threw away their monopoly and effectively lost their place in the market. All through lack of up-front design and architecture.</p>
<p>For all companies where stakeholders are either so stressed or so inexperienced that they opt for the lower left of Fowler's quadrant, Agile is contraindicated.</p>
