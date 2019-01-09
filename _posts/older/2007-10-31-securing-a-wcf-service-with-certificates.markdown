---
author: aabs
comments: true
date: 2007-10-31 05:03:38+00:00
layout: post
link: https://aabs.wordpress.com/2007/10/31/securing-a-wcf-service-with-certificates/
slug: securing-a-wcf-service-with-certificates
title: Securing a WCF service with Certificates
wordpress_id: 452
categories:
- programming
- WCF
---

For a client and server to talk securely with message level encryption, they both require certificates to encrypt and decrypt messages from the other. Those certificates need to be produced and installed on each machine. This post shows how that can be done, and what settings are required to work with the certificates. Thanks to Mitch Denny, who wrote a [very good post](http://notgartner.wordpress.com/2007/09/06/using-certificate-based-authentication-and-protection-with-windows-communication-foundation-wcf/) on the use of certificates, which helped a lot more than some of the official documentation. This post assumes that you're using Juval Lowy of IDesign's [ServiceModelEx](http://idesign.net/idesign/DesktopDefault.aspx?tabindex=5&tabid=11) extensions for WCF. If you're not, you should really consider these additions - they add declarative security and better validation amongst many other enhancements. 

First you need to create a certificate for both the client and the server. Most examples demonstrate the scenario on a single machine thus glossing over the fact that in that situation you only need the one certificate. It obscures the real process of certificate exchange which I’ll show here. 

The following is a little script (called setupcert.cmd) that will create a new certificate using makecert.exe, will add it to the ‘My’ cert store on the location machine hive of the registry. It will then export that cert for use on the other machine. 

<blockquote>set CERT_NAME=%1
>     certmgr.exe -del -r LocalMachine -s My -c -n %CERT_NAME%
>     makecert.exe -sr LocalMachine -ss MY -a sha1 -n CN=%CERT_NAME% -sky exchange -pe
>     certmgr.exe /put /c /r LocalMachine /n %CERT_NAME% /s my %CERT_NAME%.publickey.cer
> 
> [](http://11011.net/software/vspaste)

> 
> </blockquote>




You pass one parameter to the script, which is the common name you want to give to the certificate. For instance, you could call it “ServerSide” or “ClientSide”.


Run the script on the client side.


<blockquote>

> 
> setupcert ClientSide
> 
> </blockquote>




You should now have a file in the current directory called ClientSide.publickey.cer. Copy this file over to the server side. You are now be able to install the client side cert on the server side.


Double click on the client side cert file. You should get a warning about the unknown provenance of the cert.


[![image](http://aabs.files.wordpress.com/2007/10/image-thumb1.png)](http://aabs.files.wordpress.com/2007/10/image1.png)


Click the Open button. Then click on the ‘Install Certificate…’ button.


[![image](http://aabs.files.wordpress.com/2007/10/image-thumb2.png)](http://aabs.files.wordpress.com/2007/10/image2.png)


You should now be running the certificate install wizard (or equivalent).


[![image](http://aabs.files.wordpress.com/2007/10/image-thumb3.png)](http://aabs.files.wordpress.com/2007/10/image3.png)


Click Next. On the next page select to place the certificate in the store of your choice. A dialog box appears with the cert stores available. Check the ‘Show Physical Stores’ checkbox like so. Click OK to proceed.


[![image](http://aabs.files.wordpress.com/2007/10/image-thumb4.png)](http://aabs.files.wordpress.com/2007/10/image4.png)


You will now get the confirmation window.


[![image](http://aabs.files.wordpress.com/2007/10/image-thumb5.png)](http://aabs.files.wordpress.com/2007/10/image5.png)


Click Finish to complete the import. If all goes well you get this.


[![image](http://aabs.files.wordpress.com/2007/10/image-thumb6.png)](http://aabs.files.wordpress.com/2007/10/image6.png)

At this stage we have a certificate installed on the client side called ClientSide. This certificate was copied to the server side and installed in the LocalMachine trusted people store. Now the reverse has to be done. Go to the server side and invoke the script from there.


<blockquote>

> 
> setupcert ServerSide
> 
> </blockquote>




copy ServerSide.publickey.cer to the client side, and from there you should repeat the import procedure outlined above. The certificates should now be configured. To confirm that, open mmc.exe and add a certificates snap-in for the local machine. On the client side you should see the ClientSide certificate in the personal certificates store.


[![image](http://aabs.files.wordpress.com/2007/10/image-thumb7.png)](http://aabs.files.wordpress.com/2007/10/image7.png)


In the Trusted People certificates you should see the server side certificate.


[![image](http://aabs.files.wordpress.com/2007/10/image-thumb8.png)](http://aabs.files.wordpress.com/2007/10/image8.png)


The reverse should be true for the Server side certificate stores.


### Configuring the service endpoints




The client side proxy needs to reference its certificate like so.


<blockquote><span style="color:rgb(43,145,175);">ServiceProxy</span> proxy = <span style="color:rgb(0,0,255);">new</span> <span style="color:rgb(43,145,175);">ServiceProxy</span>();
>     <span style="color:rgb(43,145,175);">SecurityHelper</span>.SecureProxy<<span style="color:rgb(43,145,175);">IMyService</span>>(proxy, <span style="color:rgb(163,21,21);">"ClientSide"</span>);
>     
> 
> [](http://11011.net/software/vspaste)</blockquote>




And it can reference the server side certificate via the configuration settings.
    
    <span style="color:rgb(0,0,255);">        <</span><span style="color:rgb(163,21,21);">endpoint</span><span style="color:rgb(0,0,255);">
              </span><span style="color:rgb(255,0,0);">address</span><span style="color:rgb(0,0,255);">=</span>"<span style="color:rgb(0,0,255);">http://nitrogen:8001/MyService</span>"<span style="color:rgb(0,0,255);">
              </span><span style="color:rgb(255,0,0);">binding</span><span style="color:rgb(0,0,255);">=</span>"<span style="color:rgb(0,0,255);">wsHttpBinding</span>"<span style="color:rgb(0,0,255);">
              </span><span style="color:rgb(255,0,0);">contract</span><span style="color:rgb(0,0,255);">=</span>"<span style="color:rgb(0,0,255);">SandlNtSvc.IMyService</span>"<span style="color:rgb(0,0,255);">>
              <</span><span style="color:rgb(163,21,21);">identity</span><span style="color:rgb(0,0,255);">>
                <</span><span style="color:rgb(163,21,21);">dns</span><span style="color:rgb(0,0,255);"> </span><span style="color:rgb(255,0,0);">value</span><span style="color:rgb(0,0,255);">=</span>"<span style="color:rgb(0,0,255);">ServerSide</span>"<span style="color:rgb(0,0,255);"> />
              </</span><span style="color:rgb(163,21,21);">identity</span><span style="color:rgb(0,0,255);">>
            </</span><span style="color:rgb(163,21,21);">endpoint</span><span style="color:rgb(0,0,255);">>
    </span>

[](http://11011.net/software/vspaste)


The server side references its certificate via its security behavior attribute.
    
        [<span style="color:rgb(43,145,175);">SecurityBehavior</span>(<span style="color:rgb(43,145,175);">ServiceSecurity</span>.Anonymous, <span style="color:rgb(163,21,21);">"ServerSide"</span>)]
        <span style="color:rgb(0,0,255);">public</span> <span style="color:rgb(0,0,255);">class</span> <span style="color:rgb(43,145,175);">MyService</span> : <span style="color:rgb(43,145,175);">IMyService
    </span>    {
    

[](http://11011.net/software/vspaste)


That's all the configuration needed for the service and proxy to talk to each other.


PS. remember to delete any cert files left lying around!
