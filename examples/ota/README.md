# BluePad32 ESP OTA Update example

**This is a copy of https://github.com/ste2425/BluePad32OTAExample**

This is an example Application which performs OTA Updates on the ESP32 whilst BluePad32 can connect to controllers.

It is purly a proof of concept but proves the functionality works.

It can be used in conjunction with a correspondsing web app.

The source for the web app is [here](https://github.com/ste2425/OTAWebapp) and a version of the web app is hosted [here](https://ste2425.github.io/OTAWebapp/).

Simply flash the ESP. This is an extension of the template provided by BluePad32 [here](https://gitlab.com/ricardoquesada/esp-idf-arduino-bluepad32-template) with OTA functionality applied on top.

The BLE code is an extension of the delayed response example (with the delayed parts removed) privided by BTStack, it can be viewed [here](https://github.com/bluekitchen/btstack/blob/master/example/att_delayed_response.c).

# NOTES

`BLE_setup` must be called after `BP32.setup`.
* `BP32.enableNewBluetoothConnections` must be set to false when doing the OTA update (in this demo it is hard coded to false, more info below)
* With the ESP menuconfig the following config was set `CONFIG_ESP_MAIN_TASK_STACK_SIZE=8192`. With the default stack size when the OTA had been uploaded and was applied with `esp_ota_end(otaHandler)` a stack overflow exception was encountered.

## `BP32.enableNewBluetoothConnections` issue explained

If an OTA is uploaded (or any large amounts of data streamed to the ESP) whilst `BP32.enableNewBluetoothConnections` is set to true it causes slow downs in the upload and eventual disconnects between the ESP and the web browser. This is most likely bandwidth issues (admitedly i do not know). What ever the reason the advise is to set this to false when streaming large amounts of data and enable again (if needed) once finished. Small adhoc BLE communication is fine whilst this is set to true. Its only streaming large data like a binary for OTA updates.

## Generating a binary to update

The easiest option is to modify the variable `test_string` in `ble.cpp` and compile. Then copy the binary from the build folder and give it to the UI. In the UI when the `read` button is clicked `test_string` is the value the characteristic returns. So change this allows you to do a before and after and prove the OTA worked.
