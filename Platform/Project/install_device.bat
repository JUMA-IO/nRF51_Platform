xcopy build_device\ble_device_platform.hex ..\Image\ /F /Y
xcopy ..\Image\ble_device_radio.hex  ..\..\Product\sdk\images\ /F /Y
xcopy ..\Image\ble_device_platform.hex  ..\..\Product\sdk\images\ /F /Y

xcopy ..\Image\ble_device_radio.hex  ..\..\basic_image_download\common\ /F /Y
xcopy ..\Image\ble_device_platform.hex  ..\..\basic_image_download\common\ /F /Y