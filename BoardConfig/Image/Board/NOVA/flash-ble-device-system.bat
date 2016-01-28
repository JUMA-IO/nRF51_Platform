nrfjprog --eraseall

nrfjprog --program ..\..\common\ble_device_radio.hex

nrfjprog --program ..\..\common\ble_device_platform.hex

nrfjprog --program BoardConfig.hex

nrfjprog --reset

pause

