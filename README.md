# nRF51_Platform
******
JUMA.IO Platform for nRF51 SoC.

This is the most Easy-to-Use Bluetooth Smart 4.0 SDK for embedded development in the world.
##Building:
********

If you plan to develop an app using this SDK, please use the stable branch or the last released tarball. The master branch is continuously evolving, could be unstable and could change very often.

####Build to image files：

``
Platform/Project/device.uvprojx
``

####Build to .lib files:
``
Interface/Project/juma_sdk.uvprojx
``

####Python Building tools:
The SDK will also provide a .lib for user's app to build with. It is automatically generated using Python.

Following the steps below to build the platform, interface and test with Examples

1. run 'python update.py' in 'Interface' folder. 
2. open and build
``
Interface/Project/juma_sdk.uvprojx
``
3. open SDK and build
``
Platform/Project/device.uvprojx
``
4. copy 'Product\Examples\Template' to folder 'Product\Projects' and build your own app.
Voila!

##More Info:
******
When you download repository and open project, you have to set correct your target, for device you use. If you use other device in your project, then set your project and copy settings there from target options.

More information below [http://juma.io](http://www.juma.io)

First time with Discovery SDK? [http://juma.io/sdk.html](http://www.juma.io/sdk.html)

How to use these API? [http://juma.io/doc/zh/embedded_sdk.html](http://www.juma.io/doc/zh/embedded_sdk.html)

