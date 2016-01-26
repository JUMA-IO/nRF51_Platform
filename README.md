# nRF51_Platform
******
JUMA.IO Platform for nRF51 SoC.

This is the most Easy-to-Use Bluetooth Smart 4.0 SDK for embedded development in the world.
#Building 
********

If you plan to develop an app using this SDK, please use the stable branch or the last released tarball. The master branch is continuously evolving, could be unstable and could change very often.


You can open the following file to supplement the SDK：

``
Platform/Project/device.uvprojx
``

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
4. copy 'Example/projects' to folder 'Product/sdk/' and build your own app.
Voila!