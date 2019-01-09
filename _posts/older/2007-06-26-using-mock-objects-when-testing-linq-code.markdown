---
author: aabs
comments: true
date: 2007-06-26 02:56:49+00:00
layout: post
link: https://aabs.wordpress.com/2007/06/26/using-mock-objects-when-testing-linq-code/
slug: using-mock-objects-when-testing-linq-code
title: Using Mock Objects When Testing LINQ Code
wordpress_id: 398
tags:
- C#
- LINQ
- programming
---

I was wondering the other day whether LINQ could be used with NMock easily. One problem with testing code that has not been written to work with unit tests is that if you test business logic, you often end up making multiple round-trips to the database for each test run. With a very large test suite that can turn a few minute's work into hours for a test suite. the best approach to this is to use mock data access components to dispense canned results, rather than going all the way through to the database.

After a little thought it became clear that all you have to do is override the **_IOrderedQueryable<T>.GetEnumerator()_** method to return an enumerator to a set of canned results and you could pretty much impersonate a LINQ to SQL Table (which is the **_IOrderedQueryable_** implementation for LINQ to SQL). I had a spare few minutes the other day while the kids were going to sleep and I decided to give it a go, to see what was involved.

I'm a great believer in the medicinal uses of mock objects. Making your classes testable using mocking enforces a level of encapsulation that adds good structure to your code. I find that the end results are often much cleaner if you design your systems with mocking in mind.

Lets start with a class that you were querying over in your code. This is the type that you are expecting to get back from your query. 
    
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">MyEntity
    </span>{
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">string</span> Name
        {
            <span style="color:rgb(0,0,255);">get</span> { <span style="color:rgb(0,0,255);">return</span> name; }
            <span style="color:rgb(0,0,255);">set</span> { name = <span style="color:rgb(0,0,255);">value</span>; }
        }
    
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">int</span> Age
        {
            <span style="color:rgb(0,0,255);">get</span> { <span style="color:rgb(0,0,255);">return</span> age; }
            <span style="color:rgb(0,0,255);">set</span> { age = <span style="color:rgb(0,0,255);">value</span>; }
        }
    
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">string</span> Desc
        {
            <span style="color:rgb(0,0,255);">get</span> { <span style="color:rgb(0,0,255);">return</span> desc; }
            <span style="color:rgb(0,0,255);">set</span> { desc = <span style="color:rgb(0,0,255);">value</span>; }
        }
    
        <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">string</span> name;
        <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">int</span> age;
        <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">string</span> desc;
    }
    

