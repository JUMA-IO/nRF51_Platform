xcopy build_device\ble_device_platform.hex ..\Image\ /F /Y

xcopy ..\Image\ble_device_radio.hex  ..\..\Product\sdk\images\ /F /Y
xcopy ..\Image\ble_device_platform.hex  ..\..\Product\sdk\images\ /F /Y
xcopy ..\Image\flash-ble-device-system.bat	..\..\Product\sdk\images\ /F /Y

xcopy ..\Image\ble_device_radio.hex  ..\..\BoardConfig\Image\common\ /F /Y
xcopy ..\Image\ble_device_platform.hex  ..\..\BoardConfig\Image\common\ /F /Y
xcopy ..\Image\flash-ble-device-system.bat  ..\..\BoardConfig\Image\common\ /F /Y


