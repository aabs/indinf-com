---
author: aabs
comments: true
date: 2007-06-28 21:33:55+00:00
excerpt: I thought it was worth describing this configuration solution, since it was
  so neat, and easy, and had all of the benefits of text based configuration and strongly
  typed inline configuration.
layout: post
link: https://aabs.wordpress.com/2007/06/29/dynamic-strongly-typed-configuration-in-c/
slug: dynamic-strongly-typed-configuration-in-c
title: Dynamic Strongly-Typed Configuration in C#
wordpress_id: 401
tags:
- C#
- Configuration
- programming
---

I've written at great length in the past about the perils of configuration, and I thought I'd written as much as I was willing on the topic. But I thought it was worth describing this solution, since it was so neat, and easy, and had most of the benefits of text based configuration and strongly typed inline configuration. I was recently messing about with some WCF P2P code, and the setup code had some configuration that looked like a likely candidate for a strongly typed configuration object that wouldn't change frequently. I think this solution neatly addresses one of the main objections to hard coded configuration, which is that we do sometimes need to change configuration data at runtime without having to take down the servers or recompile them. 

The idea behind this solution stems from the use of a plug-in architecture such as the forthcoming System.AddIn namespace to arrive in VS2008. In that you get the options to load a namespace from a designated directory and make use of types found inside of the assembly. Why not use the same approach with configuration? We can dynamically load configuration assemblies and then use a single configuration setting to specify which type from those assemblies would be used as the new configuration. This has all the benefits normally reserved for text based dynamic configuration such as System.Configuration.ConfigurationManager, but with the added benefits of strong typing, inheritance, calculated configuration settings and added performance of POCOs.  

My WCF program was a simple chat client that I hope to be able to use between members of my family. Typical configurations were MeshAddress, and CredentialType that are unlikely to ever change frequently. Each of these configuration settings was defined on an interface called IChatClientConfig. Implementing that full interface was my default configuration class called DefaultChatConfig. That provided all of my defaults, and is perfectly usable. I then specialized that class with some others, for example with a different mesh address for chatting with people at work. A class diagram for the configuration objects are shown below.  

[![clip_image001](http://aabs.files.wordpress.com/2007/06/clip-image001-thumb.png)](http://aabs.files.wordpress.com/2007/06/clip-image001.png)  

Each class just provides a new implementation for the field that it provides a different value for.  

Loading the configuration is extremely simple. First you have to say which one of those classes you want to use for your configuration.
    
    <span style="color:rgb(0,0,255);"><</span><span style="color:rgb(163,21,21);">appSettings</span><span style="color:rgb(0,0,255);">>
        <</span><span style="color:rgb(163,21,21);">add</span><span style="color:rgb(0,0,255);"> </span><span style="color:rgb(255,0,0);">key</span><span style="color:rgb(0,0,255);">=</span>"<span style="color:rgb(0,0,255);">P2PConfigSettings</span>"<span style="color:rgb(0,0,255);"> </span><span style="color:rgb(255,0,0);">value</span><span style="color:rgb(0,0,255);">=</span>"<span style="color:rgb(0,0,255);">ChatClient.Configuration.TechChatConfig, ChatClient.Configuration, Version=1.0.0.0</span>"<span style="color:rgb(0,0,255);">/>
    </</span><span style="color:rgb(163,21,21);">appSettings</span><span style="color:rgb(0,0,255);">>
    </span>

[](http://11011.net/software/vspaste)


This simple app setting is the fully qualified type name of the TechChatConfig class on the bottom right of the diagram above. Which will be a default chat configuration with whatever tech chat configuration added. That's all the prerequisites for loading configuration. Not all I need to do to load the configuration is this:
    
    <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(43,145,175);">IChatClientConfig</span> GetConfigObject()
    {
        <span style="color:rgb(0,0,255);">string</span> configType = <span style="color:rgb(43,145,175);">ConfigurationManager</span>.AppSettings[<span style="color:rgb(163,21,21);">"P2PConfigSettings"</span>];
        <span style="color:rgb(43,145,175);">Type</span> t = <span style="color:rgb(43,145,175);">Type</span>.GetType(configType);
        <span style="color:rgb(0,0,255);">return</span> <span style="color:rgb(43,145,175);">Activator</span>.CreateInstance(t) <span style="color:rgb(0,0,255);">as</span> <span style="color:rgb(43,145,175);">IChatClientConfig</span>;
    }
    

[](http://11011.net/software/vspaste)


Get whatever type I specified as a string from the configuration file, get the type specified by that string create and instance and return it. Simple. That configuration could be then stored as a singleton or whatever you need to do.
    
    [<span style="color:rgb(43,145,175);">ServiceBehavior</span>(InstanceContextMode = <span style="color:rgb(43,145,175);">InstanceContextMode</span>.Single)]
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">partial</span> <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">Window1</span> : <span style="color:rgb(43,145,175);">IPeerChat
    </span>{
        <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(43,145,175);">IChatClientConfig</span> configuration;
    

[](http://11011.net/software/vspaste)


In my case I just stored it in the window object I was using it for - my chat client only has one window! Now I can just use it, whenever I do any comms.
    
    <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(43,145,175);">NetPeerTcpBinding</span> CreateBindingForMesh()
    {
        <span style="color:rgb(43,145,175);">NetPeerTcpBinding</span> binding = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">NetPeerTcpBinding</span>();
        binding.Resolver.Mode = config.PeerResolverMode;
        binding.Security.Transport.CredentialType = config.CredentialType;
        binding.MaxReceivedMessageSize = config.MaxReceivedMessageSize;
        <span style="color:rgb(0,0,255);">return</span> binding;
    }
    

[](http://11011.net/software/vspaste)[](http://11011.net/software/vspaste)


So you see that the process is very simple. With the addition of an AddIn model we could use a file system monitor to watch the configuration file, detect changes and reload the configuration object singleton using the mechanism described above. That fulfils most of the requirements that we have for type safety, performance, dynamism, intelligence, and object orientation. Very few configuration scenarios that fall outside of the bounds of this solution should be solved using local configuration settings anyway - in those cases you really ought to be looking at an administration console and database.
