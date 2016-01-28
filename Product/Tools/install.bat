xcopy build\app.hex ..\Output\ /F /Y

..\..\..\Tools\Bin2otabin\mingw5\bin2otabin.exe	build\app.bin
xcopy build\app.ota.bin ..\Output\ /F /Y
xcopy ..\..\..\Tools\Resource\flash-application.bat ..\Output\ /F /Y
