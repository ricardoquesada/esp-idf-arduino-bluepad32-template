/****************************************************************************
http://retro.moe/unijoysticle2

Copyright 2021 Ricardo Quesada

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
****************************************************************************/

#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif  // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO

#include <Arduino.h>
#include <Bluepad32.h>

//
// README FIRST, README FIRST, README FIRST
//
// Bluepad32 has a built-in interactive console.
// By default it is enabled (hey, this is a great feature!).
// But it is incompatible with Arduino "Serial" class.
//
// Instead of using "Serial" you can use Bluepad32 "Console" class.
// It is somewhat similar to Serial but not exactly the same.
//
// Should you want to still use "Serial", you have to disable the Bluepad32's console
// from "sdkconfig.defaults" with:
//    CONFIG_BLUEPAD32_USB_CONSOLE_ENABLE=n

ControllerPtr myControllers[BP32_MAX_CONTROLLERS];

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_CONTROLLERS; i++) {
        if (myControllers[i] == nullptr) {
            Console.printf("CALLBACK: Controller is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            ControllerProperties properties = ctl->getProperties();
            Console.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName(), properties.vendor_id,
                           properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Console.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_CONTROLLERS; i++) {
        if (myControllers[i] == ctl) {
            Console.printf("CALLBACK: Controller is disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            break;
        }
    }

    if (!foundController) {
        Console.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

// Arduino setup function. Runs in CPU 1
void setup() {
    Console.printf("Firmware: %s\n", BP32.firmwareVersion());

    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedController, &onDisconnectedController);

    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" controllers to reconnect.
    // But might also fix some connection / re-connection issues.
    BP32.forgetBluetoothKeys();
}

void processGamepad(ControllerPtr gamepad) {
    // There are different ways to query whether a button is pressed.
    // By query each button individually:
    //  a(), b(), x(), y(), l1(), etc...

    if (gamepad->a()) {
        static int colorIdx = 0;
        // Some gamepads like DS4 and DualSense support changing the color LED.
        // It is possible to change it by calling:
        switch (colorIdx % 3) {
            case 0:
                // Red
                gamepad->setColorLED(255, 0, 0);
                break;
            case 1:
                // Green
                gamepad->setColorLED(0, 255, 0);
                break;
            case 2:
                // Blue
                gamepad->setColorLED(0, 0, 255);
                break;
        }
        colorIdx++;
    }

    if (gamepad->b()) {
        // Turn on the 4 LED. Each bit represents one LED.
        static int led = 0;
        led++;
        // Some gamepads like the DS3, DualSense, Nintendo Wii, Nintendo Switch
        // support changing the "Player LEDs": those 4 LEDs that usually indicate
        // the "gamepad seat".
        // It is possible to change them by calling:
        gamepad->setPlayerLEDs(led & 0x0f);
    }

    if (gamepad->x()) {
        // Duration: 255 is ~2 seconds
        // force: intensity
        // Some gamepads like DS3, DS4, DualSense, Switch, Xbox One S support
        // rumble.
        // It is possible to set it by calling:
        gamepad->setRumble(0xc0 /* force */, 0xc0 /* duration */);
    }

    // Another way to query the buttons, is by calling buttons(), or
    // miscButtons() which return a bitmask.
    // Some gamepads also have DPAD, axis and more.
    Console.printf(
        "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, "
        "%4d, brake: %4d, throttle: %4d, misc: 0x%02x, battery=%d\n",
        gamepad->index(),        // Controller Index
        gamepad->dpad(),         // DPAD
        gamepad->buttons(),      // bitmask of pressed buttons
        gamepad->axisX(),        // (-511 - 512) left X Axis
        gamepad->axisY(),        // (-511 - 512) left Y axis
        gamepad->axisRX(),       // (-511 - 512) right X axis
        gamepad->axisRY(),       // (-511 - 512) right Y axis
        gamepad->brake(),        // (0 - 1023): brake button
        gamepad->throttle(),     // (0 - 1023): throttle (AKA gas) button
        gamepad->miscButtons(),  // bitmak of pressed "misc" buttons
        gamepad->battery()       // 0=Unk, 1=Empty, 255=full
    );

    // You can query the axis and other properties as well. See ArduinoController.h
    // For all the available functions.
}

void processMouse(ControllerPtr mouse) {
    Console.printf("idx=%d, deltaX:%4d, deltaY:%4d, buttons: 0x%04x, misc: 0x%02x, battery=%d\n",
                   mouse->index(),        // Controller Index
                   mouse->deltaX(),       // Mouse delta X
                   mouse->deltaY(),       // Mouse delta Y
                   mouse->buttons(),      // bitmask of pressed buttons
                   mouse->miscButtons(),  // bitmak of pressed "misc" buttons
                   mouse->battery()       // 0=Unk, 1=Empty, 255=full
    );
}

void processBalanceBoard(ControllerPtr balance) {
    Console.printf("idx=%d, tl:%4d, tr:%4d, bl: %4d, br: %4d, temperature=%d, battery=%d\n",
                   balance->index(),  // Controller Index
                   balance->topLeft(), balance->topRight(), balance->bottomLeft(), balance->bottomRight(),
                   balance->temperature(),
                   balance->battery()  // 0=Unk, 1=Empty, 255=full
    );
}

// Arduino loop function. Runs in CPU 1
void loop() {
    // This call fetches all the controller info from the NINA (ESP32) module.
    // Just call this function in your main loop.
    // The controllers pointer (the ones received in the callbacks) gets updated
    // automatically.
    BP32.update();

    // It is safe to always do this before using the controller API.
    // This guarantees that the controller is valid and connected.
    for (int i = 0; i < BP32_MAX_CONTROLLERS; i++) {
        ControllerPtr myController = myControllers[i];

        if (myController && myController->isConnected()) {
            if (myController->isGamepad())
                processGamepad(myController);
            else if (myController->isMouse())
                processMouse(myController);
            else if (myController->isBalanceBoard())
                processBalanceBoard(myController);
        }
    }
    delay(150);
}