[](http://11011.net/software/vspaste)


Now you need to create a new context object derived from the DLINQ **_DataContext_** class, but providing a new constructor function. You can create other ways to insert the data you want your query to return, but the constructor is all that is necessary for this simple example.
    
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">MockContext</span> : <span style="color:rgb(43,145,175);">DataContext
    </span>{
    <span style="color:rgb(0,0,255);">    #region</span> constructors
    
        <span style="color:rgb(0,0,255);">public</span> MockContext(<span style="color:rgb(43,145,175);">IEnumerable</span> col):<span style="color:rgb(0,0,255);">base</span>(<span style="color:rgb(163,21,21);">""</span>)
        {
            User = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">MockQuery</span><<span style="color:rgb(43,145,175);">MyEntity</span>>(col);
        }
        <font color="#008000">// other constructors removed for readability</font>
    <span style="color:rgb(0,0,255);">    #endregion
    </span>    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(43,145,175);">MockQuery</span><<span style="color:rgb(43,145,175);">MyEntity</span>> User;
    }

[](http://11011.net/software/vspaste)


Note that you are passing in an untyped **_IEnumerable_** rather than an **_IEnumerable<T>_** or a concrete collection class. The reason is that when you make use of projections in LINQ, the type gets transformed along the way. Consider the following:
    
    var q = from u <span style="color:rgb(0,0,255);">in</span> db.User
            where u.Name.Contains(<span style="color:rgb(163,21,21);">"Andrew"</span>) && u.Age < 40
            select <span style="color:rgb(0,0,255);">new</span> {u.Age};
    

[](http://11011.net/software/vspaste)


The result of **_db.User_** is an **_IOrderedQueryable<User>_** query class which is derived from **_IEnumerable<User>_**. But the result that goes into **_q_** is an **_IEnumerable_** of some anonymous type created specially for the occasion. there is a step along the way when the **_IQueryable<User>_** gets replaced with an **_IQueryable<AnonType>_**. If I set the type on the enumerator of the canned results, I would have to keep track of them with each call to **_CreateQuery_** in my Mock Query class. By using **_IEnumerable_**, I can just pass it around till I need it, then just enumerate the collection with a custom iterator, casting the types to what I ultimately need as I go.




The query object also has a constructor that takes an **_IEnumerable_**, and it keeps that till **_GetEnumerator()_** gets called later on. **_CreateQuery_** and **_CloneQueryForNewType_** just pass the **_IEnumerable _**around till the time is right. **_GetEnumerator_** just iterates the collection in the **_cannedResponse_** iterator casting them to the return type expected for the resulting query.
    
    <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">MockQuery</span><T> : <span style="color:rgb(43,145,175);">IOrderedQueryable</span><T>
    {
        <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">readonly</span> <span style="color:rgb(43,145,175);">IEnumerable</span> cannedResponse;
    
        <span style="color:rgb(0,0,255);">public</span> MockQuery(<span style="color:rgb(43,145,175);">IEnumerable</span> cannedResponse)
        {
            <span style="color:rgb(0,0,255);">this</span>.cannedResponse = cannedResponse;
        }
    
        <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(43,145,175);">Expression</span> expression;
        <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(43,145,175);">Type</span> elementType;
    
    <span style="color:rgb(0,0,255);">    #region</span> IQueryable<T> Members
    
        <span style="color:rgb(43,145,175);">IQueryable</span><S> <span style="color:rgb(43,145,175);">IQueryable</span><T>.CreateQuery<S>(<span style="color:rgb(43,145,175);">Expression</span> expression)
        {
            <span style="color:rgb(43,145,175);">MockQuery</span><S> newQuery = CloneQueryForNewType<S>();
            newQuery.expression = expression;
            <span style="color:rgb(0,0,255);">return</span> newQuery;
        }
    
        <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(43,145,175);">MockQuery</span><S> CloneQueryForNewType<S>()
        {
            <span style="color:rgb(0,0,255);">return</span> <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">MockQuery</span><S>(cannedResponse);
        }
    <span style="color:rgb(0,0,255);">    #endregion
    
        #region</span> IEnumerable<T> Members
        <span style="color:rgb(43,145,175);">IEnumerator</span><T> <span style="color:rgb(43,145,175);">IEnumerable</span><T>.GetEnumerator()
        {
            <span style="color:rgb(0,0,255);">foreach</span> (T t <span style="color:rgb(0,0,255);">in</span> cannedResponse)
            {
                <span style="color:rgb(0,0,255);">yield</span> <span style="color:rgb(0,0,255);">return</span> t;
            }
        }
    <span style="color:rgb(0,0,255);">    #endregion
    </span><span style="color:rgb(0,0,255);">
        #region</span> IQueryable Members
        <span style="color:rgb(43,145,175);">Expression</span> <span style="color:rgb(43,145,175);">IQueryable</span>.Expression
        {
            <span style="color:rgb(0,0,255);">get</span> { <span style="color:rgb(0,0,255);">return</span> System.Expressions.<span style="color:rgb(43,145,175);">Expression</span>.Constant(<span style="color:rgb(0,0,255);">this</span>); }
        }
    
        <span style="color:rgb(43,145,175);">Type</span> <span style="color:rgb(43,145,175);">IQueryable</span>.ElementType
        {
            <span style="color:rgb(0,0,255);">get</span> { <span style="color:rgb(0,0,255);">return</span> elementType; }
        }
    <span style="color:rgb(0,0,255);">    #endregion
    </span>}
    

[](http://11011.net/software/vspaste)


For the sake of readability I have left out the required interface methods that were not implemented, since they play no part in this solution. Now lets look at a little test harness:
    
    <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">Program
    </span>{
        <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">void</span> Main(<span style="color:rgb(0,0,255);">string</span>[] args)
        {
            <span style="color:rgb(43,145,175);">MockContext</span> db = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">MockContext</span>(GetMockResults());
    
            var q = from u <span style="color:rgb(0,0,255);">in</span> db.User
                    where u.Name.Contains(<span style="color:rgb(163,21,21);">"Andrew"</span>) && u.Age < 40
                    select u;
            <span style="color:rgb(0,0,255);">foreach</span> (<span style="color:rgb(43,145,175);">MyEntity</span> u <span style="color:rgb(0,0,255);">in</span> q)
            {
                <span style="color:rgb(43,145,175);">Debug</span>.WriteLine(<span style="color:rgb(0,0,255);">string</span>.Format(<span style="color:rgb(163,21,21);">"entity {0}, {1}, {2}"</span>, u.Name, u.Age, u.Desc));
            }
        }
    
        <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(43,145,175);">IEnumerable</span> GetMockResults()
        {
            <span style="color:rgb(0,0,255);">for</span> (<span style="color:rgb(0,0,255);">int</span> i = 0; i < 20; i++)
            {
                <span style="color:rgb(43,145,175);">MyEntity</span> r = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">MyEntity</span>();
                r.Name = <span style="color:rgb(163,21,21);">"name "</span> + i;
                r.Age = 30 + i;
                r.Desc = <span style="color:rgb(163,21,21);">"desc "</span> + i;
                <span style="color:rgb(0,0,255);">yield</span> <span style="color:rgb(0,0,255);">return</span> r;
            }
        }
    }

[](http://11011.net/software/vspaste)


The only intrusion here is the explicit use of **_MockContext_**. In the production code that is to be tested, you can't just go inserting **_MockContext_** where you would have used the SqlMetal generated context. You need to use a class factory that will allow you to provide the **_MockContext_** on demand in a unit test, but dispense a true LINQ to SQL context when in production. That way, all client code will just use mock data without knowing it.




Here's the pattern that I generally follow. I got it from the Java community, but I can't remember where:
    
    <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">DbContextClassFactory
    </span>{
        <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">Environment
    </span>    {
            <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">bool</span> inUnitTest = <span style="color:rgb(0,0,255);">false</span>;
    
            <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(0,0,255);">bool</span> InUnitTest
            {
                <span style="color:rgb(0,0,255);">get</span> { <span style="color:rgb(0,0,255);">return</span> <span style="color:rgb(43,145,175);">Environment</span>.inUnitTest; }
                <span style="color:rgb(0,0,255);">set</span> { <span style="color:rgb(43,145,175);">Environment</span>.inUnitTest = <span style="color:rgb(0,0,255);">value</span>; }
            }
            <span style="color:rgb(0,0,255);">private</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(43,145,175);">DataContext</span> objectToDispense = <span style="color:rgb(0,0,255);">null</span>;
    
            <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">static</span> <span style="color:rgb(43,145,175);">DataContext</span> ObjectToDispense
            {
                <span style="color:rgb(0,0,255);">get</span> { <span style="color:rgb(0,0,255);">return</span> <span style="color:rgb(43,145,175);">Environment</span>.objectToDispense; }
                <span style="color:rgb(0,0,255);">set</span> { <span style="color:rgb(43,145,175);">Environment</span>.objectToDispense = <span style="color:rgb(0,0,255);">value</span>; }
            }
        }
    
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">object</span> GetDB()
        {
            <span style="color:rgb(0,0,255);">if</span> (<span style="color:rgb(43,145,175);">Environment</span>.InUnitTest)
                <span style="color:rgb(0,0,255);">return</span> <span style="color:rgb(43,145,175);">Environment</span>.ObjectToDispense;
            <span style="color:rgb(0,0,255);">return</span> <span style="color:rgb(0,0,255);">new</span> TheRealContext() <span style="color:rgb(0,0,255);">as</span> <span style="color:rgb(43,145,175);">DataContext</span>;
        }
    }
    

[](http://11011.net/software/vspaste)


Now you can create your query like this:
    
    DbContextClassFactory.Environment.ObjectToDispense = <span style="color:rgb(0,0,255);">new</span> MockContext(GetMockResults());
    var q = from u <span style="color:rgb(0,0,255);">in</span> DbContextClassFactory.GetDB() where ...

[](http://11011.net/software/vspaste)


And your client code will use the **_MockContext_** if there is one, otherwise it will use a LINQ to SQL context to talk to the real database. Perhaps we should call this _Mockeries_ rather than Mock Queries. What do you think?
