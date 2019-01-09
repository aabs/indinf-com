---
author: aabs
comments: true
date: 2007-10-22 04:56:26+00:00
layout: page
header: no
link: https://aabs.wordpress.com/2007/10/22/silverlight-htmlelement-wrappers/
slug: silverlight-htmlelement-wrappers
title: Silverlight HtmlElement wrappers
wordpress_id: 431
categories:
- programming
---

I've been glancing at the HtmlElement class in Silverlight today. It's a nice system, but it's clearly a work in progress. Everything I've said in the past about lumping all of your object model into a single class applies to this. The thing that irritated me was that it wouldn't have been hard for the Silverlight 1.1 team to make the developer experience more palatable even if they were using the one class under the hood.`
[sourcecode language='csharp']private void Test1()
{
  HtmlDocument document = HtmlPage.Document;
  document.GetElementByID("Submit1").AttachEvent("onclick",
    (object obj, EventArgs args) =>
      {
        Text1.SetAttribute("value", (++counter).ToString());
        Text1.SetStyleAttribute("color", "blue");
      }
    );
}[/sourcecode]
`

Not a lot you can say about this other than "_Where's the classes, dude?_". It wouldn't have been hard, to give us something like this:
`
[sourcecode language='csharp']private void Test1()
{
  HtmlDocument document = HtmlPage.Document;
  document.GetElementByID("Submit1").AttachEvent("onclick",
    (object obj, EventArgs args) =>
      {
      Text1.Text = (++counter).ToString();
      Text1.Style["color"] = "blue";
      }
    );
}[/sourcecode]
`

Don't believe me? Well, here's a wrapper around an HtmlElement that refers to an input element of type text.
`
[sourcecode language='csharp']public class TextBox : HtmlElement
{
  HtmlElement elem;
  public TextBox(HtmlElement elem)
  {
    this.elem = elem;
    Style = new StyleSet(elem);
  }
  public string Text
  {
    get
    {
      return elem.GetAttribute("value");
    }
    set
    {
      elem.SetAttribute("value", value.ToString());
    }
  }
  public StyleSet Style
  {
    get;
    set;
  }
}[/sourcecode]
`

This class uses a StyleSet class that provides an indexer into the styles of the HtmlElement:
`
[sourcecode language='csharp']public class StyleSet
{
  private HtmlElement Element{get;set;}
  public StyleSet(HtmlElement elem)
  {
    Element = elem;
  }
  public string this[string key]
  {
    get
    {
      return Element.GetStyleAttribute(key);
    }
    set
    {
      Element.SetStyleAttribute(key, value);
    }
  }
}[/sourcecode]
`

As you can see, this is pretty simple stuff. All I need to do is provide a conversion when I want to get to the element in my code behind:
`
[sourcecode language='csharp']public TextBox Text1
{
  get
  {
    HtmlDocument document = HtmlPage.Document;
    return new TextBox(document.GetElementByID("Text1"));
  }
}[/sourcecode]
`

This wraps the HtmlElement in a TextBox instance, and from there on you can access it in way that appeals to programmers.
