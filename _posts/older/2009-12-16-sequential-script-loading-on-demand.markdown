---
author: aabs
comments: true
date: 2009-12-16 00:26:13+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2009/12/16/sequential-script-loading-on-demand/
slug: sequential-script-loading-on-demand
title: Sequential script loading on demand
wordpress_id: 676
categories:
- functional programming
- programming
tags:
- functional programming
- javascript
- jquery
---

This little script uses the JQuery getScript command, enforcing sequential loading order to ensure script dependencies are honoured:

`

    
    function LoadScriptsSequentially(scriptUrls, callback)
    {
        if (typeof scriptUrls == 'undefined') throw "Argument Error: URL array is unusable";
        if (scriptUrls.length == 0 && typeof callback == 'function') callback();
        $.getScript(scriptUrls.shift(), function() { LoadScriptsSequentially(scriptUrls, callback); });
    }


`Here's how you use it:

`

    
    function InitialiseQueryFramework(runTests)
    {
        LoadScriptsSequentially([
            "/js/inheritance.js",
            "/js/expressions.js",
            "/js/refData.js",
            "/js/queryRenderer.js",
            "/js/sha1.js",
            "/js/queryCache.js",
            "/js/unit-tests.js"],
            function()
            {
                queryFramework = new QueryManager("#query");
                if (runTests) RunTestSuite();
            });
    }


`I love javascript now and can't understand why I avoided it for years. I particularly love the hybrid fusion of functional and procedural paradigms that possible in JS. You can see that at work in the parameters being passed into the recursive call to LoadScriptsSequentially.

What do you think? Is there a better/nicer way to do this?
