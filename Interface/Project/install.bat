xcopy build\juma_sdk.lib ..\Output\ /F /Y
xcopy ..\..\Interface\Output\juma_sdk.lib  ..\..\Product\sdk\lib\ /F /Y
xcopy ..\..\Interface\Source\juma_sdk.sct  ..\..\Product\sdk\lib\ /F /Y
xcopy ..\..\Interface\Include\juma_sdk_types.h ..\..\Product\sdk\include\ /F /Y
xcopy ..\..\Interface\Include\juma_sdk_api.h ..\..\Product\sdk\include\ /F /Y