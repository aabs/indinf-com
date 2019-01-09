
---
layout: page
permalink: /blog/
title: blog
description: Some memento's of a life spent banging my head against a monitor
---

<ul class="post-list">
{% for post in site.blog reversed %}
    <li>
        <h2><a class="post-title" href="{{ post.url | prepend: site.baseurl }}">{{ post.title }}</a></h2>
        <p class="post-meta">{{ post.date | date: '%B %-d, %Y — %H:%M' }}</p>
      </li>
{% endfor %}
</ul>
