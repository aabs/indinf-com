---
author: aabs
comments: true
date: 2006-12-12 06:40:05+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2006/12/12/problems-with-virtual-server-on-vista/
slug: problems-with-virtual-server-on-vista
title: Problems with Virtual Server on Vista
wordpress_id: 328
tags:
- programming
---

I recently installed Vista on my laptop and needed to install Virtual Server 2005 R2. I ran into loads of problems, such as "std::wstring::find() failed" and "LsaRegisterLogonProcess()" failed.


If you experience any of those problems, please check that you have done the following:


  * Done all of the recommendations in this post: [http://blogs.technet.com/mattmcspirit/archive/2006/10/18/running-virtual-server-on-vista.aspx?CommentPosted=true#commentmessage](http://blogs.technet.com/mattmcspirit/archive/2006/10/18/running-virtual-server-on-vista.aspx?CommentPosted=true#commentmessage)
		
  * Set up the VirtualServer virtual directory to use the "Classic .NET AppPool"

  * Check that the Classic .NET AppPool is using LocalSystem identity

  * Check that ONLY Windows Authentication is set for the  virtual directory. This was a killer for me, because I didn't realize that IIS7 will just pick the first authentication mechanism it comes upon. So if Anonymous access or basic authentication are also enabled, then things start to go very wrong.


Hope that helps.
