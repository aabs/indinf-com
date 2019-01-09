---
author: aabs
comments: true
date: 2007-05-31 05:24:28+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2007/05/31/windows-live-writer-beta-2-vs-paste-and-wordpress/
slug: windows-live-writer-beta-2-vs-paste-and-wordpress
title: Windows Live Writer Beta 2, VS Paste and Wordpress
wordpress_id: 383
---

Here's an example I pasted from [LinqToRdf](http://code.google.com/p/linqtordf/).
    
    <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">void</span> GenerateBinaryExpression(<span style="color:rgb(0,128,128);">Expression</span> e, <span style="color:rgb(0,0,255);">string</span> op)
    {
        <span style="color:rgb(0,0,255);">if</span> (e == <span style="color:rgb(0,0,255);">null</span>)
            <span style="color:rgb(0,0,255);">throw</span> <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(0,128,128);">ArgumentNullException</span>(<span style="color:rgb(128,0,0);">"e was null"</span>);
        <span style="color:rgb(0,0,255);">if</span> (op == <span style="color:rgb(0,0,255);">null</span>)
            <span style="color:rgb(0,0,255);">throw</span> <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(0,128,128);">ArgumentNullException</span>(<span style="color:rgb(128,0,0);">"op was null"</span>);
        <span style="color:rgb(0,0,255);">if</span> (op.Length == 0)
            <span style="color:rgb(0,0,255);">throw</span> <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(0,128,128);">ArgumentNullException</span>(<span style="color:rgb(128,0,0);">"op.Length was empty"</span>);
        <span style="color:rgb(0,128,128);">BinaryExpression</span> be = e <span style="color:rgb(0,0,255);">as</span> <span style="color:rgb(0,128,128);">BinaryExpression</span>;
        <span style="color:rgb(0,0,255);">if</span> (be != <span style="color:rgb(0,0,255);">null</span>)
        {
            QueryAppend(<span style="color:rgb(128,0,0);">"("</span>);
            Dispatch(be.Left);
            QueryAppend(<span style="color:rgb(128,0,0);">")"</span>+op+<span style="color:rgb(128,0,0);">"("</span>);
            Dispatch(be.Right);
            QueryAppend(<span style="color:rgb(128,0,0);">")"</span>);
            Log(<span style="color:rgb(128,0,0);">"+ :{0} Handled"</span>, e.NodeType);
        }
    }




If this displays properly for you, then I am a happy man.

[](http://11011.net/software/vspaste)
