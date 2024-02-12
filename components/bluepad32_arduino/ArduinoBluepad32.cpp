// Copyright 2021 - 2021, Ricardo Quesada, http://retro.moe
// SPDX-License-Identifier: Apache-2.0 or LGPL-2.1-or-later

#include "ArduinoBluepad32.h"

#include "sdkconfig.h"

#include <bt/uni_bt.h>
#include <uni_log.h>
#include <uni_version.h>
#include <uni_virtual_device.h>

Bluepad32::Bluepad32() : _prevConnectedControllers(0), _controllers(), _onConnect(), _onDisconnect() {}

const char* Bluepad32::firmwareVersion() const {
    return "Bluepad32 for Arduino v" UNI_VERSION;
}

bool Bluepad32::update() {
    bool data_updated = false;
    int connectedControllers = 0;
    int status;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        status = arduino_get_controller_data(i, &_controllers[i]._data);
        if (status == UNI_ARDUINO_ERROR_INVALID_DEVICE)
            continue;

        // If at least one controller has data, we return true.
        if (status == UNI_ARDUINO_ERROR_SUCCESS)
            data_updated = true;

        // Update individual controller.
        _controllers[i]._hasData = (status == UNI_ARDUINO_ERROR_SUCCESS);

        // Update Idx in case it is the first time to get updated.
        _controllers[i]._idx = i;
        connectedControllers |= (1 << i);
    }

    // No changes in connected controllers. No need to call onConnected or onDisconnected.
    if (connectedControllers == _prevConnectedControllers)
        return data_updated;

    logi("connected in total: 0x%02x (flag)\n", connectedControllers);

    // Compare bit by bit, and find which one got connected and which one disconnected.
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        int bit = (1 << i);
        int current = connectedControllers & bit;
        int prev = _prevConnectedControllers & bit;

        // No changes in this controller, skip
        if (current == prev)
            continue;

        if (current) {
            logi("controller connected: %d\n", i);
            _controllers[i].onConnected();
            _onConnect(&_controllers[i]);
        } else {
            _onDisconnect(&_controllers[i]);
            _controllers[i].onDisconnected();
            logi("controller disconnected: %d\n", i);
        }
    }

    _prevConnectedControllers = connectedControllers;

    return data_updated;
}

void Bluepad32::forgetBluetoothKeys() {
    uni_bt_del_keys_safe();
}

void Bluepad32::enableNewBluetoothConnections(bool enabled) {
    uni_bt_enable_new_connections_safe(enabled);
}

void Bluepad32::setup(const GamepadCallback& onConnect, const GamepadCallback& onDisconnect) {
    _onConnect = onConnect;
    _onDisconnect = onDisconnect;
}

const uint8_t* Bluepad32::localBdAddress() {
    static bd_addr_t addr;
    uni_bt_get_local_bd_addr_safe(addr);
    return addr;
}

void Bluepad32::enableVirtualDevice(bool enabled) {
    uni_virtual_device_set_enabled(enabled);
}

void Bluepad32::enableBLEService(bool enabled) {
    uni_bt_enable_service_safe(enabled);
}

Bluepad32 BP32;
